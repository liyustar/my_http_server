#include "HttpUtility.h"
#include <stdio.h>
#include <string.h>

Http_message::Http_message()
{
	method = HM_NONE;
}

Http_message::~Http_message()
{
}

int Http_message::parseStartLine(char *startLine)
{
	// sample: "GET /index.html HTTP/1.0"
	char method_str[BUFSIZ], request_uri[BUFSIZ], http_version[BUFSIZ];
	if(3 == sscanf(startLine, "%s /%s HTTP/%s\r\n",
			method_str, request_uri, http_version) )
		printf("start Line: %s %s %s\n",
			method_str, request_uri, http_version);
	else
	{
		perror("start Line error");
		return -1;
	}

	method = parseMethodStr(method_str);
	uri = request_uri;
	version = http_version;

	return 0;
}

int Http_message::parseHeader(char *header)
{
	// sample: "Host: www.liyuxing.com"
	char header_field[64], header_value[BUFSIZ];
	if(2 == sscanf("%s:%s\r\n",
				header_field, header_value))
		printf("Header--: %s %s\n",
				header_field, header_value);
	else
	{
		perror("Header error");
		return -1;
	}

	pair<string, string> header_item;
	header_item.first = header_field;
	header_item.second = header_value;

	headers.insert(header_item);
	return 0;
}

Http_message::http_method Http_message::parseMethodStr(char *str)
{
	size_t count = 8;
	if(0 == strcmp("GET", str))	return HM_GET;
	else if(0 == strcmp("PUT", str))	return HM_PUT;
	else if(0 == strcmp("DELETE", str))	return HM_DELETE;
	else if(0 == strcmp("POST", str))	return HM_POST;
	else if(0 == strcmp("HEAD", str))	return HM_HEAD;
	else return HM_NONE;
}

string Http_message::buildMessage()
{
	string msg;
	msg += getMethodStr(method) + " "
			+ "/" + uri + " " + "HTTP/" + version + "\r\n";

	map<string,string>::iterator pos;
	for( pos=headers.begin(); pos!=headers.end(); ++pos) {
	   msg += pos->first + ": " + pos->second + "\r\n";
	}
	msg += "\r\n";

	// add body;
	return msg;
}

string Http_message::getMethodStr(http_method hm)
{
	string str;
	switch(hm) {
		case HM_NONE:	str="NONE";	break;
		case HM_GET:	str="GET";	break;
		case HM_PUT:	str="PUT";	break;
		case HM_DELETE:	str="DELETE";	break;
		case HM_POST:	str="POST";	break;
		case HM_HEAD:	str="HEAD";	break;
	}
	return str;
}



