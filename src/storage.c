#include "../include/storage.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Table* db_open(const char* filename) {
    Pager* pager = pager_open(filename);
    Table* table = (Table*)malloc(sizeof(Table));
    table->pager = pager;
    table->total_rows = pager->file_length / ROW_SIZE;
    table->total_pages = pager->file_length / PAGE_SIZE;
    table->stats = new_stats();
    return table;
}

void db_close(Table* table) {
    Pager *pager = table->pager;
    
    uint32_t full_pages = table->total_rows / ROWS_PER_PAGE;
    /**
        The reason we are doing in that way because we need to rely on the length of the file to use it in another sessions to retrieve the number of rows
        , we cant store the whole 4kb page in the file because it will gives us wrong data in this formula
        total_rows = file_length / ROW_SIZE
    */
    //First finish the full pages that are 4kb
    for (uint32_t i = 0 ; i < full_pages ; i++) {
        if (pager->cache_pages[i] == NULL) continue;
        flush_pages(pager, i, PAGE_SIZE);
        free(pager->cache_pages[i]);
        pager->cache_pages[i] = NULL;
    }
    
    //then finish the last page if it contains rows
    uint32_t remaining_rows = table->total_rows % ROWS_PER_PAGE ;
    if (remaining_rows > 0) {
        uint32_t last_page_number = full_pages;
        if(pager->cache_pages[last_page_number]) {
            flush_pages(pager , last_page_number , remaining_rows * ROW_SIZE);
            free(pager->cache_pages[last_page_number]);
            pager->cache_pages[last_page_number] = NULL;
        }
    }
    
    int result = close(table->pager->file_discriptor);
    
    if (result == -1) {
        printf("Error closing db file.\n");
        exit(EXIT_FAILURE);
    }
    free(pager);
    free(table);
}

void* row_slot(Table* table , uint32_t row_number) {
    uint32_t page_number = row_number / ROWS_PER_PAGE;
    void* page = get_page(table->pager, page_number);
    uint32_t row_modulo = row_number % ROWS_PER_PAGE;
    uint32_t row_start_offset = row_modulo * ROW_SIZE;
    return page + row_start_offset;
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


//Observability
Stats* new_stats() {
    Stats* stats = malloc(sizeof(Stats));
    stats->total_rows = 0;
    stats->total_allocated_pages = 0;
    stats->total_bytes = 0;
    return stats;
}

void debug(Table* table) {
    
}