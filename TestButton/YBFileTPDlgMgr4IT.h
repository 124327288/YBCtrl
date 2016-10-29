#ifndef __YBFILETPDLGMGR4IT_H__
#define __YBFILETPDLGMGR4IT_H__
#include "YBFileTPDlg.h"
#include <deque>

class YBFileTPDlgMgr4IT {
public:
	typedef std::basic_string< TCHAR > tstring_type;

private:
	YBFileTPDlgMgr4IT();
public:
	static YBFileTPDlgMgr4IT* getInstance();
	virtual ~YBFileTPDlgMgr4IT();

public:
	void setMsgRecvHWND( HWND hWndMsgRecv ) { m_hWndMsgRecv = hWndMsgRecv; };
	void setStatus( YBFileTPDlg::EFILETPTYPE eFileTPType, tstring_type tstrTitle, __int64 i64CurPos, __int64 i64MaxPos );
	void onEvtFinish( YBFileTPDlg::EFILETPTYPE eFileTPType, bool bSuccess );

private:
	bool _hasMsgRecvHWND() const { return ( TRUE == ::IsWindow( m_hWndMsgRecv ) ); }
	void _init();
	void _unInit();
	YBFileTPDlg* _getFileTPDlg( YBFileTPDlg::EFILETPTYPE eFileTPType );
	void _adjustTPDlgPos();
	void _movewindow( bool bDlgDLVisible, bool bDlgULVisible );

private:
	unsigned int m_uFlag;
	HWND m_hWndMsgRecv;
	YBFileTPDlg m_dlgFileTPDlg4Download;
	YBFileTPDlg m_dlgFileTPDlg4Upload;
};

#endif //__YBFILETPDLGMGR4IT_H__