#include "config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../ethernet/util.h"
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

	tunip = j.value("tun", "10.0.0.111");

	dns = j.value("dns", "1.1.1.1");

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

	uint32_t sip = str2ip(server_ip);
	RouteItem* it = route.getRoute(sip);
	if (it == NULL) {
		cout << "can't get route to server" << endl;
		exit(-1);
	}
	gateway = it;

	//set mtu to 1400
	char mtu_fmt[] = "netsh interface ipv4 set subinterface %d mtu=1400";
	char buf[1024];
	sprintf_s(buf, mtu_fmt, gateway->ifIndex);
	system(buf);

	//set dns to 1.1.1.1
	char dns_fmt[] = "netsh interface ipv4 set dnsservers %d static %s primary";
	sprintf_s(buf, dns_fmt, gateway->ifIndex, dns.c_str());
	system(buf);
}

string Config::to_string() {
	char fmt[] = "role: %s\nserver_ip: %s\nserver_port: %d\nprotocol: %s\ntoken: %s\nroute: %s\n";
	char buf[2048];
	sprintf_s(buf, 2048, fmt, role.c_str(), server_ip.c_str(), server_port, protocol.c_str(), token.c_str(), route.toString().c_str());
	return buf;
}

