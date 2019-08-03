#pragma once
#include <cstdio>
#include <string>
#include <cstdint>
#include <vector>
#include "windivert.h"
#include "../ethernet/frame.h"
#include "config.h"
using namespace std;

class Tun {
public:
	static const int BUFFSIZE = 2000;
	Tun(Config* config);
	~Tun();
	
	HANDLE handle;
	uint8_t buf[BUFFSIZE];
	Config* config;

	bool start();
	vector<uint8_t> read();
	bool write(vector<uint8_t>& data);
};