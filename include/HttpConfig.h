#ifndef HTTPCONFIG_H
#define HTTPCONFIG_H

#include <iostream>
#include <limits>
#include <fstream>
#include <string>

using namespace std;

class Http_config {
	public:
		string config_file;
		string working_directory;
		string log_file;
		int worker_processes;

	public:
		Http_config(string file)
			: config_file(file) {
				init(config_file);
			}
		Http_config() {}
		void init(string file);
};

template <typename T>
int search_in_file(string file, string key, T &value)
{
	string str;
	ifstream input(file.c_str(), ios::in);
	if (!input) {
		cerr << "file error" << endl;
		return -1;
	}

	while(1)
	{
		input >> str;

		if (input.eof() || input.bad()) {
			break;
		} else if (str.at(0) == '#') {
			input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');	// skip line
		} else {
			if (str.compare(key) == 0) {
				input >> value;
				input.close();
				return 1;	// find it
			} else {
			input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');	// skip line
			}
		}
	}
	// cannot find
	input.close();
	return 0;
}

#endif
