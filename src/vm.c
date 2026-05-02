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
    Cursor* cursor = table_end(table);
    serialize_row(row, cursor_value(cursor));
    table->total_rows += 1;

    free(cursor);

    return EXCUTE_SUCCESS;
}

ExcuteResult execute_select(Statement* statement , Table* table) {
    Row row;
    Cursor* cursor = table_start(table);
    while (!(cursor->end_of_table)) {
        deserialize_row(cursor_value(cursor), &row);
        print_row(&row);
        cursor_advance(cursor);
    }
    free(cursor);
    return EXCUTE_SUCCESS;
}

void print_row(Row* row) {
    printf("(%d, %s, %s)\n", row->id, row->email, row->password);
}
