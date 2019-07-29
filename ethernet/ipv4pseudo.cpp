#include "ipv4pseudo.h"
#include "util.h"

IPv4Pseudo::IPv4Pseudo() {}

int IPv4Pseudo::header_length() {
	return 12;
}

int IPv4Pseudo::read(uint8_t* buf, int max_size) {
	if (max_size < 12) return -1;

	int p = 0;
	src = read32(buf + p); p += 4;
	dst = read32(buf + p); p += 4;
	reserved = read8(buf + p); p += 1;
	protocol = read8(buf + p); p += 1;
	len = read16(buf + p); p += 2;

	return p;
}

int IPv4Pseudo::write(uint8_t* buf, int max_size) {
	if (max_size < 12) return -1;

	int p = 0;
	write32(src, buf + p); p += 4;
	write32(dst, buf + p); p += 4;
	write8(reserved, buf + p); p += 1;
	write8(protocol, buf + p); p += 1;
	write16(len, buf + p); p += 2;

	return p;
}

string IPv4Pseudo::to_string() {
	return NULL;
}