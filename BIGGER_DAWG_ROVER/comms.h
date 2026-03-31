#ifndef COMMS_H
#define COMMS_H

#include <stdint.h>

// Payload from the last received message — read by other modules
extern uint8_t comms_payload[21];

// Set by ISR when a complete message arrives; cleared by main after processing
extern volatile uint8_t comms_new_402;
extern volatile uint8_t comms_new_502;

void comms_init(void);
void comms_send_get_info(void);
void comms_send_get_data(void);
void comms_send_move(uint8_t dirA, uint8_t speedA, uint8_t dirB, uint8_t speedB);
void comms_send_color_info(uint8_t color);
void comms_send_mag_info(uint8_t pyramid_num);

#endif
