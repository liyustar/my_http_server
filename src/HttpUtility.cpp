#include "HttpUtility.h"
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

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
	if(3 == sscanf(startLine, "%s /%s HTTP/%s",
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

void star_printf(char *s, int len)
{
	for(int i=0; i<len; i++)
		printf("%d ", *(s+i));
	printf("\n");
}

// 解析头部字串
// 		1.  field: value	一定要有':' value不为空
// 		2.  value	两端可能会有空格' '
// 		3.  对参数有修改
// 		4.  不支持多行header
int Http_message::parseHeader(char *header)
{
	// sample: "Host: www.liyuxing.com"
	char *pos;
	pair<string, string> header_item;
	pos = strstr(header, ":");
	*pos = '\0';
	header_item.first = header;
	while(isspace(*(++pos)))
		;
	header = pos;
	pos = strstr(header, "\r\n");
	*pos = '\0';
	header_item.second = header;

	cout << "header--:" << header_item.first << ": " << header_item.second << endl;

	/*
	if(2 == sscanf(header, "%s%s",
				header_field, header_value))
	{
		printf("Header--: %s %s\n",
				header_field, header_value);
		// header_field 包含':'符号
	}
	else
	{
		perror("Header error");
		return -1;
	}
	*/

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



