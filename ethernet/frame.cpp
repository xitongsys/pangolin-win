#include "frame.h"
#include "const.h"
#include "util.h"
#include <iostream>
using namespace std;

Frame::Frame() {
	ethernet = Ethernet();
	ipv4 = IPv4();
	tcp = TCP();
	udp = UDP();
	content = vector<uint8_t>(0);
}

int Frame::read(int layer, uint8_t* buf, int max_size) {
	content = vector<uint8_t>(0);
	int p = 0;
	int rn = 0;

	if (layer <= 2) {
		rn = ethernet.read(buf + p, max_size - p);
		if (rn < 0) return -1;
		p += rn;
	}

	rn = ipv4.read(buf + p, max_size - p);
	if (rn < 0) return -1;
	p += rn;

	switch (ipv4.protocol) {
	case TCPID:
		rn = tcp.read(buf + p, max_size - p);
		if (rn < 0) return -1;
		p += rn;

		for (int i = 0; i < ipv4.len - ipv4.header_length() - tcp.header_length(); i++) {
			uint8_t a = read8(buf + p);
			content.push_back(a);
			p++;
		}
		break;

	case UDPID:
		rn = udp.read(buf + p, max_size - p);
		if (rn < 0) return -1;
		p += rn;
		for (int i = 0; i < ipv4.len - ipv4.header_length() - udp.header_length(); i++) {
			uint8_t a = read8(buf + p);
			content.push_back(a);
			p++;
		}
		break;
	}

	return p;
}

int Frame::write(int layer, uint8_t* buf, int max_size) {
	int p = 0;
	int wn = 0;

	if (layer <= 2) {
		wn = ethernet.write(buf + p, max_size - p);
		if (wn < 0) return -1;
		p += wn;
	}

	wn = ipv4.write(buf + p, max_size - p);
	if (wn < 0) return -1;
	p += wn;

	switch (ipv4.protocol)
	{
	case TCPID:
		wn = tcp.write(buf + p, max_size - p);
		if (wn < 0) return -1;
		p += wn;
		break;

	case UDPID:
		wn = udp.write(buf + p, max_size - p);
		if (wn < 0) return -1;
		p += wn;
		break;
	}

	if (p + content.size() > max_size) return -1;

	for (int i = 0; i < content.size(); i++) {
		write8(content[i], buf + p); p++;
	}

	int header_len = 0;
	if (layer <= 2) {
		header_len += ethernet.header_length();
	}
	header_len += ipv4.header_length();

	if (ipv4.protocol == TCPID) {
		int checksum_pos = header_len + 16;
		set_checksum(layer, buf, checksum_pos, p);
	}
	else if (ipv4.protocol == UDPID) {
		int checksum_pos = header_len + 6;
		set_checksum(layer, buf, checksum_pos, p);
	}

	return p;
}


void Frame::set_checksum(int layer, uint8_t* buf, int pos, int size) {
	IPv4Pseudo ipv4p;
	ipv4p.src = ipv4.src;
	ipv4p.dst = ipv4.dst;
	ipv4p.reserved = 0;
	ipv4p.protocol = ipv4.protocol;

	buf[pos] = 0; 
	buf[pos + 1] = 0;
	if (ipv4.protocol == TCPID) {
		ipv4p.len = tcp.header_length() + content.size();
	}
	else if (ipv4.protocol == UDPID) {
		ipv4p.len = udp.header_length() + content.size();
	}

	uint32_t s = 0;
	uint8_t* ipv4p_buf = new uint8_t[ipv4p.header_length()];
	ipv4p.write(ipv4p_buf, ipv4p.header_length());
	for (int i = 0; i < ipv4p.header_length(); i += 2) {
		s += read16(ipv4p_buf + i);
	}
	
	int prefix_pos = 0;
	if (layer <= 2) {
		prefix_pos += ethernet.header_length();
	}
	prefix_pos += ipv4.header_length();

	for (int i = prefix_pos; i < size - 1; i += 2) {
		s += read16(buf + i);
	}

	if ((size - prefix_pos) % 2 == 1) {
		s += ((uint32_t)(buf[size - 1])) << 8;
	}

	while ((s >> 16) > 0) {
		s = (s >> 16) + (s & 0xffff);
	}

	uint16_t checksum = ~(uint16_t)(s);
	write16(checksum, buf + pos);
}