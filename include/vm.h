#ifndef VM_H
#define VM_H

#include "compiler.h"
#include "storage.h"

typedef enum {
    EXCUTE_SUCCESS,
    EXCUTE_TABLE_FULL,
    EXCUTE_ERROR
} ExcuteResult;

ExcuteResult execute_statement(Statement* statement, Table* table);
ExcuteResult execute_insert(Statement* statement , Table* table);
ExcuteResult execute_select(Statement* statement , Table* table);

void print_row(Row* row);

#endif