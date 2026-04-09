#ifndef ORE_STOCKPILE_H
#define ORE_STOCKPILE_H

// Configure servo GPIO — call once during startup
void stockpile_init(void);

// Call after a new 0x0502 message arrives to update flap servo position
void stockpile_parse_502(void);

#endif
