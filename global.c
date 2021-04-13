#include "global.h"

#include "mcc_generated_files/device_config.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/tmr3.h"

/**
 * Things to do when TMR1 rolls over. TBD...
 */
void tmr1_timeout_callback(void){
    preamble_rcvd = false;
    frame_sync_rcvd = false;
}

/**
 * 
 * @param bits array containing the individual bits, each bit occupying a single byte
 * @return first 8 bits of bits packaged into a single uint8_t
 */
uint8_t rx_package_bits(uint8_t* bits){
    uint8_t retval = 0;
    for(int8_t i = 0; i < 8; i++){
        if(bits[i]){
            retval |= bits[i] << (7 - i);
        }
    }
    return retval;
}


/**
 * to be called once 8-bits have been received.
 * stuffs the bits into a uint8_t and stores in rx_bytes at rx_byte_ind
 * increments rx_byte_ind
 */
void rx_byte_rcvd(){
    for(uint8_t i = 0; i < 8; i++){
        if(rx_bits[i]){
            rx_bytes[rx_byte_ind] |= rx_bits[i] << (7 - i);
        }
    }
    rx_byte_ind++;
}

/**
 * to be called once a 16-bit word has been received.
 * stuffs the bits into a uint16_t
 * increments the rx_word_ind counter and resets the rx_bit_ind counter
 */
/*void rx_word_rcvd(){
    rx_words[rx_word_ind] = 0;
    for(uint8_t i = 0; i < 16; i++){
        if(rx_bits[i]){
            rx_words[rx_word_ind] |= rx_bits[i] << (15 - i);
        }
    }
    rx_word_ind++;
    rx_bit_ind = 0;
}*/

/**
 * To be called once we've received a whole packet.
 */
void reset_rx_state_machine(){
    rx_bit_ind = 0;
    rx_word_ind = 0;
    preamble_rcvd = false;
    TMR1_StopTimer();
    TMR1_WriteTimer(0); // reset timer
    rx_state = RX_IDLE;
}