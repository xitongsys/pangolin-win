#include "Ptcp_client.h"

Ptcp_client::Ptcp_client(Config* config, Tun* tun) {
	this->config = config;
	this->tun = tun;
}

Ptcp_client::~Ptcp_client() {
}


bool Ptcp_client::start() {
	WORD socketVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(socketVersion, &wsaData) != 0) {
		return false;
	}

	int len = sizeof(server_info);
	memset(&server_info, 0, len);
	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(config->server_port);
	inet_pton(AF_INET, config->server_ip.c_str(), (void*)& server_info.sin_addr.S_un.S_addr);

	sk = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sk == SOCKET_ERROR) {
		return false;
	}

	u_long mode = 1;
	ioctlsocket(sk, FIONBIO, &mode);

	thread send_thread(&Ptcp_client::send, this);
	thread recv_thread(&Ptcp_client::recv, this);

	send_thread.join();
	recv_thread.join();
}

void Ptcp_client::send() {
	while (true) {
		vector<uint8_t> data = tun->read();
		if (data.size() > 0 && data.size() < BUFFSIZE) {
			for (int i = 0; i < data.size(); i++) {
				send_buf[i] = data[i];
			}
			Frame frame;
			if (frame.read(3, (uint8_t*)send_buf, BUFFSIZE) <= 0) continue;
			int wn = frame.write(3, (uint8_t*)send_buf, BUFFSIZE);

			sendto(sk, send_buf, wn, 0, (sockaddr*)& server_info, sizeof(sockaddr));
		}
	}
}

void Ptcp_client::recv() {
	int len = sizeof(server_info);
	while (true) {
		int rl = recvfrom(sk, recv_buf, 2500, 0, (sockaddr*)& server_info, &len);
		if (rl > 0) {
			vector<uint8_t> data;
			for (int i = 0; i < rl; i++) {
				data.push_back(recv_buf[i]);
				//printf("%X ", data[i]);
			}
			tun->write(data);
		}
	}
}