#pragma once
#include <cstdio>
#include "config.h"
#include "tun.h"
using namespace std;


class Udp_client {
public:
	Udp_client(Config *config, Tun* tun);
	~Udp_client();

	Config* config;
	Tun* tun;

	bool start();
};