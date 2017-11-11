#include <string.h>
#include "mcc_generated_files/mcc.h"
#include "cmd_consol.h"
#include "clcd.h"
#include <ctype.h>
#include <stdio.h>
CMD_CONSOL_DATA cmd_data;

void clear_cmd_data() {
    memset(&cmd_data, 0, sizeof (CMD_CONSOL_DATA));
    //cmd_data.dis_frq = 1;
    cmd_data.scroll = -1;
    cmd_data.end_col = cmd_data.start_col = -1;
    cmd_data.end_row = cmd_data.end_row = -1;
}

void collect_row(char *str, char *data_row) {
#if DEV_LOG
    printf("row1 = %s\r\n", str);
#endif
    char row = 0;
    int i = 0;
    for (i = 0; i < strlen(str); i++) {
        if (isdigit(str[i]) == 1)
            row = row * 10 + (str[i] - '0');
        else
            return;
    }
#if DEV_LOG
    printf("row2 = %d\r\n", row);
#endif
    if (row >= clcd_config_data.clcd_max_row) {
        *data_row = -1;
    } else {
        *data_row = row;
    }
}

void get_uid(char *str, unsigned int *data_uid) {
    unsigned int uid;
    unsigned int i = 0;
    for (i = 0; i < strlen(str); i++) {
        if (isdigit(str[i]) == 1)
            uid = uid * 10 + (str[i] - '0');
        else
            return;
    }
    if (uid >= 1 && uid <= 100) {
        *data_uid = 0;
    } else {
        *data_uid = uid;
    }
}

char get_str(char* str, char* data_str) {
    if (strlen(str) < MAX_DATA_LEN) {
        strcpy(data_str, str);
        valid_cmd = true;
        cmd_data.data_set = true;
        memset(str, 0, strlen(str));
        return SUCCESS;
    }
    return FAILURE;
}

char get_direction(char* str, char *data_direction) {
    char direction = 0;
    int i = 0;
    for (i = 0; i < strlen(str); i++) {
        if (isdigit(str[i]) == 1)
            direction = direction * 10 + (str[i] - '0');
        else
            return FAILURE;
    }
#if DEV_LOG
    printf("direction = %d\r\n", direction);
#endif
    if (direction >= clcd_config_data.clcd_max_row) {
        *data_direction = -1;
        return FAILURE;
    } else {
        *data_direction = direction;
        return SUCCESS;
    }
}

char get_dis_frq(char* str, char *data_frq) {
    char frq = 0;
    int i = 0;
    for (i = 0; i < strlen(str); i++) {
        if (isdigit(str[i]) == 1) {
            frq = frq * 10 + (str[i] - '0');
        } else {
            *data_frq = 1;

            return FAILURE;
        }
    }
#if DEV_LOG
    printf("display_frq = %d\r\n", frq);
#endif

    *data_frq = frq;
    return SUCCESS;
}

char get_over_write_data(char* str, char *over_write_data) {
    char over_write = 0;
    int i = 0;
    for (i = 0; i < strlen(str); i++) {
        if (isdigit(str[i]) == 1) {
            over_write = over_write * 10 + (str[i] - '0');
        } else {
            *over_write_data = 0;
            return FAILURE;
        }
    }
#if DEV_LOG
    printf("over_write = %d\r\n", over_write);
#endif
    *over_write_data = over_write;
    return SUCCESS;
}

void collect_column(char *str, char *data_column) {
#if DEV_LOG
    printf("col1 = %s\r\n", str);
#endif
    int i = 0;
    char column = 0;
    for (i = 0; i < strlen(str); i++) {
        if (isdigit(str[i]) == 1) {
            column = column * 10 + (str[i] - '0');
            //printf("strlen = %d, i=%d, col.. = %d\r\n", strlen(str), i, column);
            //printf("str[%d] = %d\r\n", i, str[i]);
        } else {
            //printf("i = %d, col..2 = %s\r\n", i, str);
            //printf("str[%d] = %d\r\n", i, str[i]);
            return;
        }

    }
#if DEV_LOG
    printf("Col2 = %d\r\n", column);
#endif
    if (column >= clcd_config_data.clcd_max_col) {
        *data_column = -1;
    } else {
        *data_column = column;
    }
}

