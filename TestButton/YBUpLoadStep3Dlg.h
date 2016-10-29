#ifndef __YBUPLOADSTEP3DLG_H__
#define __YBUPLOADSTEP3DLG_H__
#include "YBStepDlg.h"

class YBUpLoadStep3Dlg : public YBStepDlg{
public:
	YBUpLoadStep3Dlg();
	virtual ~YBUpLoadStep3Dlg();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

};

#endif //__YBUPLOADSTEP3DLG_H__