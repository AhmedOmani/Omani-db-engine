#ifndef COMPILER_H
#define COMPILER_H

#include "repl.h"
#include "storage.h"

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_SYNTAX_ERROR,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct {
    StatementType type;
    Row insertion_row;
} Statement;


MetaCommandResult do_meta_command(InputBuffer* input_buffer, Table* table) ;
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement);

int parse_insert_query(InputBuffer* input_buffer , Row* row);

void debug(Table* table);

#endif
