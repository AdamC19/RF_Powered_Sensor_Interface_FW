#include "tag.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * Section: Global variable definitions
 */

volatile TagSession_t sessions[4]; // array of sessions (b00 thru b11) 

/* tag meta-state and such */
volatile TagState_t tag_state;

/* Flags and stuff common to all sessions */
volatile uint8_t selected = false;
volatile uint8_t c_flag = 0;
volatile uint16_t slot_counter = 0;
volatile bool truncate = false;
volatile uint16_t rcvd_rn16 = 0; // any access command will send along a RN16, store it in this
volatile uint16_t tag_handle = 0;

/* Mask set by a Select command */
volatile uint8_t mask_bytes[MASK_SIZE_BYTES];

/* globals for stuff set by Query command */
volatile uint8_t tr_cal_divide_ratio = 8; // default to 8, may change based on 
volatile uint8_t blf_period; // backscatter link frequency, but it's the period in number of clock cycles
volatile uint8_t cycles_per_symbol = 1; // will be set by a query command
volatile bool use_pilot_tone = false; // will be set by query command
volatile uint8_t select = 0; // select which tags respond to this query
volatile uint8_t session_number = 0; // chooses a session for the inventory round
volatile TagSession_t* current_session; // pointer to the session for the current inventory round
volatile uint8_t target = 0;
volatile uint8_t slots_q = 0;
volatile uint8_t slots_in_round = 0;

volatile uint16_t rand16 = RNG_SEED;

/* for low level reception operations */
volatile RxStates_t rx_state = RX_IDLE;
volatile uint8_t rt_cal = 0;
volatile uint8_t pivot = 0;
volatile uint8_t tr_cal = 0;
volatile uint8_t rx_bytes[RX_BUFFER_SIZE];
volatile int8_t rx_bit_ind = 7; // to shift first bit to the MSB position
volatile uint8_t rx_bits[16];
volatile bool preamble_rcvd = false;
volatile bool frame_sync_rcvd = false;
volatile uint8_t rcvd_cmd = CMD_NONE;

/* variables for low-level transmission */
volatile bool backscatter_done = false; // indicates that data has been sent

/* tag memory banks */
volatile uint8_t epc_mem_bank[EPC_MEM_BANK_SIZE];
volatile uint8_t tid_mem_bank[TID_MEM_BANK_SIZE];
volatile uint8_t user_mem_bank[USER_MEM_BANK_SIZE];


void init_sessions(){
    sessions[0].session_id = 0;
    sessions[0].inventoried = 0;
    
    sessions[1].session_id = 1;
    sessions[1].inventoried = 0;
    
    sessions[2].session_id = 2;
    sessions[2].inventoried = 0;
    
    sessions[3].session_id = 3;
    sessions[3].inventoried = 0;
}

/**
 * Allocate memory for this tag's logical interface
 * 
 * @return pointer to the allocated memory
 */
void init_tag_mem(uint8_t* tag_mem){
    memset(tag_mem, 0, TAG_MEM_SIZE);
}


void init_tag_data(TagData_t* tag_data){
    tag_data->killed = false;
    tag_data->state = TAG_STATE_READY;
}


void tag_mem_write_bit(uint8_t* mem, uint8_t addr, uint8_t value){
    if(value)
        mem[addr >> 3] |= 0x80 >> ((addr & 0x03));
    else
        mem[addr >> 3] &= ~(0x80 >> ((addr & 0x03)));
}


uint8_t tag_mem_read_bit(uint8_t* mem, uint8_t addr){
    return ( mem[addr >> 3] >> (7 - (addr & 0x03)) ) & 1;
}


void tag_mem_write_byte(uint8_t* mem, uint8_t addr, uint8_t value){
    mem[addr >> 3] = value;
}


uint8_t tag_mem_read_byte(uint8_t* mem, uint8_t addr){
    return mem[addr >> 3];
}

void tag_mem_write16(uint8_t* mem, uint8_t addr, uint16_t value){
    uint8_t ind = addr >> 3;
    mem[ind] = (uint8_t)((value >> 8) & 0xFF);
    mem[ind + 1] = (uint8_t)(value & 0xFF);
}

