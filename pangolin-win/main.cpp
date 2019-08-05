#include <cstdio>
#include <iostream>
#include "udp_client.h"
#include "ptcp.h"
#include "ptcp_client.h"
#include "config.h"
using namespace std;
#pragma comment(lib, "Ws2_32.lib")

int main() {
	cout << "welcome to pangolin" << endl;
	Config config("config.txt");
	cout << config.to_string() << endl;
	Tun tun(&config);
	Ptcp ptcp(&config);
	Ptcp_client ptcp_client(&config, &ptcp, &tun);
	ptcp_client.start();

	return 0;
}