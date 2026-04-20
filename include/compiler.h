#ifndef COMPILER_H
#define COMPILER_H

#include "repl.h"
#include <stdint.h>
#include <sys/types.h>
#include <stddef.h>

#define EMAIL_COLUMN_LENGTH 256
#define PASSOWRD_COLUMN_LENGTH 16

#define PAGE_SIZE 4096
#define TABLE_MAX_PAGES 100

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

typedef enum {
    EXCUTE_SUCCESS,
    EXCUTE_TABLE_FULL,
    EXCUTE_ERROR
} ExcuteResult;

typedef struct {
    uint32_t id;
    char email[EMAIL_COLUMN_LENGTH];
    char password[PASSOWRD_COLUMN_LENGTH];
} Row;

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

#define ID_SIZE size_of_attribute(Row, id)
#define EMAIL_SIZE size_of_attribute(Row , email)
#define PASSWORD_SIZE size_of_attribute(Row , password)

#define ID_OFFSET offsetof(Row, id)
#define EMAIL_OFFSET offsetof(Row, email)
#define PASSWORD_OFFSET offsetof(Row, password)

#define ROW_SIZE (ID_SIZE + EMAIL_SIZE + PASSWORD_SIZE)
#define ROWS_PER_PAGE (PAGE_SIZE / ROW_SIZE)
#define TABLE_MAX_ROWS (ROWS_PER_PAGE * TABLE_MAX_PAGES)

typedef struct {
    uint32_t total_rows;
    uint32_t total_allocated_pages;
    u_int64_t total_bytes;
} Stats;

typedef struct {
    StatementType type;
    Row insertion_row;
} Statement;

typedef struct {
    uint32_t total_rows;
    uint32_t total_pages;
    void* pages[TABLE_MAX_PAGES];
    Stats* stats;
} Table;

MetaCommandResult do_meta_command(InputBuffer* input_buffer, Table* table) ;
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement);

ExcuteResult execute_statement(Statement* statement, Table* table);
ExcuteResult execute_insert(Statement* statement , Table* table);
ExcuteResult execute_select(Statement* statement , Table* table);

int parse_insert_query(InputBuffer* input_buffer , Row* row);
void* row_slot(Table* table , uint32_t row_number);

Table* new_table();
void close_table(Table* table);

void serialize_row(Row* source, void* destination);
void deserialize_row(void* source, Row* destination);

void debug(Table* table);

Stats* new_stats();
void print_stats(Stats* stats);

#endif
