#pragma once
#include <string>
#include "json.hpp"
#include "route.h"
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
	string tunip;

public:
	Route route;
	RouteItem *gateway;

	string to_string();
};
