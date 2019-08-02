#include <cstdio>
#include <iostream>
#include "tun.h"
#include "udp_client.h"
#include "config.h"
using namespace std;
#pragma comment(lib, "Ws2_32.lib")

int main() {
	cout << "welcome to pangolin" << endl;
	Config config("config.txt");
	cout << config.to_string() << endl;
	Tun tun(config.server_ip);
	Udp_client udp_client(&config, &tun);

	if (!tun.start()) {
		exit(1);
	}

	udp_client.start();

	return 0;
}