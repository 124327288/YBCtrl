#ifndef __YBUPLOADSTEP5DLG_H__
#define __YBUPLOADSTEP5DLG_H__
#include "YBStepDlg.h"

class YBUpLoadStep5Dlg : public YBStepDlg{
public:
	YBUpLoadStep5Dlg();
	virtual ~YBUpLoadStep5Dlg();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

};

#endif //__YBUPLOADSTEP5DLG_H__