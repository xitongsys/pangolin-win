#include "frame.h"
#include "const.h"
#include "util.h"

Frame::Frame() {
	ethernet = Ethernet();
	ipv4 = IPv4();
	tcp = TCP();
	udp = UDP();
	content = vector<uint8_t>(0);
}

int Frame::read(uint8_t* buf, int max_size) {
	int p = 0;
	int rn = 0;

	rn = ethernet.read(buf, max_size - p);
	if (rn < 0) return -1;
	p += rn;

	rn = ipv4.read(buf, max_size - p);
	if (rn < 0) return -1;
	p += rn;

	switch (ipv4.protocol) {
	case TCPID:
		rn = tcp.read(buf, max_size - p);
		if (rn < 0) return -1;
		p += rn;
		break;

	case UDPID:
		rn = udp.read(buf, max_size - p);
		if (rn < 0) return -1;
		p += rn;
		break;
	}

	for (; p < ipv4.len; p++) {
		uint8_t a = read8(buf + p);
		content.push_back(a);
	}

	return p;
}

int Frame::write(uint8_t* buf, int max_size) {
	int p = 0;
	int wn = 0;

	wn = ethernet.write(buf, max_size - p);
	if (wn < 0) return -1;
	p += wn;

	wn = ipv4.write(buf, max_size - p);
	if (wn < 0) return -1;
	p += wn;

	switch (ipv4.protocol)
	{
	case TCPID:
		wn = tcp.write(buf, max_size - p);
		if (wn < 0) return -1;
		p += wn;
		break;

	case UDPID:
		wn = udp.write(buf, max_size - p);
		if (wn < 0) return -1;
		p += wn;
		break;
	}

	if (p + content.size() > max_size) return -1;

	for (int i = 0; i < content.size(); i++) {
		write8(content[i], buf + p); p++;
	}

	int header_len = ethernet.header_length() + ipv4.header_length();

	if (ipv4.protocol == TCPID) {
		int checksum_pos = header_len + 16;
		set_checksum(buf, checksum_pos, p);
	}
	else if (ipv4.protocol == UDPID) {
		int checksum_pos = header_len + 6;
		set_checksum(buf, checksum_pos, p);
	}

	return p;
}


void Frame::set_checksum(uint8_t* buf, int pos, int size) {
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

	int prefix_pos = ethernet.header_length() + ipv4.header_length();
	for (int i = prefix_pos; i < size; i += 2) {
		s += read16(buf + i);
	}

	if ((size - prefix_pos) % 2 == 1) {
		s += (uint32_t)(buf[size - 1]) << 16;
	}

	while ((s >> 16) > 0) {
		s = (s >> 16) + (s & 0xffff);
	}

	write16(~(uint16_t)(s), buf + pos);
}