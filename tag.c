#include "tag.h"
#include <stdlib.h>
#include <string.h>

/**
 * Allocate memory for this tag's logical interface
 * 
 * @return pointer to the allocated memory
 */
uint8_t* init_tag_mem(){
    uint8_t* tag_mem = (uint8_t*)malloc(TAG_MEM_SIZE);
    memset(tag_mem, 0, TAG_MEM_SIZE);
    return tag_mem;
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