char parse_cmd_str(char *str) {
#if LOG
    printf("%s..2\r\n", str);
#endif
    char *temp;
    char *ptr = str;
    temp = strtok(ptr, TOKENER);
    if (temp != NULL) {
        if (strcmp(temp, MAX_COLUMN_CMD) == 0) {
            //        temp = strtok(ptr, TOKENER);
            if (temp != NULL) {
                strcpy(CMD, temp);
                valid_cmd = true;
                memset(str, 0, strlen(str));
                return OK_CMD;
            }
        } else if (strcmp(temp, MAX_ROW_CMD) == 0) {
            //temp = strtok(ptr, TOKENER);
            if (temp != NULL) {
                strcpy(CMD, temp);
                valid_cmd = true;
                memset(str, 0, strlen(str));
                return OK_CMD;
            }
        } else if (strcmp(temp, KLM_CLCD_SUPPORT) == 0) {
            //temp = strtok(ptr, TOKENER);
            if (temp != NULL) {
                strcpy(CMD, temp);
                valid_cmd = true;
                memset(str, 0, strlen(str));
#if DEV_LOG
                printf(".....\r\n");
#endif
                return OK_CMD;
            }
        } else if (strcmp(temp, KLM_INIT) == 0) {
            //temp = strtok(ptr, TOKENER);
            if (temp != NULL) {
                strcpy(CMD, temp);
                temp = strtok(NULL, TOKENER);
#if DEV_LOG
                printf("%s..\r\n", temp);
#endif
                char row = 0, column = 0;
                if (temp != NULL) {
                    row = atoi(temp);
                    if (row > 0) {
                        temp = strtok(NULL, TOKENER);
                        if (temp != NULL) {
                            column = atoi(temp);
                            if (column > 0) {
                                clcd_init_col = column;
                                clcd_init_row = row;
                                valid_cmd = true;
                                memset(str, 0, strlen(str));
                                return OK_CMD;
                            }
                        }
                    }
                }
            }

        } else if (strcmp(temp, KLM_MAX_DISPLAY_STRING_RANGE) == 0) {

            //temp = strtok(ptr, TOKENER);
            if (temp != NULL) {
                strcpy(CMD, temp);
                valid_cmd = true;
                memset(str, 0, strlen(str));
                return OK_CMD;
            }

        } else if (strcmp(temp, KLM_SET_DISPLAY_STRING) == 0) {
            if (clcd_config_data.init_done == true) {
                clear_cmd_data();
                //temp = strtok(ptr, TOKENER);
                if (temp != NULL) {
#if DEV_LOG
                    printf("temp = %s, ", temp);
#endif
                    strcpy(CMD, temp);
                    temp = strtok(NULL, TOKENER);
                    if (temp != NULL) {
#if DEV_LOG
                        printf("temp = %s, ", temp);
#endif
                        get_uid(temp, &cmd_data.uid);
                        if (cmd_data.uid != 0) {
                            temp = strtok(NULL, TOKENER);
                            if (temp != NULL) {
                                if (strlen(temp) < MAX_DATA_LEN) {
                                    strcpy(cmd_data.data, temp);
                                    valid_cmd = true;
                                    cmd_data.data_set = true;
                                    memset(str, 0, strlen(str));
                                    return OK_CMD;
                                } else {
                                    return ERROR_DATA_LEN;
                                }
                            }
                        } else {
                            return ERROR_UID;
                        }
                    }
                }
            } else {
                return ERROR_INIT_NOT_DONE;
            }
        } else if (strcmp(temp, KLM_SET_SCROLL) == 0) {
#if DEV_LOG
            printf("%s...4\r\n", temp);
#endif
            if (clcd_config_data.init_done == true) {
                clear_cmd_data();
                //temp = strtok(ptr, TOKENER);
                if (temp != NULL) {
                    strcpy(CMD, temp);
                    temp = strtok(NULL, TOKENER);
                    if (temp != NULL) {
#if DEV_LOG
                        printf("%s...5\r\n", temp);
#endif
                        get_uid(temp, &cmd_data.uid);
                        if (cmd_data.uid != 0) {
                            temp = strtok(NULL, TOKENER);
                            if (temp != NULL) {
#if DEV_LOG
                                printf("%s...6\r\n", temp);
#endif
                                get_direction(temp, &cmd_data.scroll);
                                if (cmd_data.scroll < 3 && cmd_data.scroll != -1) {
                                    cmd_data.scroll_set = true;
                                    valid_cmd = true;
                                    memset(str, 0, strlen(str));
                                    return OK_CMD;
                                } else {
                                    return ERROR_SCROLL;
                                }
                            }
                        } else {
                            return ERROR_UID;
                        }
                    }
                }
            } else {
                return ERROR_INIT_NOT_DONE;
            }
        } else if (strcmp(temp, KLM_SET_ROW_COL) == 0) {
            if (clcd_config_data.init_done == true) {
                clear_cmd_data();
                //temp = strtok(ptr, TOKENER);
                if (temp != NULL) {
                    strcpy(CMD, temp);
                    temp = strtok(NULL, TOKENER);
                    if (temp != NULL) {
                        get_uid(temp, &cmd_data.uid);
                        if (cmd_data.uid != 0) {
#if DEV_LOG
                            printf("uid = %d,\r\n", cmd_data.uid);
#endif
                            temp = strtok(NULL, TOKENER);
                            if (temp != NULL) {
                                collect_row(temp, &cmd_data.start_row);
#if DEV_LOG
                                printf("cmd_data.start_row = %d,\r\n", cmd_data.start_row);
#endif
                                if (cmd_data.start_row != INVALID_ROW_COL) {
                                    temp = strtok(NULL, TOKENER);
                                    if (temp != NULL) {
                                        collect_column(temp, &cmd_data.start_col);
#if DEV_LOG
                                        printf("cmd_data.start_col = %d,\r\n", cmd_data.start_col);
#endif
                                        if (cmd_data.start_row != INVALID_ROW_COL) {
                                            temp = strtok(NULL, TOKENER);
                                            if (temp != NULL) {
                                                collect_row(temp, &cmd_data.end_row);
#if DEV_LOG
                                                printf("cmd_data.end_row = %d,\r\n", cmd_data.end_row);
#endif
                                                if (cmd_data.end_row != INVALID_ROW_COL) {
                                                    temp = strtok(NULL, TOKENER);
                                                    if (temp != NULL) {
#if DEV_LOG
                                                        printf("col = %s\r\n", temp);
#endif
                                                        collect_column(temp, &cmd_data.end_col);
#if DEV_LOG
                                                        printf("cmd_data.end_col = %d,\r\n", cmd_data.end_col);
#endif
                                                        if (cmd_data.end_row != INVALID_ROW_COL) {
                                                            cmd_data.row_col_set = true;
                                                            valid_cmd = true;
                                                            memset(str, 0, strlen(str));
                                                            return OK_CMD;
                                                        } else {
                                                            return ERROR_ROW_COL;
                                                        }
                                                    }
                                                } else {
                                                    return ERROR_ROW_COL;
                                                }
                                            }
                                        } else {
                                            return ERROR_ROW_COL;
                                        }
                                    }
                                } else {
                                    return ERROR_ROW_COL;
                                }
                            }
                        } else {
                            return ERROR_UID;
                        }
                    }
                }
            } else {
                return ERROR_INIT_NOT_DONE;
            }
        } else if (strcmp(temp, KLM_UPDATE) == 0) {
            strcpy(CMD, temp);
            memset(str, 0, strlen(str));
            valid_cmd = true;
            return OK_CMD;
        } else if (strcmp(temp, KLM_SET_STRING) == 0) {
            if (clcd_config_data.init_done == true) {
                clear_cmd_data();
                //temp = strtok(ptr, TOKENER);
                if (temp != NULL) {
                    strcpy(CMD, temp);
#if DEV_LOG
                    printf("CMD=%s\r\n", CMD);
#endif
                    temp = strtok(NULL, TOKENER_STR);
                    if (temp != NULL) {
                        if (get_str(temp, cmd_data.data) == SUCCESS) {
#if DEV_LOG
                            printf("data=%s\r\n", cmd_data.data);
#endif
                            temp = strtok(NULL, TOKENER);
                            if (temp != NULL) {
                                collect_row(temp, &cmd_data.start_row);
#if DEV_LOG
                                printf("cmd_data.start_row = %d,\r\n", cmd_data.start_row);
#endif
                                if (cmd_data.start_row != 255) {
                                    temp = strtok(NULL, TOKENER);
                                    if (temp != NULL) {
                                        collect_column(temp, &cmd_data.start_col);
#if DEV_LOG
                                        printf("cmd_data.start_col = %d,\r\n", cmd_data.start_col);
#endif
                                        if (cmd_data.start_row != 255) {
                                            temp = strtok(NULL, TOKENER);
                                            if (temp != NULL) {
                                                collect_row(temp, &cmd_data.end_row);
#if DEV_LOG
                                                printf("cmd_data.end_row = %d,\r\n", cmd_data.end_row);
#endif
                                                if (cmd_data.end_row != 255) {
                                                    temp = strtok(NULL, TOKENER);
                                                    if (temp != NULL) {
#if DEV_LOG
                                                        printf("col = %s\r\n", temp);
#endif
                                                        collect_column(temp, &cmd_data.end_col);
#if DEV_LOG
                                                        printf("cmd_data.end_col = %d,\r\n", cmd_data.end_col);
#endif
                                                        if (cmd_data.end_col != 255) {
                                                            temp = strtok(NULL, TOKENER);
                                                            if (temp != NULL) {
                                                                if (get_direction(temp, &cmd_data.scroll) == SUCCESS) {
                                                                    temp = strtok(NULL, TOKENER);
                                                                    if (temp != NULL) {
                                                                        if (get_dis_frq(temp, &cmd_data.dis_frq) == SUCCESS) {
                                                                            temp = strtok(NULL, TOKENER);
                                                                            if (temp != NULL) {
                                                                                if (get_over_write_data(temp, &cmd_data.over_write) == SUCCESS) {
                                                                                    cmd_data.row_col_set = true;
                                                                                    valid_cmd = true;
                                                                                    memset(str, 0, strlen(str));
                                                                                    return OK_CMD;
                                                                                } else {
                                                                                    return ERROR_OVER_WRITE;
                                                                                }
                                                                            }
                                                                        } else {
                                                                            return ERROR_FREQUENCY;
                                                                        }
                                                                    }
                                                                } else {
                                                                    return ERROR_SCROLL;
                                                                }
                                                            }
                                                        } else {
                                                            return ERROR_ROW_COL;
                                                        }
                                                    }
                                                } else {
                                                    return ERROR_ROW_COL;
                                                }
                                            }
                                        } else {
                                            return ERROR_ROW_COL;
                                        }
                                    }
                                } else {
                                    return ERROR_ROW_COL;
                                }
                            }
                        } else {
                            return ERROR_DATA_LEN;
                        }
                    }
                }
            } else {
                return ERROR_INIT_NOT_DONE;
            }
        } else if (strcmp(temp, KLM_CMD) == 0) {
            strcpy(CMD, temp);
            memset(str, 0, strlen(str));
            valid_cmd = true;
            return OK_CMD;
        }
        memset(str, 0, strlen(str));
        return ERROR_CMD;
    }
    memset(str, 0, strlen(str));
    return ERROR_CMD;
}

