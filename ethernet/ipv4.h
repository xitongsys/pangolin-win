#pragma once
#include <cstdint>
#include "header.h"

class IPv4 : public Header {
public:
	uint8_t version;
	uint8_t tos;
	uint16_t len;
	uint16_t id;
	uint16_t offset;
	uint8_t ttl;
	uint8_t protocol;
	uint16_t checksum;
	uint32_t src;
	uint32_t dst;
	uint8_t* opt;

public:
	IPv4();
	~IPv4();

	int header_length();
	int read(uint8_t* buf, int max_size);
	int write(uint8_t* buf, int max_size);
	char* to_string();

private:
	void set_checksum(uint8_t* buf, int pos, int size);
};
