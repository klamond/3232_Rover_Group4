#include "task_linefollow.h"
#include "comms.h"
#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 32000000UL

// Tune these if needed
#define LINE_THRESHOLD_L 800
#define LINE_THRESHOLD_M 750
#define LINE_THRESHOLD_R 800

// Left/right channels are swapped because that matched your hardware better
#define LINE_CH_L 0x0A
#define LINE_CH_M 0x09
#define LINE_CH_R 0x08

static uint16_t adcL, adcM, adcR;

static void drive_signed(int16_t left, int16_t right)
{
    uint8_t dirA, speedA, dirB, speedB;

    if (left > 100)   left = 100;
    if (left < -100)  left = -100;
    if (right > 100)  right = 100;
    if (right < -100) right = -100;

    // Left motor normal
    if (left == 0) {
        dirA = 0;
        speedA = 0;
    } else if (left > 0) {
        dirA = 1;
        speedA = (uint8_t)left;
    } else {
        dirA = 2;
        speedA = (uint8_t)(-left);
    }

    // Right motor flipped in software
    if (right == 0) {
        dirB = 0;
        speedB = 0;
    } else if (right > 0) {
        dirB = 2;
        speedB = (uint8_t)right;
    } else {
        dirB = 1;
        speedB = (uint8_t)(-right);
    }

    comms_send_move(dirA, speedA, dirB, speedB);
}

static void ADC_Init(void)
{
    TRISBbits.TRISB0 = 1;
    ANSELBbits.ANSB0 = 1;

    TRISBbits.TRISB1 = 1;
    ANSELBbits.ANSB1 = 1;

    TRISBbits.TRISB2 = 1;
    ANSELBbits.ANSB2 = 1;

    ADREFbits.ADPREF = 0;
    ADREFbits.ADNREF = 0;
    ADCLKbits.ADCCS = 0b111111;
    ADACQ = 0x01;
    ADCON0bits.ADFRM0 = 1;
    ADCON0bits.ADON = 1;
}

static uint16_t adc_read(uint8_t channel)
{
    ADPCH = channel;
    __delay_us(5);

    ADCON0bits.ADGO = 1;
    while (ADCON0bits.ADGO);

    return ((uint16_t)ADRESH << 8) | ADRESL;
}

static void line_follow_update(void)
{
    static int8_t last_dir = 0;        // -1 = right, 0 = center, 1 = left
    static uint8_t pulse = 0;
    static uint8_t recover_count = 0;

    uint8_t slow_cycle;
    uint8_t L, M, R;

    adcL = adc_read(LINE_CH_L);
    adcM = adc_read(LINE_CH_M);
    adcR = adc_read(LINE_CH_R);

    L = (adcL > LINE_THRESHOLD_L);
    M = (adcM > LINE_THRESHOLD_M);
    R = (adcR > LINE_THRESHOLD_R);

    // Keep this simple: mostly full-speed updates, occasional slower one
    pulse++;
    if (pulse >= 5) {
        pulse = 0;
    }
    slow_cycle = (pulse == 4);

    if (M && !L && !R) {
        last_dir = 0;
        recover_count = 0;
        if (slow_cycle) drive_signed(40, 40);
        else            drive_signed(55, 55);
    }
    else if (L && M && !R) {
        last_dir = 1;
        recover_count = 0;
        if (slow_cycle) drive_signed(35, 55);
        else            drive_signed(45, 70);
    }
    else if (L && !M && !R) {
        last_dir = 1;
        recover_count = 0;
        if (slow_cycle) drive_signed(30, 60);
        else            drive_signed(40, 80);
    }
    else if (!L && M && R) {
        last_dir = -1;
        recover_count = 0;
        if (slow_cycle) drive_signed(55, 35);
        else            drive_signed(70, 45);
    }
    else if (!L && !M && R) {
        last_dir = -1;
        recover_count = 0;
        if (slow_cycle) drive_signed(60, 30);
        else            drive_signed(80, 40);
    }
    else if (L && M && R) {
        recover_count = 0;
        if (slow_cycle) drive_signed(40, 40);
        else            drive_signed(55, 55);
    }
    else if (L && !M && R) {
        recover_count = 0;
        if (last_dir > 0) {
            if (slow_cycle) drive_signed(35, 55);
            else            drive_signed(45, 70);
        } else if (last_dir < 0) {
            if (slow_cycle) drive_signed(55, 35);
            else            drive_signed(70, 45);
        } else {
            if (slow_cycle) drive_signed(40, 40);
            else            drive_signed(55, 55);
        }
    }
    else {
        // Lost line: short search only
        if (recover_count < 2) {
            recover_count++;

            if (last_dir > 0) {
                if (slow_cycle) drive_signed(85, -85);
                else            drive_signed(90, -90);
            } else if (last_dir < 0) {
                if (slow_cycle) drive_signed(-85, 85);
                else            drive_signed(-90, 90);
            } else {
                drive_signed(-70, -70);
            }
        } else {
            recover_count = 0;
            drive_signed(0, 0);
        }
    }
}

void task_linefollow_parse_502(void)

{
    static uint8_t initialized = 0;
    uint16_t mode = ((uint16_t)comms_payload[11] << 8) | comms_payload[10];

    if (!initialized) {
        ADC_Init();
        initialized = 1;
    }

    if (mode > 1450 && mode < 1550) {
        line_follow_update();
    }
}


