#include "../include/compiler.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Table* new_table() {
    Table* table = (Table*)malloc(sizeof(Table));
    table->total_rows = 0 ;
    table->total_pages = 0;
    for (uint32_t i = 0 ; i < TABLE_MAX_PAGES ; i++) {
        table->pages[i] = NULL;
    }
    table->stats = new_stats();
    return table;
}

void free_table(Table* table) {
    for (uint32_t i = 0 ; table->pages[i] ; i++) { // loop until a page is null
        free(table->pages[i]);
    }
    free(table->stats);
    free(table);
}

MetaCommandResult do_meta_command(InputBuffer *input_buffer, Table* table) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        close_input_buffer(input_buffer);
        free_table(table);
        exit(EXIT_SUCCESS);
    } 
    
    else if (strcmp(input_buffer->buffer, ".debug") == 0) {
        debug(table);
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

int parse_insert_query(InputBuffer* input_buffer , Row* row) {
    int args_assigned = sscanf(input_buffer->buffer, "insert %u %s %s", &row->id, row->email, row->password);
    return args_assigned;
}




void print_stats(Stats* stats) {
    printf("Total rows: %u\n", stats->total_rows);
    printf("Total allocated pages: %u\n", stats->total_allocated_pages);
    
    double KB = (double)stats->total_bytes / 1024;
    double MB = (double)stats->total_bytes / (1024 * 1024);

    printf("Total bytes in KB: %.2f\n", KB);
    printf("Total bytes in MB: %.2f\n", MB);
}

