 /**
   CMP2 Generated Driver File
 
   @Company
     Microchip Technology Inc.
 
   @File Name
     cmp2.c
 
   @Summary
     This is the generated driver implementation file for the CMP2 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs
 
   @Description
     This source file provides implementations for driver APIs for CMP2.
     Generation Information :
         Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
         Device            :  PIC16F18346
         Driver Version    :  2.11
     The generated drivers are tested against the following:
         Compiler          :  XC8 2.31 and above or later
         MPLAB             :  MPLAB X 5.45
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
#include "cmp2.h"
/**
  Section: CMP2 APIs
*/

void CMP2_Initialize(void)
{
    /* Disable CMP2 interrupt before configuring the CMP2
       control register. Although a comparator is disabled, an interrupt can be
       generated by changing the output polarity with the CxPOL bit of the
       CMxCON0 register,or by switching the comparator on or off with the CxON
       bit of the CMxCON0 register.*/
    PIE2bits.C2IE = 0;

	// C2HYS enabled; C2SP hi_speed; C2ON enabled; C2POL not inverted; C2SYNC asynchronous;                          
    CM2CON0 = 0x86;
	
	// C2INTN no_intFlag; C2INTP intFlag_pos; C2PCH CIN+; C2NCH CIN1-;                          
    CM2CON1 = 0x81;
	
    // Clearing IF flag before enabling the interrupt.
    PIR2bits.C2IF = 0;

    // Enabling CMP2 interrupt.
    PIE2bits.C2IE = 1;
}

bool CMP2_GetOutputStatus(void)
{
	return (CMOUTbits.MC2OUT);
}

void CMP2_ISR(void)
{
    // clear the CMP2 interrupt flag
    PIR2bits.C2IF = 0;
}

/**
 End of File
*/
