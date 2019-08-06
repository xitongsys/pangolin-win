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

	if (config.protocol == "ptcp") {
		cout << "starting ptcp client..." << endl;
		Ptcp_client ptcp_client(&config, &ptcp, &tun);
		if (!ptcp_client.start()) {
			cout << "start failed" << endl;
		}
	}
	else {
		cout << "starting udp client" << endl;
		Udp_client udp_client(&config, &tun);
		if (!udp_client.start()) {
			cout << "start failed" << endl;
		}
	}
	return 0;
}