#ifndef __YBFSDOWNLOADER_H__
#define __YBFSDOWNLOADER_H__
#include "frame/YBCtrlBase.h"
#include "button/YBButton.h"
#include "progressbar/YBProgressBar.h"
#include <ShellAPI.h>

#define WM_SHOWTASK (WM_USER+200)
#define YBFSDLID_BTN_MINIMIZE (0x00000300)
#define YBFSDLID_BTN_CLOSE (0x00000301)
#define YBFSDLID_PROGRESSBAR (0x00000302)
#define YBFSDLID_BTN_CANCEL (0x00000303)

class YBFSDownLoader : public ybctrl::YBCtrlBase {
public:
	typedef enum _ENUMBTNSTATUS {
		EBTN_STATUS_CANCEL = 0,
		EBTN_STATUS_INSTALL = 1
	} ENUMBTNSTATUS; 
	typedef std::basic_string< TCHAR > tstring_type;

public:
	YBFSDownLoader();
	virtual ~YBFSDownLoader();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE();

public:
	HWND create( int nWidth, int nHeight, HWND hWndParent );
	void setPos( __int64 i64PosCur, __int64 i64PosMax );
	void onFinished( bool bSuccess );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_SHOWTASK( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDBLCLK( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_CTLCOLOR( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	void _clipWnd();
	void _setWndTransparent();
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageId );
	void _drawStatus( HDC hDC );
	void _drawBk( HDC hDC );
	void _drawTitle( HDC hDC );
	void _loadFont();
	void _loadImage();
	void _init();
	bool _getTitleRect( RECT& rcTitle ) const;

private:
	ENUMBTNSTATUS m_eBtnStatus;
	NOTIFYICONDATA m_nid;
	ybctrl::YBButton m_btnMin;
	ybctrl::YBButton m_btnClose;
	ybctrl::YBButton m_btnCancel;
	ybctrl::YBProgressBar m_progressBar;
	unsigned int m_uImageIdBk;
	unsigned int m_uFontIdTitle;
	unsigned int m_uFontIdBtn;
	tstring_type m_tstrTitle;
	//
	unsigned int m_uImageIdPBBk;
	unsigned int m_uImageIdPBFT;
	unsigned int m_uImageIdMinBtnNor;
	unsigned int m_uImageIdMinBtnHov;
	unsigned int m_uImageIdMinBtnPre;
	unsigned int m_uImageIdCloseBtnNor;
	unsigned int m_uImageIdCloseBtnHor;
	unsigned int m_uImageIdCloseBtnPre;
	unsigned int m_uImageIdCancelBtnNor;
	unsigned int m_uImageIdCancelBtnHor;
	unsigned int m_uImageIdCancelBtnPre;
};

#endif //__YBFSDOWNLOADER_H__