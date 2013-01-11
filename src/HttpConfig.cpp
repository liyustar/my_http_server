#include "HttpConfig.h"
void Http_config::init(string file)
{
	search_in_file(file, "working_directory", working_directory);
	search_in_file(file, "log_file", log_file);
	search_in_file(file, "worker_processes", worker_processes);
}

	


