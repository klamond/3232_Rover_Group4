#ifndef MOVEMENT_H
#define MOVEMENT_H

// Call after a new 0x0502 message arrives to parse joystick and drive motors
void movement_parse_502(void);

#endif
