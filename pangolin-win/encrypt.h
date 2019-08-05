#pragma once
#include <cstdint>
#include <vector>
#include "modes.h"
#include "aes.h"
#include "filters.h"

using namespace std;

class Encrypt{
public:
	static const int BLOCKSIZE = CryptoPP::AES::BLOCKSIZE;
	CryptoPP::byte key[BLOCKSIZE], iv[BLOCKSIZE];
	CryptoPP::AES::Encryption aesEncryption;
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption;
	CryptoPP::AES::Decryption aesDecryption;
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption;

	Encrypt(string key);
	~Encrypt();

	string encrypt(string& data);
	string decrypt(string& data);
};

