#pragma once
#include <cstdint>
#include "header.h"

class UDP : public Header{
public:
	uint16_t src_port;
	uint16_t dst_port;
	uint16_t len;
	uint16_t checksum;

public:
	UDP();
	~UDP();

	int header_length();
	int read(uint8_t* buf, int max_size);
	int write(uint8_t* buf, int max_size);
	char* to_string();
};
