


#include "task_magnetic.h"
#include "comms.h"
#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 32000000UL


uint16_t ADC;
void magnet_init(){
    ANSELAbits.ANSA2 = 0;       //this is the LED
    TRISAbits.TRISA2 = 0;
    LATAbits.LATA2 = 0;

    ANSELBbits.ANSB0 = 1;       //I made this the out of the sensor
    TRISBbits.TRISB0 = 1;  
    
    ADCLKbits.ADCCS = 0b111111;
    
    ADREFbits.ADPREF = 00;
    ADREFbits.ADNREF = 0;
    
    ADPCH = 0b001000;   // the output location 
    ADACQ = 0b00000001;
    
    ADCON0bits.ADON = 1;
    ADCON0bits.ADFM0 = 1;
    
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    ADCON0bits.ADGO = 1;
}

void magnetic_parse_502(void) {
    uint16_t flag = (comms_payload[11] << 8) | comms_payload[10];   //switch B

    if (flag == 2000) {
        ADC = ((uint16_t)ADRESH << 8);
        ADC = (ADC + ADRESL) & 0x3FF;
        if(0x150 > ADC && ADC > 0x125){ //the threshold // neutral is 0x013D-0x013E
            LATAbits.LATA2 = 0;         // off it between the upper and lower bounds (neutral position)
        }
        else{
            LATAbits.LATA2 = 1;
        }
        return;  // 
    }

    if (flag == 1000) {
        LATAbits.LATA2 = 0;
        uint16_t pyramid_num_raw = (comms_payload[17] << 8) | comms_payload[16];    // dial A
        uint8_t pyramid_num;
        if (pyramid_num_raw <= 1167) {
            pyramid_num = 1;  // 
        } else if (pyramid_num_raw <= 1333) {
            pyramid_num = 2;  // 
        } else if (pyramid_num_raw <= 1500) {
            pyramid_num = 3;  // 
        } else if (pyramid_num_raw <= 1667) {
            pyramid_num = 4;  // 
        } else if (pyramid_num_raw <= 1833) {
            pyramid_num = 5;  // 
        } else {
            pyramid_num = 6;  // 
        }
        comms_send_mag_info(pyramid_num);
    }
}