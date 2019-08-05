#include "Ptcp_client.h"

Ptcp_client::Ptcp_client(Config* config, Ptcp* ptcp, Tun* tun) {
	this->config = config;
	this->ptcp = ptcp;
	this->tun = tun;
}

Ptcp_client::~Ptcp_client() {
}

bool Ptcp_client::start() {
	if (!tun->start()) return false;
	if (!ptcp->start()) return false;
	if (!ptcp->dial()) return false;
	if (!login()) return false;

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

			vector<uint8_t> data;
			data.push_back(Ptcp::PTCP_PACKETTYPE_DATA);
			for (int i = 0; i < wn; i++) {
				data.push_back(send_buf[i]);
			}
			ptcp->send(data);
		}
	}
}

void Ptcp_client::recv() {
	int len = sizeof(server_info);
	while (true) {
		vector<uint8_t> data = ptcp->recv();
		if (data.size() > 1 && data[0] == Ptcp::PTCP_PACKETTYPE_DATA) {
			vector<uint8_t> data_send;
			for (int i = 1; i < data.size(); i++) {
				data_send.push_back(data[i]);
			}
			tun->write(data_send);
		}
	}
}

bool login_check(vector<uint8_t>& data) {
	if (data.size() <= 1) return false;
	cout << data[0] << " " << data[1] << endl;
	if (data[0] == Ptcp::PTCP_PACKETTYPE_LOGIN || data[1] == Ptcp::PTCP_LOGINMSG_SUCCESS) return true;
	return false;
}

bool Ptcp_client::login() {
	vector<uint8_t> data;
	data.push_back(Ptcp::PTCP_PACKETTYPE_LOGIN);
	for (uint8_t c : config->token) {
		data.push_back(c);
	}

	ptcp->send_until(data, login_check);
	return true;
}