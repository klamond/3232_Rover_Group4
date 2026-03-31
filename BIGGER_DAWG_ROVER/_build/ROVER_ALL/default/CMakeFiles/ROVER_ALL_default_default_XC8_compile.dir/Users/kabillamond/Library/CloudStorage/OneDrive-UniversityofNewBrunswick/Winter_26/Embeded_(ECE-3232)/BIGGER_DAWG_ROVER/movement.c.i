# 1 "/Users/kabillamond/Library/CloudStorage/OneDrive-UniversityofNewBrunswick/Winter 26/Embeded (ECE-3232)/BIGGER_DAWG_ROVER/movement.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 295 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/Applications/microchip/xc8/v3.10/pic/include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "/Users/kabillamond/Library/CloudStorage/OneDrive-UniversityofNewBrunswick/Winter 26/Embeded (ECE-3232)/BIGGER_DAWG_ROVER/movement.c" 2
# 1 "/Users/kabillamond/Library/CloudStorage/OneDrive-UniversityofNewBrunswick/Winter 26/Embeded (ECE-3232)/BIGGER_DAWG_ROVER/movement.h" 1




void movement_parse_502(void);
# 2 "/Users/kabillamond/Library/CloudStorage/OneDrive-UniversityofNewBrunswick/Winter 26/Embeded (ECE-3232)/BIGGER_DAWG_ROVER/movement.c" 2
# 1 "/Users/kabillamond/Library/CloudStorage/OneDrive-UniversityofNewBrunswick/Winter 26/Embeded (ECE-3232)/BIGGER_DAWG_ROVER/comms.h" 1



# 1 "/Applications/microchip/xc8/v3.10/pic/include/c99/stdint.h" 1 3



# 1 "/Applications/microchip/xc8/v3.10/pic/include/c99/musl_xc8.h" 1 3
# 5 "/Applications/microchip/xc8/v3.10/pic/include/c99/stdint.h" 2 3
# 26 "/Applications/microchip/xc8/v3.10/pic/include/c99/stdint.h" 3
# 1 "/Applications/microchip/xc8/v3.10/pic/include/c99/bits/alltypes.h" 1 3
# 133 "/Applications/microchip/xc8/v3.10/pic/include/c99/bits/alltypes.h" 3
typedef unsigned short uintptr_t;
# 148 "/Applications/microchip/xc8/v3.10/pic/include/c99/bits/alltypes.h" 3
typedef short intptr_t;
# 164 "/Applications/microchip/xc8/v3.10/pic/include/c99/bits/alltypes.h" 3
typedef signed char int8_t;




typedef short int16_t;




typedef __int24 int24_t;




typedef long int32_t;





typedef long long int64_t;
# 194 "/Applications/microchip/xc8/v3.10/pic/include/c99/bits/alltypes.h" 3
typedef long long intmax_t;





typedef unsigned char uint8_t;




typedef unsigned short uint16_t;




typedef __uint24 uint24_t;




typedef unsigned long uint32_t;





typedef unsigned long long uint64_t;
# 235 "/Applications/microchip/xc8/v3.10/pic/include/c99/bits/alltypes.h" 3
typedef unsigned long long uintmax_t;
# 27 "/Applications/microchip/xc8/v3.10/pic/include/c99/stdint.h" 2 3

typedef int8_t int_fast8_t;

typedef int64_t int_fast64_t;


typedef int8_t int_least8_t;
typedef int16_t int_least16_t;

typedef int24_t int_least24_t;
typedef int24_t int_fast24_t;

typedef int32_t int_least32_t;

typedef int64_t int_least64_t;


typedef uint8_t uint_fast8_t;

typedef uint64_t uint_fast64_t;


typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;

typedef uint24_t uint_least24_t;
typedef uint24_t uint_fast24_t;

typedef uint32_t uint_least32_t;

typedef uint64_t uint_least64_t;
# 148 "/Applications/microchip/xc8/v3.10/pic/include/c99/stdint.h" 3
# 1 "/Applications/microchip/xc8/v3.10/pic/include/c99/bits/stdint.h" 1 3
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
# 149 "/Applications/microchip/xc8/v3.10/pic/include/c99/stdint.h" 2 3
# 5 "/Users/kabillamond/Library/CloudStorage/OneDrive-UniversityofNewBrunswick/Winter 26/Embeded (ECE-3232)/BIGGER_DAWG_ROVER/comms.h" 2


extern uint8_t comms_payload[21];


extern volatile uint8_t comms_new_402;
extern volatile uint8_t comms_new_502;

void comms_init(void);
void comms_send_get_info(void);
void comms_send_get_data(void);
void comms_send_move(uint8_t dirA, uint8_t speedA, uint8_t dirB, uint8_t speedB);
void comms_send_color_info(uint8_t color);
void comms_send_mag_info(uint8_t pyramid_num);
# 3 "/Users/kabillamond/Library/CloudStorage/OneDrive-UniversityofNewBrunswick/Winter 26/Embeded (ECE-3232)/BIGGER_DAWG_ROVER/movement.c" 2




void movement_parse_502(void) {
    uint16_t Raw_X_Axis = (comms_payload[1] << 8) | comms_payload[0];
    uint16_t Raw_Y_Axis = (comms_payload[3] << 8) | comms_payload[2];

    int16_t X_Centered = Raw_X_Axis - 1500;
    int16_t Y_Centered = Raw_Y_Axis - 1500;

    if (X_Centered > -5 && X_Centered < 5) X_Centered = 0;
    if (Y_Centered > -5 && Y_Centered < 5) Y_Centered = 0;

    int16_t throttle = X_Centered / 5;
    int16_t steering = Y_Centered / 5;

    int16_t left = throttle + steering;
    int16_t right = throttle - steering;

    if (left > 100) left = 100;
    if (left < -100) left = -100;
    if (right > 100) right = 100;
    if (right < -100) right = -100;

    uint8_t dirA, speedA, dirB, speedB;

    if (left == 0) { dirA = 0; speedA = 0; }
    else if (left > 0) { dirA = 1; speedA = (uint8_t)left; }
    else { dirA = 2; speedA = (uint8_t)(-left); }

    if (right == 0) { dirB = 0; speedB = 0; }
    else if (right > 0) { dirB = 1; speedB = (uint8_t)right; }
    else { dirB = 2; speedB = (uint8_t)(-right); }

    comms_send_move(dirA, speedA, dirB, speedB);
}
