#include <stdint.h>

struct PageData {
    char *page;
    char *header;

    size_t headerSize;
    size_t pageSize;
}; 

struct PageData loadPage();
void freePage(struct PageData page);
