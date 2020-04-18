#pragma once
#include <winsock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <iostream>
#include <string>


class Client {
public:
	Client(const char* host, const int port);
	int start();

	int init();

	int sendData();
	void selecting();
private:
	const char *host;
	int port;
	SOCKET client{};
};