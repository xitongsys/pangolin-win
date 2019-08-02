#pragma once
#include <vector>
#include "ethernet.h"
#include "ipv4.h"
#include "tcp.h"
#include "udp.h"
#include "ipv4pseudo.h"

using std::vector;

class Frame {
public:
	Ethernet ethernet;
	IPv4 ipv4;
	TCP tcp;
	UDP udp;
	vector<uint8_t> content;

public:
	Frame();
	int read(int layer, uint8_t* buf, int max_size);
	int write(int layer, uint8_t* buf, int max_size);

private:
	void set_checksum(uint8_t* buf, int pos, int size);
};