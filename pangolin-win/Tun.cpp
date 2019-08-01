#include "tun.h"
#include "../ethernet/util.h"
#include "../ethernet/const.h"
#include <tchar.h>
#include <thread>
#include <iostream>

BOOL LoadNpcapDlls()
{
	_TCHAR npcap_dir[512];
	UINT len;
	len = GetSystemDirectory(npcap_dir, 480);
	if (!len) {
		fprintf(stderr, "Error in GetSystemDirectory: %x", GetLastError());
		return FALSE;
	}
	_tcscat_s(npcap_dir, 512, _T("\\Npcap"));
	if (SetDllDirectory(npcap_dir) == 0) {
		fprintf(stderr, "Error in SetDllDirectory: %x", GetLastError());
		return FALSE;
	}
	return TRUE;
}

Tun::Tun(string name) {
	adhandle = NULL;
	adname = name;
}

Tun::~Tun(){}

vector<uint8_t> Tun::read() {
	vector<uint8_t> res;
	Frame frame;
	struct pcap_pkthdr* header;
	const u_char* data;
	if (pcap_next_ex(adhandle, &header, &data) <= 0) {
		return res;
	}

	int rn = frame.read((uint8_t*)data, header->caplen);
	if (rn < 0) return res;

	uint32_t a = str2ip("149.129.49.157");
	uint32_t bi = str2ip("162.105.129.103");
	uint32_t c = str2ip("10.172.116.96");
	if (frame.ipv4.dst == a || frame.ipv4.src == a) return res;
	if (frame.ipv4.src != c) return res;

	int b = frame.ethernet.header_length();
	for (int i = b; i < header->caplen; i++) {
		res.push_back(data[i]);
	}

	return res;
}

bool Tun::write(vector<uint8_t>& data) {
	if (data.size() <= 0) return true;

	Ethernet ethernet;
	ethernet.type = 0x0800;
	ethernet.dst[0] = 0xff;
	ethernet.dst[1] = 0xff;
	ethernet.dst[2] = 0xff;
	ethernet.dst[3] = 0xff;
	ethernet.dst[4] = 0xff;
	ethernet.dst[5] = 0xff;

	ethernet.src[0] = 0xff;
	ethernet.src[1] = 0xff;
	ethernet.src[2] = 0xff;
	ethernet.src[3] = 0xff;
	ethernet.src[4] = 0xff;
	ethernet.src[5] = 0xff;

	int wn = ethernet.write(buf, BUFFSIZE);
	if (wn < 0) {
		return false;
	}

	for (int i = 0; i < data.size(); i++, wn++) {
		buf[wn] = data[i];
	}

	pcap_sendpacket(adhandle, buf, wn);
	return true;
}

bool Tun::write_test(vector<uint8_t>& data) {
	Frame frame;
	frame.ethernet.type = 0x0800;
	frame.ethernet.dst[0] = 0xff;
	frame.ethernet.dst[1] = 0xff;
	frame.ethernet.dst[2] = 0xff;
	frame.ethernet.dst[3] = 0xff;
	frame.ethernet.dst[4] = 0xff;
	frame.ethernet.dst[5] = 0xff;
	
	frame.ethernet.src[0] = 0xff;
	frame.ethernet.src[1] = 0xff;
	frame.ethernet.src[2] = 0xff;
	frame.ethernet.src[3] = 0xff;
	frame.ethernet.src[4] = 0xff;
	frame.ethernet.src[5] = 0xff;

	frame.ipv4.version = 0x45;
	frame.ipv4.tos = 0;
	frame.ipv4.len = 20 + 8 + data.size();
	frame.ipv4.id = 0;
	frame.ipv4.offset = 0;
	frame.ipv4.ttl = 255;
	frame.ipv4.protocol = UDPID;
	frame.ipv4.checksum = 0;
	frame.ipv4.src = str2ip("10.172.116.96");
	//frame.ipv4.dst = str2ip("149.129.49.157");
	frame.ipv4.dst = str2ip("10.172.116.96");

	frame.udp.src_port = 12345;
	frame.udp.dst_port = 12347;
	frame.udp.len = 8 + data.size();
	frame.udp.checksum = 0;

	frame.content = data;

	uint8_t buf[1500];
	int wn = frame.write(buf, 1500);

	pcap_sendpacket(adhandle, buf, wn);
	return true;
}

bool Tun::start() {
	if (adhandle != NULL) return false;
	if (!LoadNpcapDlls()) return false;

	if ((adhandle = pcap_open(adname.c_str(),
		65536,
		PCAP_OPENFLAG_PROMISCUOUS,
		1000,
		NULL,
		errbuf)) == NULL) {
		return false;
	}
	return true;
}

