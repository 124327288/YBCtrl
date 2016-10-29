#ifndef __YBTESTBAR_H__
#define __YBTESTBAR_H__
#include "YBTopMostBar.h"
#include "button/YBButtonX.h"

class YBTestBar : public YBTopMostBar {
public:
	YBTestBar();
	virtual ~YBTestBar();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND Create( int x, int y, int nWidth, int nHeight, HINSTANCE hInstance );

private:
	ybctrl::YBButtonX m_btnxELawyer;
	ybctrl::YBButtonX m_btnxBaiDu;
};


#endif //__YBTESTBAR_H__