/* 
 * File:   tag.h
 * Author: Adam Cordingley
 *
 * Created on April 7, 2021
 */

#ifndef TAG_H
#define	TAG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "global.h"
#include <stdint.h>
#include <stdbool.h>

/* gives us 128 bits of rx memory, unlikely to need more than this */
#define RX_BUFFER_SIZE      16
    
#define RSVD_MEM_BANK_ADDR  0x00
#define EPC_MEM_BANK_ADDR   0x01
#define TID_MEM_BANK_ADDR   0x02
#define USER_MEM_BANK_ADDR  0x03

#define RSVD_MEM_BANK_SIZE  (0x20/16)
#define EPC_MEM_BANK_SIZE   (0x10/16)
#define TID_MEM_BANK_SIZE   (0x40/16)
#define USER_MEM_BANK_SIZE  0x02

/* Inventoried flag values */
#define INVENTORIED_A       0
#define INVENTORIED_B       1
    
/* Size in bytes of the Mask which can be set by a Select command */
#define MASK_SIZE_BYTES     32

#define TAG_MEM_SIZE        (RSVD_MEM_BANK_SIZE + EPC_MEM_BANK_SIZE + TID_MEM_BANK_SIZE + USER_MEM_BANK_SIZE)

#define CRC5_PRESET         9
#define CRC16_PRESET        0xFFFF    
#define CRC16_OK            0x1D0F

#define SESSION_S0          0
#define SESSION_S1          1
#define SESSION_S2          2
#define SESSION_S3          3


/* begin mandatory command code definitions */
#define CMD_QUERY_REP       0x00
#define CMD_ACK             (1 << 6)
#define CMD_QUERY           (8 << 4)
#define CMD_QUERY_ADJUST    (9 << 4)
#define CMD_SELECT          (10 << 4)
#define CMD_RSVD            (11 << 4)
#define CMD_NAK             (12 << 4)
#define CMD_REQ_RN          ((12 << 4) | 1)
#define CMD_READ            ((12 << 4) | 2)
#define CMD_WRITE           ((12 << 4) | 3)
#define CMD_KILL            ((12 << 4) | 4)
#define CMD_LOCK            ((12 << 4) | 5)
#define CMD_NONE            0xFF
    
/* begin mandatory-command-code unique lengths where applicable */
#define LEN_QUERY_REP       4
#define LEN_ACK             18
#define LEN_QUERY           22
#define LEN_QUERY_ADJUST    9
#define LEN_NAK             8
#define LEN_REQ_RN          40

/* Pseudo-Random Number Generator stuff */
#define RNG_INCREMENT       12345
#define RNG_MULTIPLIER      1103515245
#define RNG_MODULUS         (1<<31)
#define RNG_SEED            314159
#define TAG_HANDLE_MAX      32767
    
/* Tag meta-states, see page ___ of EPC Global Gen 2 protocol standard*/
typedef enum tag_states_enum 
{       TAG_STATE_NEW_ROUND,
        TAG_STATE_READY_INIT,
        TAG_STATE_READY,
        TAG_STATE_ARBITRATE_INIT,
        TAG_STATE_ARBITRATE,
        TAG_STATE_REPLY_INIT,
        TAG_STATE_REPLY,
        TAG_STATE_ACKNOWLEDGED_INIT,
        TAG_STATE_ACKNOWLEDGED,
        TAG_STATE_OPEN_INIT,
        TAG_STATE_OPEN,
        TAG_STATE_SECURED_INIT,
        TAG_STATE_SECURED,
        TAG_STATE_KILLED_INIT,
        TAG_STATE_KILLED
}TagState_t;

/* states for low-level reception */
typedef enum rx_states_enum 
{       RX_IDLE,
        RX_WAIT_DELIM,
        RX_WAIT_DATA0,
        RX_GET_RTCAL,
        RX_FRAME_SYNC_RCVD,
        RX_GET_DATA
}RxStates_t;


typedef struct tag_data_struct {
    bool killed;
    uint16_t slot_counter;
    TagState_t state;
}TagData_t;

/* structure for storing data for a single session (of which there are 4) */
typedef struct tag_session_struct {
    uint8_t session_id;
    uint8_t inventoried; // inventoried flag (A or B)
}TagSession_t;


/* array to hold the 4 sessions */
extern volatile TagSession_t sessions[];

/* command processed indicator/flag */
extern volatile bool cmd_processed;

/* tag meta-state and such */
extern volatile TagState_t tag_state;

/* variables for low-level reception */
extern volatile RxStates_t rx_state;
extern volatile uint8_t rt_cal;
extern volatile uint8_t pivot;
extern volatile uint8_t tr_cal;
extern volatile int8_t rx_bit_ind;
extern volatile uint8_t rx_byte_ind;
extern volatile uint8_t rx_bytes[];
extern volatile bool preamble_rcvd;
extern volatile bool frame_sync_rcvd;
extern volatile uint8_t rcvd_cmd;

/* variables for low-level transmission */
extern volatile bool backscatter_done;

/* Flags and stuff common to all sessions */
extern volatile bool selected;
extern volatile uint8_t c_flag;
extern volatile uint16_t slot_counter;
extern volatile bool truncate;
extern volatile uint16_t rcvd_rn16;
extern volatile uint16_t tag_handle;

/* Mask set by a Select command */
extern volatile uint8_t mask_bytes[];

/* for stuff set by Query command */
extern volatile uint8_t tr_cal_divide_ratio;
extern volatile uint8_t blf_period; 
extern volatile uint8_t cycles_per_symbol;
extern volatile bool use_pilot_tone;
extern volatile uint8_t select; 
extern volatile uint8_t session_number;
extern volatile TagSession_t* current_session;
extern volatile uint8_t target;
extern volatile uint8_t slots_q;
extern volatile uint8_t slots_in_round;

extern volatile uint32_t rand;

/* tag memory banks */
extern volatile uint8_t epc_mem_bank[];
extern volatile uint8_t tid_mem_bank[];
extern volatile uint8_t user_mem_bank[];

/* Tag function prototypes */
void init_sessions();
void init_tag_mem(uint8_t* tag_mem);
void init_tag_data(TagData_t* tag_data);
void tag_mem_write_bit(uint8_t* mem, uint8_t addr, uint8_t value);
uint8_t tag_mem_read_bit(uint8_t* mem, uint8_t addr);
void tag_mem_write_byte(uint8_t* mem, uint8_t addr, uint8_t value);
uint8_t tag_mem_read_byte(uint8_t* mem, uint8_t addr);
void tag_mem_write16(uint8_t* mem, uint8_t addr, uint16_t value);
uint16_t tag_mem_read16(uint8_t* mem, uint8_t addr);
uint16_t rn16(uint16_t max);
uint8_t crc5(uint8_t* data, uint8_t bits);
uint16_t crc16(uint8_t* data, uint8_t bits);
uint8_t get_bits(uint8_t* bytes, uint8_t bit_addr, uint8_t len);
uint32_t get_ebv8(uint8_t* bytes, uint8_t bit_addr, uint8_t* size);
void decrement_slot_counter();
void handle_query_cmd();
void handle_query_adjust_cmd();
void handle_query_rep_cmd();
void handle_select_cmd();
void handle_ack_cmd();
void handle_nak_cmd();
void handle_req_rn_cmd();
void handle_read_cmd();
void handle_write_cmd();
void handle_kill_cmd();
void handle_lock_cmd();
bool check_mask_match(uint8_t* bank, uint8_t addr, uint8_t len);
void run_tag_state_machine();
void backscatter_tag_handle();

#ifdef	__cplusplus
}
#endif

#endif	/* TAG_H */

