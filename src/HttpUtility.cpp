#include "HttpUtility.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <wait.h>
#include <iostream>
#include <string>

using namespace std;

Http_message::Http_message()
{
	method = HM_NONE;
	statusCode = 0;
	isSendBody = false;
}

Http_message::Http_message(string ver)
{
	version = ver;
	method = HM_NONE;
	statusCode = 0;
	isSendBody = false;
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

string Http_message::buildMsgHeader()
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

string Http_message::buildResponeHeader()
{
	string msg;
	// HTTP/1.1 200 OK
	msg = "HTTP/";
	msg += version + " "
			+ itostr(statusCode) + " "
			+ getCodeStr(statusCode) + "\r\n";

	map<string,string>::iterator pos;
	for( pos=headers.begin(); pos!=headers.end(); ++pos) {
	   msg += pos->first + ": " + pos->second + "\r\n";
	}
	msg += "\r\n";

	return msg;
}

string Http_message::buildResponeMsg()
{
	string msg;
	msg += buildResponeHeader();
	if(isSendBody)
		msg += body;
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

string Http_message::getCodeStr(int code)
{
	string str;
	switch(code) {
		case 200:	str=REASON_200;	break;
		case 404:	str=REASON_404;	break;
		case 501:	str=REASON_501;	break;
		default:	str="";	break;
	}
	return str;
}

void Http_message::makeHeader(int status, string contentType,
		const char *arg) 
{
	statusCode = status;
	headers.insert(make_pair("Content-type", contentType));
	switch(status) {
		case 200:
			break;
		case 404:
			body = BODY_404;
			isSendBody = true;
			break;
		case 501:
			body = BODY_501;
			isSendBody = true;
			break;
		default:
			perror("not implement this Status Code\n");
			exit(1);
	}
}

/*
 * HTTP message 的处理类(method)
 */
void process_rq(Http_message msg, int clntfd) 
{
	int pid;
	// create a new process and return if not the child
	if ( (pid=fork()) != 0)
	{
		waitpid(pid, NULL, 0);
		return;
	}

	if (Http_message::HM_GET != msg.method)
		cannot_do(clntfd);
	else if ( !isexist(msg.uri.c_str()) )
		do_404(msg.uri.c_str(), clntfd);
	/*
	else if ( isadir(msg.uri.c_str()) )
		do_ls(msg.uri.c_str(), clntfd);
	else if ( ends_in_cgi(msg.uri.c_str()) )
		do_exec(msg.uri.c_str(), clntfd);
	else
		do_cat(msg.uri.c_str(), clntfd);
		*/
}

void cannot_do(int fd)
{
	Http_message respone("1.1");
	string rsp_buf;
	respone.makeHeader(501, "text/plain");
	rsp_buf = respone.buildResponeMsg();
	if (send(fd, rsp_buf.data(), rsp_buf.size(), 0) != rsp_buf.size())
	{
		perror("send 501 error\n");
		exit(1);
	}
	exit(0);
}

void do_404(const char *item, int fd)
{
	Http_message respone("1.1");
	string rsp_buf;
	respone.makeHeader(404, "text/plain", item);
	rsp_buf = respone.buildResponeMsg();
	if (send(fd, rsp_buf.data(), rsp_buf.size(), 0) != rsp_buf.size())
	{
		perror("send 404 error\n");
		exit(1);
	}
	exit(0);
}

// void do_ls(char *dir, int fd);
// void do_exec(char *prog, int fd);
// void do_cat(char *filename, int fd);

int isadir(const char *filename)
{
	struct stat info;
	return (stat(filename, &info) != -1
			&& S_ISDIR(info.st_mode));
}

int isexist(const char *filename)
{
	struct stat info;
	return (stat(filename, &info) != -1);
}

int ends_in_cgi(const char *filename)
{
	if (strcmp(file_type(filename), "cgi") == 0)
		return true;
	else
		return false;
}

const char * file_type(const char *filename)
{
	const char *cp;
	if ( (cp = strrchr(filename, '.')) == NULL)
		return "";
	else
		return cp + 1;
}

string itostr(int num)
{
	string str;
	char numstr[12];
	sprintf(numstr, "%d", num);
	str = numstr;
	return str;
}
