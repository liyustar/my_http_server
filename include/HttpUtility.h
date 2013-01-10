#ifndef HTTPUTILITY_H
#define HTTPUTILITY_H

#include <string>
#include <map>

using namespace std;

// reason phrases
#define REASON_200 "OK"
#define REASON_404 "Not Found"
#define REASON_501 "Not Implemented"

#define BODY_404 "The item you requested is not found\r\n"
#define BODY_501 "That command is not yet implemented\r\n"

// P124
class Http_message {
	public:
		enum http_method {
			HM_NONE	=	0,
			HM_GET	=	1,
			HM_PUT	=	2,
			HM_DELETE	=	3,
			HM_POST	=	4,
			HM_HEAD	=	5
		};

	// members
	public:
		http_method method;
		int statusCode;
		string version;
		string uri;
		map<string,string> headers;	// maybe need use multimap
		string body;	// the body can contain arbitrary binary data.

		bool isSendBody;

	public:
		Http_message();
		Http_message(string ver);
		~Http_message();
		int parseStartLine(char *startLine);
		int parseHeader(char *header);	// can not parse multiline headers

		string buildMsgHeader();
		string buildResponeHeader();
		string buildResponeMsg();

		string getMethodStr(http_method hm);
		string getCodeStr(int code);
		http_method parseMethodStr(char *str);

		void makeHeader(int status, string contentType, const char *arg=NULL);
};

// HTTP message 的处理类(method)
// uup P376
void process_rq(Http_message msg, int clntfd);
void cannot_do(int fd, Http_message::http_method method);
void do_404(const char *item, int fd, Http_message::http_method method);
void do_ls(const char *dir, int fd, Http_message::http_method method);
void do_exec(const char *prog, int fd, Http_message::http_method method);
void do_copy(const char *filename, int fd, Http_message::http_method method);
void do_cat(const char *filename, int fd, Http_message::http_method method);

int isadir(const char *filename);
int isexist(const char *filename);
int ends_in(const char *suffix, const char *filename);

const char * file_type(const char *filename);
string mime_type(const char *suffix);

string itostr(int num);

#endif
