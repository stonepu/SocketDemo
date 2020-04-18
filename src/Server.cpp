#include "Server.h"
#include <iostream>
#include <list>
#include <sstream>

using std::stringstream;
#pragma comment(lib,"WS2_32.lib")

//constexpr int BUFFER_SIZE = 1024;

Server::Server(int port) {
	this->port = port;
	//�������ʧЧ
	//serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

int Server::init()
{
	int ret = 0;
	WSADATA data;
	WORD w = MAKEWORD(2, 2);
	ret = WSAStartup(w, &data);
	if (ret) {
		std::cout << "socket start error " << ret << std::endl;
		return ret;
	}
	std::cout << "socket start up success." << std::endl;

	SOCKADDR_IN serverAddr{0};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	ret = bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR_IN));
	if (ret == SOCKET_ERROR) {
		std::cout << "bind error" << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return ret;
	}

	ret = listen(serverSocket, 10);
	if (ret == SOCKET_ERROR) {
		std::cout << "listen error" << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return ret;
	}
	std::cout << "socket init success" << std::endl;
	return 0;
}

int Server::start()
{
	int ret = 0;
	ret = init();
	if (ret) {
		std::cout << "server init error" << std::endl;
		return -1;
	}
	selecting();
	return 0;
}

void Server::selecting() {
	//std::cout << "debug: server select" << std::endl;
	int ret = 0;
	char buff[BUFFER_SIZE]{ 0 };

	while (true)
	{
		fd_set fdread;
		FD_ZERO(&fdread);

		//������socket������fdread��
		FD_SET(serverSocket, &fdread);
		for (size_t i = 0; i < client.size(); ++i) {
			FD_SET(client[i], &fdread);
		}

		ret = select(serverSocket + 1, &fdread, nullptr, nullptr, nullptr);
		if (ret < 0) {
			std::cout << "over" << std::endl;
			break;
		}

		//���serverSocket����λ����˵������Ϣ������serverSocekt��ʹ�ú���Ҫ������λ
		if (FD_ISSET(serverSocket, &fdread)) {
			FD_CLR(serverSocket, &fdread);
			SOCKADDR_IN clientAddr{};
			int addrLen = sizeof(SOCKADDR_IN);

			//�����µ�socket��ָ�����û�����ͨ�ţ� select��ִ��accept������������
			SOCKET clientId = accept(serverSocket, (struct sockaddr* )&clientAddr, &addrLen);
			if (clientId == INVALID_SOCKET) {
				std::cout << "accept error" << std::endl;
			}

			//��client Socket����vector�У��Ա��������fdread�У�
			client.push_back(clientId);
			std::cout << "client" << clientId <<" connect success." << std::endl;
		}

		//������Ϣ��ʱ����ӦclientId�ᱻ����fdread��������
		for (size_t i = 0; i < fdread.fd_count; ++i) {
			auto clientId = fdread.fd_array[i];
			//FD_CLR(clientId, &fdread);  //ʹ�ú�������λ
			//memset(buff, 0, 1024);
			auto msgLen = recv(clientId, buff, 1024, 0);
			std::cout << "get msg from" << clientId << ": " << buff << std::endl;
			//����Ϣ�㲥,���Լ�����
			sendData(buff, clientId);
		}
	}
	std::cout << "server is closed." << std::endl;
}

int Server::sendData(char* data, const SOCKET& clientId) {
	int ret = 0;
	stringstream ss;
	ss << clientId << ":" << data;
	auto msg = ss.str();
	for (size_t i = 0; i < client.size(); ++i) {
		if (client[i] != clientId) {
			std::cout << "debug: send msg to" << client[i] << std::endl;
			ret = send(client[i], msg.c_str(), msg.length(), 0);
			if (ret < 0) {
				std::cout << "send data to " << client[i] << " error" << std::endl;
			}
		}
	}
	return 0;
}

