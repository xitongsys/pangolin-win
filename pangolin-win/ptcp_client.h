#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstdio>
#include <iostream>
#include <thread>
#include "config.h"
#include "tun.h"
#include "../ethernet/util.h"


class Ptcp_client {
public:
	static const int BUFFSIZE = 2000;
	Ptcp_client(Config* config, Tun* tun);
	~Ptcp_client();

	Config* config;
	Tun* tun;
	char send_buf[BUFFSIZE];
	char recv_buf[BUFFSIZE];

	SOCKET sk;
	struct sockaddr_in server_info;

	bool start();
	void send();
	void recv();
};