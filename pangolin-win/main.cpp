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
	Tun tun("rpcap://\\Device\\NPF_{BFFE366C-363D-41B9-8599-C92FADBF22C6}");
	Udp_client udp_client(&config, &tun);

	tun.start();
	udp_client.start();

	return 0;
}