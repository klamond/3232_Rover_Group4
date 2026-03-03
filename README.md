# Rover Repo

Embedded C firmware for the Harvest Rover, targeting a Microchip PIC18 microcontroller compiled with XC8. The project is built and managed through MPLAB X (NetBeans-generated Makefile, CMake support).

---

## Project Structure

```
RoverRepo/
├── main.c          # Entry point, hardware init, main loop
├── comms.c/.h      # UART serial protocol (receive/transmit)
├── movement.c/.h   # Joystick-to-motor skid-steering control
├── task_solar.c/.h # Solar panel MOSFET control
└── newxc8_header.h # Microchip XC8 template header (boilerplate)
```

---

## Adding a New Task

Each piece of functionality that runs in the main loop is a **task** — a self-contained module consisting of two files:

- `taskname.c` — implementation
- `taskname.h` — public interface (header guard, function declarations, extern data)

Follow these steps to add a new task.

### 1. Create `taskname.h`

Use header guards. Declare only what other modules need to call.

```c
#ifndef TASKNAME_H
#define TASKNAME_H

// Declare any functions called from main.c or other modules
void taskname_parse_502(void);

#endif
```

Mirror the pattern used by existing tasks:
- [movement.h](movement.h) — single parse function, no exposed data
- [task_solar.h](task_solar.h) — single parse function, no exposed data

### 2. Create `taskname.c`

Include your own header and any dependencies (`comms.h` for payload access, `<xc.h>` for GPIO/registers).

```c
#include "taskname.h"
#include "comms.h"
#include <xc.h>
#include <stdint.h>

void taskname_parse_502(void) {
    // Read from comms_payload[] and act on the data
}
```

The shared `comms_payload[21]` array (declared `extern` in [comms.h](comms.h)) holds the raw bytes of the most recently received 0x0502 message. Use it to extract sensor or control values.

### 3. Add `#include` to `main.c`

Open [main.c](main.c) and add your header alongside the existing includes:

```c
#include "comms.h"
#include "movement.h"
#include "task_solar.h"
#include "taskname.h"   // <-- add this line
```

### 4. Call your function from the main loop

Inside the `if (comms_new_502)` block in `main()`, call your parse function:

```c
if (comms_new_502) {
    comms_new_502 = 0;
    movement_parse_502();
    solar_parse_502();
    taskname_parse_502();   // <-- add this line
}
```

The flag `comms_new_502` is set by the UART ISR when a complete 0x0502 message arrives. Clear it once (already done with `comms_new_502 = 0`) and then each task reads from the shared `comms_payload[]` buffer.

---

## Payload Layout (`comms_payload[]`)

The 0x0502 message carries up to 21 bytes. Known byte assignments:

| Bytes | Content | Used by |
|-------|---------|---------|
| [0–1] | X-axis joystick (little-endian, center = 1500) | `movement.c` |
| [2–3] | Y-axis joystick (little-endian, center = 1500) | `movement.c` |
| [8–9] | Switch A value (little-endian) | `task_solar.c` |

Document any new bytes your task consumes in this table.

---

## Communications Protocol

Messages are framed as:

```
0xFE  0x19  ID_LSB  ID_MSB  LEN_LSB  LEN_MSB  [payload bytes...]
```

| Field | Size | Notes |
|-------|------|-------|
| Sync | 2 bytes | Always `0xFE 0x19` |
| Message ID | 2 bytes | Little-endian. `0x0402` = info reply, `0x0502` = data reply |
| Payload length | 2 bytes | Little-endian, 0–21 |
| Payload | 0–21 bytes | Stored in `comms_payload[]` |

The main loop calls `comms_send_get_data()` every 100 ms to request a fresh 0x0502 frame.

---

## Coding Conventions

- **File naming:** task files are prefixed `task_` (e.g., `task_solar.c`). Non-task modules use a plain name (e.g., `movement.c`, `comms.c`).
- **Function naming:** `modulename_action()` — e.g., `solar_parse_502()`, `comms_send_move()`.
- **No dynamic allocation:** use fixed-size globals or stack variables only.
- **ISR safety:** `comms_payload[]` is written by the ISR. Read it only after the `comms_new_502` flag is set and before clearing it or calling any function that might re-enable further writes.
- **GPIO:** pin assignments and directions are set in `gpio_init()` in [main.c](main.c). Add any new pins there.
