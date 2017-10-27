#include <string.h>
#include "mcc_generated_files/mcc.h"
#include "cmd_consol.h"
#include "clcd.h"
#include <ctype.h>
CMD_CONSOL_DATA cmd_data;

void clear_cmd_data() {
    memset(&cmd_data, 0, sizeof (CMD_CONSOL_DATA));
    //cmd_data.dis_frq = 
    cmd_data.scroll = -1;
    cmd_data.end_col = cmd_data.start_col = -1;
    cmd_data.end_row = cmd_data.end_row = -1;
}

void collect_row(char *str, char *data_row) {
    char row = 0;
    int i = 0;
    for (i = 0; i < strlen(str); i++) {
        if (isdigit(str[i]) == 1)
            row = row * 10 + (str[i] - '0');
        else
            return;
    }
    printf("row = %d\r\n", row);
    if (row >= clcd_config_data.clcd_max_row) {
        *data_row = -1;
    } else {
        *data_row = row;
    }
}

void collect_column(char *str, char *data_column) {
    printf("col..1 = %s\r\n", str);
    int i = 0;
    char column = 0;
    for (i = 0; i < strlen(str); i++) {
        if (isdigit(str[i]) == 1){
            column = column * 10 + (str[i] - '0');
            printf("strlen = %d, i=%d, col.. = %d\r\n",strlen(str),i, column);
            printf("str[%d] = %d\r\n", i, str[i]);
        }
        else{
            printf("i = %d, col..2 = %s\r\n",i, str);
            printf("str[%d] = %d\r\n", i, str[i]);
            return;
        }
        
    }
    printf("Col = %d\r\n", column);
    if (column >= clcd_config_data.clcd_max_col) {
        *data_column = -1;
    } else {
        *data_column = column;
    }
}

