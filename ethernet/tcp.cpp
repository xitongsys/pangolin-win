#include "tcp.h"
#include "util.h"

TCP::TCP() {
	opt = NULL;
}

TCP::~TCP() {
	if (opt != NULL) delete opt;
}

int TCP::header_length() {
	return (offset >> 4) * 4;
}

int TCP::read(uint8_t* buf, int max_size) {
	if (max_size < 20) return -1;

	int p = 0;
	src_port = read16(buf + p); p += 2;
	dst_port = read16(buf + p); p += 2;
	seq = read32(buf + p); p += 4;
	ack = read32(buf + p); p += 4;
	offset = read8(buf + p); p += 1;
	flags = read8(buf + p); p += 1;
	win = read16(buf + p); p += 2;
	checksum = read16(buf + p); p += 2;
	urg_pointer = read16(buf + p); p += 2;

	int ln = header_length();
	if (ln > max_size) return -1;
	if (opt != NULL) delete opt;
	opt = new uint8_t(ln - 20);
	for (int i = 0; i < ln - 20; i++) {
		opt[i] = read8(buf + p); p++;
	}
	return p;
}

int TCP::write(uint8_t* buf, int max_size) {
	if (max_size < header_length()) return -1;

	int p = 0;
	write16(src_port, buf + p); p += 2;
	write16(dst_port, buf + p); p += 2;
	write32(seq, buf + p); p += 4;
	write32(ack, buf + p); p += 4;
	write8(offset, buf + p); p += 1;
	write8(flags, buf + p); p += 1;
	write16(win, buf + p); p += 2;
	write16(checksum, buf + p); p += 2;
	write16(urg_pointer, buf + p); p += 2;

	int ln = header_length();
	for (int i = 0; i < ln - 20; i++) {
		write8(opt[i], buf + p); p++;
	}
	return p;
}

char* TCP::to_string() {
	return NULL;
}