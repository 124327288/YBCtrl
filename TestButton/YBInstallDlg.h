#ifndef __YBINSTALLDLG_H__
#define __YBINSTALLDLG_H__
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "scrollwnd/YBScrollWnd.h"
#include "button/YBButton.h"

#define YBINSTALLDLGID_LISTBOX (0x00000301)
#define YBINSTALLDLGID_BTN_INSTALL (0x00000302)
#define YBINSTALLDLGID_BTN_CANCEL (0x00000303)

class YBInstallDlg : public ybctrl::YBCtrlBase {
public:
	YBInstallDlg();
	~YBInstallDlg();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( int nWidth, int nHeight, HWND hWndParent );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_SHOWTASK( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	void _clipWnd();
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageId );
	void _drawStatus( HDC hDC );
	void _drawBk( HDC hDC );
	void _loadFont();
	void _loadImage();
	void _init();
	void _insertdata();

private:
	unsigned int m_uImageIdIDBk;
	ybctrl::YBScrollWnd m_listbox4IF;
	ybctrl::YBButton m_btn4Install;
	ybctrl::YBButton m_btn4Cancel;

private:
	unsigned int m_uImageIdScrBarBkH;
	unsigned int m_uImageIdScrBarBkV;
	unsigned int m_uImageIdScrTArrPreNor;
	unsigned int m_uImageIdScrTArrNor;
	unsigned int m_uImageIdScrTArrHov;
	unsigned int m_uImageIdScrTArrPre;
	unsigned int m_uImageIdScrBArrPreNor;
	unsigned int m_uImageIdScrBArrNor;
	unsigned int m_uImageIdScrBArrHov;
	unsigned int m_uImageIdScrBArrPre;
	unsigned int m_uImageIdScrFTNor;
	unsigned int m_uImageIdScrFTHov;
	unsigned int m_uImageIdScrFTPre;
	//
	unsigned int m_uImageIdWSAWMenuNor;
	unsigned int m_uImageIdWSAWMenuHov;
	// Btn
	unsigned int m_uImageIdBtnNor;
	unsigned int m_uImageIdBtnHov;
	unsigned int m_uImageIdBtnPre;
	unsigned int m_uFontIdBtn;
};

#endif //__YBINSTALLDLG_H__