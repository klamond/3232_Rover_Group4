/*
 * File:   main.c
 * Author: kabil
 *
 * Created on January 30, 2026, 4:10 PM
 */

// CONFIG1
#pragma config FEXTOSC = OFF
#pragma config RSTOSC = HFINT32
#pragma config CLKOUTEN = OFF
#pragma config CSWEN = ON
#pragma config FCMEN = OFF

// CONFIG2
#pragma config MCLRE = ON
#pragma config PWRTE = OFF
#pragma config LPBOREN = OFF
#pragma config BOREN = ON
#pragma config BORV = LO
#pragma config ZCD = OFF
#pragma config PPS1WAY = ON
#pragma config STVREN = ON

// CONFIG3
#pragma config WDTCPS = WDTCPS_31
#pragma config WDTE = OFF
#pragma config WDTCWS = WDTCWS_7
#pragma config WDTCCS = SC

// CONFIG4
#pragma config WRT = OFF
#pragma config SCANE = available
#pragma config LVP = ON

// CONFIG5
#pragma config CP = OFF
#pragma config CPD = OFF

#include <xc.h>
#include <stdint.h>
#include <stdlib.h>

#include "comms.h"
#include "movement.h"
#include "task_solar.h"
#include "ore_process.h"

#define _XTAL_FREQ 32000000UL

static void gpio_init(void) {
    // MOSFET gate on RA0: digital output, start OFF
    TRISAbits.TRISA0 = 0;
    ANSELAbits.ANSA0 = 0;
    LATAbits.LATA0   = 0;

    // S2 button on RA5: digital input
    TRISAbits.TRISA5 = 1;
    ANSELAbits.ANSA5 = 0;
}

int main(void) {
    gpio_init();
    comms_init();

    while (1) {
        __delay_ms(100);
        comms_send_get_data();

        if (comms_new_502) {
            comms_new_502 = 0;
            movement_parse_502();
            solar_parse_502();
            ore_process_parse_502();
        }
    }

    return EXIT_SUCCESS;
}
