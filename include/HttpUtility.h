#ifndef HTTPUTILITY_H
#define HTTPUTILITY_H

#include <string>
#include <map>

using namespace std;

// reason phrases
#define Reason_200 "OK"
#define Reason_404 "Not Found"
#define Reason_501 "Not Implemented"

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
		~Http_message();
		int parseStartLine(char *startLine);
		int parseHeader(char *header);	// can not parse multiline headers

		string buildMsgHeader();
		string getMethodStr(http_method hm);
		http_method parseMethodStr(char *str);

		void makeHeader(int status, string contentType, char *arg=NULL);
};

// HTTP message 的处理类(method)
// uup P376
void process_rq(Http_message msg, int clntfd);
void cannot_do(int fd);
void do_404(char *item, int fd);
void do_ls(char *dir, int fd);
void do_exec(char *prog, int fd);
void do_cat(char *filename, int fd);

int isadir(char *filename);
int not_exist(char *filename);
int ends_in_cgi(char *filename);

char * file_type(char *filename);

#endif
