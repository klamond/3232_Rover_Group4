#ifndef TASK_LINEFOLLOW_H
#define TASK_LINEFOLLOW_H

// Optional setup hook if called from main after a new 0x0502 message arrives.
// Does not decide mode anymore; it only ensures line-follow hardware is initialized.
void task_linefollow_parse_502(void);

// Run one line-follow sensor read + motor update directly.
void task_linefollow_run(void);

#endif
