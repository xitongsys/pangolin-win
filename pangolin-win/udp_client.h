#pragma once
#include <cstdio>
#include "config.h"
#include "tun.h"
using namespace std;


class Udp_client {
public:
	static const int BUFFSIZE = 2000;
	Udp_client(Config *config, Tun* tun);
	~Udp_client();

	Config* config;
	Tun* tun;
	char buf[BUFFSIZE];

	bool start();
};