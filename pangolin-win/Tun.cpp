#include "tun.h"
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
	readQueue.try_pop(res);
	return res;
}

bool Tun::write(vector<uint8_t>& data) {
	if (writeQueue.unsafe_size() > BUFFSIZE) return false;
	writeQueue.push(data);
	return true;
}

vector<uint8_t> Tun::_read() {
	vector<uint8_t> res;
	writeQueue.try_pop(res);
	return res;
}

bool Tun::_write(vector<uint8_t>& data) {
	if (readQueue.unsafe_size() > BUFFSIZE) return false;
	readQueue.push(data);
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

	read_thread = new thread(&Tun::packet_read, this);
	write_thread = new thread(&Tun::packet_write, this);
	return true;
}

void Tun::packet_read() {
	Frame frame;
	struct pcap_pkthdr* header;
	const u_char* data;
	while (true) {
		if (pcap_next_ex(adhandle, &header, &data) <= 0) continue;

		int rn = frame.read((uint8_t*)data, header->caplen);
		if (rn < 0) return;

		int b = frame.ethernet.header_length();
		vector<uint8_t> vs;
		for (int i = b; i < header->caplen; i++) {
			vs.push_back(data[i]);
		}
		_write(vs);
	}
}

void Tun::packet_write() {
	Frame frame;
	struct pcap_pkthdr* header;
	const u_char* data;
	uint8_t buf[6553];

	while (true) {
		vector<uint8_t> vs = _read();
		if (vs.size() <= 0) continue;
		
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

		int wn = ethernet.write(buf, 6553);
		if (wn < 0) continue;
		for (int i = 0; i < vs.size(); i++, wn++) {
			buf[wn] = vs[i];
		}

		pcap_sendpacket(adhandle, buf, wn);
	}
}