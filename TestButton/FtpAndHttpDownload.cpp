#include "stdafx.h"
#include "FtpAndHttpDownload.h"
#include <windows.h>
#include <wininet.h>
#include <tchar.h>
#include <AtlBase.h>
#include <stdio.h>
#pragma comment(lib, "wininet.lib")

#define MAXBLOCKSIZE 1024

FtpAndHttpDownload::FtpAndHttpDownload() {

}

FtpAndHttpDownload::~FtpAndHttpDownload() {

}

bool FtpAndHttpDownload::HttpDownload(const TCHAR* HttpUrl, const TCHAR* save_as) {
	char BlockBuffer[MAXBLOCKSIZE];
	ULONG ReadSize = 1;
	HINTERNET hInternet = NULL;
	FILE* fp = NULL;

	if ((HttpUrl == NULL) || (save_as == NULL))
	{
		return false;
	}
	hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL)
	{
		int error = GetLastError();
		return false;
	}
	HINTERNET hSession = InternetOpenUrl(hInternet, (LPCTSTR)HttpUrl, NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (hSession == NULL)
	{
		TCHAR InfoBuffer[MAXBLOCKSIZE];
		ULONG ErrNo = 0;
		ULONG BufferLen = 1024;
		InternetGetLastResponseInfo(&ErrNo, InfoBuffer, &BufferLen);
		InternetCloseHandle(hInternet);
		int error = GetLastError();
		return false;
	}
	
	if (( 0 == fopen_s( &fp, CT2A(save_as), "wb")) != NULL )
	{
		while (ReadSize)
		{
			InternetReadFile(hSession, BlockBuffer, MAXBLOCKSIZE - 1, &ReadSize);
			fwrite(BlockBuffer, sizeof(char), ReadSize, fp);
		}
		fclose(fp);
	}
	DWORD index = 0;
	HttpQueryInfo( hSession, HTTP_QUERY_CONTENT_TYPE, BlockBuffer, &ReadSize, &index );
	InternetCloseHandle(hSession);
	hSession = NULL;
	InternetCloseHandle(hInternet);
	hInternet = NULL;

	return true;
}

bool FtpAndHttpDownload::FTPdownload(const TCHAR* FtpUrl, const TCHAR* user, const TCHAR* password, const TCHAR* save_as) {
	HINTERNET hFtpSession = NULL;
	HINTERNET hFtpFile = NULL;
	TCHAR* FtpServerName = NULL;
	TCHAR* remoteFile = NULL;

	if ((FtpUrl == NULL) || (save_as == NULL))
	{
		return 1;
	}
	hFtpSession = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hFtpSession == NULL)
	{
		int error = GetLastError();
		return false;
	}

	//split the sever name and remote file string
	FtpServerName = _tcsdup(FtpUrl);
	TCHAR* pos = _tcschr(FtpServerName, '/');
	if (pos == NULL) {
		return false;
	} else {
		remoteFile = pos + 1;
		*pos = '\0';
	}

	hFtpFile = InternetConnect(hFtpSession, FtpServerName, INTERNET_DEFAULT_FTP_PORT, user, password, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if (hFtpFile == NULL)
	{
		InternetCloseHandle(hFtpSession);
		int error = GetLastError();
		return false;
	}

	if (FALSE == FtpGetFile(hFtpFile, remoteFile, save_as, TRUE, FTP_TRANSFER_TYPE_BINARY, FILE_ATTRIBUTE_ARCHIVE, 0))
	{
		//_tprintf(_T("get ftp file failed!\n"));
		return false;
	}

	free(FtpServerName);
	InternetCloseHandle(hFtpFile);
	hFtpFile = NULL;
	InternetCloseHandle(hFtpSession);
	hFtpSession = NULL;

	return true;
}