void reply_to_host() {
    if (strcmp(CMD, KLM_CMD) == 0) { //KLM
        memset(out_str, 0, sizeof (out_str));
        sprintf(out_str, "OK\r\n");
        put_string_usart1(out_str);
        puts("OK\r\n");
    } else if (strcmp(CMD, KLM_CLCD_SUPPORT) == 0) { // KLM+LCDS?
        printf("%s\r\n%s\r\n", max_display_support(), CMD_SUCCESS_MSG);
    } else if (strcmp(CMD, MAX_COLUMN_CMD) == 0) { //KLM+MC?
        printf("%d\r\n%s\r\n", clcd_max_col(), CMD_SUCCESS_MSG);
    } else if (strcmp(CMD, MAX_ROW_CMD) == 0) { //KLM+MR?
        printf("%d\r\n%s\r\n", clcd_max_row(), CMD_SUCCESS_MSG);
    } else if (strcmp(CMD, KLM_INIT) == 0) { //KLM+LCDS=
        init_clcd_cusotom_lcd(clcd_init_row, clcd_init_col);
        memset(out_str, 0, sizeof (out_str));
        sprintf(out_str, "%s\r\n", CMD_SUCCESS_MSG);
        put_string_usart1(out_str);
    } else if (strcmp(CMD, KLM_MAX_DISPLAY_STRING_RANGE) == 0) { //KLM+GMDS?
        if (max_display_str() != 0) {
            printf("%d,%c-%c\r\n", max_display_str(), min_uid_range(), max_uid_range());
        } else {
            printf("%d\r\n", max_display_str());
        }
        printf("%s.., OK\r\n", KLM_MAX_DISPLAY_STRING_RANGE);
    } else if (strcmp(CMD, KLM_SET_DISPLAY_STRING) == 0) { //KLM+SDSTR
        printf("%s.., OK\r\n", KLM_SET_DISPLAY_STRING);
    } else if (strcmp(CMD, KLM_SET_SCROLL) == 0) { //  KLM+SDSCROL
        printf("%s.., OK\r\n", KLM_SET_SCROLL);
    } else if (strcmp(CMD, KLM_SET_ROW_COL) == 0) { //  KLM+SDSERC
        printf("%s.., OK\r\n", KLM_SET_ROW_COL);
    } else if (strcmp(CMD, KLM_UPDATE) == 0) { //  KLM+UPDATE
        if (cmd_data.data_set == true && cmd_data.row_col_set == true && cmd_data.scroll_set == true) {
            char ch = update_string_data(&cmd_data);
            if (ch == CONFLICT_STRING)
                printf("%s\r\n", CMD_ERROR_MSG);
            else
                printf("%s.., OK\r\n", KLM_UPDATE);
        } else {
            if (cmd_data.data_set == false) {
                printf("%s\r\n", DATA_STR_NOT_SET_MSG);
            } else if (cmd_data.row_col_set == false) {
                printf("%s\r\n", ROW_COL_NOT_SET_MSG);
            } else if (cmd_data.scroll_set == false) {
                printf("%s\r\n", SCROLL_NOT_SET_MSG);
            }
        }
    } else if (strcmp(CMD, KLM_SET_STRING) == 0) { //KLM+SSTR
        unsigned char ch = set_string_data(&cmd_data);
        if (ch == CONFLICT_STRING) {
            print_conflict_ids();
            memset(out_str, 0, sizeof (out_str));
            sprintf(out_str, "%s\r\n", ERROR_CONFLICT_MSG);
            put_string_usart1(out_str);
        } else if (ch == CONFLICT_STRING_CHECK_AND_OVERWRITE) {
            ch = set_string_data(&cmd_data);
            if (ch == CONFLICT_STRING_OVERWRITE) {
                cmd_data.over_write = NOT_OVERWRITE;
                ch = set_string_data(&cmd_data);
                memset(out_str, 0, sizeof (out_str));
                sprintf(out_str, "%d\r\nOK\r\n", ch);
                put_string_usart1(out_str);
            }
        } else if (ch == CONFLICT_STRING_OVERWRITE) {
            cmd_data.over_write = NOT_OVERWRITE;
            ch = set_string_data(&cmd_data);
            memset(out_str, 0, sizeof (out_str));
            sprintf(out_str, "%d\r\nOK\r\n", ch);
            put_string_usart1(out_str);
        } else if (ch == CONFLICT_STRING_NOT_CONFLICT_WITH_OVERWRITE_ARGUMENT) {
            cmd_data.over_write = NOT_OVERWRITE;
            ch = set_string_data(&cmd_data);
            memset(out_str, 0, sizeof (out_str));
            sprintf(out_str, "%d\r\nOK\r\n", ch);
            put_string_usart1(out_str);
        } else {
            memset(out_str, 0, sizeof (out_str));
            sprintf(out_str, "%d\r\nOK\r\n", ch);
            put_string_usart1(out_str);
        }
    }
}

