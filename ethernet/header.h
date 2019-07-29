#pragma once
#include <cstdint>
#include <string>
using std::string;

class Header {
public:
	virtual int header_length() { return 0; }
	virtual int read(uint8_t* buf, int max_size) { return 0; }
	virtual int write(uint8_t* buf, int max_size) { return 0; }
	virtual string to_string() { return NULL; }
};
