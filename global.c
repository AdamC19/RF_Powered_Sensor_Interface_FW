#include "global.h"

#include "mcc_generated_files/device_config.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/tmr0.h"

/**
 * to be called once a 16-bit word has been received.
 * stuffs the bits into a uint16_t
 * increments the rx_word_ind counter and resets the rx_bit_ind counter
 */
void rx_word_rcvd(){
    rx_words[rx_word_ind] = 0;
    for(uint8_t i = 0; i < 16; i++){
        if(rx_bits[i]){
            rx_words[rx_word_ind] |= rx_bits[i] << (15 - i);
        }
    }
    rx_word_ind++;
    rx_bit_ind = 0;
}

/**
 * To be called once we've received a whole packet.
 */
void reset_rx_state_machine(){
    rx_bit_ind = 0;
    rx_word_ind = 0;
    preamble_rcvd = false;
    TMR0_StopTimer();
    TMR0_WriteTimer(0); // reset timer
    rx_state = RX_IDLE;
}