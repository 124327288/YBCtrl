#ifndef __BASE64CODER_H__
#define __BASE64CODER_H__
#include <string>
using namespace std;

class Base64Coder {
public:
	Base64Coder();
	~Base64Coder();

public:
	//����
	string Encode( const unsigned char* Data,int nDataLength );
	//����
	string Decode(const char* Data,int DataByte,int& OutByte);
};

#endif //__BASE64CODER_H__