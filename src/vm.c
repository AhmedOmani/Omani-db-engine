#include "../include/vm.h"
#include "../include/storage.h"
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
    Row row;
    for (uint32_t i = 0 ; i < table->total_rows ; i++) {
        deserialize_row(row_slot(table , i), &row);
        print_row(&row);
    }
    return EXCUTE_SUCCESS;
}

void print_row(Row* row) {
    printf("(%d, %s, %s)\n", row->id, row->email, row->password);
}
