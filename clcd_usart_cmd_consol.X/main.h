/* 
 * File:   main.h
 * Author: Noyel_IQ_Office
 *
 * Created on 11 November, 2017, 12:47 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "mcc_generated_files/mcc.h"
#include "delay.h"
#include "clcd.h"
#include "cmd_consol.h"

    typedef enum {
        CMD_CONSOLE_STATE,
        MAKE_DISPLAY_STATE,
        DISPLAY_STATE,
    } MAIN_STATE;
    MAIN_STATE main_state;

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

