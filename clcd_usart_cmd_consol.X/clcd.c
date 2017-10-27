#include "clcd.h"
#include "delay.h"


//unsigned char lcd_display_array[MAX_ROW][MAX_COL] = {0};
static char data_count=0;
static char conflict_data_count=0;
CLCD_CONFIG_DATA clcd_config_data;

void reset_clcd() {
    clear_display_array();
    reset_display_structure_count();
}

void init_CLCD_config(void) {
    memset(&clcd_config_data, 0, sizeof (CLCD_CONFIG_DATA));
    //Config PORTB as output port, for sending the data
    DATA_BUS_DIRECTION = 0x00;

    //Config PORTD as output port, for sending the control signals
    LCD_CONTROL_BUS_DIRECTION = (unsigned char) (LCD_CONTROL_BUS_DIRECTION | 0xF0);
    //LCD_BACK_LIGHT_SetLow();
    EN = 0;
    _delay_MS(250);

    //EIGHT_BIT_TWO_LINE_DISPLAY;
    write_lcd_cmd(TWO_LINE_EIGHT_BIT_DISPLAY_CMD);
    _delay_MS(250);

    DISPLAY_ON_CURSOR_ON;
    _delay_MS(250);

    CLEAR_DISPLAY;
    _delay_MS(250);

    ENTRY_MODE;
    _delay_MS(250);

    write_lcd_cmd(DISPLAY_ON_CURSOR_BLINK_CMD);
    _delay_MS(250);

    memset(lcd_display_array, ' ', sizeof (lcd_display_array));

    clcd_display_string_config();

    reset_clcd();
}

void init_clcd_cusotom_lcd(char clcd_init_row, char clcd_init_col) {
    init_CLCD_config();
    data_count=0;
    memset(clcd_display_string_details, 0, sizeof(clcd_display_string_details));
    memset(conflict_arr, 0, sizeof(conflict_arr));
    clcd_config_data.init_done = true;
    printf("row = %d", clcd_init_row);
    clcd_config_data.clcd_max_row = clcd_init_row;
    printf("col = %d", clcd_init_col);
    clcd_config_data.clcd_max_col = clcd_init_col;
    printf("string_count = %d", (clcd_init_col / DEFAULT_STRING_DIVIDER) * clcd_init_row);
    clcd_config_data.clcd_max_display_string = (clcd_init_col / DEFAULT_STRING_DIVIDER) * clcd_init_row;
    clcd_config_data.uid_range = START_UID + clcd_config_data.clcd_max_display_string;
    printf("uid_range = %d\r\n", clcd_config_data.uid_range);
}

void init_clcd_lcd_16x2() {
    init_CLCD_config();
    clcd_config_data.init_done = true;
    clcd_config_data.clcd_max_col = 16;
    clcd_config_data.clcd_max_row = 2;
    clcd_config_data.clcd_max_display_string = 8;
}

void init_clcd_lcd_20x4() {
    init_CLCD_config();
    clcd_config_data.init_done = true;
    clcd_config_data.clcd_max_col = 20;
    clcd_config_data.clcd_max_row = 4;
    clcd_config_data.clcd_max_display_string = 20;
}

char max_display_str() {
    if (clcd_config_data.init_done == false)
        return 0;
    else
        return clcd_config_data.clcd_max_display_string;
}

char min_uid_range() {
    if (clcd_config_data.init_done == false)
        return 0;
    else
        return START_UID;
}

char max_uid_range() {
    if (clcd_config_data.init_done == false)
        return 0;
    else
        return clcd_config_data.uid_range - 1;
}

char* max_display_support()
{
    return MAX_DISPLAY_SUPPORT;
}

char clcd_max_row() {
    if (clcd_config_data.init_done == false)
        return 0;
    else
        return clcd_config_data.clcd_max_row;
}

char clcd_max_col() {
    if (clcd_config_data.init_done == false)
        return 0;
    else
        return clcd_config_data.clcd_max_col;
}

