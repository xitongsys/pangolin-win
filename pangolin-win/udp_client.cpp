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
	struct sockaddr_in server_info;
	int len = sizeof(server_info);
	server_info.sin_family = AF_INET;
	server_info.sin_port = config->server_port;
	server_info.sin_addr.s_addr = inet_addr(config->server_ip.c_str());

	SOCKET sk = socket(AF_INET, SOCK_DGRAM, 0);
	char buf[65536];
	while (true) {
		vector<uint8_t> data = tun->read();
		cout << data.size() << endl;
		sendto(sk, buf, data.size(), 0, (sockaddr*)&server_info, len);

		if (recvfrom(sk, buf, sizeof(buf), 0, (sockaddr*)& server_info, &len) != SOCKET_ERROR) {
			vector<uint8_t> data;
			for (int i = 0; i < len; i++) {
				data.push_back(buf[i]);
				cout << buf[i] << endl;
			}
			tun->write(data);
		}
	}
}