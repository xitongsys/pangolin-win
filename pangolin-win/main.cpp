#include <cstdio>
#include <iostream>
#include "tun.h"
using namespace std;


int main() {
	cout << "welcome to pangolin" << endl;
	Tun tun("rpcap://\\Device\\NPF_{BFFE366C-363D-41B9-8599-C92FADBF22C6}");
	cout << tun.start() << endl;
	while (true) {
		vector<uint8_t> data = tun.read();
		for (int i = 0; i < data.size(); i++) {
			//cout << data[i] << endl;
		}
		tun.write(data);
	}
	return 0;
}