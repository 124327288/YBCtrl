#ifndef __YBPAGE2_H__
#define __YBPAGE2_H__
#include "frame\YBCtrlBase.h"
#include "button\YBButton.h"
#include "YBTopMostBar.h"
#include "frame\YBCtrlImageBase.h"
#include "frame\YBCtrlImageStatic.h"
#include "dialog\YBModalDialog.h"
#include "YBITPopupPB.h"
#include "scrollbar\YBNWScrollBar.h"
#include "YBFileTPDlg.h"
#include <map>
#include "YBFSDownLoader.h"
#include "YBFSRcMenu.h"
#include "YBWebEditDlg.h"
#include "YBInstallDlg.h"
#include "YBUpLoadWarmTipDlg.h"
#include "YBDetectPCEnvironmentDlg.h"
#include "splitter/YBSplitter.h"
#include "PictureCtrlDlg.h"
#include "ImageEditorDlg2.h"

USING_NAMESPACE_YBCTRL
namespace ybctrl {
	class YBCtrlImageBase;
	class YBCtrlImageStatic;
};
namespace Gdiplus{
	class Image;
}
//
class YBPage2 : public YBTopMostBar{
public:
	typedef std::basic_string< TCHAR > tstring_type;

public:
	YBPage2();
	virtual ~YBPage2();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE();

public:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONUP( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSELEAVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_RBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_TIMER( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEWHEEL( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent );

private:
	void loadImage();
	void loadFont();
	void _init();

private:
	unsigned int m_uMenuBkImageId;
	unsigned int m_fontIdMenuTxt;
	unsigned int m_uFontIdBtn;
	unsigned int m_uImageIdScrBarBk;
	unsigned int m_uImageIdScrTArrPreNor;
	unsigned int m_uImageIdScrTArrNor;
	unsigned int m_uImageIdScrTArrHov;
	unsigned int m_uImageIdScrTArrPre;
	unsigned int m_uImageIdScrBArrPreNor;
	unsigned int m_uImageIdScrBArrNor;
	unsigned int m_uImageIdScrBArrHov;
	unsigned int m_uImageIdScrBArrPre;
	unsigned int m_uImageIdScrFTNor;
	unsigned int m_uImageIdScrFTHov;
	unsigned int m_uImageIdScrFTPre;
	//
public:
	ybctrl::YBButton m_btnTestModalDlg;
	ybctrl::YBButton m_btnTestMsgBox;
	ybctrl::YBButton m_btnTestPopupPB;
	ybctrl::YBButton m_btnTestFileTPDlg;
	ybctrl::YBButton m_btnTestFSDownLoader;
	ybctrl::YBButton m_btnTestWebEditDlg;
	ybctrl::YBButton m_btnTestInstallFileDlg;
	ybctrl::YBButton m_btnTestUpLoadWarmTipDlg;
	ybctrl::YBButton m_btnTestYBPopupMB;
	ybctrl::YBButton m_btnTestDPCDlg;
	ybctrl::YBButton m_btnTestImageEditor;
	YBITPopupPB m_popupPB;
	ybctrl::YBNWScrollBar m_nwScrollBar;
	RECT m_rcNWScrBarBk;
	YBFileTPDlg m_testFileTPDlg;
	YBFSDownLoader m_testFSDownLoader;
	//
	YBFSRcMenu m_test4FSMenu;
	YBWebEditDlg m_test4WebEditDlg;
	//
	YBInstallDlg m_test4InstallFileDlg;
	//
	YBUpLoadWarmTipDlg m_test4UpLoadWarmTipDlg;
	//
	YBDetectPCEnvironmentDlg m_test4DetectPCEDlg;
	//
	ybctrl::YBSplitter m_test4Splitter;
	//PictureCtrlDlg m_dlgTest;
	ImageEditorDlg m_dlg4ImageEditor;
};

#endif //__YBDIALOG_H__