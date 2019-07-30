#include <cstdio>
#include <cstring>
#include "ipv4.h"
#include "util.h"

IPv4::IPv4() {
	opt = NULL;
}

IPv4::~IPv4() {
	if (opt != NULL) {
		delete opt;
	}
}

int IPv4::header_length() {
	return (version & (0xf)) * 4;
}

int IPv4::read(uint8_t* buf, int max_size) {
	if (max_size < 20) return -1;
	int p = 0;
	version = read8(buf + p); p += 1;
	tos = read8(buf + p); p += 1;
	len = read16(buf + p); p += 2;
	id = read16(buf + p); p += 2;
	offset = read16(buf + p); p += 2;
	ttl = read8(buf + p); p += 1;
	protocol = read8(buf + p); p += 1;
	checksum = read16(buf + p); p += 2;
	src = read32(buf + p); p += 4;
	dst = read32(buf + p); p += 4;

	return p;

	int ln = header_length();
	if (ln > max_size) return -1;
	if (opt != NULL) delete opt;
	opt = new uint8_t[ln - 20];
	for (int i = 0; i < ln - 20; i++) {
		opt[i] = read8(buf + p); p++;
	}
	return p;
}

int IPv4::write(uint8_t* buf, int max_size) {
	if (max_size < 20) return -1;
	int p = 0;
	write8(version, buf + p); p += 1;
	write8(tos, buf + p); p += 1;
	write16(len, buf + p); p += 2;
	write16(id, buf + p); p += 2;
	write16(offset, buf + p); p += 2;
	write8(ttl, buf + p); p += 1;
	write8(protocol, buf + p); p += 1;
	write16(checksum, buf + p); p += 2;
	write32(src, buf + p); p += 4;
	write32(dst, buf + p); p += 4;

	int ln = header_length();
	if (ln > max_size) return -1;

	if (opt != NULL) {
		for (int i = 0; i < ln - 20; i++) {
			write8(opt[i], buf + p); p++;
		}
	}
	set_checksum(buf, 10, p);
	return p;
}

string IPv4::to_string() {
	char fmt[] = "version: %X\ntos: %X\nlen: %X\nid: %X\noffset: %X\nttl: %X\nprotocol: %X\nchecksum: %X\nsrc: %s\ndst: %s\n";
	char buf[1024];
	sprintf_s(buf, fmt, version, tos, len, id, offset, ttl, protocol, checksum, ip2str(src).c_str(), ip2str(dst).c_str());
	return string(buf);
}

void IPv4::set_checksum(uint8_t* buf, int pos, int size) {
	buf[pos] = 0; buf[pos + 1] = 0;
	uint32_t s = 0;
	for (int i = 0; i < size; i += 2) {
		uint32_t v = read16(buf + i);
		s += v;
	}
	while ((s >> 16) > 0) {
		s = (s >> 16) + (s & 0xffff);
	}

	uint16_t checksum = ~uint16_t(s);
	write16(checksum, buf + pos);
}