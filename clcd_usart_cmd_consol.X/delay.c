#include "delay.h"
#include "mcc_generated_files/tmr5.h"
#include "mcc_generated_files/tmr1.h"

void delay(int delay)
{
    int val = 0, i;
    for (val = 0; val < delay; val++) {
        for(i=0; i<350; i++)
        {
            
        }
    }
}

//10us delay create by TMR5
void _delay_10uS(int delay) {
    int val = 0;
    for (val = 0; val < delay; val++) {
        TMR5_Reload();
        TMR5_StartTimer();
        while (!TMR5_HasOverflowOccured());
        TMR5_StopTimer();
        PIR5bits.TMR5IF = 0;
    }
}

// 1ms delay create by TMR1
void _delay_MS(int delay) {
    int val = 0;
    for (val = 0; val < delay; val++) {
        TMR1_Reload();
        TMR1_StartTimer();
        while (!TMR1_HasOverflowOccured());
        TMR1_StopTimer();
        PIR1bits.TMR1IF = 0;
    }
}
