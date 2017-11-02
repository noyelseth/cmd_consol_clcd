/* 
 * File:   clcd.h
 * Author: Noyel
 *
 * Created on October 19, 2017, 4:38 PM
 */

#ifndef CLCD_H
#define	CLCD_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include "mcc_generated_files/pin_manager.h"
#include "delay.h"
#include "cmd_consol.h"


    char LCD_16x2_DISPLAY;
    char LCD_20x4_DISPLAY;

    // LCD_Config_CMD
#define TWO_LINE_EIGHT_BIT_DISPLAY_CMD      0X38            /*  0x001110XX      (RB7-RB0)
                                                                (0x001 DL N F X X)
                                                                DL = 1 (8 BIT_MODE DATA_BUS)
                                                                N = 1   (2-LINE DISPLAY)
                                                                F = 0 (5x8 DOT FORMAT CHAR DISPLAY)
                                                                Note: F = 1 (5x11 DOT FORMAT Char Display)
                                                            */
#define TWO_LINE_ELEVEN_BIT_DISPLAY_CMD      0X3C           // 0x001111XX

#define CLEAR_DISPLAY_CMD       0x01
#define RETURN_HOME_CMD     0x02

#define DECREMENT_CURSOR_CMD    0x04    //(shift to left)
#define INCREMET_CURSOR_CMD     0x06    //(shift to right)

#define SHIFT_DISPLAY_RIGHT_CMD 0x05
#define SHIFT_DISPLAY_LEFT_CMD  0x07

#define DISPLAY_OFF_CURSOR_OFF_CMD  0x08
#define DISPLAY_OFF_CURSOR_ON_CMD   0x0A
#define DISPLAY_ON_CURSOR_OFF_CMD   0x0C
#define DISPLAY_ON_CURSOR_ON_CMD    0x0E
#define DISPLAY_ON_CURSOR_BLINK_CMD 0x0F

#define SHIFT_CURSOR_POSITION_TO_LEFT_CMD   0X10
#define SHIFT_CURSOR_POSITION_TO_RIGHT_CMD   0X14

#define SHIFT_ENTIRE_DISPLAY_TO_LEFT_CMD    0x18
#define SHIFT_ENTIRE_DISPLAY_TO_RIGHT_CMD    0x1C


#define EIGHT_BIT_TWO_LINE_DISPLAY		write_lcd_cmd(TWO_LINE_EIGHT_BIT_DISPLAY_CMD);
#define DISPLAY_ON_CURSOR_ON			write_lcd_cmd(DISPLAY_ON_CURSOR_ON_CMD);
#define CLEAR_DISPLAY					write_lcd_cmd(CLEAR_DISPLAY_CMD);
#define ENTRY_MODE						write_lcd_cmd(INCREMET_CURSOR_CMD);

#define MAX_ROW     4
#define MAX_COL     20

#define MAX_CORDINATE_OF_ROW_1      0x93
#define MAX_CORDINATE_OF_ROW_2      0xD3
#define MAX_CORDINATE_OF_ROW_3      0xA7
#define MAX_CORDINATE_OF_ROW_4      0xE7

#define LINE1_HOME						0x80
#define LINE2_HOME						0xC0
#define LINE3_HOME						0x94
#define LINE4_HOME						0xD4

#define FIRST_ROW   0
#define SECOND_ROW   1
#define THIRD_ROW   2
#define FOURTH_ROW   3


#define DEFAULT_STRING_DIVIDER          4

#define START_UID       'A'

#define MAX_DISPLAY_SUPPORT     "4,20"

    unsigned char lcd_display_array[MAX_ROW][MAX_COL];

    typedef struct {
        bool init_done;
        char clcd_max_row;
        char clcd_max_col;
        char uid_range;
        char clcd_max_display_string;
    } CLCD_CONFIG_DATA;
    extern CLCD_CONFIG_DATA clcd_config_data;
    void init_CLCD_config(void);
    void init_clcd_cusotom_lcd(char clcd_init_row, char clcd_init_col);

#define RS      LCD_RS_RA2_PORT
#define RW      LCD_RW_RA1_PORT
#define EN      LCD_EN_RA0_PORT
#define BUSY    LCD_D7_RD7_PORT

