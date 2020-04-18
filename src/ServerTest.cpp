#include <winsock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <iostream>
int port = 30375;

int main111() {
	int ret = 0;
	WSADATA data;
	WORD w = MAKEWORD(2, 2);
	ret = WSAStartup(w, &data);
	if (ret) {
		std::cout << "socket start error " << ret << std::endl;
		return ret;
	}
	std::cout << "socket start up success." << std::endl;

	SOCKADDR_IN serverAddr{ 0 };
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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



	SOCKADDR_IN clientAddr{};
	int addrLen = sizeof(SOCKADDR_IN);

	//�����µ�socket��ָ�����û�����ͨ�ţ� select��ִ��accept������������
	SOCKET clientId = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
	if (clientId == INVALID_SOCKET) {
		std::cout << "accept error" << std::endl;
	}

	std::cout << "connect success" << std::endl;

	system("pause");
	return 0;
}