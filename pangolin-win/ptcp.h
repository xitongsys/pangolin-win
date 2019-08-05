#pragma once
#include <cstdio>
#include <string>
#include <cstdint>
#include <vector>
#include "windivert.h"
#include "../ethernet/util.h"
#include "../ethernet/frame.h"
#include "config.h"



class Ptcp {
public:
	static const int BUFFSIZE = 2500;
	Ptcp(Config *config);
	~Ptcp();

public:
	Config* config;
	HANDLE handle;
	char send_buf[BUFFSIZE];
	char recv_buf[BUFFSIZE];

public:
	bool start();
	bool stop();
	int send(vector<uint8_t>& data);
	vector<uint8_t> recv();
};

