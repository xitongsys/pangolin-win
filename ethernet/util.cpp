#pragma once
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
using namespace std;

uint8_t read8(uint8_t* buf) {
	return buf[0];
}

uint16_t read16(uint8_t* buf) {
	return
		(uint16_t(buf[0]) << 8) +
		(uint16_t(buf[1]) << 0);
}

uint32_t read32(uint8_t* buf) {
	return
		(uint32_t(buf[0]) << 24) +
		(uint32_t(buf[1]) << 16) +
		(uint32_t(buf[2]) << 8) +
		(uint16_t(buf[3]) << 0);
}

uint64_t read64(uint8_t* buf) {
	return
		(uint64_t(buf[0]) << 56) +
		(uint64_t(buf[1]) << 48) +
		(uint64_t(buf[2]) << 40) +
		(uint64_t(buf[3]) << 32) +
		(uint64_t(buf[4]) << 24) +
		(uint64_t(buf[5]) << 16) +
		(uint64_t(buf[6]) << 8) +
		(uint64_t(buf[7]) << 0);
}

void write8(uint8_t v, uint8_t* buf) {
	buf[0] = v;
}

void write16(uint16_t v, uint8_t* buf) {
	buf[0] = (uint8_t)(v >> 8) & 0xff;
	buf[1] = (uint8_t)(v >> 0) & 0xff;
}

void write32(uint32_t v, uint8_t* buf) {
	buf[0] = (uint8_t)(v >> 24) & 0xff;
	buf[1] = (uint8_t)(v >> 16) & 0xff;
	buf[2] = (uint8_t)(v >> 8) & 0xff;
	buf[3] = (uint8_t)(v >> 0) & 0xff;
}

void write64(uint64_t v, uint8_t* buf) {
	buf[0] = (uint8_t)(v >> 56) & 0xff;
	buf[1] = (uint8_t)(v >> 48) & 0xff;
	buf[2] = (uint8_t)(v >> 40) & 0xff;
	buf[3] = (uint8_t)(v >> 32) & 0xff;
	buf[4] = (uint8_t)(v >> 24) & 0xff;
	buf[5] = (uint8_t)(v >> 16) & 0xff;
	buf[6] = (uint8_t)(v >> 8) & 0xff;
	buf[7] = (uint8_t)(v >> 0) & 0xff;
}

string ip2str(uint32_t ip) {
	char fmt[] = "%d.%d.%d.%d";
	char buf[16];
	sprintf_s(buf, 16, fmt, (ip >> 24) & 0xff, (ip >> 16) & 0xff, (ip >> 8) & 0xff, ip & 0xff);
	return buf;
}

uint32_t str2ip(string s) {
	int ln = s.size();
	uint32_t a, b, c, d;
	sscanf_s(s.c_str(), "%d.%d.%d.%d", &a, &b, &c, &d);
	return (a << 24) + (b << 16) + (c << 8) + d;
}