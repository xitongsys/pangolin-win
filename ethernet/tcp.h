#pragma once
#include <cstdint>
#include "header.h"

class TCP : public Header {
public:
	uint16_t src_port;
	uint16_t dst_port;
	uint32_t seq;
	uint32_t ack;
	uint8_t offset;
	uint8_t flags;
	uint16_t win;
	uint16_t checksum;
	uint16_t urg_pointer;
	uint8_t *opt;

public:
	TCP();
	~TCP();

	int header_length();
	int read(uint8_t* buf, int max_size);
	int write(uint8_t* buf, int max_size);
	string to_string();
};
