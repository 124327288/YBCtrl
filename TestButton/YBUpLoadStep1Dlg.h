#ifndef __YBUPLOADSTEP1DLG_H__
#define __YBUPLOADSTEP1DLG_H__
#include "YBStepDlg.h"

class YBUpLoadStep1Dlg : public YBStepDlg{
public:
	YBUpLoadStep1Dlg();
	virtual ~YBUpLoadStep1Dlg();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

};

#endif //__YBUPLOADSTEP1DLG_H__