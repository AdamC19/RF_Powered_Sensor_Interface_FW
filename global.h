#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <xc.h>
#include "mcc_generated_files/device_config.h"
#include <stdint.h>
#include <stdbool.h>

#define MODE_CHARGING       0
#define MODE_RECEIVING      1
#define MODE_SAMPLING       2
#define MODE_TRANSMITTING   3

#define FALLING     0
#define RISING      1


/* Macros */
#define SET_RX_BITLINE      PORTCbits.RC3 = 1
#define RESET_RX_BITLINE    PORTCbits.RC3 = 0
#define ENABLE_I_SRC        PORTCbits.RC5 = 1
#define DISABLE_I_SRC       PORTCbits.RC5 = 0
#define ENABLE_I_SINK       PORTCbits.RC6 = 1
#define DISABLE_I_SINK      PORTCbits.RC6 = 0
#define DISABLE_I2C         (PMD4 |= _PMD4_MSSP1MD_MASK)
#define ENABLE_I2C          (PMD4 &= ~_PMD4_MSSP1MD_MASK)
#define DISABLE_UART        (PMD4 |= _PMD4_UART1MD_MASK)
#define ENABLE_UART         (PMD4 &= ~_PMD4_UART1MD_MASK)

typedef enum rx_states_enum 
{       RX_IDLE,
        RX_WAIT_DELIM,
        RX_WAIT_DATA0,
        RX_GET_RTCAL,
        RX_FRAME_SYNC_RCVD,
        RX_GET_DATA
}RxStates_t;

extern volatile uint8_t edge;
extern volatile RxStates_t rx_state;
extern volatile uint8_t rt_cal;
extern volatile uint8_t pivot;
extern volatile uint8_t tr_cal;
extern volatile uint8_t edges;
extern volatile uint16_t falling_edge;
extern volatile uint16_t rising_edge;
extern volatile uint8_t rx_bit_ind;
extern volatile uint8_t rx_bits[];
extern volatile uint8_t rx_word_ind;
extern volatile uint16_t rx_words[];
extern volatile bool preamble_rcvd;
extern volatile bool frame_sync_rcvd; // may not be needed
extern volatile bool word_rcvd;
extern uint8_t* tag_mem;

void rx_word_rcvd();
void reset_rx_state_machine();

#endif // _GLOBAL_H_