/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15.1
        Device            :  PIC18F46K22
        Version           :  1.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/


#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set LCD_EN_RA0 aliases
#define LCD_EN_RA0_TRIS               TRISAbits.TRISA0
#define LCD_EN_RA0_LAT                LATAbits.LATA0
#define LCD_EN_RA0_PORT               PORTAbits.RA0
#define LCD_EN_RA0_ANS                ANSELAbits.ANSA0
#define LCD_EN_RA0_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define LCD_EN_RA0_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define LCD_EN_RA0_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define LCD_EN_RA0_GetValue()           PORTAbits.RA0
#define LCD_EN_RA0_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define LCD_EN_RA0_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define LCD_EN_RA0_SetAnalogMode()  do { ANSELAbits.ANSA0 = 1; } while(0)
#define LCD_EN_RA0_SetDigitalMode() do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set LCD_RW_RA1 aliases
#define LCD_RW_RA1_TRIS               TRISAbits.TRISA1
#define LCD_RW_RA1_LAT                LATAbits.LATA1
#define LCD_RW_RA1_PORT               PORTAbits.RA1
#define LCD_RW_RA1_ANS                ANSELAbits.ANSA1
#define LCD_RW_RA1_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define LCD_RW_RA1_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define LCD_RW_RA1_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define LCD_RW_RA1_GetValue()           PORTAbits.RA1
#define LCD_RW_RA1_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define LCD_RW_RA1_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define LCD_RW_RA1_SetAnalogMode()  do { ANSELAbits.ANSA1 = 1; } while(0)
#define LCD_RW_RA1_SetDigitalMode() do { ANSELAbits.ANSA1 = 0; } while(0)

// get/set LCD_RS_RA2 aliases
#define LCD_RS_RA2_TRIS               TRISAbits.TRISA2
#define LCD_RS_RA2_LAT                LATAbits.LATA2
#define LCD_RS_RA2_PORT               PORTAbits.RA2
#define LCD_RS_RA2_ANS                ANSELAbits.ANSA2
#define LCD_RS_RA2_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define LCD_RS_RA2_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define LCD_RS_RA2_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define LCD_RS_RA2_GetValue()           PORTAbits.RA2
#define LCD_RS_RA2_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define LCD_RS_RA2_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define LCD_RS_RA2_SetAnalogMode()  do { ANSELAbits.ANSA2 = 1; } while(0)
#define LCD_RS_RA2_SetDigitalMode() do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()    do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()   do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()   do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()         PORTCbits.RC6
#define RC6_SetDigitalInput()   do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()  do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetAnalogMode() do { ANSELCbits.ANSC6 = 1; } while(0)
#define RC6_SetDigitalMode()do { ANSELCbits.ANSC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()    do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()   do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()   do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()         PORTCbits.RC7
#define RC7_SetDigitalInput()   do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()  do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetAnalogMode() do { ANSELCbits.ANSC7 = 1; } while(0)
#define RC7_SetDigitalMode()do { ANSELCbits.ANSC7 = 0; } while(0)

// get/set LCD_D0_RD0 aliases
#define LCD_D0_RD0_TRIS               TRISDbits.TRISD0
#define LCD_D0_RD0_LAT                LATDbits.LATD0
#define LCD_D0_RD0_PORT               PORTDbits.RD0
#define LCD_D0_RD0_ANS                ANSELDbits.ANSD0
#define LCD_D0_RD0_SetHigh()            do { LATDbits.LATD0 = 1; } while(0)
#define LCD_D0_RD0_SetLow()             do { LATDbits.LATD0 = 0; } while(0)
#define LCD_D0_RD0_Toggle()             do { LATDbits.LATD0 = ~LATDbits.LATD0; } while(0)
#define LCD_D0_RD0_GetValue()           PORTDbits.RD0
#define LCD_D0_RD0_SetDigitalInput()    do { TRISDbits.TRISD0 = 1; } while(0)
#define LCD_D0_RD0_SetDigitalOutput()   do { TRISDbits.TRISD0 = 0; } while(0)
#define LCD_D0_RD0_SetAnalogMode()  do { ANSELDbits.ANSD0 = 1; } while(0)
#define LCD_D0_RD0_SetDigitalMode() do { ANSELDbits.ANSD0 = 0; } while(0)

