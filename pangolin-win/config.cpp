#include "config.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
using json = nlohmann::json;

Config::Config(string fname) {
	ifstream infile(fname);
	json j = json::parse(infile);

	role = j.value("role", "client");
	server_addr = j.value("server", "0.0.0.0:12345");
	int i = 0;
	while (i < server_addr.size() && server_addr[i]!=':') {
		i++;
	}

	if (i < server_addr.size()) {
		server_ip = server_addr.substr(0, i);
		stringstream ss; ss << server_addr.substr(i + 1, server_addr.size());
		ss >> server_port;
	}

	protocol = j.value("protocol", "udp");
	auto tokens = j["tokens"];
	for (auto it = tokens.begin(); it != tokens.end(); it++) {
		token = *it;
		break;
	}
}

string Config::to_string() {
	char fmt[] = "role: %s\nserver_ip: %s\nserver_port: %d\nprotocol: %s\ntoken: %s\n";
	char buf[1024];
	sprintf_s(buf, 1024, fmt, role.c_str(), server_ip.c_str(), server_port, protocol.c_str(), token.c_str());
	return buf;
}

