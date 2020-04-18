#include <iostream>
#include <string>
#include <cstring>
#include "Client.h"
#include "Server.h"


using std::cin;
using std::cout;
using std::endl;
using std::string;

int main (int args, char* arg[]) {
	cout << "welcome" << endl;
	if(args == 4){
		if (!strcmp(arg[1], "client")) {
			char* host = arg[2];
			int port = atoi(arg[3]);
			auto client = new Client(host, port);
			int ret = client->start();
		}
		else {
			cout << "params error2" << endl;
			goto _END;
		}
	}
	else if (args == 3) {
		if (!strcmp(arg[1], "server")) {
			int port = atoi(arg[2]);
			std::cout << "port: " << port << std::endl;
			auto server = new Server(port);
			int ret = server->start();
		}
		else {
			cout << "params error1" << endl;
			goto _END;
		}
	}
	else {
		cout << "params error111" << endl;
	}

_END:

	char* s1 = "test";
	//string str(s1);
	auto rsp = strcmp(s1, "test");
	cout << rsp << endl;
	//cout << str << endl;
	system("pause");
	return 0;
}