uint16_t tag_mem_read16(uint8_t* mem, uint8_t addr){
    uint8_t ind = addr >> 3;
    return (mem[ind] << 8) | mem[ind + 1];
}

/**
 * Function to very crudely generate a pseudo-random number <= a max value
 * 
 * @param max maximum value of the random number (inclusive)
 * @param seed some initial value to start with
 * @return a very bad pseudo-random number
 */
uint16_t rn16(uint16_t max){
    // advance random number generator
    rand = (RNG_MULTIPLIER * rand + RNG_INCREMENT) % RNG_MODULUS;
    
    uint16_t rand16_mask = 0xFFFF;
    uint16_t rand16 = rand & rand16_mask;
    
    // chop off high order bits until we're less than or equal to max
    while(rand16 > max){
        rand16_mask = rand16_mask >> 1;
        rand16 = rand & rand16_mask;
    }
    return rand16;
}


/**
 * Compute the CRC-5 of the given number of bits using the given bytes
 * 
 * @param data the bytes containing the bits to iterate over
 * @param bits number of bits to run through the CRC-5 circuit
 * @return the CRC-5
 */
uint8_t crc5(uint8_t* data, uint8_t bits){
    uint8_t dat;
    uint8_t q0 = CRC5_PRESET & 1;
    uint8_t q1 = (CRC5_PRESET >> 1) & 1;
    uint8_t q2 = (CRC5_PRESET >> 2) & 1;
    uint8_t q3 = (CRC5_PRESET >> 3) & 1;
    uint8_t q4 = (CRC5_PRESET >> 4) & 1;
    uint8_t xor_a, xor_b;
    
    uint8_t data_ind = 0;
    int8_t bit_ind = 7;
    for(uint8_t i = 0; i < bits; i++){
        dat = (data[data_ind] >> bit_ind) & 1;
        
        // compute combinational state
        xor_a = q4 ^ dat;
        xor_b = q2 ^ xor_a;
        
        // pulse the clock
        q4 = q3;
        q3 = xor_b;
        q2 = q1;
        q1 = q0;
        q0 = xor_a;
        
        bit_ind--;
        if(bit_ind < 0){
            data_ind++;
            bit_ind = 7;
        }
    }
    return ( q0 | (q1 << 1) | (q2 << 2) | (q3 << 3) | (q4 << 4) );
}


uint16_t crc16(uint8_t* data, uint8_t bits){
    
    uint16_t q = CRC16_PRESET;
    
    uint8_t xor_a, xor_b, xor_c;
    
    uint8_t dat;
    uint8_t data_ind = 0;
    int8_t bit_ind = 7;
    for(uint8_t i = 0; i < bits; i++){
        dat = (data[data_ind] >> bit_ind) & 1;
        
        // compute combinational state
        xor_a = ((q >> 15) & 1) ^ dat;
        xor_b = xor_a ^ ((q >> 4) & 1);
        xor_c = xor_a ^ ((q >> 11) & 1);
        
        // pulse clock
        // first shift everything 
        q = q << 1;
        
        // xor_a -> q0
        if(xor_a)
            q |= 1;
        
        // xor_b -> q5
        if(xor_b)
            q |= (1 << 5);
        else
            q &= ~(1 << 5);
        
        // xor_c -> q12
        if(xor_c)
            q |= (1 << 12);
        else
            q &= ~(1 << 12);
    }
    
    return q;
}

/**
 * Extract len many bits from a bit vector, stored in bytes, starting at bit address bit_addr
 * @param bytes array of bytes containing the bits of interest
 * @param bit_addr start of bits of interest, first bit has address 0
 * @param len number of bits to extract
 * @return single byte containing the bits extracted, right-aligned
 */
uint8_t get_bits(uint8_t* bytes, uint8_t bit_addr, uint8_t len){
    if(len > 8){
        return 0;
    }
    uint8_t retval = 0;
    
    uint8_t byte_ind, bit_ind;
    uint8_t end_addr = bit_addr + len; // non-inclusive
    uint8_t left_shift = len; // will be used for shifting the bits into place before ORing them w/ retval
    while(bit_addr < end_addr){
        byte_ind = bit_addr >> 3; // divide by 8
        bit_ind = 7 - (bit_addr % 8); // figure out the right bit shift (lowest bit address is MSB)
        bit_addr++; // 
        left_shift--; // decrement (MSB ends up in len-1 position)
        retval |= ((bytes[byte_ind] >> bit_ind) & 1) << left_shift;
    }
    return retval;
}

