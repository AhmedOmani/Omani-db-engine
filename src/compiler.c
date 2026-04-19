#include "../include/compiler.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

MetaCommandResult do_meta_command(InputBuffer *input_buffer, stats* stats) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        close_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    } 
    
    else if (strcmp(input_buffer->buffer, ".stats") == 0) {
        print_stats(stats);
        return META_COMMAND_SUCCESS;
    } 
    
    else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement) {
    
    if (strncmp(input_buffer->buffer , "insert" , 6) == 0) {
        //Set the type of the statement
        statement->type = STATEMENT_INSERT;

        //Parse the insert query and store the values in the row
        Row row = statement->insertion_row;
        int args_assigned = parse_insert_query(input_buffer, &row);

        if (args_assigned < 3 || args_assigned > 3) {
            return PREPARE_SYNTAX_ERROR;
        }
        statement->insertion_row = row;

        return PREPARE_SUCCESS;
    }

    else if (strcmp(input_buffer->buffer , "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement, stats* stats) {
    if (statement->type == STATEMENT_INSERT) {
        printf("id: %u, email: %s, password: %s\n", statement->insertion_row.id, statement->insertion_row.email, statement->insertion_row.password);
        
        printf("This is an insert statement\n");
        stats->total_rows++;
        stats->total_bytes += sizeof(Row);
    }
    
    else if (statement->type == STATEMENT_SELECT) {
        printf("This is an select statement\n");
    }
}

int parse_insert_query(InputBuffer* input_buffer , Row* row) {
    int args_assigned = sscanf(input_buffer->buffer, "insert %u %s %s", &row->id, row->email, row->password);
    return args_assigned;
}

stats* new_stats() {
    stats* stats = malloc(sizeof(stats));
    stats->total_rows = 0;
    stats->total_allocated_pages = 0;
    stats->total_bytes = 0;
    return stats;
}

void print_stats(stats* stats) {
    printf("Total rows: %u\n", stats->total_rows);
    printf("Total allocated pages: %u\n", stats->total_allocated_pages);
    
    double KB = (double)stats->total_bytes / 1024;
    double MB = (double)stats->total_bytes / (1024 * 1024);

    printf("Total bytes in KB: %.2f\n", KB);
    printf("Total bytes in MB: %.2f\n", MB);
}