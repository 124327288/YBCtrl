#ifndef _FTPANDHTTPDOWNLOAD_H__
#define _FTPANDHTTPDOWNLOAD_H__

class FtpAndHttpDownload {
public:
	FtpAndHttpDownload();
	~FtpAndHttpDownload();

public:
	bool HttpDownload( const TCHAR* HttpUrl, const TCHAR* save_as );
	bool FTPdownload(const TCHAR* FtpUrl, const TCHAR* user, const TCHAR* password, const TCHAR* save_as);

};

#endif //_FTPANDHTTPDOWNLOAD_H__