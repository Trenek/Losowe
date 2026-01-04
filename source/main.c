#include <stdio.h>
#include <socket.h>

#include "page.h"

#define PORT 8080
#define BUFFER_SIZE 1024

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

#define IF(x, y) if (!(x)) { errcode = y; } else

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
