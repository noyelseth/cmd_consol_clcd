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
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs  - 1.45
        Device            :  PIC18F46K22
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "mcc_generated_files/mcc.h"
#include "delay.h"
#include "clcd.h"
#include "cmd_consol.h"
void heart_beat()
{
    LED_HEART_BEAT_Toggle();
}
bool display = false;
void make_array()
{
    static char i=0;
    heart_beat();
    //if(i==3)
    //{
    display = true;//i=0;}
    //else 
       // i++;
}

void display_screen()
{
   display_text();
}
/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    TMR5_StopTimer();
    TMR1_StopTimer();
    TMR0_StopTimer();
    
    TMR0_SetInterruptHandler(make_array);
    //TMR3_SetInterruptHandler(display_screen);

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighEnable();

    // Enable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowEnable();

    // Disable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighDisable();

    // Disable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowDisable();

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    console_init();
    _delay_MS(1000);
    //init_CLCD_config();
    printf("Init_complete..\r\n");
    //_delay_MS(1000);
//    clear_display_array();
//    char str[50] = "Iquester Solution LLP..";
//    char str1[5] = "OK";
//    char str2[10] = "Cancel";
    //print_str(LINE1_HOME, 10, 15, str, SCROLL_RIGHT_TO_LEFT);
//    print_str(LINE1_HOME, 0, 16, str, SCROLL_RIGHT_TO_LEFT);
    //print_str(LINE2_HOME, 0, 2, str1, SCROLL_RIGHT_TO_LEFT);
    //print_str(LINE2_HOME, 12, 16, str2, SCROLL_RIGHT_TO_LEFT);
    //print_str(LINE1_HOME, 6, 11, str, SCROLL_RIGHT_TO_LEFT);
   // print_str(LINE1_HOME, 11, 15, str1, SCROLL_RIGHT_TO_LEFT);
    
    //print_str(LINE2_HOME, 4, 10, str, SCROLL_RIGHT_TO_LEFT);
    
    TMR0_StartTimer();
    
    //char *ch = (char*)malloc(1);
    while (1)
    {
        // Add your application code
        cmd_consol_task();
        if(display == true){make_display();display = false;}
    }
}
/**
 End of File
*/