// get/set LCD_D1_RD1 aliases
#define LCD_D1_RD1_TRIS               TRISDbits.TRISD1
#define LCD_D1_RD1_LAT                LATDbits.LATD1
#define LCD_D1_RD1_PORT               PORTDbits.RD1
#define LCD_D1_RD1_ANS                ANSELDbits.ANSD1
#define LCD_D1_RD1_SetHigh()            do { LATDbits.LATD1 = 1; } while(0)
#define LCD_D1_RD1_SetLow()             do { LATDbits.LATD1 = 0; } while(0)
#define LCD_D1_RD1_Toggle()             do { LATDbits.LATD1 = ~LATDbits.LATD1; } while(0)
#define LCD_D1_RD1_GetValue()           PORTDbits.RD1
#define LCD_D1_RD1_SetDigitalInput()    do { TRISDbits.TRISD1 = 1; } while(0)
#define LCD_D1_RD1_SetDigitalOutput()   do { TRISDbits.TRISD1 = 0; } while(0)
#define LCD_D1_RD1_SetAnalogMode()  do { ANSELDbits.ANSD1 = 1; } while(0)
#define LCD_D1_RD1_SetDigitalMode() do { ANSELDbits.ANSD1 = 0; } while(0)

// get/set LCD_D2_RD2 aliases
#define LCD_D2_RD2_TRIS               TRISDbits.TRISD2
#define LCD_D2_RD2_LAT                LATDbits.LATD2
#define LCD_D2_RD2_PORT               PORTDbits.RD2
#define LCD_D2_RD2_ANS                ANSELDbits.ANSD2
#define LCD_D2_RD2_SetHigh()            do { LATDbits.LATD2 = 1; } while(0)
#define LCD_D2_RD2_SetLow()             do { LATDbits.LATD2 = 0; } while(0)
#define LCD_D2_RD2_Toggle()             do { LATDbits.LATD2 = ~LATDbits.LATD2; } while(0)
#define LCD_D2_RD2_GetValue()           PORTDbits.RD2
#define LCD_D2_RD2_SetDigitalInput()    do { TRISDbits.TRISD2 = 1; } while(0)
#define LCD_D2_RD2_SetDigitalOutput()   do { TRISDbits.TRISD2 = 0; } while(0)
#define LCD_D2_RD2_SetAnalogMode()  do { ANSELDbits.ANSD2 = 1; } while(0)
#define LCD_D2_RD2_SetDigitalMode() do { ANSELDbits.ANSD2 = 0; } while(0)

// get/set LCD_D3_RD3 aliases
#define LCD_D3_RD3_TRIS               TRISDbits.TRISD3
#define LCD_D3_RD3_LAT                LATDbits.LATD3
#define LCD_D3_RD3_PORT               PORTDbits.RD3
#define LCD_D3_RD3_ANS                ANSELDbits.ANSD3
#define LCD_D3_RD3_SetHigh()            do { LATDbits.LATD3 = 1; } while(0)
#define LCD_D3_RD3_SetLow()             do { LATDbits.LATD3 = 0; } while(0)
#define LCD_D3_RD3_Toggle()             do { LATDbits.LATD3 = ~LATDbits.LATD3; } while(0)
#define LCD_D3_RD3_GetValue()           PORTDbits.RD3
#define LCD_D3_RD3_SetDigitalInput()    do { TRISDbits.TRISD3 = 1; } while(0)
#define LCD_D3_RD3_SetDigitalOutput()   do { TRISDbits.TRISD3 = 0; } while(0)
#define LCD_D3_RD3_SetAnalogMode()  do { ANSELDbits.ANSD3 = 1; } while(0)
#define LCD_D3_RD3_SetDigitalMode() do { ANSELDbits.ANSD3 = 0; } while(0)

// get/set LCD_D4_RD4 aliases
#define LCD_D4_RD4_TRIS               TRISDbits.TRISD4
#define LCD_D4_RD4_LAT                LATDbits.LATD4
#define LCD_D4_RD4_PORT               PORTDbits.RD4
#define LCD_D4_RD4_ANS                ANSELDbits.ANSD4
#define LCD_D4_RD4_SetHigh()            do { LATDbits.LATD4 = 1; } while(0)
#define LCD_D4_RD4_SetLow()             do { LATDbits.LATD4 = 0; } while(0)
#define LCD_D4_RD4_Toggle()             do { LATDbits.LATD4 = ~LATDbits.LATD4; } while(0)
#define LCD_D4_RD4_GetValue()           PORTDbits.RD4
#define LCD_D4_RD4_SetDigitalInput()    do { TRISDbits.TRISD4 = 1; } while(0)
#define LCD_D4_RD4_SetDigitalOutput()   do { TRISDbits.TRISD4 = 0; } while(0)
#define LCD_D4_RD4_SetAnalogMode()  do { ANSELDbits.ANSD4 = 1; } while(0)
#define LCD_D4_RD4_SetDigitalMode() do { ANSELDbits.ANSD4 = 0; } while(0)

