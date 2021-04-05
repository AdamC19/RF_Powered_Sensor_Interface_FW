/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F18346
        Driver Version    :  2.00
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

#include "mcc_generated_files/mcc.h"

/* User Includes */
#include "global.h"
#include <string.h>

/* Macros */

/* Global Variables */
volatile uint8_t edge = FALLING;
volatile uint8_t rt_cal = 0;
volatile uint8_t pivot = 0;
volatile uint8_t tr_cal = 0;
volatile RxStates_t rx_state = RX_IDLE;
volatile uint8_t edges = 0;
volatile uint16_t falling_edge = 0;
volatile uint16_t rising_edge  = 0;
volatile uint8_t rx_bit_ind = 0;
volatile uint8_t rx_bits[16];
volatile uint8_t rx_word_ind = 0;
volatile uint16_t rx_words[16];
volatile bool preamble_rcvd = false;
volatile bool frame_sync_rcvd = false;
volatile bool word_rcvd = false;

uint8_t mode = MODE_CHARGING;

/* Function Prototypes */
void init();
void enter_listen_mode();
void enter_normal_mode();
void enter_hi_perf_mode();
void debug(uint8_t* str);

/*
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    init();
    
    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    debug("Entering main loop\r\n");
    while (1)
    {
        // Add your application code
        switch(mode){
            case MODE_CHARGING:{
                break;
            }case MODE_RECEIVING:{
                break;
            }case MODE_SAMPLING:{
                break;
            }case MODE_TRANSMITTING:{
                break;
            }default:{
                break;
            }
        }
    }
}

void init(){
    enter_listen_mode(); // do this immediately so clocks are right and all for the interrupts
    mode = MODE_CHARGING;
    
    DISABLE_I_SINK;
    DISABLE_I_SRC;
}

/**
 * @brief enter the mode to listen for received frames\
 * 
 * The CPU is put into Doze mode. The peripherals continue to operate at the system clock
 * frequency. Recover on interrupt is set so the ISR is executed at the system clock
 * speed. Doze on exit is set so we return to Doze mode after an ISR.
 */
void enter_listen_mode(){
  // disable I2C clock
  DISABLE_I2C;
  DISABLE_UART;

  // set OSCFRQ to use the internal oscillator at 16MHz
  OSCFRQ &= 0xF0; // clear
  OSCFRQ |= 0x06; // set nominal freq to 16MHz

  // set NOSC<2:0> to request HFINTOSC at 16MHz
  OSCCON1 &= ~(_OSCCON1_NOSC_MASK); // reset bits
  OSCCON1 |= (0x6 << _OSCCON1_NOSC_POSITION) & _OSCCON1_NOSC_MASK; // set accordingly

  // enable ROI (Recover On Interrupt) so interrupt executes at full speed
  CPUDOZE |= _CPUDOZE_ROI_MASK;

  // enable DOE (Doze On Exit) so we go back to doze mode after interrupt handling
  CPUDOZE |= _CPUDOZE_DOE_MASK;
  
  // now set DOZE<2:0> to desired ratio
  CPUDOZE |= (3 << _CPUDOZE_DOZE_POSITION) & _CPUDOZE_DOZE_MASK; // 3 gives a ratio of 1:16
  // finally enter DOZE mode (set DOZEN bit)
  CPUDOZE |= _CPUDOZE_DOZEN_MASK;
}


void enter_normal_mode(){
  // exit DOZE mode (reset DOZEN bit)
  CPUDOZE &= ~_CPUDOZE_DOZEN_MASK;

  // set OSCFRQ to use the internal oscillator at 2MHz
  OSCFRQ &= 0xF0; // clear
  OSCFRQ |= 0x01; // set nominal freq to 2MHz

  // set NOSC<2:0> to 6 (110) to use HFINTOSC at 1MHz
  OSCCON1 &= ~_OSCCON1_NOSC_MASK; // first ensure all bits are reset
  OSCCON1 |= (0x6 << _OSCCON1_NOSC_POSITION) & _OSCCON1_NOSC_MASK; // now set

  ENABLE_I2C;
  ENABLE_UART; 
  
}


void enter_hi_perf_mode(){
  // exit DOZE mode (reset DOZEN bit)
  CPUDOZE &= ~_CPUDOZE_DOZEN_MASK;

  // set OSCFRQ to use the internal oscillator at 16MHz
  OSCFRQ &= 0xF0; // clear
  OSCFRQ |= 0x06; // set nominal freq to 16MHz

  // set NOSC<2:0> to request HFINTOSC at 16MHz
  OSCCON1 &= ~(_OSCCON1_NOSC_MASK); // reset bits
  OSCCON1 |= (0x6 << _OSCCON1_NOSC_POSITION) & _OSCCON1_NOSC_MASK; // set accordingly

  ENABLE_I2C;
  ENABLE_UART;

}


void debug(uint8_t* str){
    ENABLE_UART;
    
    if(EUSART_is_tx_ready()){
        uint8_t len = (uint8_t)strlen(str);
        for(uint8_t i = 0; i < len; i++){
            EUSART_Write(str[i]);
        }
    }
    DISABLE_UART;
}

/**
 End of File
*/