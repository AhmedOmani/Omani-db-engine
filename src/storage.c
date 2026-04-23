#include "../include/storage.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

//Always remember when you are struggle at the question of "where is the start of the row ?"
//This funtion will gives you thr answer.
void* row_slot(Table* table , uint32_t row_number) {
    uint32_t page_number = row_number / ROWS_PER_PAGE;
    void* page = table->pages[page_number];
    if(page == NULL) {
        page = table->pages[page_number] = malloc(PAGE_SIZE);
        table->total_pages += 1;
    }
    //The core idea is we need to know which slot we are currently stand on right now
    //if page has 3 slots -> 0 , 1 , 2 slots
    //if row_number = 0 -> 0 % 3 = 0 -> first slot
    //if row_number = 1 -> 1 % 3 = 1 -> second slot
    //if row_number = 2 -> 2 % 3 = 2 -> third slot
    //and after we know that we need to find the exact byte offset
    //that we start putting data into and that is equal to size of the row times the row offset
    uint32_t row_offset = row_number % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;

    //At the end we return the address of the page + byte offset and tha tis the address / place we start putting data into
    return (char*)page + byte_offset;
}

void serialize_row(Row* source, void *destination) {
    memcpy((char*)destination + ID_OFFSET, &(source->id) , ID_SIZE);
    memcpy((char*)destination + EMAIL_OFFSET , &(source->email) , EMAIL_SIZE);
    memcpy((char*)destination + PASSWORD_OFFSET , &(source->password) , PASSWORD_SIZE);
}

void deserialize_row(void* source, Row* destination) {
    memcpy(&(destination->id) , (char*)source + ID_OFFSET , ID_SIZE);
    memcpy(&(destination->email), (char*)source + EMAIL_OFFSET , EMAIL_SIZE);
    memcpy(&(destination->password) , (char*)source + PASSWORD_OFFSET , PASSWORD_SIZE);
}

void debug(Table* table) {
    printf("\n====== DATABASE ENGINE DEBUG ======\n");
    printf("Total Rows: %u | Total Pages Allocated: %u\n", table->total_rows, table->total_pages);
    
    
    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
      
        if (table->pages[i] != NULL) {
            printf("\n--> [PAGE %u] Memory Address: %p\n", i, table->pages[i]);
            
        
            uint32_t start_row = i * ROWS_PER_PAGE;
            uint32_t end_row = start_row + ROWS_PER_PAGE;
            
            
            if (end_row > table->total_rows) {
                end_row = table->total_rows;
            }
            
          
            for (uint32_t r = start_row; r < end_row; r++) {
                Row row;
                deserialize_row(row_slot(table, r), &row);
                printf("    Row Offset: %u (Absolute Row: %u) | id: %u | email: '%s' | password: '%s'\n", r - start_row, r, row.id, row.email, row.password);
            }
        }
    }
    printf("===================================\n");
}

//Observability
Stats* new_stats() {
    Stats* stats = malloc(sizeof(Stats));
    stats->total_rows = 0;
    stats->total_allocated_pages = 0;
    stats->total_bytes = 0;
    return stats;
}