/* 
 * File:   cmd_consol.h
 * Author: Noyel_IQ_Office
 *
 * Created on October 25, 2017, 3:33 PM
 */

#ifndef CMD_CONSOL_H
#define	CMD_CONSOL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>

#define DEV_LOG     1
    #define LOG     1
    
#define     SUCCESS     1
#define     FAILURE     0
    // CMDs    
#define KLM_CMD     "KLM"       //KLM
    #define KLM_CMD_HEADER     "KLM+"

#define KLM_CLCD_SUPPORT    "LCDS?"
    
#define MAX_COLUMN_CMD  "MC?"
#define MAX_ROW_CMD     "MR?"

#define KLM_INIT        "LCDS"      //KLM+LCDS=4,16

#define KLM_MAX_DISPLAY_STRING_RANGE    "GMDS?"  //

#define KLM_SET_DISPLAY_STRING      "SDSTR"       // KLM+SDSTR=39,HELLO_WORLD        //KLM+SDSTR=B,HELLO_WORLD
#define KLM_SET_SCROLL              "SDSCROL"        // KLM+SDSCROL=39,0, KLM+SDSCROL=A,1, KLM+SDSCROL=A,2
                                                            //KLM+SDSCROL=B,1
#define KLM_SET_ROW_COL             "SDSERC"  // KLM+SDSERC=39,3,3,3,9       //KLM+SDSERC=B,0,3,0,8 
#define KLM_SET_FREQUENCY           "SDFRQ"     //KLM+SDFRQ=39,1(sec)
    
#define KLM_UPDATE                  "UPDATE"    //KLM+UPDATE
    
#define KLM_SET_STRING                    "SSTR"   //KLM+SSTR=hello_world...___,0,1,3,10,2,1,1
                                                   //KLM+SSTR=hello_world...___,3,1,3,5,1,1,0
                                                    //KLM+SSTR=hello_world...___,2,7,2,15,1,1,0
                                                    //KLM+SSTR=hello_world...___,2,7,3,15,1,1,0
    
#define KLM_CLEAR_SCREEN                  "CLS"     // KLM+CLS
    
    // Error Message
#define CMD_LENGTH_OVER_MSG         "ERR_CMD_LEN"
#define CMD_SUCCESS_MSG     "OK"
#define CMD_ERROR_MSG               "ERROR"
#define CMD_INIT_NOT_DONE_MSG       "ERR_INIT"
#define DATA_STR_NOT_SET_MSG        "ERR_STR_SET"
#define ROW_COL_NOT_SET_MSG         "ERR_ROW_COL"
#define SCROLL_NOT_SET_MSG          "ERR_SCROLL_SET"
#define ERROR_CONFLICT_MSG          "ERR_CONFLICT"
    
// ERROR Code
#define OK_CMD                  0
#define ERROR_CMD               1
#define ERROR_DATA_LEN          2
#define ERROR_ROW_COL           3   
#define ERROR_SCROLL            4
#define ERROR_FREQUENCY         5
#define ERROR_OVER_WRITE        6
#define ERROR_CONFLICT          7
#define ERROR_UID               8
#define ERROR_INIT_NOT_DONE     9
    
#define OVERWRITE                       1
#define NOT_OVERWRITE                   0
    
#define INVALID_ROW_COL         255



#define MAX_LEN 150
#define MAX_DATA_LEN    40
#define CMD_LEN 15

#define TOKENER         "-', /="  
#define TOKENER_STR         ","  
#define END_OF_LINE     '\n'

    char out_str[100];
    char uart_str[MAX_LEN] = {0};
    char str_data[MAX_DATA_LEN] = {0};
    char CMD[CMD_LEN] = {0};
    bool valid_cmd = false;
    char clcd_init_row = 0;
    char clcd_init_col = 0;
    bool check = false;

    typedef enum {
        CMD_CONSOL_INIT,
        CMD_CONSOL_READ_CMD,
        CMD_CONSOL_REPLY_HOST,
    } CMD_CONSOL_STATE;

    typedef struct {
        unsigned int uid;
        char data[MAX_DATA_LEN];
        bool data_set;
        char start_row;
        char start_col;
        char end_row;
        char end_col;
        bool row_col_set;
        char scroll;
        bool scroll_set;
        char dis_frq;
        bool dis_frq_set;
        char over_write;
    } CMD_CONSOL_DATA;

    CMD_CONSOL_STATE cmd_consol_state;
    extern CMD_CONSOL_DATA cmd_data;
    void console_init();
    bool cmd_consol_task();
#ifdef	__cplusplus
}
#endif

#endif	/* CMD_CONSOL_H */

