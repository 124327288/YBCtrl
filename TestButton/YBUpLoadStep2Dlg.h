#ifndef __YBUPLOADSTEP2DLG_H__
#define __YBUPLOADSTEP2DLG_H__
#include "YBStepDlg.h"
#include "checkbox/YBCheckBox.h"

class YBUpLoadStep2Dlg : public YBStepDlg{
public:
	YBUpLoadStep2Dlg();
	virtual ~YBUpLoadStep2Dlg();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	ybctrl::YBCheckBox m_checkBoxDocument;
	ybctrl::YBCheckBox m_checkBoxDataBank;
	ybctrl::YBCheckBox m_checkBoxMailAttachment;
};

#endif //__YBUPLOADSTEP1DLG_H__