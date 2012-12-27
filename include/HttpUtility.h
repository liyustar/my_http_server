#ifndef HTTPUTILITY_H
#define HTTPUTILITY_H

#include <string>
#include <map>

using namespace std;

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

	private:
		http_method method;
		string version;
		string uri;
		map<string,string> headers;	// maybe need use multimap
		string body;	// the body can contain arbitrary binary data.

	public:
		Http_message();
		~Http_message();
		int parseStartLine(char *startLine);
		int parseHeader(char *header);	// can not parse multiline headers

		string buildMessage();
		string getMethodStr(http_method hm);
		http_method parseMethodStr(char *str);
};
	



#endif
