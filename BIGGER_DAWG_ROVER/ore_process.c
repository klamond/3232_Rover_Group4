#include "ore_process.h"
#include "comms.h"

void ore_process_parse_502(void) {
    uint16_t flag = (comms_payload[15] << 8) | comms_payload[14];

    if (flag == 1000) {
        return;  // No ore detected
    }

    if (flag == 2000) {
        uint16_t color_raw = (comms_payload[19] << 8) | comms_payload[18];
        uint8_t color;

        if (color_raw <= 1333) {
            color = 1;  // Red
        } else if (color_raw <= 1666) {
            color = 2;  // Yellow
        } else {
            color = 3;  // Blue
        }

        comms_send_color_info(color);
    }
}
