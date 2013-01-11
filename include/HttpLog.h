#ifndef HTTPLOG_H
#define HTTPLOG_H

#include "HttpConfig.h"
#include <string>
using namespace std;

extern Http_config http_config;

class Http_log {
	public:
		string logfile;
	public:
		Http_log() {
			logfile = http_config.log_file;
		}
		void addlog(string str);
};

#endif
