#pragma once
#include <winsock2.h>

SOCKET webInitTCP(const char* ip, sockaddr_in* addr, unsigned short port);