void copy_data(CLCD_STRING_DISPLAY_DETAILS *clcd_str_data, CMD_CONSOL_DATA *data) {
    char row1, col1, row2, col2;
    clcd_str_data->uid = data->uid;
    strcpy(clcd_str_data->string, data->data);
    clcd_str_data->strlen = strlen(clcd_str_data->string);
    clcd_str_data->start_row = data->start_row;
    clcd_str_data->start_col = data->start_col;
    clcd_str_data->end_row = data->end_row;
    clcd_str_data->end_col = data->end_col;
    clcd_str_data->start_point = (clcd_str_data->start_row * clcd_config_data.clcd_max_col) + clcd_str_data->start_col;
    clcd_str_data->end_point = (clcd_str_data->end_row * clcd_config_data.clcd_max_col) + clcd_str_data->end_col;
    clcd_str_data->direction = data->scroll;
    clcd_str_data->display_char_count = clcd_str_data->end_point - clcd_str_data->start_point;
    printf("display_char_count = %d\r\n", clcd_str_data->display_char_count);
}

bool conflict_check(char exist_uid_idx, char new_start_point, char new_end_point)
{
    int i = 0;
    bool conflict = false;
    
    for (i = 0; i < clcd_config_data.clcd_max_display_string; i++) {
        if(exist_uid_idx != i && clcd_display_string_details[i].uid !=0)
        {
            if(new_start_point >= clcd_display_string_details[i].start_point  && new_start_point <= clcd_display_string_details[i].end_point )
            {
                conflict_arr[conflict_data_count++] = clcd_display_string_details[i].uid;
                i++;
                conflict = true;
                continue;
            }
            if(new_end_point >= clcd_display_string_details[i].start_point && new_end_point >= clcd_display_string_details[i].end_point)
            {
                conflict_arr[conflict_data_count++] = clcd_display_string_details[i].uid;
                i++;conflict = true;
                continue;
            }
            if(new_start_point <= clcd_display_string_details[i].start_point && new_end_point >= clcd_display_string_details[i].end_point){
                conflict_arr[conflict_data_count++] = clcd_display_string_details[i].uid;
                i++;conflict = true;
                continue;
            }
        }
    }
    
    printf("conflict: uid\r\n");
    for (i = 0; i < clcd_config_data.clcd_max_display_string; i++) {
        if(conflict_arr[i]!=0)
        printf("%c, ",conflict_arr[i]);
    }
    printf("\r\n");
    return conflict;
}


char update_string_data(CMD_CONSOL_DATA *data) {
    int i = 0;
    char exist_uid_idx=-1;
    char start_point, end_point;
    
    start_point = (data->start_row * clcd_config_data.clcd_max_col) + data->start_col;
    end_point = (data->end_row * clcd_config_data.clcd_max_col) + data->end_col;
    
    if (data_count < clcd_config_data.clcd_max_display_string) {
        for (i = 0; i < clcd_config_data.clcd_max_display_string; i++) {
            if (clcd_display_string_details[i].uid == data->uid) {
                exist_uid_idx = i;
                conflict_arr[conflict_data_count++] = clcd_display_string_details[i].uid;
                conflict_check(exist_uid_idx, start_point, end_point);
                return CONFLICT_STRING;
            }else
            {
                if(conflict_check(exist_uid_idx, start_point, end_point) == true)
                    return CONFLICT_STRING;
            }
        }
        memset(&clcd_display_string_details[data_count], 0, sizeof (CLCD_STRING_DISPLAY_DETAILS));
        copy_data(&clcd_display_string_details[data_count], data);
        data_count++;
        make_display();
    } else {

    }
    return 0;
}

char str[] = "INIT_SUCCESS";

void clcd_display_string_config() {
    memset(lcd_display_array, ' ', sizeof (lcd_display_array));
    printf("sizeof(str) = %d\r\n", sizeof (str));
    memcpy(lcd_display_array[0], str, sizeof (str));
    write_str(lcd_display_array[0]);
}

void set_cursor(unsigned char row_number, unsigned char col_number) {
    if (col_number > MAX_COL) {
        col_number = 0;
    }
    write_lcd_cmd(col_number + row_number);
}

void set_cursor_at_home() {
    write_lcd_cmd(RETURN_HOME_CMD);
}

