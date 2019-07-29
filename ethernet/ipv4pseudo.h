#pragma once
#include <cstdint>
#include "header.h"

class IPv4Pseudo : public Header {
public:
	uint32_t src;
	uint32_t dst;
	uint8_t reserved;
	uint8_t protocol;
	uint16_t len;

public:
	IPv4Pseudo();

	int header_length();
	int read(uint8_t* buf, int max_size);
	int write(uint8_t* buf, int max_size);
	string to_string();
};

