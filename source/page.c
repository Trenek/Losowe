#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "page.h"

#define BUFFER 1024

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

char *fileExtensions[] = {
    "html",
    "ico",
    "gif",
    "mp3"
};
constexpr size_t extensionsQ = sizeof(fileExtensions) / sizeof(char *);

char *contentTypes[] = {
    "text/html",
    "image/x-icon",
    "image/gif",
    "audio/mpeg"
};

char *getContentType(char *file) {
    char *extension = strchr(file, '.') + 1;
    char *result = NULL;

    for (int i = 0; i < extensionsQ; i += 1) {
        if (0 == strncmp(extension, fileExtensions[i], strlen(fileExtensions[i]))) {
            result = contentTypes[i];
        }
    }

    return result;
}

struct PageData loadData(char *fileName) {
    char buffer[1024] = {};
    size_t dataSize = 0;

    struct PageData result = {
        .data = loadFile(fileName, &dataSize)
    };
    result.dataSize = dataSize;

    size_t headerLength = sprintf(buffer, 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "\r\n", getContentType(fileName), result.dataSize);

    result.header = malloc(headerLength + 1),
    result.headerSize = headerLength * sizeof(char),

    memcpy(result.header, buffer, result.headerSize);

    return result;
}

void freeData(struct PageData page) {
    free(page.header);
    free(page.data);
}
