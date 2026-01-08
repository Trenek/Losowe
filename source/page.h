#include <stdint.h>

struct PageData {
    char *header;
    char *data;

    size_t headerSize;
    size_t dataSize;
}; 

struct PageData loadData(char *fileName);
void freeData(struct PageData page);
