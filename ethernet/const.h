#pragma once
#include <cstdint>

const uint8_t TCPID = 6;
const uint8_t UDPID = 17;
const uint8_t IMCPID = 1;

const uint8_t FIN = 0x01;
const uint8_t SYN = 0x02;
const uint8_t RST = 0x04;
const uint8_t PSH = 0x08;
const uint8_t ACK = 0x10;
const uint8_t URG = 0x20;
const uint8_t ECE = 0x40;
const uint8_t CWR = 0x80;
	
