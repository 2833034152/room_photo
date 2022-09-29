#include <stdio.h>
#include <iostream>
#include "network_tools.h"
#pragma comment (lib, "WS2_32.lib")

SOCKET webInitTCP(const char* ip, sockaddr_in* addr, unsigned short port) {
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(1, 1), &wsaData);
	if (err != 0) {
		return -1;
	}

	SOCKET retSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	addr->sin_family = PF_INET;
	addr->sin_addr.S_un.S_addr = (ip == 0) ? 0 : inet_addr(ip);
	addr->sin_port = htons(port);

	return retSocket;
}