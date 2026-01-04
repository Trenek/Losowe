#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "page.h"

static long getFileSize(FILE *fp) {
    long result = 0;

    fseek(fp, 0, SEEK_END);
    result = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return result;
}

static char *loadFile(char *name, size_t *size) {
    FILE *fp = fopen(name, "rb");
    size_t length = getFileSize(fp);

    char *result = malloc(sizeof(char) * (length + 1));

    fread(result, 1, length, fp);
    result[length] = 0;

    fclose(fp);

    *size = length;
    return result;
}

struct PageData loadPage() {
    char header[] = 
        "HTTP/1.1 200 OK\n"
        "Content-Type: text/html\n"
        "Content-Length: %ld\n"
        "\n";

    long pageSize = 0;

    struct PageData result = {
        .header = malloc(sizeof(header)),
        .headerSize = sizeof(header) / sizeof(char),
        .page = loadFile("page/page.html", &result.pageSize)
    };

    memcpy(result.header, header, sizeof(header));

    return result;
}

void freePage(struct PageData page) {
    free(page.header);
    free(page.page);
}
