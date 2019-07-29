#pragma once
#include <cstdint>

class Header {
public:
	virtual int header_length();
	virtual int read(uint8_t* buf, int max_size);
	virtual int write(uint8_t* buf, int max_size);
	virtual char* to_string();
};