void console_init() {
    cmd_consol_state = CMD_CONSOL_INIT;
    clear_cmd_data();
    valid_cmd = false;
    clcd_init_row = 0;
    clcd_init_col = 0;
    check = false;
    memset(CMD, 0, sizeof (CMD));
    memset(str_data, 0, sizeof (str_data));
    memset(uart_str, 0, sizeof (uart_str));
}

bool cmd_consol_task() {
    bool res = false;
    if (EUSART1_DataReady > 0) {
        static int i = 0;
        unsigned char ch = 0;
        while (EUSART1_DataReady) {
            ch = EUSART1_Read();
            if (ch == '\n' || ch == '\r') {
                i = 0;
                check = true;
                printf("\r\n");
                break;
            } else if (ch == 8) {
                uart_str[--i] = 0;
                printf("%c", ch);
                continue;
            } else {
                uart_str[i] = ch;
                printf("%c", uart_str[i]);
            }
            /*
              if (strcmp(uart_str, KLM_CMD_HEADER) == 0) {
                memset(uart_str, 0, sizeof (uart_str));
                i = 0;
                continue;
            }
             */
            i++;
        }
        if (i > MAX_LEN) {
            puts(CMD_LENGTH_OVER_MSG);
            i = 0;
        }
    }
    switch (cmd_consol_state) {
        case CMD_CONSOL_INIT:
        {
            memset(CMD, 0, sizeof (CMD));
            memset(uart_str, 0, sizeof (uart_str));
            valid_cmd = false;
            memset(out_str, 0, sizeof (out_str));
            sprintf(out_str, "Init_Done\r\n");
            put_string_usart1(out_str);
            printf("%s..\r\n", out_str);
            cmd_consol_state = CMD_CONSOL_READ_CMD;
            break;
        }
        case CMD_CONSOL_READ_CMD:
        {
            static int i = 0;

            if (check == true) {
                check = false;
                char temp_uart_str[MAX_LEN] = {0};
                memset(temp_uart_str, 0, sizeof (temp_uart_str));
                strcpy(temp_uart_str, uart_str);
                if (strstr(temp_uart_str, KLM_CMD_HEADER) != NULL) {
#if DEV_LOG
                    printf("%s\r\n", uart_str);
#endif
                    memset(uart_str, 0, sizeof (uart_str));
                    strcpy(uart_str, temp_uart_str + strlen(KLM_CMD_HEADER));
                } else {
#if DEV_LOG
                    printf("%s\r\n", uart_str);
#endif
                    strcpy(uart_str, temp_uart_str);
                }
                char val = parse_cmd_str(uart_str);
                if (OK_CMD == val) {
                    memset(temp_uart_str, 0, sizeof (temp_uart_str));
                    memset(uart_str, 0, sizeof (uart_str));
                    cmd_consol_state = CMD_CONSOL_REPLY_HOST;
                } else if (ERROR_INIT_NOT_DONE == val) {
                    memset(out_str, 0, sizeof (out_str));
                    sprintf(out_str, "%s\r\n", CMD_INIT_NOT_DONE_MSG);
                    put_string_usart1(out_str);
                    printf("%s\r\n\r\n", out_str);
                } else {
                    memset(out_str, 0, sizeof (out_str));
                    sprintf(out_str, "%s\r\n", CMD_ERROR_MSG);
                    put_string_usart1(out_str);
                    printf("%s\r\n\r\n", out_str);
                }
            }
            break;
        }
        case CMD_CONSOL_REPLY_HOST:
        {
            if (valid_cmd == true) {
                reply_to_host();
                valid_cmd = false;
                //valid_cmd = true;
            }
            cmd_consol_state = CMD_CONSOL_READ_CMD;
            res = true;
            break;
        }
    }
    return res;

}
