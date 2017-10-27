/* 
 * File:   delay.h
 * Author: Noyel
 *
 * Created on October 19, 2017, 4:29 PM
 */

#ifndef DELAY_H
#define	DELAY_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
    
    void _delay_10uS(int delay);

    void _delay_MS(int delay);

    void delay(int delay);


#ifdef	__cplusplus
}
#endif

#endif	/* DELAY_H */