/**
 * Get binary value of a number stored as an Extensible Bit Vector
 * @param bytes array of bytes containing the bit vector
 * @param bit_addr the start address of the bit vector
 * @param size pointer to uint8_t which will be incremented by number of bits in this EBV
 * @return the value of the bit vector in standard 2's complement
 */
uint32_t get_ebv8(uint8_t* bytes, uint8_t bit_addr, uint8_t* size){
    uint32_t retval = 0;
    uint8_t block = 0;
    
    do{
        retval <<= 7; // shift data left to make room for new data
        block = get_bits(bytes, bit_addr, 8); // get the block at current bit address
        retval |= block & 0x7F; // OR in the new data (exclude the extension bit)
        bit_addr += 8; // bit address will now point to next block for the next iteration
        *size += 8; // increment this too
    }while((block >> 7) & 1); // continue to next block iff extension bit is 1
    
    return retval;
}

/**
 * Ensures that slot_counter is correctly decremented 
 * (mainly that it wraps around to 0x7FFF after going down from 0)
 */
void decrement_slot_counter(){
    if(slot_counter == 0 || slot_counter > 0x7FFF){
        slot_counter = 0x7FFF;
    }else{
        slot_counter -= 1;
    }
}

/**
 * processes bits from rx_bytes as required for a Query command
 */
void handle_query_cmd(){
    if(crc5(rx_bytes, LEN_QUERY) != 0)
        return; // bad CRC, do not pass go etc.
    
    rcvd_cmd = CMD_QUERY;
    // ==== HANDLE QUERY COMMAND ====
    // determine BLF period
    if((rx_bytes[0] >> 3) & 1){
        // DR = 64/3
        blf_period = (tr_cal * 3) / 64;
    }else{
        // DR = 8
        blf_period = tr_cal / 8;
    }

    // cycles per symbol (M)
    switch((rx_bytes[0] >> 1) & 3){
        case 0:
            cycles_per_symbol = 1;
            break;
        case 1:
            cycles_per_symbol = 2;
            break;
        case 2:
            cycles_per_symbol = 4;
            break;
        case 3:
            cycles_per_symbol = 8;
            break;
        default:
            cycles_per_symbol = 1;
            break;
    }

    // pilot tone
    use_pilot_tone = (bool)(rx_bytes[0] & 1);

    // Sel: which tags respond to this Query
    select = (rx_bytes[1] >> 6) & 3;

    // Session: chooses a session for this inventory round
    session_number = (rx_bytes[1] >> 4) & 3;
    current_session = &(sessions[session_number]);

    // Target (0=A, 1=B)
    target = (rx_bytes[1] >> 3) & 1;

    // Q = [0,15], number of slots in the round = 2^Q - 1
    slots_q = ((rx_bytes[1] & 0x07) << 1) | ((rx_bytes[2] >> 7) & 1);
    slots_in_round = (1 << slots_q) - 1;
    slot_counter = rn16(slots_in_round);
    
}

/**
 * adjusts slots_in_round based on the data in a QueryAdjust command
 * session bits must match the current session number
 */
void handle_query_adjust_cmd(){
    rcvd_cmd = CMD_QUERY_ADJUST;
    if(((rx_bytes[0] >> 4) & 3) == current_session->session){
        // change Q (slots_in_round) based on last 3 bits
        uint8_t up_dn = ((rx_bytes[0] & 3) << 1) | ((rx_bytes[1] >> 7) & 1);
        if(up_dn == 6){
            slots_in_round += 1;
        }else if(up_dn == 3){
            slots_in_round -= 1;
        }
    }
}

/**
 * decrement slot counter
 * if session matches the session number set by last Query command 
 * and slot_counter == 0 after decrement: 
 * then reply with a 16-bit random number
 */
