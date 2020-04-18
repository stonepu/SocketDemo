#include "Client.h"

#pragma comment(lib,"WS2_32.lib")

using std::cin;
using std::string;

constexpr int BUFFER_SIZE = 1024;



Client::Client(const char * host, const int port)
{
	this->host = host;
	this->port = port;
	//client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


}

int Client::start()
{
	int ret = 0;
	ret = init();
	if (ret < 0) {
		std::cout << "init error " << std::endl;
		return ret;
	}

	std::thread t(&Client::selecting, this);
	t.detach();
	sendData();
}

int Client::init()
{
	int ret = 0;
	WORD w = MAKEWORD(2, 2);
	WSADATA data;
	ret = WSAStartup(w, &data);
	if (ret < 0) {
		std::cout << "socket init error" << std::endl;
		return ret;
	}

	SOCKADDR_IN serverAddr;
	//memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	//serverAddr.sin_addr.S_un.S_addr = inet_addr(host);

	std::cout << "debug host:" << host << std::endl;
	std::cout << "debug prot: " << port << std::endl;

	inet_pton(AF_INET, host, (void*)&serverAddr.sin_addr.s_addr);


	client = socket(AF_INET, SOCK_STREAM, 0);
	ret = connect(client, (sockaddr *)&serverAddr, sizeof(SOCKADDR_IN));
	if (ret == SOCKET_ERROR) {
		std::cout << "connect error " << std::endl;
		closesocket(client);
		WSACleanup();
		return ret;
	}

	std::cout << "connect to server success." << std::endl;
	return 0;
}

int Client::sendData()
{
	int ret = 0;
	while (1) {
		std::string msg;
		getline(cin, msg);
		ret = send(client, msg.c_str(), msg.length(), 0);
		if (ret == SOCKET_ERROR) {
			std::cout << "send msg error " << std::endl;
			break;
		}
	}
	return ret;
}

void Client::selecting()
{
	char buff[BUFFER_SIZE]{ 0 };
	int ret = 0;
	while (1) {
		fd_set fdread;
		FD_ZERO(&fdread);
		FD_SET(client, &fdread);
		ret = select(client + 1, &fdread, nullptr, nullptr, nullptr);
		if (ret < 0) {
			std::cout << "disconncet from sever" << std::endl;
			break;
		}
		if (FD_ISSET(client, &fdread)) {
			FD_CLR(client, &fdread);
			ret = recv(client, buff, BUFFER_SIZE - 1, 0);
			if (ret == SOCKET_ERROR) {
				std::cout << "recv msg error" << std::endl;
				break;
			}
			std::string msg(buff);
			std::cout << "got msg from server: " << msg << std::endl;
		}
	}	
}
