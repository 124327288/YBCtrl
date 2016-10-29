#ifndef __YBFSRCMENU_H__
#define __YBFSRCMENU_H__
#include "frame/YBCtrlBase.h"
#include "menu/YBCtrlMenu.h"
#include "frame/YBCtrlImageStatic.h"

#define WM_YBFSRCMENU_CLICK_EVT (0x00003235)
#define WM_YBFSRCMENU_CLICK_DIVIDEPARA1_EVT (0x00002359)
#define WM_YBFSRCMENU_CLICK_DIVIDEPARA2_EVT (0x00002360)

#define YBFSRCMENUID_MENU4FS (0x00000300)
#define YBFSRCMENU_HEADER_HEIGHT (57)
#define YBFSRCMENU_HEADER_TSPACE (17)

#define TOPBAR_TSPACE (26)
#define BOTTOMBAR_BSPACE (26)
#define LEFTBAR_LSPACE (0)
#define RIGHTBAR_RSPACE (0)
// click Evt
#define YBFSRCMENU_CMDEVT_ITEMDOWN1 (0x00000300)
#define YBFSRCMENU_CMDEVT_ITEMDOWN2 (0x00000301)
#define YBFSRCMENU_CMDEVT_ITEMDOWN3 (0x00000302)
#define YBFSRCMENU_CMDEVT_ITEMDOWN4 (0x00000303)
#define YBFSRCMENU_CMDEVT_ITEMDOWN5 (0x00000304)
#define YBFSRCMENU_CMDEVT_ITEMDOWN6 (0x00000305)
#define YBFSRCMENU_CMDEVT_ITEMDOWN7 (0x00000306)
#define YBFSRCMENU_CMDEVT_ITEMDOWN8 (0x00000307)
#define YBFSRCMENU_CMDEVT_ITEMDOWN9 (0x00000308)
#define YBFSRCMENU_CMDEVT_ITEMDOWN10 (0x00000309)
#define YBFSRCMENU_CMDEVT_ITEMDOWN11 (0x000003010)

class YBFSRcMenu : public ybctrl::YBCtrlBase {
public:
	typedef std::basic_string< TCHAR > tstring_type;

public:
	YBFSRcMenu( unsigned int uMenuWidth );
	virtual ~YBFSRcMenu();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	bool trackPopupMenu( HWND hWndParent );
	void setItemStatus( unsigned int uItemId, tstring_type tstrTxt, bool bDisable );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_NCACTIVATE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	void _clipWnd();
	void _movewnd();
	void _drawStatus( HDC hDC );
	void _drawBk( HDC hDC );
	void _drawHeaderBk( HDC hDC );
	void _drawLine( HDC hDC );
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageId ) const;
	void _init();
	void _loadImage();
	void _loadFont();
	void _destorySelf();
	bool _getHeaderRect( RECT& rcHeader ) const;

private:
	ybctrl::YBCtrlMenu m_menu4FS;
	unsigned int m_uImageIdBk;
	unsigned int m_uImageIdHeaderBk;
	unsigned int m_uWidth;
	unsigned int m_uHeight;
	unsigned int m_uFontIdFSRCMTxtNor;
	unsigned int m_uFontIdFSRCMTxtHov;
	unsigned int m_uFontIdFSRCMTxtDis;
	unsigned int m_uFontIdFSRCMIconTxt;
	unsigned int m_uFontIdFSRCMNumTxt;
	unsigned int m_uImageIdMenuIconNor;
	unsigned int m_uImageIdMenuIconHor;
	unsigned int m_uImageIdMenuIconDis;
	unsigned int m_uImageIdMenuItemBk;
	unsigned int m_uImageIdMenuHover;
	unsigned int m_uImageIdMenuItemDisable;
	HPEN m_hPenLine;
	//
	unsigned int m_uImageIdFSRCMHov_Bk;
	unsigned int m_uImageIdFSRCMBk_Hov;
	unsigned int m_uImageIdFSRCMBk_Dis;
	unsigned int m_uImageIdFSRCMDis_Bk;
	unsigned int m_uImageIdFSRCMDis_Dis;
	unsigned int m_uImageIdFSRCMDis_Hov;
	unsigned int m_uImageIdFSRCMHov_Dis;
	unsigned int m_uImageIdFSRCMBk_Bk;
	//
	unsigned int m_uImageIdFSRCMHov_Bk_Bk;
	unsigned int m_uImageIdFSRCMBk_Bk_Hov;
	unsigned int m_uImageIdFSRCMBk_Hov_Bk;
	unsigned int m_uImageIdFSRCMBk_Bk_Bk;
	unsigned int m_uImageIdFSRCMDis_Bk_Hov;
	unsigned int m_uImageIdFSRCMDis_Bk_Bk;
	unsigned int m_uImageIdFSRCMDis_Hov_Bk;
	unsigned int m_uImageIdFSRCMBk_Dis_Bk;
	unsigned int m_uImageIdFSRCMBk_Dis_Hov;
	unsigned int m_uImageIdFSRCMHov_Dis_Bk;
	unsigned int m_uImageIdFSRCMBk_Bk_Dis;
	unsigned int m_uImageIdFSRCMBk_Hov_Dis;
	unsigned int m_uImageIdFSRCMHov_Bk_Dis;
	unsigned int m_uImageIdFSRCMDis_Dis_Hov;
	unsigned int m_uImageIdFSRCMDis_Dis_Bk;
	unsigned int m_uImageIdFSRCMDis_Hov_Dis;
	unsigned int m_uImageIdFSRCMDis_Bk_Dis;
	unsigned int m_uImageIdFSRCMBk_Dis_Dis;
	unsigned int m_uImageIdFSRCMHov_Dis_Dis;
	unsigned int m_uImageIdFSRCMDis_Dis_Dis;
	//
	HWND m_hWndMsgRecv;

public:
	unsigned int m_uItemId1;
	unsigned int m_uItemId2;
	unsigned int m_uItemId3;
	unsigned int m_uItemId4;
	unsigned int m_uItemId5;
	unsigned int m_uItemId6;
	unsigned int m_uItemId7;
	unsigned int m_uItemId8;
	unsigned int m_uItemId9;
	unsigned int m_uItemId10;
	unsigned int m_uItemId11;
};

#endif //__YBFSRCMENU_H__