void display_and_cursor_control(unsigned char mode) {
    switch (mode) {
        case 0: // DISPLAY_OFF_CURSOR_OFF
            write_lcd_cmd(DISPLAY_OFF_CURSOR_OFF_CMD);
            break;
        case 1: // DISPLAY_OFF_CURSOR_ON
            write_lcd_cmd(DISPLAY_OFF_CURSOR_ON_CMD);
            break;
        case 2: // DISPLAY_ON_CURSOR_BLINK
            write_lcd_cmd(DISPLAY_ON_CURSOR_BLINK_CMD);
            break;
        case 3: // DISPLAY_ON_CURSOR_OFF
            write_lcd_cmd(DISPLAY_ON_CURSOR_OFF_CMD);
            break;
        case 4: // DISPLAY_ON_CURSOR_ON
            write_lcd_cmd(DISPLAY_ON_CURSOR_ON_CMD);
            break;
    }
}

//void scroll_text_right_to_left() {
//    write_lcd_cmd(SHIFT_DISPLAY_LEFT_CMD);
//    //write_lcd_cmd(DECREMENT_CURSOR_CMD);
//}
//
//void scroll_text_left_to_right() {
//    write_lcd_cmd(SHIFT_DISPLAY_RIGHT_CMD);
//    //write_lcd_cmd(INCREMET_CURSOR_CMD);
//}

void char_enter_mode_left_to_right() {
    //    printf("0x%x\r\n", ((0x00 | INCREMET_CURSOR_CMD))& ~INCREMET_CURSOR_CMD);
    //    write_lcd_cmd(((0x00 | INCREMET_CURSOR_CMD))& ~INCREMET_CURSOR_CMD);
    //    _delay_MS(250);
    write_lcd_cmd(INCREMET_CURSOR_CMD);
}

void scroll_display_right() {
    write_lcd_cmd(SHIFT_ENTIRE_DISPLAY_TO_RIGHT_CMD);
}

void scroll_display_left() {
    write_lcd_cmd(SHIFT_ENTIRE_DISPLAY_TO_LEFT_CMD);
}

void is_busy(void) {
    //Config RD7 as an input port
    //LCD_D7_RD7_SetDigitalInput();
    DATA_BUS_DIRECTION = 0xFF;
    //printf("portd.7 = %d\r\n", (int)LCD_D7_RD7_PORT);
    RS = CMD_MODE;
    RW = LCD_READ_MODE;
    _delay_10uS(1);
    do {
        LCD_STROBE;
    } while (BUSY == 1);

    //printf("portd.7 = %d\r\n", (int)LCD_D7_RD7_PORT);
    RW = LCD_WRITE_MODE;
    //Config PORTD as an output
    //LCD_D7_RD7_SetDigitalOutput();
    DATA_BUS_DIRECTION = 0x00;
}

void write_lcd_cmd(unsigned char cmd) {
    DATA_BUS = cmd;
    RS = CMD_MODE;
    RW = LCD_WRITE_MODE;
    LCD_STROBE;
}

void write_lcd_data(unsigned char data) {
    RS = DATA_MODE;
    RW = LCD_WRITE_MODE;
    DATA_BUS = data;
    LCD_STROBE;
}

void write_str(const char* str) {
    //printf("hello_world\r\n");
    unsigned char i = 0;
    while (i < clcd_config_data.clcd_max_col) {
        //if(*str != NULL)
        write_lcd_data(*str++);
        //else
        //write_lcd_data(' ');
        //_delay_MS(15);
        i++;
    }
}

void put_string(unsigned char row_number, unsigned char col_number, const char* str) {
    int str_len = strlen(str);
    unsigned char lcd_location, i = 0;

    lcd_location = row_number + col_number;
    //printf("%s, strlen %d, lcd_location 0x%x\r\n", str, str_len, lcd_location);
    write_lcd_cmd(lcd_location);
    //Wait,if LCD is busy
    is_busy();
    write_str(str);
}

void put_char(unsigned char row_number, unsigned char col_number, const char ch) {
    unsigned char lcd_location, i = 0;
    lcd_location = row_number + col_number;

    write_lcd_cmd(lcd_location);
    //Wait,if LCD is busy
    is_busy();

    write_lcd_data(ch);
    _delay_MS(15);
}