void handle_query_rep_cmd(){
    rcvd_cmd = CMD_QUERY_REP;
    if(((rx_bytes[0] >> 4) & 3) == current_session->session){
        decrement_slot_counter();
    }
    // if...
    //   we've received a prior Query
    //   slot_counter == 0
    // then REPLY
}


void handle_select_cmd(){
    uint8_t bit_addr = 4; // start right after the command bits
    uint8_t targ = get_bits(rx_bytes, bit_addr, 3); // to indicate whether this command modifies the selected or inventoried flag
    bit_addr += 3;
    
    // most basic check, could save us some computation
    if(targ >= 5){
        // ignore this Select command
        return;
    }
    
    uint8_t action = get_bits(rx_bytes, bit_addr, 3); // will be used in various ways later
    bit_addr += 3;
    
    uint8_t membank = get_bits(rx_bytes, 10, 2); // which memory bank is being selected
    bit_addr += 2;
    
    uint32_t pointer = get_ebv8(rx_bytes, 12, &bit_addr); // starting bit address for mask comparison (EBV formatting and bit addressing)
    // bit_addr is incremented as needed by the get_ebv8() function
    
    uint8_t mask_len = get_bits(rx_bytes, bit_addr, 8); // number of bits in this mask
    bit_addr += 8;
    
    // === figure out what the Mask is ===
    // if Mask matches the bits specified by Pointer and Length, then tag is matching
    uint8_t mask_size = mask_len >> 3; // number of bytes the mask will cover (except the last few bits that don't make up a whole byte)
    uint8_t i;
    for(i = 0; i < mask_size; i ++){
        mask_bytes[i] = get_bits(rx_bytes, bit_addr, 8);
        bit_addr += 8;
    }
    uint8_t mask_bits_left = mask_size % 8;
    mask_bytes[i + 1] = get_bits(rx_bytes, bit_addr, mask_bits_left);
    bit_addr += mask_bits_left;
    // === end Mask figuring ===
    
    uint8_t trunc = get_bits(rx_bytes, bit_addr, 1); // if truncate, truncate reply to those EPC bits that follow Mask
    bit_addr += 1;
    
    // another basic check to see if we can ignore this command
    if(trunc && (targ != 4 || membank != 1) ){
        return;
    }
    
    uint8_t total_len = bit_addr + 16; // had to read thru the mask EBV anyway to know the length
    
    // === CRC-16 CHECK ===
    if(crc16(rx_bytes, total_len) != CRC16_OK){
        // ERROR: bad CRC-16
        return;
    }
    // === CRC-16 Passed ===
    
    // all checks passed, command presumed good, we can now set globals flags and things
    rcvd_cmd = CMD_SELECT;
    
    truncate = trunc; //  set global
    
    uint8_t* bank; // pointer to whichever memory bank is selected
    switch(membank){
        case 0:{
            // TODO: search for at least 1 file whose FileType matches Mask
            break;
        }case 1:{
            bank = epc_mem_bank;
            break;
        }case 2:{
            bank = tid_mem_bank;
            break;
        }case 3:{
            bank = user_mem_bank;
            break;
        }default:{
            // hmm, something weird happened
            return;
        }
    }
    
    bool is_matching = check_mask_match(bank, pointer, mask_len); // do the comparison
    
    if(is_matching){ // === BEGIN TAG MATCHING ===
        
        switch(action){
            case 0:
            case 1:
            {
                if(targ == 4){
                    selected = true; // assert selected flag
                }else{
                    // targ corresponds to session S0 thru S3, and we should set inventoried to A
                    sessions[targ].inventoried = INVENTORIED_A;
                }
                break;
            }case 3:{
                if(targ == 4){
                    selected = !selected; // toggle selected flag
                }else{
                    // targ corresponds to session S0 thru S3
                    // and we should switch inventoried flag (A->B or B->A)
                    if(sessions[targ].inventoried == INVENTORIED_A)
                        sessions[targ].inventoried = INVENTORIED_B;
                    else
                        sessions[targ].inventoried = INVENTORIED_A;
                }
                break;
            }
            case 4:
            case 5:{
                if(targ == 4){
                    selected = false; // de-assert selected flag
                }else{
                    // targ corresponds to session S0 thru S3, and we should set inventoried to B
                    sessions[targ].inventoried = INVENTORIED_B;
                }
                break;
            }
            case 2:
            case 6:
            case 7:
            default:{
                // do nothing
                return;
            }
        }
    } // === END TAG MATCHING ===
    else { // *** BEGIN TAG NOT-MATCHING ***
        switch(action){
            case 0:
            case 2:{
                if(targ == 4){
                    selected = false; // de-assert selected flag
                }else{
                    // targ corresponds to session S0 thru S3
                    // and we should set that session's inventoried flag to B
                    sessions[targ].inventoried = INVENTORIED_B;
                }
                break;
            }
            case 4:
            case 6:{
                if(targ == 4){
                    selected = true; // assert selected flag
                }else{
                    // targ corresponds to a session, and we should set inventoried to A
                    sessions[targ].inventoried = INVENTORIED_A;
                }
                break;
            }case 7:{
                if(targ == 4){
                    selected = !selected; // toggle selected flag
                }else{
                    // targ corresponds to session S0 thru S3
                    // and we should switch inventoried flag (A->B or B->A)
                    if(sessions[targ].inventoried == INVENTORIED_A)
                        sessions[targ].inventoried = INVENTORIED_B;
                    else
                        sessions[targ].inventoried = INVENTORIED_A;
                }
                break;
            }
            case 1:
            case 3:
            case 5:
            default:{
                // do nothing
                return;
            }
        }
    } // *** END TAG NOT-MATCHING ***
    
    
    // Tag shall not reply to a Select
}

