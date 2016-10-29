#ifndef __YBBUTTON4CNDLG_H__
#define __YBBUTTON4CNDLG_H__
#include "button/YBButton.h"

class YBButton4CNDlg : public ybctrl::YBButton {
public:
	YBButton4CNDlg();
	~YBButton4CNDlg();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance );

private:
	void _onWM_MOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	WORD m_wordCtrlID;
};

#endif //__YBBUTTON4CNDLG_H__