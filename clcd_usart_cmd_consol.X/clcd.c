#include "clcd.h"
#include "delay.h"
#include "mcc_generated_files/eusart1.h"
#include <time.h> 

//unsigned char lcd_display_array[MAX_ROW][MAX_COL] = {0};
static char data_count = 0;
static char uid_count = START_UID;
static char conflict_data_count = 0;
CLCD_CONFIG_DATA clcd_config_data;

void reset_clcd() {
    clear_display_array();
    reset_display_structure_count();
}

void init_CLCD_config(void) {
    memset(&clcd_config_data, 0, sizeof (CLCD_CONFIG_DATA));
    //Config PORTB as output port, for sending the data
    DATA_BUS_DIRECTION_1 = 0x00;

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

    write_lcd_cmd(DISPLAY_ON_CURSOR_OFF_CMD);
    _delay_MS(250);

    memset(lcd_display_array, ' ', sizeof (lcd_display_array));

    clcd_display_string_config();

    reset_clcd();
}

void init_clcd_cusotom_lcd(char clcd_init_row, char clcd_init_col) {
    init_CLCD_config();
    data_count = 0;
    memset(clcd_display_string_details, 0, sizeof (clcd_display_string_details));
    memset(conflict_arr, 0, sizeof (conflict_arr));
    clcd_config_data.init_done = true;
#if DEV_LOG
    printf("row = %d", clcd_init_row);
#endif
    clcd_config_data.clcd_max_row = clcd_init_row;
#if DEV_LOG
    printf("col = %d", clcd_init_col);
#endif
    clcd_config_data.clcd_max_col = clcd_init_col;
#if DEV_LOG
    printf("string_count = %d", (clcd_init_col / DEFAULT_STRING_DIVIDER) * clcd_init_row);
#endif
    clcd_config_data.clcd_max_display_string = (clcd_init_col / DEFAULT_STRING_DIVIDER) * clcd_init_row;
    clcd_config_data.uid_range = START_UID + clcd_config_data.clcd_max_display_string;
#if DEV_LOG
    printf("uid_range = %d\r\n", clcd_config_data.uid_range);
#endif
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

char* max_display_support() {
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
    clcd_str_data->display_char_count = (clcd_str_data->end_point - clcd_str_data->start_point) + 1;
    printf("display_char_count = %d\r\n", clcd_str_data->display_char_count);
}

bool conflict_check(char exist_uid_idx, char new_start_point, char new_end_point) {
    int i = 0;
    bool conflict_bool = false;

    for (i = 0; i < clcd_config_data.clcd_max_display_string;) {
        if (exist_uid_idx != i && clcd_display_string_details[i].uid != 0) {
            if (new_start_point >= clcd_display_string_details[i].start_point && new_start_point <= clcd_display_string_details[i].end_point) {
                conflict_arr[conflict_data_count++] = clcd_display_string_details[i].uid;
                i++;
                printf("ex_sp <= new_sp <=ex_ep\r\n");
                conflict_bool = true;
                continue;
            }
            if (new_end_point >= clcd_display_string_details[i].start_point && new_end_point <= clcd_display_string_details[i].end_point) {
                conflict_arr[conflict_data_count++] = clcd_display_string_details[i].uid;
                printf("ex_sp <= new_ep <=ex_ep\r\n");
                i++;
                conflict_bool = true;
                continue;
            }
            if (new_start_point <= clcd_display_string_details[i].start_point && new_end_point >= clcd_display_string_details[i].end_point) {
                conflict_arr[conflict_data_count++] = clcd_display_string_details[i].uid;
                i++;
                conflict_bool = true;
                continue;
            }
        }
        i++;
    }

    //    printf("conflict: uid\r\n");
    //    for (i = 0; i < clcd_config_data.clcd_max_display_string; i++) {
    //        if(conflict_arr[i]!=-1)
    //        printf("%c, ",conflict_arr[i]);
    //    }
    //printf("\r\n");
    return conflict_bool;
}

bool conflict(char new_start_point, char new_end_point) {
    int i = 0;
    bool conflict_bool = false;

    for (i = 0; i < clcd_config_data.clcd_max_display_string;) {
        if (clcd_display_string_details[i].uid != 0) {
            if (new_start_point >= clcd_display_string_details[i].start_point && new_start_point <= clcd_display_string_details[i].end_point) {
                conflict_arr[conflict_data_count++] = clcd_display_string_details[i].uid;
                i++;
#if DEV_LOG
                printf("ex_sp <= new_sp <=ex_ep\r\n");
#endif
                conflict_bool = true;
                continue;
            }
            if (new_end_point >= clcd_display_string_details[i].start_point && new_end_point <= clcd_display_string_details[i].end_point) {
                conflict_arr[conflict_data_count++] = clcd_display_string_details[i].uid;
#if DEV_LOG
                printf("ex_sp <= new_ep <=ex_ep\r\n");
#endif
                i++;
                conflict_bool = true;
                continue;
            }
            if (new_start_point <= clcd_display_string_details[i].start_point && new_end_point >= clcd_display_string_details[i].end_point) {
                conflict_arr[conflict_data_count++] = clcd_display_string_details[i].uid;
                i++;
                conflict_bool = true;
                continue;
            }
        }
        i++;
    }

    //    printf("conflict: uid\r\n");
    //    for (i = 0; i < clcd_config_data.clcd_max_display_string; i++) {
    //        if(conflict_arr[i]!=-1)
    //        printf("%c, ",conflict_arr[i]);
    //    }
    //printf("\r\n");
    return conflict_bool;
}

void copy_display_struct(CLCD_STRING_DISPLAY_DETAILS *clcd_str_data, CMD_CONSOL_DATA *data, char uid) {
    clcd_str_data->uid = uid;
    strcpy(clcd_str_data->string, data->data);
#if DEV_LOG
    printf("display_data = %s\r\n", clcd_str_data->string);
#endif
    clcd_str_data->strlen = strlen(clcd_str_data->string);
    clcd_str_data->start_row = data->start_row;
    clcd_str_data->start_col = data->start_col;
    clcd_str_data->end_row = data->end_row;
    clcd_str_data->end_col = data->end_col;
    clcd_str_data->start_point = (clcd_str_data->start_row * clcd_config_data.clcd_max_col) + clcd_str_data->start_col;
    clcd_str_data->end_point = (clcd_str_data->end_row * clcd_config_data.clcd_max_col) + clcd_str_data->end_col;
    clcd_str_data->direction = data->scroll;
    clcd_str_data->display_char_count = (clcd_str_data->end_point - clcd_str_data->start_point) + 1;
    clcd_str_data->blink_interval = data->dis_frq;
#if DEV_LOG
    printf("display_char_count = %d\r\n", clcd_str_data->display_char_count);
#endif
}

int get_new_uid() {
    int i = 1;
    srand(i);
    int uid = (rand() % 100) + 1; // in the range 1 to 100
    for (i = 0; i < clcd_config_data.clcd_max_display_string;) {
        if (clcd_display_string_details[i].uid != 0 && clcd_display_string_details[i].uid != uid) {
            i++;
            continue;
        } else if (clcd_display_string_details[i].uid == 0) {
            i++;
            continue;
        } else {
            uid = (rand() % 100) + 1; // in the range 1 to 100
            i = 0;
            continue;
        }
    }
    return uid;
}

void print_conflict_ids() {
    int conflict_count = 0, j;
    for (conflict_count = 0; conflict_count < conflict_data_count; conflict_count++) {
        if (conflict_count == conflict_data_count - 1) {
            memset(out_str, 0, sizeof (out_str));
            sprintf(out_str, "%d\r\n", conflict_arr[conflict_count]);
            put_string_usart1(out_str);
            printf("%d\r\n", conflict_arr[conflict_count]);
        } else {
            memset(out_str, 0, sizeof (out_str));
            sprintf(out_str, "%d, ", conflict_arr[conflict_count]);
            put_string_usart1(out_str);
            printf("%d, ", conflict_arr[conflict_count]);
        }
    }
}

unsigned char set_string_data(CMD_CONSOL_DATA *data) {
    char i = 0;
    char uid = 0;
    char start_point, end_point;
    //    memset(conflict_arr, -1, sizeof (conflict_arr));
    //    conflict_data_count = 0;
    start_point = (data->start_row * clcd_config_data.clcd_max_col) + data->start_col;
    end_point = (data->end_row * clcd_config_data.clcd_max_col) + data->end_col;
    uid = get_new_uid();
#if DEV_LOG
    printf("UID = %d, start_pt = %d, end_pt = %d\r\n", uid, start_point, end_point);
#endif
    if (data->over_write != OVERWRITE) {
        memset(conflict_arr, -1, sizeof (conflict_arr));
        conflict_data_count = 0;
        if (conflict(start_point, end_point) != true) {
            if (data_count < clcd_config_data.clcd_max_display_string) {
                for (i = 0; i < clcd_config_data.clcd_max_display_string; i++) {
                    if (clcd_display_string_details[i].uid == 0) {
                        memset(&clcd_display_string_details[i], 0, sizeof (CLCD_STRING_DISPLAY_DETAILS));
                        copy_display_struct(&clcd_display_string_details[i], data, uid);
                        data_count++;
                        return uid;
                    }
                }
            } else {
                // TODO:
            }
        } else {
            return CONFLICT_STRING;
        }
    } else {
        if (conflict_data_count != 0) {
#if DEV_LOG
            printf("conflict...2\r\n");
#endif
            int conflict_count = 0, j;
            for (conflict_count = 0; conflict_count < conflict_data_count; conflict_count++) {
                for (j = 0; j < clcd_config_data.clcd_max_display_string; j++) {
                    if (clcd_display_string_details[j].uid == conflict_arr[conflict_count]) {
#if DEV_LOG
                        printf("%d..j=%d,conflict_count=%d \r\n", clcd_display_string_details[j].uid, j, conflict_count);
#endif
                        memset(&clcd_display_string_details[j], 0, sizeof (CLCD_STRING_DISPLAY_DETAILS));
                        data_count--;
                        break;
                    }
                }
            }
            memset(conflict_arr, -1, sizeof (conflict_arr));
            conflict_data_count = 0;
            return CONFLICT_STRING_OVERWRITE;
        } else {
            if (conflict(start_point, end_point) == true) {
                //set_string_data(data);      // recursive call for over write new string with old conflicted string
                return CONFLICT_STRING_CHECK_AND_OVERWRITE;
            } else {
                return CONFLICT_STRING_NOT_CONFLICT_WITH_OVERWRITE_ARGUMENT;
            }
        }
    }
    return 0;
}

char update_string_data(CMD_CONSOL_DATA *data) {
    int i = 0;
    char exist_uid_idx = -1;
    char start_point, end_point;
    memset(conflict_arr, -1, sizeof (conflict_arr));
    conflict_data_count = 0;
    start_point = (data->start_row * clcd_config_data.clcd_max_col) + data->start_col;
    end_point = (data->end_row * clcd_config_data.clcd_max_col) + data->end_col;

    printf("UID = %c, start_pt = %d, end_pt = %d\r\n", data->uid, start_point, end_point);

    if (data_count < clcd_config_data.clcd_max_display_string) {
        for (i = 0; i < clcd_config_data.clcd_max_display_string; i++) {
            if (clcd_display_string_details[i].uid == data->uid) {
                exist_uid_idx = i;
                conflict_arr[conflict_data_count++] = clcd_display_string_details[i].uid;
                conflict_check(exist_uid_idx, start_point, end_point);
                return CONFLICT_STRING;
            } else {
                if (conflict_check(exist_uid_idx, start_point, end_point) == true) {
                    printf("conflict: uid\r\n");
                    for (i = 0; i < clcd_config_data.clcd_max_display_string; i++) {
                        if (conflict_arr[i] != -1)
                            printf("%c, ", conflict_arr[i]);
                    }
                    printf("\r\n");
                    return CONFLICT_STRING;
                }
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
#if DEV_LOG
    printf("sizeof(str) = %d\r\n", sizeof (str));
#endif
    memcpy(lcd_display_array[0], str, sizeof (str));
    write_str(lcd_display_array[0]);
}

void set_cursor(unsigned char row_number, unsigned char col_number) {
    if (col_number > MAX_COL) {
        col_number = 0;
    }
    write_lcd_cmd(col_number + row_number);
    //Wait,if LCD is busy
    is_busy();
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
    LCD_D7_RC5_SetDigitalInput();
    //DATA_BUS_DIRECTION_1 = 0xFF;
    //DATA_BUS_DIRECTION_2 = DATA_BUS_DIRECTION_2 & 0xFF;
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
    LCD_D7_RC5_SetDigitalOutput();
    //DATA_BUS_DIRECTION_1 = 0x00;
    //DATA_BUS_DIRECTION_2 = DATA_BUS_DIRECTION_2 & 0xDF;
}

void write_lcd_cmd(unsigned char cmd) {
    DATA_BUS_1 = DATA_BUS_1 & 0xC0;
    DATA_BUS_1 = (cmd & 0x3F) | (DATA_BUS_1); // use 0-5 pin for databus of PORTD
    DATA_BUS_2 = DATA_BUS_2 & 0xCF;
    cmd = cmd & 0xC0;
    cmd = cmd >> 2;
    DATA_BUS_2 = (cmd | DATA_BUS_2); // 4-5 pin for data bus of PORTC
    RS = CMD_MODE;
    RW = LCD_WRITE_MODE;
    LCD_STROBE;
}

void write_lcd_data(unsigned char data) {
    RS = DATA_MODE;
    RW = LCD_WRITE_MODE;
    DATA_BUS_1 = DATA_BUS_1 & 0xC0;
    DATA_BUS_1 = (data & 0x3F) | (DATA_BUS_1); // use 0-5 pin for databus of PORTD
    DATA_BUS_2 = DATA_BUS_2 & 0xCF;
    data = data & 0xC0;
    data = data >> 2;
    DATA_BUS_2 = (data | DATA_BUS_2); // 4-5 pin for data bus of PORTC
    LCD_STROBE;
}

void write_str(const char* str) {
    //printf("hello_world\r\n");
    unsigned char i = 0;
    while (i < clcd_config_data.clcd_max_col) {
        if (*str != NULL) {
            write_lcd_data(*str++);
        } else {
            write_lcd_data(' ');
            str++;
        }
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
            strncpy(lcd_display_array[line] + clcd_display_string_details->start_col,
                    clcd_display_string_details->string +
                    ((clcd_display_string_details->strlen - 1) - clcd_display_string_details->length_decount_for_right_scroll),
                    clcd_display_string_details->display_char_count);
        }
    } else {
        if (clcd_display_string_details->string[FIRST_ROW] != NULL) {
            strncpy(lcd_display_array[line] + clcd_display_string_details->start_col +
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
        //printf("%d", (clcd_display_string_details->end_col - clcd_display_string_details->length_decount_for_left_scroll - 1));
        strncpy(lcd_display_array[line]+(clcd_display_string_details->end_col - clcd_display_string_details->length_decount_for_left_scroll),
                clcd_display_string_details->string, (clcd_display_string_details->display_char_count -
                (clcd_display_string_details->display_char_count - (clcd_display_string_details->length_decount_for_left_scroll + 1))));
    } else {
        if (clcd_display_string_details->string[clcd_display_string_details->length_count_for_left_scroll] != NULL) {
            strncpy(lcd_display_array[line] + clcd_display_string_details->start_col,
                    clcd_display_string_details->string + clcd_display_string_details->length_count_for_left_scroll,
                    clcd_display_string_details->display_char_count);
        } else {
            memset(lcd_display_array[line] + clcd_display_string_details->start_col,
                    ' ', clcd_display_string_details->display_char_count);
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
    char display_char_count = clcd_display_string_details->display_char_count; //clcd_display_string_details->end_point - clcd_display_string_details->start_point+1;
    char str_len = strlen(clcd_display_string_details->string);
    line = clcd_display_string_details->start_row;
    j = clcd_display_string_details->start_col;
    for (i = 0; i < str_len;) {
        for (j; j < clcd_config_data.clcd_max_col; j++) {
            lcd_display_array[line][j] = clcd_display_string_details->string[i++];
            if (i >= str_len)break;
            if (i >= display_char_count)break;
        }
        //printf("i=%d,\r\n", i);
        j = 0;
        line++;
        if (i >= display_char_count)break;
    }
}

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
    
    memset(lcd_display_array, ' ', sizeof (lcd_display_array));
    for (count = 0; count < clcd_config_data.clcd_max_display_string; count++) {
        if (strlen(clcd_display_string_details[count].string) != 0 && clcd_display_string_details[count].direction == SCROLL_LEFT_TO_RIGHT) {
            if (clcd_display_string_details[count].blink_interval_count == clcd_display_string_details[count].blink_interval) {
                scroll_left_to_right(&clcd_display_string_details[count]);
                clcd_display_string_details[count].blink_interval_count = 0;
            } else {
                clcd_display_string_details[count].blink_interval_count++;
            }
        } else if (strlen(clcd_display_string_details[count].string) != 0 && clcd_display_string_details[count].direction == SCROLL_RIGHT_TO_LEFT) {
            if (clcd_display_string_details[count].blink_interval_count == clcd_display_string_details[count].blink_interval) {
                scroll_right_to_left(&clcd_display_string_details[count]);
                clcd_display_string_details[count].blink_interval_count = 0;
            } else {
                clcd_display_string_details[count].blink_interval_count++;
            }
        } else if (strlen(clcd_display_string_details[count].string) != 0 && clcd_display_string_details[count].direction == SCROLL_OFF) {
            if (clcd_display_string_details[count].blink_interval_count == clcd_display_string_details[count].blink_interval) {
                scroll_off(&clcd_display_string_details[count]);
                clcd_display_string_details[count].blink_interval_count = 0;
            } else {
                clcd_display_string_details[count].blink_interval_count++;
            }
        }
    }
    print_data_ready = true;
    display_text();
}

void display_text() {
    if (print_data_ready == true) {
        //CLEAR_DISPLAY; //_delay_MS(250);
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