/**
 * ACK command only contains a RN16. This function extracts it and stores it in rcvd_rn16
 */
void handle_ack_cmd(){
    rcvd_rn16 = (get_bits(rx_bytes, 2, 8) << 8) | get_bits(rx_bytes, 10, 8);
    rcvd_cmd = CMD_ACK;
}

/**
 * Only thing to do is set the rcvd_cmd variable to CMD_NAK
 * No other actions are required here
 */
void handle_nak_cmd(){
    rcvd_cmd = CMD_NAK;
}

/**
 * Req_RN prompts a tag to backscatter a new random number
 */
void handle_req_rn_cmd(){
    if(crc16(rx_bytes, LEN_REQ_RN) != CRC16_OK){
        return;
    }
    // CRC-16 is good, so proceed
    rcvd_rn16 = (rx_bytes[1] << 8) | rx_bytes[2];
    
    rcvd_cmd = CMD_REQ_RN;
    
    // other actions will be taken in run_tag_state_machine() based on the tag_state
}


void handle_read_cmd(){
    rcvd_cmd = CMD_READ;
}


void handle_write_cmd(){
    rcvd_cmd = CMD_WRITE;
}

/**
 * Unrecoverable state as per the standard.
 * Our application shall not implement this as such.
 */
void handle_kill_cmd(){
    rcvd_cmd = CMD_KILL;
}


void handle_lock_cmd(){
    rcvd_cmd = CMD_LOCK;
}

/**
 * Check if len many bits in bank starting at addr match what's in mask_mem
 * @param bank our tag memory bank to compare to mask
 * @param addr the bit address of our memory bank to begin comparison at
 * @param len number of bits to compare
 * @return 
 */
bool check_mask_match(uint8_t* bank, uint8_t addr, uint8_t len){
    uint8_t octets = len >> 8;
    uint8_t bits_left = len % 8;
    uint8_t temp;
    uint8_t i;
    // memory shuffle
    for(i = 0; i < octets; i++){
        temp = get_bits(bank, addr, 8);
        if(mask_mem[i] != temp){
            return false;
        }
        addr += 8;
    }
    if( bits_left > 0 && (get_bits(bank, addr, bits_left) << (8 - bits_left)) != mask_mem[i]){
        return false;
    }
    
    // passed all the tests
    return true;
}


/**
 * Run the Tag state machine that cycles through tag Meta-States
 * see Figure 6.21 (page 52) of EPC Global Gen 2 protocol standard Version 2.0.1
 */
