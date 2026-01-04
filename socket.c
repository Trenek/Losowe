#include <stdio.h>

#ifdef _WIN32
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0501
	#endif
	#include <winsock2.h>
#else
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <unistd.h>
	#include <string.h>
	#include <stdlib.h>
#endif

int sockInit(void){
	#ifdef _WIN32
		WSADATA wsa_data;
		return WSAStartup(MAKEWORD(1,1), &wsa_data);
	#else
		return 0;
	#endif
}

int sockQuit(void){
	#ifdef _WIN32
		return WSACleanup();
	#else
		return 0;
	#endif
}

#ifdef _WIN32
	#define SOCKET_ERROR_ SOCKET_ERROR
    #define SO_REUSE SO_REUSEADDR
#else
	#define SO_ERROR_ SO_ERROR
    #define SO_REUSE SO_REUSEADDR | SO_REUSEPORT
#endif

int sockClose(int sock){
  int status = 0;
  #ifdef _WIN32
	status = shutdown(sock, SD_BOTH);
	if (status == 0) { status = closesocket(sock); }
  #else
	status = shutdown(sock, SHUT_RDWR);
	if (status == 0) { status = close(sock); }
  #endif
  return status;
}

#define PORT 8080
#define BUFFER_SIZE 1024

#define IF(x, y) if (!(x)) { errcode = y; } else

long getFileSize(FILE *fp) {
    long result = 0;

    fseek(fp, 0, SEEK_END);
    result = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return result;
}

char *loadFile(char *name, size_t *size) {
    FILE *fp = fopen(name, "rb");
    size_t length = getFileSize(fp);

    char *result = malloc(sizeof(char) * (length + 1));

    fread(result, 1, length, fp);
    result[length] = 0;

    fclose(fp);

    *size = length;
    return result;
}

struct PageData {
    char *page;
    char *header;

    size_t headerSize;
    size_t pageSize;
}; 

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

bool sendSite(int clinet_socket, size_t addrlen, struct sockaddr_in addr, struct PageData page) {
    char buffer[BUFFER_SIZE] = {0};

    int new_socket = accept(clinet_socket, (struct sockaddr *)&addr, (void*)&addrlen);
    bool result = new_socket >= 0;

    recv(new_socket, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);

    send(new_socket, page.header, page.headerSize, 0);
    send(new_socket, page.page, page.pageSize, 0);

    sockClose(new_socket);

    return result;
}

int main(int argc, char *argv[]){
    int errcode = 0;

    int client_socket;
    int opt = 1;

    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT),
    };

    struct PageData page = loadPage();

	IF (0 == sockInit(), 1)
	IF (0 <= (client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)), 2)
    IF (!setsockopt(client_socket, SOL_SOCKET, SO_REUSE, (char *)&opt, sizeof(opt)), 3)
    IF (0 >= bind(client_socket, (struct sockaddr *)&address, sizeof(address)), 4)
    IF (0 >= listen(client_socket, 3), 5) while (
        sendSite(client_socket, sizeof(address), address, page)
    );

    freePage(page);

    sockClose(client_socket);
    sockQuit();
	
	return errcode;
}
