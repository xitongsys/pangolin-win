#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstdio>
#include <iostream>
#include <cstdint>
#include <thread>
#include "config.h"
#include "ptcp.h"
#include "tun.h"
#include "encrypt.h"
#include "../ethernet/util.h"

bool login_check(vector<uint8_t>& data);

class Ptcp_client {
public:
	static const int BUFFSIZE = 2000;
	Ptcp_client(Config* config, Ptcp* ptcp, Tun* tun);
	~Ptcp_client();

	Config* config;
	Ptcp* ptcp;
	Tun* tun;
	Encrypt* encrypt;

	char send_buf[BUFFSIZE];
	char recv_buf[BUFFSIZE];

	SOCKET sk;
	struct sockaddr_in server_info;

	bool start();
	void send();
	void recv();
	bool login();
};