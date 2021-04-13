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

/* max number of bits we can receive */
#define RX_BIT_DEPTH        64

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


void tmr1_timeout_callback(void);
uint8_t rx_package_bits(uint8_t* bits);
void rx_byte_rcvd();
//void rx_word_rcvd();
void reset_rx_state_machine();

#endif // _GLOBAL_H_