char parse_cmd_str(char *str) {
    printf("%s..2\r\n", str);
    char *temp;
    char *ptr = str;
    if ((temp = strstr(ptr, MAX_COLUMN_CMD)) != NULL) {
        temp = strtok(ptr, TOKENER);
        if (temp != NULL) {
            strcpy(CMD, temp);
            valid_cmd = true;
            memset(str, 0, strlen(str));
            return OK_CMD;
        }
    } else if ((temp = strstr(ptr, MAX_ROW_CMD)) != NULL) {
        temp = strtok(ptr, TOKENER);
        if (temp != NULL) {
            strcpy(CMD, temp);
            valid_cmd = true;
            memset(str, 0, strlen(str));
            return OK_CMD;
        }
    } else if ((temp = strstr(ptr, KLM_CLCD_SUPPORT) != NULL)) {
        temp = strtok(ptr, TOKENER);
        if (temp != NULL) {
            strcpy(CMD, temp);
            valid_cmd = true;
            memset(str, 0, strlen(str));
            printf(".....\r\n");
            return OK_CMD;
        }
    } else if ((temp = strstr(ptr, KLM_INIT)) != NULL) {
        temp = strtok(ptr, TOKENER);
        if (temp != NULL) {
            strcpy(CMD, temp);
            temp = strtok(NULL, TOKENER);
            printf("%s..\r\n", temp);
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

    } else if ((temp = strstr(ptr, KLM_MAX_DISPLAY_STRING_RANGE)) != NULL) {

        temp = strtok(ptr, TOKENER);
        if (temp != NULL) {
            strcpy(CMD, temp);
            valid_cmd = true;
            memset(str, 0, strlen(str));
            return OK_CMD;
        }

    } else if ((temp = strstr(ptr, KLM_SET_DISPLAY_STRING)) != NULL) {
        if (clcd_config_data.init_done == true) {
            cmd_data.data_set = false;
            temp = strtok(ptr, TOKENER);
            if (temp != NULL) {
                printf("temp = %s, ", temp);
                strcpy(CMD, temp);
                char uid = 0;
                cmd_data.uid = -1;
                temp = strtok(NULL, TOKENER);
                if (temp != NULL) {
                    printf("temp = %s, ", temp);
                    char i = 0;
                    uid = temp[0];
                    if (uid >= START_UID && uid < clcd_config_data.uid_range) {
                        printf("uid = %d, ", uid);
                        cmd_data.uid = uid;
                        temp = strtok(NULL, TOKENER);
                        if (temp != NULL) {
                            printf("str = %s\r\n", temp);
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
        }
    } else if ((temp = strstr(ptr, KLM_SET_SCROLL)) != NULL) {
        printf("%s...4\r\n", temp);
        if (clcd_config_data.init_done == true) {
            cmd_data.scroll_set = false;
            temp = strtok(ptr, TOKENER);
            if (temp != NULL) {
                strcpy(CMD, temp);
                char scroll = 0;
                char uid = 0;
                temp = strtok(NULL, TOKENER);
                if (temp != NULL) {
                    printf("%s...5\r\n", temp);
                    char i = 0;
                    uid = temp[0];
                    cmd_data.scroll = -1;
                    if (uid >= START_UID && uid < clcd_config_data.uid_range) {
                        printf("uid = %d, ", uid);
                        if (uid == cmd_data.uid) {
                            temp = strtok(NULL, TOKENER);
                            if (temp != NULL) {
                                printf("%s...6\r\n", temp);
                                for (i = 0; i < strlen(temp); i++) {
                                    if (isdigit(temp[i]) == 1) {
                                        scroll = scroll * 10 + (temp[i] - '0');
                                    } else {
                                        //printf("%s...7\r\n", temp);
                                        return false;
                                    }
                                }
                                //printf("scroll = %d, ", scroll);
                                if (scroll < 3) {
                                    printf("scroll = %d, ", scroll);
                                    cmd_data.scroll = scroll;
                                    cmd_data.scroll_set = true;
                                    valid_cmd = true;
                                    memset(str, 0, strlen(str));
                                    return OK_CMD;
                                } else {
                                    return ERROR_SCROLL;
                                }
                            }
                        }
                    } else {
                        return ERROR_UID;
                    }
                }
            }
        }
    } else if ((temp = strstr(ptr, KLM_SET_ROW_COL)) != NULL) {
        if (clcd_config_data.init_done == true) {
            cmd_data.row_col_set = false;
            temp = strtok(ptr, TOKENER);
            if (temp != NULL) {
                strcpy(CMD, temp);
                temp = strtok(NULL, TOKENER);
                char uid = 0;
                if (temp != NULL) {
                    char i = 0;
                    uid = temp[0];
                    if (uid >= START_UID && uid < clcd_config_data.uid_range) {
                        printf("uid = %d,\r\n", uid);
                        if (uid == cmd_data.uid) {
                            temp = strtok(NULL, TOKENER);
                            if (temp != NULL) {
                                collect_row(temp, &cmd_data.start_row);
                                printf("cmd_data.start_row = %d,\r\n", cmd_data.start_row);
                                if (cmd_data.start_row != 255) {
                                    temp = strtok(NULL, TOKENER);
                                    if (temp != NULL) {
                                        collect_column(temp, &cmd_data.start_col);
                                        printf("cmd_data.start_col = %d,\r\n", cmd_data.start_col);
                                        if (cmd_data.start_row != 255) {
                                            temp = strtok(NULL, TOKENER);
                                            if (temp != NULL) {
                                                collect_row(temp, &cmd_data.end_row);
                                                printf("cmd_data.end_row = %d,\r\n", cmd_data.end_row);
                                                if (cmd_data.end_row != 255) {
                                                    temp = strtok(NULL, TOKENER);
                                                    if (temp != NULL) {
                                                        printf("col = %s\r\n", temp);
                                                        collect_column(temp, &cmd_data.end_col);
                                                        printf("cmd_data.end_col = %d,\r\n", cmd_data.end_col);
                                                        if (cmd_data.end_row != 255) {
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
                        }
                    } else {
                        return ERROR_UID;
                    }
                }
            }
        }
    } else if ((temp = strstr(ptr, KLM_UPDATE)) != NULL) {
        strcpy(CMD, temp);
        memset(str, 0, strlen(str));
        valid_cmd = true;
        return OK_CMD;
    } else if ((temp = strstr(ptr, KLM_CMD)) != NULL) {
        strcpy(CMD, temp);
        memset(str, 0, strlen(str));
        valid_cmd = true;
        return OK_CMD;
    }
    memset(str, 0, strlen(str));
    return ERROR_CMD;
}

void reply_to_host() {
    if (strcmp(CMD, KLM_CMD) == 0) { //KLM
        puts("OK\r\n");
    } else if (strcmp(CMD, KLM_CLCD_SUPPORT) == 0) { // KLM+LCDS?
        printf("%s\r\n", max_display_support());
        printf("\r\n%s.., OK\r\n", KLM_CLCD_SUPPORT);
    } else if (strcmp(CMD, MAX_COLUMN_CMD) == 0) { //KLM+MC?
        printf("%d\r\n", clcd_max_col());
        printf("\r\n%s.., OK\r\n", MAX_COLUMN_CMD);
    } else if (strcmp(CMD, MAX_ROW_CMD) == 0) { //KLM+MR?
        printf("%d\r\n", clcd_max_row());
        printf("\r\n%s.., OK\r\n", MAX_ROW_CMD);
    } else if (strcmp(CMD, KLM_INIT) == 0) { //KLM+LCDS=
        init_clcd_cusotom_lcd(clcd_init_row, clcd_init_col);
        printf("\r\n%s.., OK\r\n", KLM_INIT);
    } else if (strcmp(CMD, KLM_MAX_DISPLAY_STRING_RANGE) == 0) { //KLM+GMDS?
        if (max_display_str() != 0) {
            printf("%d,%c-%c\r\n", max_display_str(), min_uid_range(), max_uid_range());
        } else {
            printf("%d\r\n", max_display_str());
        }
        printf("\r\n%s.., OK\r\n", KLM_MAX_DISPLAY_STRING_RANGE);
    } else if (strcmp(CMD, KLM_SET_DISPLAY_STRING) == 0) { //KLM+SDSTR
        printf("\r\n%s.., OK\r\n", KLM_SET_DISPLAY_STRING);
    } else if (strcmp(CMD, KLM_SET_SCROLL) == 0) { //  KLM+SDSCROL
        printf("\r\n%s.., OK\r\n", KLM_SET_SCROLL);
    } else if (strcmp(CMD, KLM_SET_ROW_COL) == 0) { //  KLM+SDSERC
        printf("\r\n%s.., OK\r\n", KLM_SET_ROW_COL);
    } else if (strcmp(CMD, KLM_UPDATE) == 0) { //  KLM+UPDATE
        if (cmd_data.data_set == true && cmd_data.row_col_set == true && cmd_data.scroll_set == true) {
            char ch = update_string_data(&cmd_data);
            if(ch == CONFLICT_STRING)
                printf("%s\r\n",CMD_ERROR);
            else
                printf("\r\n%s.., OK\r\n", KLM_UPDATE);
        } else {
            if (cmd_data.data_set == false) {
                printf("\r\n%s\r\n", DATA_STR_NOT_SET);
            } else if (cmd_data.row_col_set == false) {
                printf("\r\n%s\r\n", ROW_COL_NOT_SET);
            } else if (cmd_data.scroll_set == false) {
                printf("\r\n%s\r\n", SCROLL_NOT_SET);
            }
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

void cmd_consol_task() {
    if (EUSART1_DataReady > 0) {
        static int i = 0;
        unsigned char ch = 0;
        while (EUSART1_DataReady) {
            ch = getch();
            if (ch == '\n' || ch == '\r') {
                i = 0;
                check = true;
                printf("\r\n");
                break;
            } else {
                uart_str[i] = ch;
                printf("%c", uart_str[i]);
            }
            i++;
        }
        if (i > MAX_LEN) {
            puts(CMD_LENGTH_OVER);
            i = 0;
        }
    }
    switch (cmd_consol_state) {
        case CMD_CONSOL_INIT:
        {
            memset(CMD, 0, sizeof (CMD));
            memset(uart_str, 0, sizeof (uart_str));
            valid_cmd = false;
            puts("Init_Done\r\n");
            cmd_consol_state = CMD_CONSOL_READ_CMD;
            break;
        }
        case CMD_CONSOL_READ_CMD:
        {
            static int i = 0;

            if (check == true) {
                check = false;
                if (parse_cmd_str(uart_str) == OK_CMD) {
                    memset(uart_str, 0, sizeof (uart_str));
                    cmd_consol_state = CMD_CONSOL_REPLY_HOST;
                } else {
                    printf("%s\r\n", CMD_ERROR);
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
            break;
        }
    }

}
