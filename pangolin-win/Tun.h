#pragma once
#include <concurrent_queue.h>
#include <cstdio>
#include <string>
#include <cstdint>
#include <vector>
#include "pcap.h"
#include "../ethernet/frame.h"
using namespace std;
using concurrency::concurrent_queue;

class Tun {
public:
	static const int BUFFSIZE = 1024;
	Tun(string name);
	~Tun();
	bool start();
	void stop();
	vector<uint8_t> read();
	bool write(vector<uint8_t>& data);
	void packet_read();
	void packet_write();

private:
	concurrent_queue<vector<uint8_t>> readQueue;
	concurrent_queue<vector<uint8_t>> writeQueue;
	vector<uint8_t> _read();
	bool _write(vector<uint8_t>& data);

	string adname;
	pcap_t* adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];
};