#define CMD_MODE 0
#define DATA_MODE 1

#define LCD_WRITE_MODE  0
#define LCD_READ_MODE  1

#define DATA_BUS    PORTD
#define DATA_BUS_DIRECTION  TRISD

#define LCD_CONTROL_BUS_DIRECTION   TRISA

#define LCD_STROBE {EN = 1; \
					delay(1);\
					EN = 0;}

#define STARTING_COLUMN_LOC            0

#define MAX_CHAR_LEN_IN_EACH_ROW    40  // if string length > 40 then from 40th char will print 2nd line. 0-39

#define BLANK_SPACE     ' '

#define ALIGN_LEFT     0
#define ALIGN_MIDDLE    1
#define ALIGN_RIGHT      2

#define DISPLAY_OFF_CURSOR_OFF_MODE         0
#define DISPLAY_OFF_CURSOR_ON_MODE          1
#define DISPLAY_ON_CURSOR_BLINK_MODE        2
#define DISPLAY_ON_CURSOR_OFF_MODE          3
#define DISPLAY_ON_CURSOR_ON_MODE           4

#define SCROLL_OFF                          0
#define SCROLL_RIGHT_TO_LEFT                1
#define SCROLL_LEFT_TO_RIGHT                2

#define MAX_DISPLAY_DATA_LEN    40
    
#define CONFLICT_STRING                             200
#define CONFLICT_STRING_CHECK_AND_OVERWRITE         201
#define CONFLICT_STRING_OVERWRITE                   202
#define CONFLICT_STRING_NOT_CONFLICT_WITH_OVERWRITE_ARGUMENT           203
    
    typedef struct {
        unsigned int uid;
        unsigned char string[MAX_DISPLAY_DATA_LEN];
        unsigned int strlen : 7;
        unsigned char start_row : 7;
        unsigned char start_col : 7;
        unsigned char start_point : 7;
        unsigned char end_row : 7;
        unsigned char end_col : 7;
        unsigned char end_point : 7;
        unsigned char direction : 3;
        unsigned char display_char_count;
        //unsigned char display_loop_count;
        unsigned char length_count_for_right_scroll : 7;
        unsigned char length_decount_for_right_scroll : 7;
        unsigned char length_count_for_left_scroll : 7;
        unsigned char length_decount_for_left_scroll : 7;
        //unsigned char eol;
    } CLCD_STRING_DISPLAY_DETAILS;

#define MAX_DISPLAY_STRUCT     20

    CLCD_STRING_DISPLAY_DETAILS clcd_display_string_details[MAX_DISPLAY_STRUCT];
    int conflict_arr[MAX_DISPLAY_STRUCT];

    int print_data_ready;

    int display_struct_count = 0;

    void clcd_display_string_config();

    void set_cursor(unsigned char row_number, unsigned char col_number);
    void set_cursor_at_home();

    void display_and_cursor_control(unsigned char mode);

    void scroll_text_right_to_left();
    void scroll_text_left_to_right();
    void char_enter_mode_left_to_right();

    void scroll_display_right();
    void scroll_display_left();

    void write_lcd_cmd(unsigned char cmd);
    void write_lcd_data(unsigned char data);

    void is_busy(void);

    void display_string(unsigned char row_number, const char* str, unsigned char alignment);

    void write_str(const char* str);

    void put_string(unsigned char row_number, unsigned char col_number, const char* str);
    void put_char(unsigned char row_number, unsigned char col_number, const char ch);

    void display_text();

    void make_display();

    void print_str(unsigned char row_number, unsigned char start_col_number,
            unsigned char end_col_number, const char* str, unsigned char direction);

    void clear_display_structure();
    void reset_display_structure_count();
    void clear_display_array();

    char clcd_max_row();
    char clcd_max_col();

    char max_display_str();
    char min_uid_range();
    char max_uid_range();
    char* max_display_support();
    char update_string_data(CMD_CONSOL_DATA *data);
    
    unsigned char set_string_data(CMD_CONSOL_DATA *data);
    void print_conflict_ids();

#ifdef	__cplusplus
}
#endif

#endif	/* CLCD_H */

