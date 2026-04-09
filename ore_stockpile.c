// Ore scooper flap servo — driven by left joystick Y axis (payload[4:5])
// Servo signal on RA1: 1ms pulse = flap down, 2ms pulse = flap up
// 50 Hz servo protocol (orange = PWM, red = +5V, brown = GND)

#include "ore_stockpile.h"
#include "comms.h"
#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 32000000UL

#define SERVO_TRIS   TRISAbits.TRISA1
#define SERVO_ANSEL  ANSELAbits.ANSA1
#define SERVO_LAT    LATAbits.LATA1

void stockpile_init(void) {
    SERVO_ANSEL = 0;  // Digital
    SERVO_TRIS  = 0;  // Output
    SERVO_LAT   = 0;  // Start low
}

static void output_servo_pulse(uint8_t flap_up) {
    // Disable Timer2 interrupt during pulse so its ISR overhead does not
    // stretch the software delay and push the pulse width out of spec.
    // UART (RCIF) interrupt remains active so comms are unaffected.
    PIE4bits.TMR2IE = 0;
    SERVO_LAT = 1;
    if (flap_up) {
        __delay_ms(2);  // 2ms pulse — flap up
    } else {
        __delay_ms(1);  // 1ms pulse — flap down
    }
    SERVO_LAT = 0;
    PIE4bits.TMR2IE = 1;
}

void stockpile_parse_502(void) {
    uint16_t raw = ((uint16_t)comms_payload[5] << 8) | comms_payload[4];

    // Pulse on every loop iteration while joystick is held at an extreme.
    // Continuous pulses are needed to reliably drive the servo to position.
    // The hysteresis zone (1250–1750) acts as deadband — no pulse, servo holds.
    if (raw >= 1750) {
        output_servo_pulse(1);
    } else if (raw <= 1250) {
        output_servo_pulse(0);
    }
}
