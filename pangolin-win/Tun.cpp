#include "tun.h"
#include "../ethernet/util.h"
#include "../ethernet/const.h"
#include <tchar.h>
#include <thread>
#include <iostream>


Tun::Tun() {
	handle = NULL;
}

Tun::~Tun(){}

vector<uint8_t> Tun::read() {
	vector<uint8_t> res;
	Frame frame;
	UINT rn = 0, wn = 0;
	WINDIVERT_ADDRESS addr;
	if(!WinDivertRecv(handle, buf, BUFFSIZE, &rn, &addr)) {
		return res;
	}

	rn = frame.read(3, buf, rn);
	if (rn <= 0) {
		return res;
	}

	cout << frame.ipv4.to_string()<<frame.tcp.to_string() << endl;

	uint32_t a = str2ip("149.129.49.157");
	uint32_t bi = str2ip("162.105.129.103");
	uint32_t c = str2ip("10.172.116.96");
	if (frame.ipv4.dst == a || frame.ipv4.src == a) {
		WinDivertSend(handle, buf, rn, &wn, &addr);
		return res;
	}
	if (frame.ipv4.src != c) return res;

	int b = frame.ethernet.header_length();
	wn = frame.write(3, buf, BUFFSIZE);
	for (int i = b; i < wn; i++) {
		res.push_back(buf[i]);
	}

	return res;
}

bool Tun::write(vector<uint8_t>& data) {

	for (int i = 0; i < data.size(); i++) {
		buf[i] = data[i];
	}

	return true;
}



bool Tun::start() {
	handle = WinDivertOpen("true", WINDIVERT_LAYER_NETWORK, 0, 0);
	return !(handle == INVALID_HANDLE_VALUE);
}
