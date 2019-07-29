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

char* Ethernet::to_string() {
	return NULL;
}
