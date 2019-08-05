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
	uint8_t read_buf[BUFFSIZE];
	uint8_t write_buf[BUFFSIZE];
	Config* config;

	bool start();
	bool stop();
	vector<uint8_t> read();
	bool write(vector<uint8_t>& data);

	vector<uint8_t> read_raw(WINDIVERT_ADDRESS* addr);
	bool write_raw(vector<uint8_t>& data, WINDIVERT_ADDRESS* addr);
};