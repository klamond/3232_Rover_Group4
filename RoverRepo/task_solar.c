#include "task_solar.h"
#include "comms.h"
#include <xc.h>
#include <stdint.h>

void solar_parse_502(void) {
    uint16_t Raw_SW_A = (comms_payload[9] << 8) | comms_payload[8];
    LATAbits.LATA0 = (Raw_SW_A == 1000) ? 1 : 0;
}
