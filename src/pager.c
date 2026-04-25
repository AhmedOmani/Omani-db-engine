#include "../include/pager.h"

Pager* pager_open(const char* filename) {
    int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
    
    if (fd == -1) {
        printf("Error opening file!");
        exit(1);
    }

    off_t file_length = lseek(fd , 0 , SEEK_END);
    
    Pager* pager = malloc(sizeof(Pager));

    //Initialize the pager
    pager->file_discriptor = fd;
    pager->file_length = file_length;
    for (uint32_t i = 0 ; i < TABLE_MAX_PAGES ; i++) 
        pager->cache_pages[i] = NULL;
    
    return pager;
}

void* get_page(Pager* pager , uint32_t page_number) {
    if (page_number >= TABLE_MAX_PAGES) {
        printf("Error: Tried to access page number %d out of %d pages\n" , page_number , TABLE_MAX_PAGES);
        exit(1);
    }

    //Here we need to fetch the page from the disk
    if (pager->cache_pages[page_number] == NULL) {
        void* page = malloc(PAGE_SIZE);
        uint32_t bytes_read = pread(pager->file_discriptor , page , PAGE_SIZE , page_number * PAGE_SIZE);
        if (bytes_read == -1) {
            printf("Error reading file!");
            exit(1);
        }
        //fill the remaining by zeros, but why ?
        if (bytes_read < PAGE_SIZE) {
            memset(page + bytes_read , 0 , PAGE_SIZE - bytes_read);
        }

        pager->cache_pages[page_number] = page;
    }

    return pager->cache_pages[page_number];
}

void flush_pages(Pager* pager , uint32_t page_number, uint32_t size) {
    if (pager->cache_pages[page_number] == NULL) return;
    
    off_t offset = lseek(pager->file_discriptor , page_number * PAGE_SIZE , SEEK_SET);
    if (offset == -1) {
        printf("Error seeking file!");
        exit(1);
    }

    ssize_t bytes_written = write(pager->file_discriptor , pager->cache_pages[page_number] , size);
    if (bytes_written == -1) {
        printf("Error writing to file!");
        exit(1);
    }
}