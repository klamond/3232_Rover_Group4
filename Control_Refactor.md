Task:
    Magnetic:
        SW1 = 2000 && SW3 = 1500 enables sensor
        SW1 = 2000 && SW3 = 2000 transmits selection
    Line: (not yet incorporated, can ignore for now)
        SW1 = 2000 && SW2 = 1000 enables line follow mode
    Optical: (also not incorporated, can also ignore for now)
        SW1 = 2000 && SW4 = 1000 Enables optical signal decode mode
    Solar: 
        SW1 = 1000 && Sw2 = 1000 turns on LED
    Ore Process:
        SW1 = 1000 && SW4 = 1000 Transmits selection

Byte Map (zero indexed):
    SW1 is bytes 8 and 9 of comms_payload
    SW2 is bytes 10 and 11 of comms_payload
    SW3 is bytes 12 and 13 of comms_payload
    SW1 is bytes 14 and 15 of comms_payload


