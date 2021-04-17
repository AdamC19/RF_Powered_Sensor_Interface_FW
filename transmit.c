/*
 * File:   transmit.c
 * Author: Braden
 *
 * Created on April 17, 2021, 12:17 PM
 */

#include <stdint.h>
#include <stdbool.h>
#include "transmit.h"
#include "mcc_generated_files/ccp1.h"
#include "mcc_generated_files/tmr3.h"

volatile uint8_t currentBit;
volatile uint8_t bitAddr;
volatile uint8_t remainingBits;
volatile uint8_t clkcnt;
volatile uint8_t premAddr;
volatile uint8_t* transmitData;
volatile bool transmitdone = false;
volatile bool preambledone;


    
void transmit(uint8_t* data, uint8_t bitlength) {
    transmitData = data;
    remainingBits = bitlength;
    clkcnt = 0;
    transmitdone = false;
    preambledone = false;
    //send preamble 0000 0000 0000 1010v1 
    if (tr_ext == true) {
        premAddr = 0;
    }
    //send preamble 1010v1
    else {
        premAddr = 12;
    }
    CCP1_SetCompareCount(FM0_halfperiod);
    TMR3_WriteTimer(0);
    TMR3_StartTimer();
}



    //Also need to transmit FM0 Preamble 
    //1010v1 if TRext = 0
    //0000000000001010v1 if TRext =1 with 12 zeros as pilot tone
    
    //loop to iterate through however many times we need DOESN'T go Here
    //for (iCount = NumberBits; iCount > 0; iCount++) { //NumberBits = the specified length of what I am to transmit, from elsewhere
        ////nextBit = Mem[iCount] //of some sort
    //remainingBits = iCount; //so that we can check 
       // CCP1_CompareISR //call CCP1_compareISR
    //}

