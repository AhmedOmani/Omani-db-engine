#ifndef STORAGE_H
#define STORAGE_H

#define EMAIL_COLUMN_LENGTH 256
#define PASSOWRD_COLUMN_LENGTH 16

#include "pager.h"

#include <stdint.h>
#include <sys/types.h>
#include <stddef.h>

typedef struct {
    uint32_t id;
    char email[EMAIL_COLUMN_LENGTH];
    char password[PASSOWRD_COLUMN_LENGTH];
} Row;

typedef struct {
    uint32_t total_rows;
    uint32_t total_allocated_pages;
    u_int64_t total_bytes;
} Stats;

typedef struct {
    uint32_t total_rows;
    uint32_t total_pages;
    Pager* pager;
    Stats* stats;
} Table;

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

void* row_slot(Table* table , uint32_t row_number);

Table* db_open(const char* filename);
void db_close(Table* table);

void serialize_row(Row* source, void* destination);
void deserialize_row(void* source, Row* destination);

void debug(Table* table);

Stats* new_stats();
void print_stats(Stats* stats);

#endif