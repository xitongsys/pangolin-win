#include "ethernet.h"
#include "util.h"
#include <string>

Ethernet::Ethernet() {}

int Ethernet::header_length() {
	return 14;
}

int Ethernet::read(uint8_t* buf, int max_size) {
	if (max_size < 14) return -1;

	int p = 0;
	for (int i = 0; i < 6; i++) {
		dst[i] = read8(buf + p); p++;
	}
	for (int i = 0; i < 6; i++) {
		src[i] = read8(buf + p); p++;
	}
	type = read16(buf + p); p += 2;

	return p;
}

int Ethernet::write(uint8_t* buf, int max_size) {
	if (max_size < 14) return -1;
	
	int p = 0;
	for (int i = 0; i < 6; i++) {
		write8(dst[i], buf + p); p++;
	}
	for (int i = 0; i < 6; i++) {
		write8(src[i], buf + p); p++;
	}
	write16(type, buf + p); p += 2;

	return p;
}

string Ethernet::to_string() {
	char fmt[] = "dst: %X:%X:%X:%X:%X:%X\nsrc: %X:%X:%X:%X:%X:%X\ntype:%X\n";
	char buf[1024];
	memset(buf, 0, sizeof(char) * 1024);
	sprintf_s(buf, 1024, fmt, 
		dst[0], dst[1], dst[2], dst[3], dst[4], dst[5],
		src[0], src[1], src[2], src[3], src[4], src[5],
		type);
	return buf;
}
