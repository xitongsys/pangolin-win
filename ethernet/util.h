#pragma once
#include <cstdint>

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
	buf[0] = v >> 8;
	buf[1] = v >> 0;
}

void write32(uint32_t v, uint8_t* buf) {
	buf[0] = v >> 24;
	buf[1] = v >> 16;
	buf[2] = v >> 8;
	buf[3] = v >> 0;
}

void write64(uint64_t v, uint8_t* buf) {
	buf[0] = v >> 56;
	buf[1] = v >> 48;
	buf[2] = v >> 40;
	buf[3] = v >> 32;
	buf[4] = v >> 24;
	buf[5] = v >> 16;
	buf[6] = v >> 8;
	buf[7] = v >> 0;
}