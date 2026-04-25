#ifndef PAGER_C
#define PAGER_C

#define TABLE_MAX_PAGES 100
#define PAGE_SIZE 4096

#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

typedef struct {
    int file_discriptor;
    uint32_t file_length;
    void* cache_pages[TABLE_MAX_PAGES];
} Pager;

Pager* pager_open(const char* filename);
void* get_page(Pager* pager , uint32_t page_number);
void flush_pages(Pager* pager, uint32_t page_number, uint32_t size);
#endif