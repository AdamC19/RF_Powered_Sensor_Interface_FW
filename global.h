#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <xc.h>
#include "device_config.h"
#include <stdint.h>
#include <stdbool.h>

#define SET_RX_BITLINE      PORTCbits.RC3 = 1
#define RESET_RX_BITLINE    PORTCbits.RC3 = 0

#define MODE_CHARGING       0
#define MODE_RECEIVING      1
#define MODE_SAMPLING       2
#define MODE_TRANSMITTING   3

extern uint8_t edges;
extern uint16_t falling_edge;
extern uint16_t rising_edge;
extern uint8_t rx_bit_ind;
extern uint8_t* rx_pulse_widths;
extern bool preamble_rcvd;
extern bool frame_sync_rcvd;

#endif // _GLOBAL_H_