#ifndef __YBPAGE1_H__
#define __YBPAGE1_H__
#include "frame\YBCtrlBase.h"
#include "checkbox\YBCheckBox.h"
#include "radiobtn\YBRadioBtn.h"
#include "static\YBStatic.h"
#include "progressbar\YBProgressBar.h"
#include "progressbar\YBProgressCtrl.h"
#include "button\YBButton.h"
#include "YBTopMostBar.h"
#include "frame\YBCtrlImageBase.h"
#include "frame\YBCtrlImageStatic.h"
#include "dialog\YBModalDialog.h"
#include "YBITDlgFileUpLoadRaw.h"
#include "YBITDlgFileUpload.h"
#include "YBCheckBox4UD.h"
#include "YBUpLoadStep1Dlg.h"
#include "YBUpLoadStep2Dlg.h"
#include "YBUpLoadStep3Dlg.h"
#include "YBUpLoadStep4Dlg.h"
#include "YBUpLoadStep5Dlg.h"

namespace ybctrl {
	class YBCtrlImageBase;
	class YBCtrlImageStatic;
};
namespace Gdiplus{
	class Image;
}
//
class YBPage1 : public YBTopMostBar{
public:
	YBPage1();
	virtual ~YBPage1();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE();

public:
	HWND create( int x, int y, int nWidth,  int nHeight, HWND hWndParent );
	void loadFont();

private:
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_VOLBTNCHECK( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void loadImage();
	void _init();

private:
	//checkbox
	ybctrl::YBCheckBox m_testCheck;
	YBCheckBox4UD m_testCheck4UD;
	ybctrl::YBProgressBar m_testProBar;
	ybctrl::YBProgressCtrl m_testProCtrl;
	unsigned int m_uNoCIIdNormal;
	unsigned int m_uNoCIIdHover;
	unsigned int m_uNoCIIdPress;
	unsigned int m_uCIIdHover;
	unsigned int m_uCIIdPress;
	unsigned int m_uCIIdNormal;
	unsigned int m_uCIIdBK;
	unsigned int m_fontIdCheckTxt;
	// progress bar
	unsigned int m_uImageIdFT;
	unsigned int m_uImageIdBk;
	unsigned int m_uFontIdProTxt;
	// progress ctrl 
	unsigned int m_uProCtrlImageIdBK;
	unsigned int m_uProCtrlImageId;
	unsigned int m_uImageIdPCNormal;
	unsigned int m_uImageIdPCHover;
	unsigned int m_uImageIdPCPress;
	// radio button
	ybctrl::YBRadioBtn m_testRadioBtn2;
	ybctrl::YBRadioBtn m_testRadioBtn;
	unsigned int m_uImageIdRBNormal;
	unsigned int m_uImageIdRBHover;
	unsigned int m_uImageIdRBPress;
	unsigned int m_uImageIdNRBNormal;
	unsigned int m_uImageIdNRBHover;
	unsigned int m_uImageIdNRBPress;
	unsigned int m_uImageIdRBBK;
	unsigned int m_uFontIdRdBtn;
	unsigned int m_uImageIdRdDisable;
	unsigned int m_uImageIdNoRdDisable;
	// static
	ybctrl::YBStatic m_testStatic;
	unsigned int m_uImageIdStatic;
	unsigned int m_uFontIdStatic;
	// YBITDlgFileUpLoadRaw
	YBITDlgFileUpLoadRaw m_testYBITDlg;

	YBUpLoadStep1Dlg m_testYBULStep1Dlg;
	YBUpLoadStep2Dlg m_testYBULStep2Dlg;
	YBUpLoadStep3Dlg m_testYBULStep3Dlg;
	YBUpLoadStep4Dlg m_testYBULStep4Dlg;
	YBUpLoadStep5Dlg m_testYBULStep5Dlg;
	//
	ybctrl::YBButton m_testBtn;
	ybctrl::YBButton m_testBtnDisable;
};

#endif //__YBPAGE1_H__