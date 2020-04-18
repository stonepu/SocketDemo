#pragma once
#include <winsock2.h>
#include <WS2tcpip.h>
#include <vector>

constexpr int BUFFER_SIZE = 1024;
constexpr int SOCKET_SIZE = 256;

class Server {
public:
	Server(int port);
	int init();
	int start();

private:
	int port;
	SOCKET serverSocket{ INVALID_SOCKET };
	std::vector<SOCKET> client{};

	void selecting();
	int sendData(char* data, const SOCKET& clientId);
	int readData();
	int setupConnect();
};