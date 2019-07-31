#include "udp_client.h"
#include <WinSock2.h>
#include <iostream>
using namespace std;

Udp_client::Udp_client(Config *config, Tun* tun) {
	this->config = config;
	this->tun = tun;
}

Udp_client::~Udp_client() {
}

bool Udp_client::start() {
	WORD socketVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(socketVersion, &wsaData) != 0) {
		return false;
	}

	struct sockaddr_in server_info;
	int len = sizeof(server_info);
	memset(&server_info, 0, len);
	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(config->server_port);
	inet_pton(AF_INET, config->server_ip.c_str(), (void*)& server_info.sin_addr.S_un.S_addr);

	SOCKET sk = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sk == SOCKET_ERROR) {
		return false;
	}

	u_long mode = 1;
	ioctlsocket(sk, FIONBIO, &mode);
	char buf[65536];
	while (true) {
		vector<uint8_t> data = tun->read();
		if (data.size() > 0) {
			for (int i = 0; i < data.size(); i++) {
				buf[i] = data[i];
			}
			sendto(sk, buf, data.size(), 0, (sockaddr*)& server_info, sizeof(sockaddr));
			cout << "send  "<<data.size() << endl;
		}

		if (recvfrom(sk, buf, sizeof(buf), 0, (sockaddr*)& server_info, &len) != SOCKET_ERROR) {
			vector<uint8_t> data;
			for (int i = 0; i < len; i++) {
				data.push_back(buf[i]);
			}
			cout << "recv  " << data.size() << endl;
			tun->write(data);
		}
	}
}