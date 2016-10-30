#ifndef __YBPAGE3_H__
#define __YBPAGE3_H__
#include "frame\YBCtrlBase.h"
#include "button\YBButton.h"
#include "YBTopMostBar.h"
#include "frame\YBCtrlImageBase.h"
#include "frame\YBCtrlImageStatic.h"
#include "YBTransitCenterDlg.h"
#include "scrollwnd\YBScrollWnd.h"
#include "YBCornerNotifyCenter.h"
#include "menu/YBCtrlMenu.h"
#include "menu/YBCtrlMenuItems.h"
#include "YBWSAWMenuBase.h"
#include "YBUploadFileMenu.h"
#include "YBFSMenuHeader.h"
#include "YBUpLoadMenu.h"
#include "YBWebSiteMenu.h"
#include "YBAppWndMenu.h"
#include "YBFSRcMenu.h"
#include "YBElawDropDownMenu.h"
#include "treectrl/YBTreeCtrl.h"

namespace ybctrl {
	class YBCtrlImageBase;
	class YBCtrlImageStatic;
};
namespace Gdiplus{
	class Image;
}
//
class YBPage3 : public YBTopMostBar{
public:
	YBPage3();
	virtual ~YBPage3();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE();

public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent );

private:
	void loadImage();
	void _init();
	void _init2();
	void _init3();
	void _init4();
	void _init4FileMenu();
	void _loadFont();
	void _initMenuItem();
	void _initTreeCtrl();

private:
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_RBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_KEYDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_NEXTDLGCTL( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

public:
	ybctrl::YBButton m_btnOpenModalDlg;
	ybctrl::YBButton m_btnOpenChildModalDlg;
	ybctrl::YBButton m_btnAddCornerNotifyDlg;
	ybctrl::YBButton m_btnMenuDlg;
	YBTransitCenterDlg m_transitcenterDlg;
	ybctrl::YBScrollWnd m_listbox4UD;
	ybctrl::YBScrollWnd m_listview4Test;
	ybctrl::YBCtrlMenu m_testMenu;
	ybctrl::YBScrollWnd m_listbox4YBWSAWMenu;
	ybctrl::YBScrollWnd m_listbox4YBWSAWMenu2;
	ybctrl::YBScrollWnd m_listbox4YBULFileMenu;
	ybctrl::YBTreeCtrl m_testTreeCtrl;
	YBAppWndMenu m_testAppWndMenu;

private:
	unsigned int m_uImageIdScrBarBkH;
	unsigned int m_uImageIdScrBarBkV;
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
	YBCornerNotifyCenter m_cornerNotifyCenter;
	unsigned int m_uFontIdMenuTxt;
	unsigned int m_uFontIdMenuDisableTxt;
	unsigned int m_uImageIdMenuIcon;
	unsigned int m_uImageIdMenuItemBk;
	unsigned int m_uImageIdMenuHover;
	unsigned int m_uImageIdMenuItemDisable;
	//
	unsigned int m_uImageIdWSAWMenuNor;
	unsigned int m_uImageIdWSAWMenuHov;
	unsigned int m_uImageIdFSMenuHeaderBk;
	//
	YBFSRcMenu m_test4FSMenu;
	YBElawDropDownMenu menu4ElawDDMenu;
};

#endif //__YBPAGE3_H__