#include "ptcp.h"

Ptcp::Ptcp(Config *config) {
	this->config = config;
}

Ptcp::~Ptcp() {}

bool Ptcp::start() {
	char fmt[] = "(ip.SrcAddr == %s or ip.DstAddr == %s) and remotePort == %d and !loopback and ifIdx == %d and !impostor";
	char filter[1024];
	sprintf_s(filter, 1024, fmt, config->server_ip.c_str(), config->server_ip.c_str(), config->server_ptcp_port, config->gateway->ifIndex);
	handle = WinDivertOpen(filter, WINDIVERT_LAYER_NETWORK, 0, 0);
	return !(handle == INVALID_HANDLE_VALUE);
}

bool Ptcp::stop() {
	WinDivertClose(handle);
}

int Ptcp::send(vector<uint8_t>& data) {
	for (int i = 0; i < data.size(); i++) {
		send_buf[i] = data[i];
	}

	UINT wn = data.size();
	WINDIVERT_ADDRESS addr;
	addr.Layer = WINDIVERT_LAYER_NETWORK;
	addr.Impostor = 1;
	addr.Loopback = 0;
	addr.Outbound = 0;
	addr.IPChecksum = 1;
	addr.TCPChecksum = 1;
	addr.UDPChecksum = 1;
	addr.Network.IfIdx = config->gateway->ifIndex;
	addr.Network.SubIfIdx = 0;

	WinDivertSendEx(handle, send_buf, BUFFSIZE, &wn, 0, &addr, sizeof(addr), NULL);
	return true;
}

vector<uint8_t> Ptcp::recv() {
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

