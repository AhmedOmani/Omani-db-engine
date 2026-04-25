#include "include/repl.h"
#include "include/compiler.h"
#include "include/vm.h"

#include <stdio.h>
#include <stdbool.h>

int main () {
    Table* table = db_open("omani.db");
    InputBuffer* input_buffer = new_input_buffer();

    while (true) {
        
        print_prompt();
        read_input(input_buffer);
        printf("%s\n" , input_buffer->buffer);

        if (input_buffer->buffer[0] == '.') {
            switch(do_meta_command(input_buffer, table)) {
                case META_COMMAND_SUCCESS:
                    continue;
                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    printf("Unrecognized meta command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }

        //Determine the type of statement
        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case PREPARE_SUCCESS:
                break;
            case PREPARE_EMAIL_TOO_LONG:
                printf("Error: Email is too long.\n");
                continue;
            case PREPARE_PASSWORD_TOO_LONG:
                printf("Error: Password is too long.\n");
                continue;
            case PREPARE_SYNTAX_ERROR:
                printf("Syntax error! Could not parse statement.\n");
                continue;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized statement '%s'\n", input_buffer->buffer);
                continue;
        }

        switch (execute_statement(&statement, table)) {
            case EXCUTE_SUCCESS:
                printf("Command excuted successfully.\n");
                break;
            case EXCUTE_TABLE_FULL:
                printf("Error: Table is full.\n");
                break;
            case EXCUTE_ERROR:
                printf("Error: Could not execute statement.\n");
                break;
        }
    }
    return 0;
}