// get/set LCD_D5_RD5 aliases
#define LCD_D5_RD5_TRIS               TRISDbits.TRISD5
#define LCD_D5_RD5_LAT                LATDbits.LATD5
#define LCD_D5_RD5_PORT               PORTDbits.RD5
#define LCD_D5_RD5_ANS                ANSELDbits.ANSD5
#define LCD_D5_RD5_SetHigh()            do { LATDbits.LATD5 = 1; } while(0)
#define LCD_D5_RD5_SetLow()             do { LATDbits.LATD5 = 0; } while(0)
#define LCD_D5_RD5_Toggle()             do { LATDbits.LATD5 = ~LATDbits.LATD5; } while(0)
#define LCD_D5_RD5_GetValue()           PORTDbits.RD5
#define LCD_D5_RD5_SetDigitalInput()    do { TRISDbits.TRISD5 = 1; } while(0)
#define LCD_D5_RD5_SetDigitalOutput()   do { TRISDbits.TRISD5 = 0; } while(0)
#define LCD_D5_RD5_SetAnalogMode()  do { ANSELDbits.ANSD5 = 1; } while(0)
#define LCD_D5_RD5_SetDigitalMode() do { ANSELDbits.ANSD5 = 0; } while(0)

// get/set LCD_D6_RD6 aliases
#define LCD_D6_RD6_TRIS               TRISDbits.TRISD6
#define LCD_D6_RD6_LAT                LATDbits.LATD6
#define LCD_D6_RD6_PORT               PORTDbits.RD6
#define LCD_D6_RD6_ANS                ANSELDbits.ANSD6
#define LCD_D6_RD6_SetHigh()            do { LATDbits.LATD6 = 1; } while(0)
#define LCD_D6_RD6_SetLow()             do { LATDbits.LATD6 = 0; } while(0)
#define LCD_D6_RD6_Toggle()             do { LATDbits.LATD6 = ~LATDbits.LATD6; } while(0)
#define LCD_D6_RD6_GetValue()           PORTDbits.RD6
#define LCD_D6_RD6_SetDigitalInput()    do { TRISDbits.TRISD6 = 1; } while(0)
#define LCD_D6_RD6_SetDigitalOutput()   do { TRISDbits.TRISD6 = 0; } while(0)
#define LCD_D6_RD6_SetAnalogMode()  do { ANSELDbits.ANSD6 = 1; } while(0)
#define LCD_D6_RD6_SetDigitalMode() do { ANSELDbits.ANSD6 = 0; } while(0)

// get/set LCD_D7_RD7 aliases
#define LCD_D7_RD7_TRIS               TRISDbits.TRISD7
#define LCD_D7_RD7_LAT                LATDbits.LATD7
#define LCD_D7_RD7_PORT               PORTDbits.RD7
#define LCD_D7_RD7_ANS                ANSELDbits.ANSD7
#define LCD_D7_RD7_SetHigh()            do { LATDbits.LATD7 = 1; } while(0)
#define LCD_D7_RD7_SetLow()             do { LATDbits.LATD7 = 0; } while(0)
#define LCD_D7_RD7_Toggle()             do { LATDbits.LATD7 = ~LATDbits.LATD7; } while(0)
#define LCD_D7_RD7_GetValue()           PORTDbits.RD7
#define LCD_D7_RD7_SetDigitalInput()    do { TRISDbits.TRISD7 = 1; } while(0)
#define LCD_D7_RD7_SetDigitalOutput()   do { TRISDbits.TRISD7 = 0; } while(0)
#define LCD_D7_RD7_SetAnalogMode()  do { ANSELDbits.ANSD7 = 1; } while(0)
#define LCD_D7_RD7_SetDigitalMode() do { ANSELDbits.ANSD7 = 0; } while(0)

// get/set LED_HEART_BEAT aliases
#define LED_HEART_BEAT_TRIS               TRISEbits.TRISE2
#define LED_HEART_BEAT_LAT                LATEbits.LATE2
#define LED_HEART_BEAT_PORT               PORTEbits.RE2
#define LED_HEART_BEAT_ANS                ANSELEbits.ANSE2
#define LED_HEART_BEAT_SetHigh()            do { LATEbits.LATE2 = 1; } while(0)
#define LED_HEART_BEAT_SetLow()             do { LATEbits.LATE2 = 0; } while(0)
#define LED_HEART_BEAT_Toggle()             do { LATEbits.LATE2 = ~LATEbits.LATE2; } while(0)
#define LED_HEART_BEAT_GetValue()           PORTEbits.RE2
#define LED_HEART_BEAT_SetDigitalInput()    do { TRISEbits.TRISE2 = 1; } while(0)
#define LED_HEART_BEAT_SetDigitalOutput()   do { TRISEbits.TRISE2 = 0; } while(0)
#define LED_HEART_BEAT_SetAnalogMode()  do { ANSELEbits.ANSE2 = 1; } while(0)
#define LED_HEART_BEAT_SetDigitalMode() do { ANSELEbits.ANSE2 = 0; } while(0)

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