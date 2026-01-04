#include <stdint.h>

struct PageData {
    char *header;
    char *data;

    size_t headerSize;
    size_t dataSize;
}; 

void freeData(struct PageData page);

struct PageData loadData(char *fileName);
