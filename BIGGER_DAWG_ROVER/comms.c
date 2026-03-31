#include "comms.h"
#include <xc.h>
#include <stdint.h>

#define MSG_ID_402  0x0402
#define MSG_ID_502  0x0502

// ============================================================================
// PUBLIC DATA (declared extern in comms.h)
// ============================================================================

uint8_t          comms_payload[21];
volatile uint8_t comms_new_402 = 0;
volatile uint8_t comms_new_502 = 0;

// ============================================================================
// INTERNAL STATE MACHINE
// ============================================================================

typedef enum {
    WAIT_SYNC_1,
    WAIT_SYNC_2,
    WAIT_ID_LSB,
    WAIT_ID_MSB,
    WAIT_PAYLOAD_LEN_LSB,
    WAIT_PAYLOAD_LEN_MSB,
    WAIT_PAYLOAD
} comms_state_t;

static comms_state_t state           = WAIT_SYNC_1;
static uint16_t      msg_id          = 0;
static uint16_t      payload_length  = 0;
static uint16_t      payload_received = 0;

static void handle_message_complete(void) {
    switch (msg_id) {
        case MSG_ID_402: comms_new_402 = 1; break;
        case MSG_ID_502: comms_new_502 = 1; break;
        default: break;
    }
}

static void process_byte(uint8_t byte) {
    switch (state) {
        case WAIT_SYNC_1:
            if (byte == 0xFE) state = WAIT_SYNC_2;
            break;
        case WAIT_SYNC_2:
            state = (byte == 0x19) ? WAIT_ID_LSB : WAIT_SYNC_1;
            break;
        case WAIT_ID_LSB:
            msg_id = byte;
            state  = WAIT_ID_MSB;
            break;
        case WAIT_ID_MSB:
            msg_id |= (byte << 8);
            state   = WAIT_PAYLOAD_LEN_LSB;
            break;
        case WAIT_PAYLOAD_LEN_LSB:
            payload_length = byte;
            state          = WAIT_PAYLOAD_LEN_MSB;
            break;
        case WAIT_PAYLOAD_LEN_MSB:
            payload_length |= (byte << 8);
            payload_received = 0;
            if (payload_length == 0) {
                handle_message_complete();
                state = WAIT_SYNC_1;
            } else {
                state = WAIT_PAYLOAD;
            }
            break;
        case WAIT_PAYLOAD:
            comms_payload[payload_received++] = byte;
            if (payload_received >= payload_length) {
                handle_message_complete();
                state = WAIT_SYNC_1;
            }
            break;
        default:
            state = WAIT_SYNC_1;
            break;
    }
}

// ============================================================================
// UART PRIMITIVES
// ============================================================================

static void push_byte(uint8_t byte) {
    while (!PIR3bits.TXIF) {}
    TX1REG = byte;
}

// ============================================================================
// PUBLIC API
// ============================================================================

void comms_init(void) {
    TX1STAbits.SYNC  = 0;   // Async mode
    RC1STAbits.SPEN  = 1;   // Enable serial port
    TX1STAbits.TXEN  = 1;   // Enable TX
    RC1STAbits.CREN  = 1;   // Enable RX

    // Unlock PPS
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0;

    // TX on RC6
    TRISCbits.TRISC6  = 0;
    ANSELCbits.ANSC6  = 0;
    RC6PPS            = 0x10;

    // RX on RC5
    TRISCbits.TRISC5  = 1;
    ANSELCbits.ANSC5  = 0;
    RXPPS             = 0x15;

    // Baud rate: 115200 @ 32 MHz
    TX1STAbits.BRGH   = 1;
    BAUD1CONbits.BRG16 = 1;
    SP1BRGL           = 68;

    // Enable UART RX interrupt
    PIE3bits.RCIE    = 1;
    INTCONbits.PEIE  = 1;
    INTCONbits.GIE   = 1;
}

void comms_send_get_info(void) {
    push_byte(0xFE);
    push_byte(0x19);
    push_byte(0x01);
    push_byte(0x04);
    push_byte(0x00);
    push_byte(0x00);
    while (TX1STAbits.TRMT == 0) {}
}

void comms_send_get_data(void) {
    push_byte(0xFE);
    push_byte(0x19);
    push_byte(0x01);
    push_byte(0x05);
    push_byte(0x00);
    push_byte(0x00);
    while (TX1STAbits.TRMT == 0) {}
}

void comms_send_move(uint8_t dirA, uint8_t speedA, uint8_t dirB, uint8_t speedB) {
    push_byte(0xFE);
    push_byte(0x19);
    push_byte(0x01);
    push_byte(0x06);
    push_byte(0x04);
    push_byte(0x00);
    push_byte(dirA);
    push_byte(speedA);
    push_byte(dirB);
    push_byte(speedB);
    while (TX1STAbits.TRMT == 0) {}
}

void comms_send_color_info(uint8_t color) {
    push_byte(0xFE);
    push_byte(0x19);
    push_byte(0x03);   // msg ID LSB
    push_byte(0x0A);   // msg ID MSB  → 0x0A03
    push_byte(0x01);   // payload len LSB (1 byte)
    push_byte(0x00);   // payload len MSB
    push_byte(color);  // 1=red, 2=yellow, 3=blue
    while (TX1STAbits.TRMT == 0) {}
}

void comms_send_mag_info(uint8_t pyramid_num) {
    push_byte(0xFE);
    push_byte(0x19);
    push_byte(0x01);         // msg ID LSB
    push_byte(0x0A);         // msg ID MSB  → 0x0A01
    push_byte(0x04);         // payload len LSB (4 bytes)
    push_byte(0x00);         // payload len MSB
    push_byte(0x03);         // task ID LSB
    push_byte(0x00);         // task ID MSB
    push_byte(pyramid_num);  // pyramid num [1,2,3,4,5,6]
    push_byte(0x00);         // 4th payload byte (padding)
    while (TX1STAbits.TRMT == 0) {}
}

// ============================================================================
// ISR
// ============================================================================

void __interrupt() ISR(void) {
    if (PIR3bits.RCIF) {
        uint8_t byte = RC1REG;
        if (RC1STAbits.OERR) {
            RC1STAbits.CREN = 0;
            RC1STAbits.CREN = 1;
        }
        process_byte(byte);
    }
}
