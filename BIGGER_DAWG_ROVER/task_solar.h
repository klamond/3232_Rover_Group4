#ifndef TASK_SOLAR_H
#define TASK_SOLAR_H

// Call after a new 0x0502 message arrives to update solar panel state
void solar_parse_502(void);

#endif
