#ifndef __YBPROGRESSBAR4DL_H__
#define __YBPROGRESSBAR4DL_H__
#include "progressbar/YBProgressBar.h"

class YBProgressBar4DL : public ybctrl::YBProgressBar{
public:
	YBProgressBar4DL();
	virtual ~YBProgressBar4DL();

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

#endif //__YBPROGRESSBAR4DL_H__