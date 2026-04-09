# 1 "/Users/kabillamond/Library/CloudStorage/OneDrive-UniversityofNewBrunswick/Winter 26/Embeded (ECE-3232)/BIGGER_DAWG_ROVER/ore_process.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 295 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/Applications/microchip/xc8/v3.10/pic/include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "/Users/kabillamond/Library/CloudStorage/OneDrive-UniversityofNewBrunswick/Winter 26/Embeded (ECE-3232)/BIGGER_DAWG_ROVER/ore_process.c" 2
# 1 "/Users/kabillamond/Library/CloudStorage/OneDrive-UniversityofNewBrunswick/Winter 26/Embeded (ECE-3232)/BIGGER_DAWG_ROVER/ore_process.h" 1



void ore_process_parse_502(void);
# 2 "/Users/kabillamond/Library/CloudStorage/OneDrive-UniversityofNewBrunswick/Winter 26/Embeded (ECE-3232)/BIGGER_DAWG_ROVER/ore_process.c" 2
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
# 3 "/Users/kabillamond/Library/CloudStorage/OneDrive-UniversityofNewBrunswick/Winter 26/Embeded (ECE-3232)/BIGGER_DAWG_ROVER/ore_process.c" 2

void ore_process_parse_502(void) {
    uint16_t SW1 = ((uint16_t)comms_payload[9] << 8) | comms_payload[8];
    uint16_t SW4 = ((uint16_t)comms_payload[15] << 8) | comms_payload[14];

    if (SW1 != 2000 || SW4 != 2000) {
        return;
    }

    uint16_t color_raw = ((uint16_t)comms_payload[19] << 8) | comms_payload[18];
    uint8_t color;

    if (color_raw <= 1333) {
        color = 1;
    } else if (color_raw <= 1666) {
        color = 2;
    } else {
        color = 3;
    }

    comms_send_color_info(color);
}