void display_string(unsigned char row_number, const char* str, unsigned char alignment) {
    int str_len = strlen(str);
    printf("%s, strlen %d\r\n", str, str_len);
    unsigned char row, col = 0;
    switch (alignment) {
        case 0:
        {
            row = row_number;
            put_string(row, STARTING_COLUMN_LOC, str);
            break;
        }
        case 1:
        {
            char mid_col = MAX_COL / 2;
            char mid_char = str_len / 2;
            printf("mid_char = %d\r\n", mid_char);
            row = row_number;
            if (mid_char > mid_col) {
                col = (mid_char - mid_col);
                put_string(row, STARTING_COLUMN_LOC, str + col);
            } else if (mid_char <= mid_col) {
                col = (mid_col - mid_char);
                put_string(row, col, str);
            }
            break;
        }
        case 2:
        {
            row = row_number;
            if (str_len > MAX_COL) {
                col = str_len - MAX_COL;
                put_string(row, STARTING_COLUMN_LOC, str + col);
            } else {
                col = MAX_COL - str_len;
                put_string(row, col, str);
            }
            break;
        }
    }
}

void clear_display_structure() {
    memset(clcd_display_string_details, 0, sizeof (clcd_display_string_details));
}

void reset_display_structure_count() {
    display_struct_count = 0;
    clear_display_structure();
}

void clear_display_array() {
    memset(lcd_display_array, ' ', sizeof (lcd_display_array));
    reset_display_structure_count();
}


static void scroll_left_to_right(CLCD_STRING_DISPLAY_DETAILS *clcd_display_string_details) {
    char line = -1;
    if (clcd_display_string_details->start_row == FIRST_ROW)
        line = FIRST_ROW;
    else if (clcd_display_string_details->start_row == SECOND_ROW)
        line = SECOND_ROW;
    if (clcd_display_string_details->start_row == THIRD_ROW)
        line = THIRD_ROW;
    if (clcd_display_string_details->start_row == FOURTH_ROW)
        line = FOURTH_ROW;

    memset(lcd_display_array[line] + clcd_display_string_details->start_col,
            ' ', clcd_display_string_details->display_char_count);

    if (clcd_display_string_details->length_count_for_right_scroll > clcd_display_string_details->display_char_count) {
        if (clcd_display_string_details->string[FIRST_ROW] != NULL) {
            memcpy(lcd_display_array[line] + clcd_display_string_details->start_col,
                    clcd_display_string_details->string +
                    ((clcd_display_string_details->strlen - 1) - clcd_display_string_details->length_decount_for_right_scroll),
                    clcd_display_string_details->display_char_count);
        }
    } else {
        if (clcd_display_string_details->string[FIRST_ROW] != NULL) {
            memcpy(lcd_display_array[line] + clcd_display_string_details->start_col +
                    clcd_display_string_details->length_count_for_right_scroll,
                    clcd_display_string_details->string,
                    clcd_display_string_details->display_char_count - clcd_display_string_details->length_count_for_right_scroll);
        }
    }
    if (clcd_display_string_details->length_count_for_right_scroll <= clcd_display_string_details->display_char_count) {
        clcd_display_string_details->length_count_for_right_scroll++;
    } else if (clcd_display_string_details->length_decount_for_right_scroll < clcd_display_string_details->strlen) {
        clcd_display_string_details->length_decount_for_right_scroll++;
        if (clcd_display_string_details->length_decount_for_right_scroll >= clcd_display_string_details->strlen) {
            clcd_display_string_details->length_count_for_right_scroll = 0;
            clcd_display_string_details->length_decount_for_right_scroll = 0;
        }
    }
}

