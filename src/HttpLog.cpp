#include "HttpLog.h"

#include <fstream>
using namespace std;

void Http_log::addlog(string str)
{
	ofstream output(logfile.c_str(), ios::out | ios::app);
	output.write(str.data(), str.size());
	output.close();
}
