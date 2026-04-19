#include "include/repl.h"
#include "include/compiler.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

int main () {
    InputBuffer* input_buffer = new_input_buffer();
    stats* stats = new_stats();

    while (true) {
        
        print_prompt();
        read_input(input_buffer);
        printf("%s\n" , input_buffer->buffer);

        if (input_buffer->buffer[0] == '.') {
            switch(do_meta_command(input_buffer, stats)) {
                case META_COMMAND_SUCCESS:
                    continue;
                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    printf("Unrecognized meta command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }

        Statement statement;

        switch (prepare_statement(input_buffer, &statement)) {
            case PREPARE_SUCCESS:
                break;
            case PREPARE_SYNTAX_ERROR:
                printf("Syntax error! Could not parse statement.\n");
                continue;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized statement '%s'\n", input_buffer->buffer);
                continue;
        }

        execute_statement(&statement, stats);
        printf("Command excuted successfully.\n");
    }
    return 0;
}