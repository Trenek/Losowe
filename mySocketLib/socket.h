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

#ifdef _WIN32
    #define SOCKET_ERROR_ SOCKET_ERROR
    #define SO_REUSE SO_REUSEADDR
#else
    #define SO_ERROR_ SO_ERROR
    #define SO_REUSE SO_REUSEADDR | SO_REUSEPORT
#endif

int sockInit(void);
int sockQuit(void);
int sockClose(int sock);