static void scroll_right_to_left(CLCD_STRING_DISPLAY_DETAILS *clcd_display_string_details) {
    //printf("%d.\n", max_char_len);
    char line = -1;
    if (clcd_display_string_details->start_row == FIRST_ROW)
        line = 0;
    else if (clcd_display_string_details->start_row == SECOND_ROW)
        line = 1;
    if (clcd_display_string_details->start_row == THIRD_ROW)
        line = 2;
    if (clcd_display_string_details->start_row == FOURTH_ROW)
        line = 3;
    //printf("%d\n",clcd_display_string_details->start_col);
    memset(lcd_display_array[line] + clcd_display_string_details->start_col,
            ' ', clcd_display_string_details->display_char_count);
    if (clcd_display_string_details->length_count_for_left_scroll > clcd_display_string_details->strlen) {
        printf("%d", (clcd_display_string_details->end_col - clcd_display_string_details->length_decount_for_left_scroll - 1));
        strncpy(lcd_display_array[line]+(clcd_display_string_details->end_col - clcd_display_string_details->length_decount_for_left_scroll - 1),
                clcd_display_string_details->string, (clcd_display_string_details->display_char_count -
                (clcd_display_string_details->display_char_count - (clcd_display_string_details->length_decount_for_left_scroll + 1))));
    } else {
        if (clcd_display_string_details->string[clcd_display_string_details->length_count_for_left_scroll] != NULL) {
            memcpy(lcd_display_array[line] + clcd_display_string_details->start_col,
                    clcd_display_string_details->string + clcd_display_string_details->length_count_for_left_scroll,
                    clcd_display_string_details->display_char_count);
        } else {
            //memset(lcd_display_array[line1] + clcd_display_string_details[count].start_col, ' ', clcd_display_string_details[count].display_char_count);
        }
    }

    if (clcd_display_string_details->length_count_for_left_scroll <= clcd_display_string_details->strlen)//max_char_len)
    {
        clcd_display_string_details->length_count_for_left_scroll++;
    } else if (clcd_display_string_details->length_count_for_left_scroll > clcd_display_string_details->strlen)//max_char_len)
    {
        clcd_display_string_details->length_decount_for_left_scroll++;
        if (clcd_display_string_details->length_decount_for_left_scroll == clcd_display_string_details->display_char_count) {
            clcd_display_string_details->length_decount_for_left_scroll = 0;
            clcd_display_string_details->length_count_for_left_scroll = 0;
            //return;
        }

    }
}

static void scroll_off(CLCD_STRING_DISPLAY_DETAILS *clcd_display_string_details) {
    char line = -1;

    int i = 0;
    int j = 0, k = 0;
    char display_char_count = clcd_display_string_details->end_point - clcd_display_string_details->start_point;
    char str_len = strlen(clcd_display_string_details->string);
    line = clcd_display_string_details->start_row;
    j = clcd_display_string_details->start_col;
    for (i = 0; i < str_len;) {
        for (j; j < clcd_config_data.clcd_max_col; j++) {
            lcd_display_array[line][j] = clcd_display_string_details->string[i++];
            if (i >= str_len)break;
            if (i >= display_char_count)break;
        }
        printf("i=%d,\r\n", i);
        j = 0;
        line++;
        if (i >= display_char_count)break;
    }
}
//    char str_len = strlen(clcd_display_string_details->string);
//    if (clcd_display_string_details->start_point >= 0 && clcd_display_string_details->start_point < clcd_config_data.clcd_max_col) {
//        line = 0;
//        j = clcd_display_string_details->start_point;
//    } else if (clcd_display_string_details->start_point >= clcd_config_data.clcd_max_col * 1 && clcd_display_string_details->start_point < clcd_config_data.clcd_max_col * 2) {
//        line = 1;
//        j = clcd_display_string_details->start_point;
//    } else if (clcd_display_string_details->start_point >= clcd_config_data.clcd_max_col * 2 && clcd_display_string_details->start_point < clcd_config_data.clcd_max_col * 3) {
//        line = 2;
//        j = clcd_display_string_details->start_point;
//    } else if (clcd_display_string_details->start_point >= clcd_config_data.clcd_max_col * 3 && clcd_display_string_details->start_point < clcd_config_data.clcd_max_col * 4) {
//        line = 3;
//        j = clcd_display_string_details->start_point;
//    }
//    for (i = 0; i < str_len; i++) {
//        
//    }
//    if (clcd_display_string_details->start_point == LINE1_HOME)
//        line = 0;
//    else if (clcd_display_string_details->line_addr == LINE2_HOME)
//        line = 1;
//
//    memset(lcd_display_array[line] + clcd_display_string_details->start_col,
//            ' ', clcd_display_string_details->display_char_count);
//
//    memcpy(lcd_display_array[line] + clcd_display_string_details->start_col,
//            clcd_display_string_details->ptr,
//            clcd_display_string_details->display_char_count);
//}

