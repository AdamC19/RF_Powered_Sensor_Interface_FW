/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set TX_SHUNT aliases
#define TX_SHUNT_TRIS                 TRISAbits.TRISA2
#define TX_SHUNT_LAT                  LATAbits.LATA2
#define TX_SHUNT_PORT                 PORTAbits.RA2
#define TX_SHUNT_WPU                  WPUAbits.WPUA2
#define TX_SHUNT_OD                   ODCONAbits.ODCA2
#define TX_SHUNT_ANS                  ANSELAbits.ANSA2
#define TX_SHUNT_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define TX_SHUNT_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define TX_SHUNT_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define TX_SHUNT_GetValue()           PORTAbits.RA2
#define TX_SHUNT_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define TX_SHUNT_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define TX_SHUNT_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define TX_SHUNT_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define TX_SHUNT_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define TX_SHUNT_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define TX_SHUNT_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define TX_SHUNT_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set RB4 procedures
#define RB4_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define RB4_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define RB4_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define RB4_GetValue()              PORTBbits.RB4
#define RB4_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define RB4_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define RB4_SetPullup()             do { WPUBbits.WPUB4 = 1; } while(0)
#define RB4_ResetPullup()           do { WPUBbits.WPUB4 = 0; } while(0)
#define RB4_SetAnalogMode()         do { ANSELBbits.ANSB4 = 1; } while(0)
#define RB4_SetDigitalMode()        do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set RB5 procedures
#define RB5_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define RB5_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define RB5_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define RB5_GetValue()              PORTBbits.RB5
#define RB5_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define RB5_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define RB5_SetPullup()             do { WPUBbits.WPUB5 = 1; } while(0)
#define RB5_ResetPullup()           do { WPUBbits.WPUB5 = 0; } while(0)
#define RB5_SetAnalogMode()         do { ANSELBbits.ANSB5 = 1; } while(0)
#define RB5_SetDigitalMode()        do { ANSELBbits.ANSB5 = 0; } while(0)

// get/set RB6 procedures
#define RB6_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define RB6_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define RB6_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define RB6_GetValue()              PORTBbits.RB6
#define RB6_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define RB6_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define RB6_SetPullup()             do { WPUBbits.WPUB6 = 1; } while(0)
#define RB6_ResetPullup()           do { WPUBbits.WPUB6 = 0; } while(0)
#define RB6_SetAnalogMode()         do { ANSELBbits.ANSB6 = 1; } while(0)
#define RB6_SetDigitalMode()        do { ANSELBbits.ANSB6 = 0; } while(0)

// get/set RB7 procedures
#define RB7_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define RB7_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define RB7_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define RB7_GetValue()              PORTBbits.RB7
#define RB7_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define RB7_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define RB7_SetPullup()             do { WPUBbits.WPUB7 = 1; } while(0)
#define RB7_ResetPullup()           do { WPUBbits.WPUB7 = 0; } while(0)
#define RB7_SetAnalogMode()         do { ANSELBbits.ANSB7 = 1; } while(0)
#define RB7_SetDigitalMode()        do { ANSELBbits.ANSB7 = 0; } while(0)

// get/set RC0 procedures
#define RC0_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define RC0_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define RC0_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define RC0_GetValue()              PORTCbits.RC0
#define RC0_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define RC0_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define RC0_SetPullup()             do { WPUCbits.WPUC0 = 1; } while(0)
#define RC0_ResetPullup()           do { WPUCbits.WPUC0 = 0; } while(0)
#define RC0_SetAnalogMode()         do { ANSELCbits.ANSC0 = 1; } while(0)
#define RC0_SetDigitalMode()        do { ANSELCbits.ANSC0 = 0; } while(0)

// get/set RC1 procedures
#define RC1_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define RC1_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define RC1_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define RC1_GetValue()              PORTCbits.RC1
#define RC1_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define RC1_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define RC1_SetPullup()             do { WPUCbits.WPUC1 = 1; } while(0)
#define RC1_ResetPullup()           do { WPUCbits.WPUC1 = 0; } while(0)
#define RC1_SetAnalogMode()         do { ANSELCbits.ANSC1 = 1; } while(0)
#define RC1_SetDigitalMode()        do { ANSELCbits.ANSC1 = 0; } while(0)

// get/set ADC_BUSY_RDY aliases
#define ADC_BUSY_RDY_TRIS                 TRISCbits.TRISC2
#define ADC_BUSY_RDY_LAT                  LATCbits.LATC2
#define ADC_BUSY_RDY_PORT                 PORTCbits.RC2
#define ADC_BUSY_RDY_WPU                  WPUCbits.WPUC2
#define ADC_BUSY_RDY_OD                   ODCONCbits.ODCC2
#define ADC_BUSY_RDY_ANS                  ANSELCbits.ANSC2
#define ADC_BUSY_RDY_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define ADC_BUSY_RDY_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define ADC_BUSY_RDY_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define ADC_BUSY_RDY_GetValue()           PORTCbits.RC2
#define ADC_BUSY_RDY_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define ADC_BUSY_RDY_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define ADC_BUSY_RDY_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define ADC_BUSY_RDY_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define ADC_BUSY_RDY_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define ADC_BUSY_RDY_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define ADC_BUSY_RDY_SetAnalogMode()      do { ANSELCbits.ANSC2 = 1; } while(0)
#define ADC_BUSY_RDY_SetDigitalMode()     do { ANSELCbits.ANSC2 = 0; } while(0)

// get/set RX_BITLINE aliases
#define RX_BITLINE_TRIS                 TRISCbits.TRISC3
#define RX_BITLINE_LAT                  LATCbits.LATC3
#define RX_BITLINE_PORT                 PORTCbits.RC3
#define RX_BITLINE_WPU                  WPUCbits.WPUC3
#define RX_BITLINE_OD                   ODCONCbits.ODCC3
#define RX_BITLINE_ANS                  ANSELCbits.ANSC3
#define RX_BITLINE_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define RX_BITLINE_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define RX_BITLINE_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define RX_BITLINE_GetValue()           PORTCbits.RC3
#define RX_BITLINE_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define RX_BITLINE_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define RX_BITLINE_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define RX_BITLINE_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define RX_BITLINE_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define RX_BITLINE_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define RX_BITLINE_SetAnalogMode()      do { ANSELCbits.ANSC3 = 1; } while(0)
#define RX_BITLINE_SetDigitalMode()     do { ANSELCbits.ANSC3 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/