void run_tag_state_machine(){
    switch(tag_state){
        case TAG_STATE_NEW_ROUND:{
            //if(rcvd_cmd == CMD_QUERY) // let's hope this is the case or else we have a problem
            // let's just assume that we wouldn't be here if there weren't a Query command
            
            bool select_valid = select < 2 || ((select >> 1) && (select & 1) == selected);
            // if mismatched inventoried or SL flags
            if(target != current_session->inventoried || !select_valid){
                tag_state = TAG_STATE_READY_INIT;
            } else
            // inventoried flag matches target and Sel bits are good
            // if(target == current_session->inventoried && select_valid)
            {
                if(slots_counter == 0){
                    tag_state = TAG_STATE_REPLY_INIT; // upon entering Reply state, we will backscatter a RN16
                }else{ // slot_counter > 0 
                    tag_state = TAG_STATE_ARBITRATE_INIT;
                }
            }
            
            break;
        }case TAG_STATE_READY_INIT:{
            
            tag_state = TAG_STATE_READY;
            break;
        }case TAG_STATE_READY:{
            
            break;
        }case TAG_STATE_ARBITRATE_INIT:{
            
            tag_state = TAG_STATE_ARBITRATE;
            break;
        }case TAG_STATE_ARBITRATE:{
            
            if(slot_counter == 0){
                // goto reply state, shall backscatter a RN16
                tag_state = TAG_STATE_REPLY_INIT;
            }
            break;
        }case TAG_STATE_REPLY_INIT:{
            // Backscatter a RN16 (only upon entering the REPLY state)
            tag_state = TAG_STATE_REPLY;
            // TODO: implement the data transmission stuff
            
            break;
        }case TAG_STATE_REPLY:{
            // if RN16 is sent, transition to ACKNOWLEDGED state
            if(backscatter_done){
                // TODO: backscatter a specific reply (see Table 6.17 from the standard)
                tag_state = TAG_STATE_ACKNOWLEDGED_INIT;
            }
            break;
        }case TAG_STATE_ACKNOWLEDGED_INIT:{
            
            
            tag_state = TAG_STATE_ACKNOWLEDGED;
            break;
        }case TAG_STATE_ACKNOWLEDGED:{
            // transition to other states depending on received command (Figure 6.21 from standard)
            
            // if no command within time T2
            // tag_state = TAG_STATE_ARBITRATE_INIT
            // break;
            
            // do things based on received command
            // === BEGIN rcvd_cmd SWITCH ===
            switch(rcvd_cmd){
                case CMD_REQ_RN:{
                    // command == Req_RN and state == Acknowledged
                    // check if the RN16 in the command matched the last RN16 we sent
                    if(rcvd_rn16 == tag_handle){
                        // generate a new handle
                        tag_handle = rn16(TAG_HANDLE_MAX);
                        // backscatter the new tag_handle
                        backscatter_tag_handle();
                        
                        // our access password = 0, cuz ain't nobody got time for that, so transition to Secured state
                        tag_state = TAG_STATE_SECURED_INIT;
                    }// else do nothing
                    break;
                }case CMD_ACK:{
                    // if RN16 in command matches
                    if(rcvd_rn16 == tag_handle){
                        // complicated, see table 6.17 (pg 48) in the standard
                        // TODO: implement this godawful part (if needed)
                    }
                    break;
                }case CMD_SELECT:{
                    // return to Ready state
                    tag_state = TAG_STATE_READY_INIT;
                }case CMD_QUERY:{
                    // New Round
                }default:{
                    break;
                }
            } 
            // === END rcvd_cmd SWITCH ===
            
            break;
        }case TAG_STATE_OPEN_INIT:{
            
            tag_state = TAG_STATE_OPEN;
            break;
        }case TAG_STATE_OPEN:{
            
            break;
        }case TAG_STATE_SECURED_INIT:{
            
            tag_state = TAG_STATE_SECURED;
            break;
        }case TAG_STATE_SECURED:{
            
            break;
        }case TAG_STATE_KILLED_INIT:{
            
            tag_state = TAG_STATE_KILLED;
            break;
        }case TAG_STATE_KILLED:{
            
            break;
        }default:{
            break;
        }
    }
}

/**
 * start backscattering of the current tag_handle
 */
void backscatter_tag_handle(){
    
    // TODO: set backscatter data buffer
    
    // TODO: prompt backscatter to start
    
    backscatter_done = false; // set this flag, might get set by some other function IDK
}

