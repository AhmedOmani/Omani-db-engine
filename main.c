#include "include/repl.h"
#include "include/compiler.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

int main () {
    InputBuffer* input_buffer = new_input_buffer();

    while (true) {
        
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buffer[0] == '.') {
            switch(do_meta_command(input_buffer)) {
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
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized statement '%s'\n", input_buffer->buffer);
                continue;
        }

        execute_statement(&statement);
        printf("Command excuted successfully.\n");
    }
    return 0;
}