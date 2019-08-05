#include "encrypt.h"

Encrypt::Encrypt(string key) {
	if (key.size() >= BLOCKSIZE) {
		key = key.substr(0, BLOCKSIZE);
	}
	else {
		while (key.size() < BLOCKSIZE) {
			key.push_back(0);
		}
	}
	for (int i = 0; i < BLOCKSIZE; i++) {
		this->key[i] = key[i];
		this->iv[i] = key[i];
	}

	aesEncryption = CryptoPP::AES::Encryption(this->key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	cbcEncryption = CryptoPP::CBC_Mode_ExternalCipher::Encryption(aesEncryption, this->iv);
	aesDecryption = CryptoPP::AES::Decryption(this->key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	cbcDecryption = CryptoPP::CBC_Mode_ExternalCipher::Decryption(aesDecryption, this->iv);
}

Encrypt::~Encrypt() {}

string Encrypt::encrypt(string& data) {
	string res;
	CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(res));
	stfEncryptor.Put(reinterpret_cast<const unsigned char*>(data.c_str()), data.length());
	stfEncryptor.MessageEnd();
	return res;
}

string Encrypt::decrypt(string& data) {
	string res;
	CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(res));
	stfDecryptor.Put(reinterpret_cast<const unsigned char*>(data.c_str()), data.size());
	stfDecryptor.MessageEnd();
	return res;
}