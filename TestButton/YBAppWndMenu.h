#ifndef __YBAPPWNDMENU_H__
#define __YBAPPWNDMENU_H__
#include "frame/YBCtrlBase.h"
#include "scrollwnd/YBScrollWnd.h"
#include "YBWSAWMenuBase.h"
#include "YBFSMenuHeader.h"

class YBAppWndMenu : public ybctrl::YBCtrlBase {
public:
	YBAppWndMenu();
	virtual ~YBAppWndMenu();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	bool trackPopupMenu( int x, int y, HWND hWndParent );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_NCACTIVATE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _drawBk( HDC hDC );
	void _init();
	void _loadFont();
	void _loadImage();
	void _destorySelf();
	void _insert();

private:
	HBRUSH m_hBrush;
	YBFSMenuHeader m_fsMenuHeader;
	ybctrl::YBScrollWnd m_listbox4WebSite;
	//
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
	// header
	unsigned int m_uFontIdHeader;
	unsigned int m_uImageIdHeader;
	// listbox
	unsigned int m_uImageIdWebSiteMenuNor;
	unsigned int m_uImageIdWebSiteMenuHor;
};

#endif //__YBAPPWNDMENU_H__