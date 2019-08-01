#pragma once
#include <cstdio>
#include <string>
#include <cstdint>
#include <vector>
#include "pcap.h"
#include "../ethernet/frame.h"
using namespace std;

class Tun {
public:
	static const int BUFFSIZE = 2000;
	Tun(string name);
	~Tun();
	
	uint8_t buf[BUFFSIZE];

	bool start();
	vector<uint8_t> read();
	bool write(vector<uint8_t>& data);
	bool write_test(vector<uint8_t>& data);

private:
	string adname;
	pcap_t* adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];
};