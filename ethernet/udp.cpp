#include "udp.h"
#include "util.h"

UDP::UDP() {}
UDP::~UDP() {}

int UDP::header_length() {
	return 8;
}

int UDP::read(uint8_t* buf, int max_size) {
	if (max_size < 8) return -1;

	int p = 0;
	src_port = read16(buf + p); p += 2;
	dst_port = read16(buf + p); p += 2;
	len = read16(buf + p); p += 2;
	checksum = read16(buf + p); p += 2;

	return p;
}

int UDP::write(uint8_t* buf, int max_size) {
	if (max_size < 8) return -1;

	int p = 0;
	write16(src_port, buf + p); p += 2;
	write16(dst_port, buf + p); p += 2;
	write16(len, buf + p); p += 2;
	write16(checksum, buf + p); p += 2;

	return p;
}

string UDP::to_string() {
	return NULL;
}
