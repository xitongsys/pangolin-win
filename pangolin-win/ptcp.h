#pragma once
#include <WS2tcpip.h>
#include <cstdio>
#include <string>
#include <cstdint>
#include <vector>
#include "config.h"
#include "windivert.h"
#include "../ethernet/util.h"
#include "../ethernet/frame.h"
#include "../ethernet/const.h"
using namespace std;

class Ptcp {
public:
	static const int BUFFSIZE = 2500;
	static const uint8_t PTCP_PACKETTYPE_LOGIN = 0;
	static const uint8_t PTCP_PACKETTYPE_DATA = 1;
	static const uint8_t PTCP_LOGINMSG_SUCCESS = 0;
	static const uint8_t PTCP_LOGINMSG_FAILED = 1;

	Ptcp(Config *config);
	~Ptcp();

public:
	Config* config;
	HANDLE handle;
	char send_buf[BUFFSIZE];
	char recv_buf[BUFFSIZE];
	int local_port, local_ip;
	int server_ip, server_port;

public:
	bool start();
	bool stop();
	bool dial();
	int send_raw(vector<uint8_t>& data);
	vector<uint8_t> recv_raw();
	int send(vector<uint8_t>& data);
	int send_until(vector<uint8_t>& data, bool (*fun)(vector<uint8_t>&data));
	vector<uint8_t> recv();
	void build_tcp(Frame &frame, vector<uint8_t>& data);
};

