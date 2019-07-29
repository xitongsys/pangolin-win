#pragma once
#include <cstdint>
#include <string>
#include "header.h"

class Ethernet : public Header{
public:
	uint8_t dst[6];
	uint8_t src[6];
	uint16_t type;

public:
	Ethernet();

	int header_length();
	int read(uint8_t* buf, int max_size);
	int write(uint8_t* buf, int max_size);
	string to_string();
};
