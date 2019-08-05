#pragma once
#include <cstdint>
#include <string>
using namespace std;

uint8_t read8(uint8_t* buf);
uint16_t read16(uint8_t* buf);
uint32_t read32(uint8_t* buf);
uint64_t read64(uint8_t* buf);

void write8(uint8_t v, uint8_t* buf);
void write16(uint16_t v, uint8_t* buf);
void write32(uint32_t v, uint8_t* buf);
void write64(uint64_t v, uint8_t* buf);

uint32_t revIp(uint32_t ip);
string ip2str(uint32_t ip);
uint32_t str2ip(string s);