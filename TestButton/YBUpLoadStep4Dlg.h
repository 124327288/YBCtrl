#ifndef __YBUPLOADSTEP4DLG_H__
#define __YBUPLOADSTEP4DLG_H__
#include "YBStepDlg.h"

class YBUpLoadStep4Dlg : public YBStepDlg{
public:
	YBUpLoadStep4Dlg();
	virtual ~YBUpLoadStep4Dlg();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

};

#endif //__YBUPLOADSTEP4DLG_H__