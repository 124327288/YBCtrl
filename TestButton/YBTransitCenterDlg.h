#ifndef __YBTRANSITCENTERDLG_H__
#define __YBTRANSITCENTERDLG_H__
#include "button/YBButtonX.h"
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "YBUpLoadPage.h"
#include "YBDownLoadPage.h"

#define YBTRACENDLG_BODY_ID (0x00000201)
#define YBTRACENDLG_BTNUPLOAD_ID (0x00000202)
#define YBTRACENDLG_BTNDOWNLOAD_ID (0x00000203)

class YBTransitCenterDlg : public ybctrl::YBCtrlBase{
public:
	typedef std::basic_string< TCHAR > tstring_type;
public:
	YBTransitCenterDlg();
	virtual ~YBTransitCenterDlg();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent );


private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _drawStatus( HDC hDC );
	void _drawBk( HDC hDC );
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageId );
	void _loadImage();
	void _loadFont();
	void _init();

private:
	unsigned int m_uImageIdBk;
	unsigned int m_uImageIdBtnNor;
	unsigned int m_uImageIdBtnHov;
	unsigned int m_uImageIdBtnPre;
	unsigned int m_uImageIdBtnXNor;
	unsigned int m_uImageIdBtnXHov;
	unsigned int m_uImageIdBtnXPre;
	//
	ybctrl::YBButtonX m_btnXUpLoad;
	ybctrl::YBButtonX m_btnXDownLoad;
	YBUpLoadPage m_uploadPage;
	YBDownLoadPage m_downloadPage;
};

#endif //__YBTRANSITCENTERDLG_H__