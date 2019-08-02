#pragma once
#include <cstdio>
#include <string>
#include <cstdint>
#include <vector>
#include "windivert.h"
#include "../ethernet/frame.h"
using namespace std;

class Tun {
public:
	static const int BUFFSIZE = 2000;
	Tun();
	~Tun();
	
	HANDLE handle;
	uint8_t buf[BUFFSIZE];

	bool start();
	vector<uint8_t> read();
	bool write(vector<uint8_t>& data);
};