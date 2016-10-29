#ifndef __YBITPOPUPPB_H__
#define __YBITPOPUPPB_H__
#include "progressbar/YBProgressBar.h"

class YBITPopupPB : public ybctrl::YBProgressBar{
public:
	YBITPopupPB();
	virtual ~YBITPopupPB();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( int x, int y, int nWidth, int nHeight, HINSTANCE hInstance );
	
private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _LoadImage();
	void _LoadFont();
	void _init();

};

#endif //__YBPITPOPUPPB_H__