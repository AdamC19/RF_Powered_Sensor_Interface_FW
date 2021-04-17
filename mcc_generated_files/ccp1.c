/**
  CCP1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    ccp1.c

  @Summary
    This is the generated driver implementation file for the CCP1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides implementations for driver APIs for CCP1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F18346
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
         MPLAB 	          :  MPLAB X 5.45
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "ccp1.h"
#include "pin_manager.h"
#include "../transmit.h"

/**
  Section: Compare Module APIs:
*/

void CCP1_Initialize(void)
{
    // Set the CCP1 to the options selected in the User Interface
	
	// CCP1MODE Toggle_cleartmr; CCP1EN enabled; CCP1FMT right_aligned; 
	CCP1CON = 0x81;    
	
	// CCPR1H 0; 
	CCPR1H = 0x00;    
	
	// CCPR1L 0; 
	CCPR1L = 0x00;    

	// Selecting Timer 3
	CCPTMRSbits.C1TSEL = 0x2;
    
    // Clear the CCP1 interrupt flag
    PIR4bits.CCP1IF = 0;

    // Enable the CCP1 interrupt
    PIE4bits.CCP1IE = 1;
}

void CCP1_SetCompareCount(uint16_t compareCount)
{
    CCP1_PERIOD_REG_T module;
    
    // Write the 16-bit compare value
    module.ccpr1_16Bit = compareCount;
    
    CCPR1L = module.ccpr1l;
    CCPR1H = module.ccpr1h;
}

bool CCP1_OutputStatusGet(void)
{
    // Returns the output status
    return(CCP1CONbits.CCP1OUT);
}

void CCP1_CompareISR(void)
{
    // Clear the CCP1 interrupt flag
    PIR4bits.CCP1IF = 0;
    //know when to toggle to send v 
    if (!preambledone) {
        if(clkcnt %2 == 1) {
            if (currentBit == 0) { //need to define nextBit in transmit.h?
                TX_SHUNT_Toggle(); //change the value of this pin? may need to import access to pin manager?
              //Need to move pointer      
            }
        }
        else {
            if (premAddr != 16) {
                TX_SHUNT_Toggle();   //note: v will be encoded as a 1            
            }
            uint8_t cind = premAddr >> 3;
            uint8_t shift = premAddr & 0x07;
            currentBit = (preambleData[cind] >> (7 - shift) & 1);
            premAddr = premAddr + 1;  
        }
        if (premAddr >= 18) { //done with preamble
            preambledone = true;
        }
 
        clkcnt = clkcnt + 1
        return;
    }
    // Add user code here
    //If statement checking whether to transmit next bit in memory or move to end of transmission stuff
    //need to get working with clock cycles and all that
    if (remainingBits != 0){ //may not need this, as I should only enter when >0
        //transmit next bit in memory
        if(clkcnt %2 == 1) {
            if (currentBit == 0) { //need to define nextBit in transmit.h?
                TX_SHUNT_Toggle(); //change the value of this pin? may need to import access to pin manager?
              //Need to move pointer      
            }
        }
        else {
            TX_SHUNT_Toggle();
            uint8_t cind = bitAddr >> 3;
            uint8_t shift = bitAddr & 0x07;
            currentBit = (transmitData[cind] >> (7 - shift) & 1);
            bitAddr = bitAddr + 1;
            remainingBits = remainingBits - 1;   
        }
    }
    //if there is not another bit to transmit, need to send end signal/dummy bit
    else {
        if(!transmitdone) {
            //do one cycle not toggling to transmit 1
            if (clkcnt % 2 == 0) {
                transmitdone = true;
            }
        }
        else {
            TMR3_StopTimer(); //stop timer 
        }
    }
    clkcnt = clkcnt + 1;
    
}
/**
 End of File
*/