#include <cstdio>
#include <iostream>
#include "tun.h"
using namespace std;


int main() {
	cout << "welcome to pangolin" << endl;
	Tun tun("name");
	tun.start();
	while (true) {
		vector<uint8_t> data = tun.read();
		tun.write(data);
	}
	return 0;
}