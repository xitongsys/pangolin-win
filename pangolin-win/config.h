#pragma once
#include <string>
#include "json.hpp"
using namespace std;

class Config {
public:
	Config(string fname);

	string role;
	string server_addr;
	string server_ip;
	int server_port;
	string protocol;
	string token;

	string to_string();
};
