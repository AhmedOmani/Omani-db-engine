#include "../include/vm.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

ExcuteResult execute_statement(Statement* statement, Table* table) {
    switch (statement->type) {
        case STATEMENT_INSERT:
            return execute_insert(statement, table);
        case STATEMENT_SELECT: 
            return execute_select(statement, table); 
    }
    return EXCUTE_ERROR;
}

ExcuteResult execute_insert(Statement* statement , Table* table) {
    if (table->total_rows >= TABLE_MAX_ROWS) {
        return EXCUTE_TABLE_FULL;
    }

    Row* row = &(statement->insertion_row);
    serialize_row(row, row_slot(table, table->total_rows));
    table->total_rows += 1;
    return EXCUTE_SUCCESS;
}



ExcuteResult execute_select(Statement* statement , Table* table) {
    printf("SELECT STATEMENT IS HERE!\n");
    return EXCUTE_SUCCESS;
}

