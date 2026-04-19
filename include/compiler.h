#ifndef COMPILER_H
#define COMPILER_H

#include "repl.h"
#include <stdint.h>
#include <sys/types.h>

#define EMAIL_COLUMN_LENGTH 255
#define PASSOWRD_COLUMN_LENGTH 16

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
    uint32_t id;
    char email[EMAIL_COLUMN_LENGTH];
    char password[PASSOWRD_COLUMN_LENGTH];
} Row;

typedef struct {
    uint32_t total_rows;
    uint32_t total_allocated_pages;
    u_int64_t total_bytes;
} stats;

typedef struct {
    StatementType type;
    Row insertion_row;
} Statement;

MetaCommandResult do_meta_command(InputBuffer* input_buffer, stats* stats) ;
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement);
void execute_statement(Statement* statement, stats* stats);
int parse_insert_query(InputBuffer* input_buffer , Row* row);
stats* new_stats();
void print_stats(stats* stats);

#endif
