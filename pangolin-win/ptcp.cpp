#include "ptcp.h"
#include <iostream>
#include <time.h>

Ptcp::Ptcp(Config *config) {
	this->config = config;
}

Ptcp::~Ptcp() {}

bool Ptcp::start() {
	srand(time(NULL));
	local_ip = config->gateway->addr;
	local_port = (rand() % 50000) + 10000;

	server_ip = str2ip(config->server_ip);
	server_port = config->server_ptcp_port;

	char fmt[] = "(ip.SrcAddr == %s or ip.DstAddr == %s) and remotePort == %d and !loopback and ifIdx == %d and !impostor";
	char filter[1024];
	sprintf_s(filter, 1024, fmt, config->server_ip.c_str(), config->server_ip.c_str(), config->server_ptcp_port, config->gateway->ifIndex);
	handle = WinDivertOpen(filter, WINDIVERT_LAYER_NETWORK, 0, 0);
	return !(handle == INVALID_HANDLE_VALUE);
}

bool Ptcp::stop() {
	WinDivertClose(handle);
	return true;
}

int Ptcp::send_until(vector<uint8_t>& data, uint64_t timeout, bool (*pfun)(vector<uint8_t>& data)) {
	uint64_t start = time(0);
	while (true) {
		send(data);
		vector<uint8_t> data_recv = recv();
		if ((*pfun)(data_recv)) {
			break;
		}
		uint64_t now = time(0);
		if (now - start >= timeout) {
			cout << now << " " << start << endl;
			return -1;
		}
	}
	return data.size();
}

int Ptcp::send(vector<uint8_t>& data) {
	Frame frame;
	uint8_t buf[BUFFSIZE];
	build_tcp(frame, data);
	frame.tcp.seq = 1;
	frame.tcp.ack = 1;
	frame.tcp.flags = PSH | ACK;
	int wn = frame.write(3, buf, BUFFSIZE);
	vector<uint8_t> data0;
	for (int i = 0; i < wn; i++) {
		data0.push_back(buf[i]);
	}
	return send_raw(data0);
}

vector<uint8_t> Ptcp::recv() {
	vector<uint8_t> data0 = recv_raw();
	vector<uint8_t> data;
	if (data0.size() == 0) return data;

	Frame frame;
	uint8_t buf0[BUFFSIZE];
	for (int i = 0; i < data0.size(); i++) {
		buf0[i] = data0[i];
	}
	frame.read(3, buf0, data0.size());
	return frame.content;
}

int Ptcp::send_raw(vector<uint8_t>& data) {
	for (int i = 0; i < data.size(); i++) {
		send_buf[i] = data[i];
	}

	UINT wn = data.size();
	WINDIVERT_ADDRESS addr;
	addr.Layer = WINDIVERT_LAYER_NETWORK;
	addr.Impostor = 1;
	addr.Loopback = 0;
	addr.Outbound = 1;
	addr.IPChecksum = 1;
	addr.TCPChecksum = 1;
	addr.UDPChecksum = 1;
	addr.Network.IfIdx = config->gateway->ifIndex;
	addr.Network.SubIfIdx = 0;

	WinDivertSendEx(handle, send_buf, BUFFSIZE, &wn, 0, &addr, sizeof(addr), NULL);
	return wn;
}

vector<uint8_t> Ptcp::recv_raw() {
	WINDIVERT_ADDRESS addr;
	vector<uint8_t> res;
	UINT rn = 0;
	UINT addrLen = sizeof(WINDIVERT_ADDRESS);
	if (!WinDivertRecvEx(handle, recv_buf, BUFFSIZE, &rn, 0, &addr, &addrLen, NULL)) {
		return res;
	}
	for (int i = 0; i < rn; i++) {
		res.push_back(recv_buf[i]);
	}
	return res;
}

bool Ptcp::dial() {
	Frame frame0;
	uint8_t buf0[BUFFSIZE];
	vector<uint8_t> data;
	build_tcp(frame0, data);
	frame0.tcp.seq = 0;
	frame0.tcp.flags = SYN;
	int wn0 = frame0.write(3, buf0, BUFFSIZE);
	vector<uint8_t> data0;
	for (int i = 0; i < wn0; i++) {
		data0.push_back(buf0[i]);
	}

	Frame frame1;
	uint8_t buf1[BUFFSIZE];
	while (true) {
		send_raw(data0);
		vector<uint8_t> data1 = recv_raw();
		if (data1.size() > 0) {
			for (int i = 0; i < data1.size(); i++) {
				buf1[i] = data1[i];
			}

			frame1.read(3, buf1, data1.size());
			if (frame1.tcp.flags == (SYN | ACK) && frame1.tcp.ack == 1) {
				break;
			}
		}
	}

	frame0.tcp.seq = 1;
	frame0.tcp.ack = 1;
	frame0.tcp.flags = ACK;
	wn0 = frame0.write(3, buf0, BUFFSIZE);
	data0.clear();
	for (int i = 0; i < wn0; i++) {
		data0.push_back(buf0[i]);
	}

	send_raw(data0);
	return true;
}

void Ptcp::build_tcp(Frame& frame, vector<uint8_t>& data) {
	frame.ipv4.version = 0x45;
	frame.ipv4.tos = 0;
	frame.ipv4.len = 20 + 20 + data.size();
	frame.ipv4.id = 0;
	frame.ipv4.offset = 0;
	frame.ipv4.ttl = 255;
	frame.ipv4.protocol = 0x06;
	frame.ipv4.checksum = 0;
	frame.ipv4.src = local_ip;
	frame.ipv4.dst = server_ip;

	frame.tcp.src_port = local_port;
	frame.tcp.dst_port = server_port;
	frame.tcp.seq = 0;
	frame.tcp.ack = 0;
	frame.tcp.offset = 0x50;
	frame.tcp.flags = 0;
	frame.tcp.win = 29200;
	frame.tcp.checksum = 0;
	frame.tcp.urg_pointer = 0;

	frame.content = data;
}

