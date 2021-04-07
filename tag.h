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

#include <stdbool.h>

#define RSVD_MEM_BANK_ADDR  0x00
#define EPC_MEM_BANK_ADDR   0x01
#define TID_MEM_BANK_ADDR   0x02
#define USER_MEM_BANK_ADDR  0x03

#define RSVD_MEM_BANK_SIZE  (0x20/16)
#define EPC_MEM_BANK_SIZE   (0x10/16)
#define TID_MEM_BANK_SIZE   (0x40/16)
#define USER_MEM_BANK_SIZE  0x02

#define TAG_MEM_SIZE        (RSVD_MEM_BANK_SIZE + EPC_MEM_BANK_SIZE + TID_MEM_BANK_SIZE + USER_MEM_BANK_SIZE)
    

typedef enum tag_states_enum 
{       TAG_STATE_READY,
        TAG_STATE_ARBITRATE,
        TAG_STATE_REPLY,
        TAG_STATE_ACKNOWLEDGED,
        TAG_STATE_OPEN,
        TAG_STATE_SECURED,
        TAG_STATE_KILLED
}TagState_t;


typedef struct tag_data_struct {
    bool killed;
    uint16_t slot_counter;
    TagState_t state;
}TagData_t;

uint8_t* init_tag_mem();
void init_tag_data(TagData_t* tag_data);
void tag_mem_write_bit(uint8_t* mem, uint8_t addr, uint8_t value);
uint8_t tag_mem_read_bit(uint8_t* mem, uint8_t addr);
void tag_mem_write_byte(uint8_t* mem, uint8_t addr, uint8_t value);
uint8_t tag_mem_read_byte(uint8_t* mem, uint8_t addr);
void tag_mem_write16(uint8_t* mem, uint8_t addr, uint16_t value);
uint16_t tag_mem_read16(uint8_t* mem, uint8_t addr);


#ifdef	__cplusplus
}
#endif

#endif	/* TAG_H */

