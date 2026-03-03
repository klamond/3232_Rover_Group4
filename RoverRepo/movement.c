#include "movement.h"
#include "comms.h"
#include <stdint.h>

#define DEADZONE 5  // Applied to centered range of -500 to +500

void movement_parse_502(void) {
    uint16_t Raw_X_Axis = (comms_payload[1] << 8) | comms_payload[0];  // Forward/Reverse
    uint16_t Raw_Y_Axis = (comms_payload[3] << 8) | comms_payload[2];  // Steering

    int16_t X_Centered = Raw_X_Axis - 1500;  // Center at 0, range -500 to +500
    int16_t Y_Centered = Raw_Y_Axis - 1500;

    if (X_Centered > -DEADZONE && X_Centered < DEADZONE) X_Centered = 0;
    if (Y_Centered > -DEADZONE && Y_Centered < DEADZONE) Y_Centered = 0;

    int16_t throttle = X_Centered / 5;  // Scale to -100 to +100
    int16_t steering = Y_Centered / 5;

    int16_t left  = throttle + steering;  // Skid steering mix
    int16_t right = throttle - steering;

    if (left  >  100) left  =  100;
    if (left  < -100) left  = -100;
    if (right >  100) right =  100;
    if (right < -100) right = -100;

    uint8_t dirA, speedA, dirB, speedB;

    if      (left  == 0) { dirA = 0; speedA = 0; }
    else if (left  >  0) { dirA = 1; speedA = (uint8_t)left; }
    else                 { dirA = 2; speedA = (uint8_t)(-left); }

    if      (right == 0) { dirB = 0; speedB = 0; }
    else if (right >  0) { dirB = 1; speedB = (uint8_t)right; }
    else                 { dirB = 2; speedB = (uint8_t)(-right); }

    comms_send_move(dirA, speedA, dirB, speedB);
}