/*
void make_display() {
    unsigned int max_char_len_right_scroll = 0, max_display_len_right_scroll = 0, max_char_len_right_scroll_struct_id = 0;
    unsigned int max_char_len_left_scroll = 0, max_display_len_left_scroll = 0, max_char_len_left_scroll_struct_id = 0;
    int count = 0;
    for (count = 0; count < MAX_DISPLAY_STRUCT; count++) {
        if (clcd_display_string_details[count].ptr != NULL && clcd_display_string_details[count].direction == SCROLL_RIGHT_TO_LEFT) 
        {
            max_char_len_right_scroll = max_char_len_right_scroll > clcd_display_string_details[count].strlen ? 
                                            max_char_len_right_scroll : clcd_display_string_details[count].strlen;
            //printf("%d\n", max_char_len_right_scroll);
            max_display_len_right_scroll = max_display_len_right_scroll > clcd_display_string_details[count].display_char_count ? 
                                                max_display_len_right_scroll : clcd_display_string_details[count].display_char_count;
        } else if (clcd_display_string_details[count].ptr != NULL && clcd_display_string_details[count].direction == SCROLL_LEFT_TO_RIGHT) 
        {
            max_char_len_left_scroll = max_char_len_left_scroll > clcd_display_string_details[count].strlen ? 
                                            max_char_len_left_scroll : clcd_display_string_details[count].strlen;
            max_display_len_left_scroll = max_display_len_left_scroll > clcd_display_string_details[count].display_char_count ? 
                                            max_display_len_left_scroll : clcd_display_string_details[count].display_char_count;
        }
    }
    //printf("%d.", max_char_len_right_scroll);

    for (count = 0; count < MAX_DISPLAY_STRUCT; count++) {
        if (clcd_display_string_details[count].direction == SCROLL_RIGHT_TO_LEFT) {
            if (clcd_display_string_details[count].line_addr == LINE1_HOME) {
                scroll_right_to_left(max_char_len_right_scroll, &clcd_display_string_details[count]);
            } 
            else if (clcd_display_string_details[count].line_addr == LINE2_HOME) {
                scroll_right_to_left(max_char_len_right_scroll, &clcd_display_string_details[count]);
            }
        } else if (clcd_display_string_details[count].direction == SCROLL_LEFT_TO_RIGHT) {
            if (clcd_display_string_details[count].line_addr == LINE1_HOME) {

            } 
            else if (clcd_display_string_details[count].line_addr == LINE2_HOME) {

            }
        }else
        {
            scroll_off(&clcd_display_string_details[count]);
        }
    }
    
    print_data_ready = true;
    display_text();
}
 */

void make_display() {
    int count = 0;
    for (count = 0; count < clcd_config_data.clcd_max_display_string; count++) {
        if (strlen(clcd_display_string_details[count].string) != 0 && clcd_display_string_details[count].direction == SCROLL_LEFT_TO_RIGHT) {
            scroll_left_to_right(&clcd_display_string_details[count]);
        }
        else if (strlen(clcd_display_string_details[count].string) != 0 && clcd_display_string_details[count].direction == SCROLL_RIGHT_TO_LEFT) {
            scroll_right_to_left(&clcd_display_string_details[count]);
        }else if (strlen(clcd_display_string_details[count].string) != 0 && clcd_display_string_details[count].direction == SCROLL_OFF) {
            scroll_off(&clcd_display_string_details[count]);
        }
    }
    print_data_ready = true;
    display_text();
}

void display_text() {
    if (print_data_ready == true) {
        CLEAR_DISPLAY; //_delay_MS(250);
        char_enter_mode_left_to_right(); //_delay_MS(250);
        set_cursor(LINE1_HOME, 0);
        write_str(lcd_display_array[0]);
        set_cursor(LINE2_HOME, 0);
        write_str(lcd_display_array[1]);
        set_cursor(LINE3_HOME, 0);
        write_str(lcd_display_array[2]);
        set_cursor(LINE4_HOME, 0);
        write_str(lcd_display_array[3]);
        print_data_ready = false;
    } else {
        print_data_ready = false;
    }
}