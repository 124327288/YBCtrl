#include "stdafx.h"
#include "YBPage3.h"
#include "button\YBButton.h"
#include "frame\YBCtrlImageDepository.h"
#include "frame\YBCtrlDefs.h"
#include "frame\YBCtrlImageStatic.h"
#include "frame\YBCtrlImageBase.h"
#include "frame\YBCtrlMisc.h"
#include "frame\YBCtrlMemDC.h"
#include "loader\YBImageLoader.h"
#include "YBListBox4UDFolder.h"
#include "YBListBox4DDFolder.h"
#include "scrollbar/YBScrollBar.h"
#include "YBTestChildModalDlg.h"
#include "frame/YBCtrlFontDepository.h"
#include "gdiplus.h"
#include "MemFuncPack.h"
#include <assert.h>
#include "menu/YBCtrlMenuItems.h"
#include "treectrl/YBNodeRoot.h"
#include "treectrl/YBNodeLeaf.h"
#include "treectrl/YBTCViewContent.h"

#define YBCTRLID_BTN_OPENDOMODALDLG (280)
#define YBCTRLID_LISTBOX4UD (281)
#define YBCTRLID_LISTBOX4DD (282)
#define YBCTRLID_BTN_OPENCHILDDOMODALDLG (283)
#define YBCTRLID_BTN_ADDCORNERNOTIFYDLG (284)
#define YBCTRLID_BTN_MENUDLG (285)
#define YBCTRLID_MENU (286)
#define YBCTRLID_FSMENUHEADER (0x00000301)
USING_NAMESPACE_YBCTRL

YBPage3::YBPage3()
: m_listbox4UD( new YBView4UDFolderInner(), 25, 26 )
, m_listbox4DD( new YBView4DDFolderInner(), 25, 26 )
, m_listbox4YBWSAWMenu( new YBView4WSAWMenuInner( 300, 26 ), 25, 26 )
, m_listbox4YBWSAWMenu2( new YBView4WSAWMenuInner( 300, 26 ), 25, 26 )
, m_listbox4YBULFileMenu( new YBView4ULFileMenuInner( 300, 26 ), 25, 26 )
, m_testMenu( 250 )
, m_test4FSMenu( 270 )
{
	loadImage();
	_init();
	_init2();
	_init3();
	_init4();
	_init4FileMenu();
	_loadFont();
	_initMenuItem();
	_initTreeCtrl();
}

YBPage3::~YBPage3(){

}

YBCTRL_WNDMSGMAP_BEGIN( YBPage3, YBTopMostBar );
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_ONWNDMSG( WM_RBUTTONDOWN, _onWM_RBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_KEYDOWN, _onWM_KEYDOWN )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_NEXTDLGCTL, _onWM_NEXTDLGCTL )
YBCTRL_WNDMSGMAP_END()

void YBPage3::_onWM_NEXTDLGCTL( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_NEXTDLGCTL );
	if ( pYBCtrlWndMsg->m_wParam == (WPARAM)m_btnOpenModalDlg.getHWND() ) {
		::MessageBox( m_hWnd, _T("打开模态对话框"), _T("1"), MB_OK );
	}
	if ( pYBCtrlWndMsg->m_wParam == (WPARAM)m_btnOpenChildModalDlg.getHWND() ) {
		::MessageBox( m_hWnd, _T("打开子模态对话框"), _T("2"), MB_OK );
	}
	if ( pYBCtrlWndMsg->m_wParam == (WPARAM)m_btnAddCornerNotifyDlg.getHWND() ) {
		::MessageBox( m_hWnd, _T("打开角落对话框"), _T("3"), MB_OK );;
	}
	if ( pYBCtrlWndMsg->m_wParam == (WPARAM)m_btnMenuDlg.getHWND() ) {
		::MessageBox( m_hWnd, _T("打开菜单对话框"), _T("4"), MB_OK );
	}
}


void YBPage3::_onWM_KEYDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_KEYDOWN );
	HWND hWndTable = ::GetNextDlgTabItem( m_hWnd, NULL, FALSE );
	if ( hWndTable != NULL ) {
		int a = 0;
	}

	if ( pYBCtrlWndMsg->m_wParam == VK_TAB ) {
		::PostMessage( m_hWnd, WM_NEXTDLGCTL, (WPARAM)hWndTable, 0 );
	}
}

void YBPage3::_onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
// 	::SetFocus( m_hWnd );
// 	menu4ElawDDMenu.m_menu4ElawDDMenu.Create( WS_EX_TOPMOST | WS_EX_TOOLWINDOW, WS_VISIBLE|WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 100, 600, m_hWnd, NULL, NULL );
}

void YBPage3::_initMenuItem() {
	unsigned int uItemHeight = 30;
	//
	YBCtrlMIDefault* pYBMenuItem1 = NULL;
	YBCtrlMenu::item_id_type uItemId = 0;
	unsigned int index = 0;
	pYBMenuItem1 = new YBCtrlMIDefault();
	pYBMenuItem1->m_tstrTxt = _T("王小一");
	pYBMenuItem1->m_uFontIdTxt = m_uFontIdMenuTxt;
	pYBMenuItem1->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem1->m_uImageIdIcon = m_uImageIdMenuIcon;
	pYBMenuItem1->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem1->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem1->m_uItemHeight = uItemHeight;
	pYBMenuItem1->setCanClickFlag( true );
	pYBMenuItem1->setDisableFlag( false );
	m_testMenu.insertItem( index, pYBMenuItem1, uItemId );
	//
	YBCtrlMIDefault* pYBMenuItem2 = NULL;
	YBCtrlMenu::item_id_type uItemId2 = 0;
	unsigned int index1 = 1;
	pYBMenuItem2 = new YBCtrlMIDefault();
	pYBMenuItem2->m_tstrTxt = _T("王小二");
	pYBMenuItem2->m_uFontIdTxt = m_uFontIdMenuTxt;
	pYBMenuItem2->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem2->m_uImageIdIcon = m_uImageIdMenuIcon;
	pYBMenuItem2->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem2->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem2->m_uItemHeight = uItemHeight;
	pYBMenuItem2->setCanClickFlag( true );
	pYBMenuItem2->setDisableFlag( false );
	m_testMenu.insertItem( index1, pYBMenuItem2, uItemId2 );
	//
	YBCtrlMIDefault* pYBMenuItem3 = NULL;
	YBCtrlMenu::item_id_type uItemId3 = 0;
	unsigned int index2 = 2;
	pYBMenuItem3 = new YBCtrlMIDefault();
	pYBMenuItem3->m_tstrTxt = _T("王小三");
	pYBMenuItem3->m_uFontIdTxt = m_uFontIdMenuTxt;
	pYBMenuItem3->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem3->m_uImageIdIcon = m_uImageIdMenuIcon;
	pYBMenuItem3->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem3->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem3->m_uItemHeight = uItemHeight;
	pYBMenuItem3->setCanClickFlag( true );
	pYBMenuItem3->setDisableFlag( false );
	pYBMenuItem3->m_bDrawDivideLine = true;
	m_testMenu.insertItem( index2, pYBMenuItem3, uItemId3 );
	//
	YBCtrlMIDefault* pYBMenuItem4 = NULL;
	YBCtrlMenu::item_id_type uItemId4 = 0;
	unsigned int index3 = 3;
	unsigned int uItemHeight4 = 40;
	pYBMenuItem4 = new YBCtrlMIDefault();
	pYBMenuItem4->m_tstrTxt = _T("王小四");
	pYBMenuItem4->m_uFontIdTxt = m_uFontIdMenuTxt;
	pYBMenuItem4->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem4->m_uImageIdIcon = m_uImageIdMenuIcon;
	pYBMenuItem4->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem4->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem4->m_uItemHeight = uItemHeight4;
	pYBMenuItem4->setCanClickFlag( true );
	pYBMenuItem4->setDisableFlag( true );
	pYBMenuItem4->m_bDrawDivideLine = false;
	m_testMenu.insertItem( index3, pYBMenuItem4, uItemId4 );
	//
	YBCtrlMIDefault* pYBMenuItem5 = NULL;
	YBCtrlMenu::item_id_type uItemId5 = 0;
	unsigned int index4 = 4;
	pYBMenuItem5 = new YBCtrlMIDefault();
	pYBMenuItem5->m_tstrTxt = _T("王小五");
	pYBMenuItem5->m_bDrawDivideLine = true;
	pYBMenuItem5->m_uFontIdTxt = m_uFontIdMenuTxt;
	pYBMenuItem5->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem5->m_uImageIdIcon = m_uImageIdMenuIcon;
	pYBMenuItem5->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem5->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem5->m_uItemHeight = uItemHeight;
	pYBMenuItem5->setCanClickFlag( true );
	pYBMenuItem5->setDisableFlag( true );
	m_testMenu.insertItem( index4, pYBMenuItem5, uItemId5 );
	//
	YBCtrlMIDefault* pYBMenuItem6 = NULL;
	YBCtrlMenu::item_id_type uItemId6 = 0;
	unsigned int index5 = 5;
	unsigned int uItemHeight6 = 30;
	pYBMenuItem6 = new YBCtrlMIDefault();
	pYBMenuItem6->m_tstrTxt = _T("王小六");
	pYBMenuItem6->m_uFontIdTxt = m_uFontIdMenuTxt;
	pYBMenuItem6->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem6->m_uImageIdIcon = m_uImageIdMenuIcon;
	pYBMenuItem6->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem6->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem6->m_uItemHeight = uItemHeight6;
	pYBMenuItem6->setCanClickFlag( true );
	pYBMenuItem6->setDisableFlag( false );
	m_testMenu.insertItem( index5, pYBMenuItem6, uItemId6 );
	//
	YBCtrlMIDefault* pYBMenuItem7 = NULL;
	YBCtrlMenu::item_id_type uItemId7 = 0;
	unsigned int index6 = 6;
	unsigned int uItemHeight7 = 30;
	pYBMenuItem7 = new YBCtrlMIDefault();
	pYBMenuItem7->m_tstrTxt = _T("王小七");
	pYBMenuItem7->m_uFontIdTxt = m_uFontIdMenuTxt;
	pYBMenuItem7->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem7->m_uImageIdIcon = m_uImageIdMenuIcon;
	pYBMenuItem7->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem7->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem7->m_uItemHeight = uItemHeight7;
	pYBMenuItem7->setCanClickFlag( true );
	pYBMenuItem7->setDisableFlag( false );
	pYBMenuItem7->m_bDrawDivideLine = true;
	m_testMenu.insertItem( index6, pYBMenuItem7, uItemId7 );
	//
	YBCtrlMIDefault* pYBMenuItem8 = NULL;
	YBCtrlMenu::item_id_type uItemId8 = 0;
	unsigned int index7 = 7;
	unsigned int uItemHeight8 = 30;
	pYBMenuItem8 = new YBCtrlMIDefault();
	pYBMenuItem8->m_tstrTxt = _T("王小八");
	pYBMenuItem8->m_uFontIdTxt = m_uFontIdMenuTxt;
	pYBMenuItem8->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem8->m_uImageIdIcon = m_uImageIdMenuIcon;
	pYBMenuItem8->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem8->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem8->m_uItemHeight = uItemHeight8;
	pYBMenuItem8->setCanClickFlag( true );
	pYBMenuItem8->setDisableFlag( false );
	m_testMenu.insertItem( index7, pYBMenuItem8, uItemId8 );
}

void YBPage3::_init() {
	ybctrl::YBViewInner* pViewInner = m_listbox4UD.getViewInner();
	ybctrl::YBViewContentBase* pViewContent = pViewInner->getViewContentObj();
	YBView4UDFolderContent* pView4UDFolderContent = ( YBView4UDFolderContent* )pViewContent;

	ybctrl::YBScrollBar* pYBScrollBarH = m_listbox4UD.getScrollBarH();
	ybctrl::YBScrollBar* pYBScrollBarV = m_listbox4UD.getScrollBarV();

	pYBScrollBarV->setImageIdBk( m_uImageIdScrBarBkV );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrTArrPreNor );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrTArrNor );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrTArrHov );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrTArrPre );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrBArrPreNor );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrBArrNor );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrBArrHov );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrBArrPre );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_NORMAL, m_uImageIdScrFTNor );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_HOVER, m_uImageIdScrFTHov );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_PRESS, m_uImageIdScrFTPre );
	//
	pYBScrollBarH->setImageIdBk( m_uImageIdScrBarBkH );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrTArrPreNor );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrTArrNor );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrTArrHov );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrTArrPre );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrBArrPreNor );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrBArrNor );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrBArrHov );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrBArrPre );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_NORMAL, m_uImageIdScrFTNor );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_HOVER, m_uImageIdScrFTHov );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_PRESS, m_uImageIdScrFTPre );
}

void YBPage3::_init2(){
	ybctrl::YBViewInner* pViewInner = m_listbox4DD.getViewInner();
	ybctrl::YBViewContentBase* pViewContent = pViewInner->getViewContentObj();
	YBView4DDFolderContent* pView4DDFolderContent = ( YBView4DDFolderContent* )pViewContent;

	ybctrl::YBScrollBar* pYBScrollBarH = m_listbox4DD.getScrollBarH();
	ybctrl::YBScrollBar* pYBScrollBarV = m_listbox4DD.getScrollBarV();

	pYBScrollBarV->setImageIdBk( m_uImageIdScrBarBkV );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrTArrPreNor );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrTArrNor );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrTArrHov );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrTArrPre );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrBArrPreNor );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrBArrNor );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrBArrHov );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrBArrPre );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_NORMAL, m_uImageIdScrFTNor );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_HOVER, m_uImageIdScrFTHov );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_PRESS, m_uImageIdScrFTPre );
	//
	pYBScrollBarH->setImageIdBk( m_uImageIdScrBarBkH );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrTArrPreNor );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrTArrNor );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrTArrHov );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrTArrPre );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrBArrPreNor );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrBArrNor );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrBArrHov );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrBArrPre );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_NORMAL, m_uImageIdScrFTNor );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_HOVER, m_uImageIdScrFTHov );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_PRESS, m_uImageIdScrFTPre );

}

void YBPage3::_init3(){
	ybctrl::YBScrollBar* pYBScrollBarH = m_listbox4YBWSAWMenu.getScrollBarH();
	ybctrl::YBScrollBar* pYBScrollBarV = m_listbox4YBWSAWMenu.getScrollBarV();

	pYBScrollBarV->setImageIdBk( m_uImageIdScrBarBkV );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrTArrPreNor );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrTArrNor );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrTArrHov );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrTArrPre );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrBArrPreNor );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrBArrNor );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrBArrHov );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrBArrPre );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_NORMAL, m_uImageIdScrFTNor );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_HOVER, m_uImageIdScrFTHov );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_PRESS, m_uImageIdScrFTPre );
	//
	pYBScrollBarH->setImageIdBk( m_uImageIdScrBarBkH );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrTArrPreNor );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrTArrNor );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrTArrHov );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrTArrPre );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrBArrPreNor );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrBArrNor );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrBArrHov );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrBArrPre );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_NORMAL, m_uImageIdScrFTNor );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_HOVER, m_uImageIdScrFTHov );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_PRESS, m_uImageIdScrFTPre );

}

void YBPage3::_init4(){
	ybctrl::YBScrollBar* pYBScrollBarH = m_listbox4YBWSAWMenu2.getScrollBarH();
	ybctrl::YBScrollBar* pYBScrollBarV = m_listbox4YBWSAWMenu2.getScrollBarV();

	pYBScrollBarV->setImageIdBk( m_uImageIdScrBarBkV );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrTArrPreNor );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrTArrNor );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrTArrHov );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrTArrPre );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrBArrPreNor );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrBArrNor );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrBArrHov );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrBArrPre );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_NORMAL, m_uImageIdScrFTNor );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_HOVER, m_uImageIdScrFTHov );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_PRESS, m_uImageIdScrFTPre );
	//
	pYBScrollBarH->setImageIdBk( m_uImageIdScrBarBkH );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrTArrPreNor );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrTArrNor );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrTArrHov );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrTArrPre );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrBArrPreNor );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrBArrNor );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrBArrHov );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrBArrPre );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_NORMAL, m_uImageIdScrFTNor );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_HOVER, m_uImageIdScrFTHov );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_PRESS, m_uImageIdScrFTPre );

}

void YBPage3::_init4FileMenu() {
	ybctrl::YBScrollBar* pYBScrollBarH = m_listbox4YBULFileMenu.getScrollBarH();
	ybctrl::YBScrollBar* pYBScrollBarV = m_listbox4YBULFileMenu.getScrollBarV();

	pYBScrollBarV->setImageIdBk( m_uImageIdScrBarBkV );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrTArrPreNor );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrTArrNor );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrTArrHov );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrTArrPre );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrBArrPreNor );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrBArrNor );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrBArrHov );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrBArrPre );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_NORMAL, m_uImageIdScrFTNor );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_HOVER, m_uImageIdScrFTHov );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_PRESS, m_uImageIdScrFTPre );
	//
	pYBScrollBarH->setImageIdBk( m_uImageIdScrBarBkH );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrTArrPreNor );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrTArrNor );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrTArrHov );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrTArrPre );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrBArrPreNor );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrBArrNor );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrBArrHov );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrBArrPre );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_NORMAL, m_uImageIdScrFTNor );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_HOVER, m_uImageIdScrFTHov );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_PRESS, m_uImageIdScrFTPre );
}

void YBPage3::_initTreeCtrl() {
	ybctrl::YBScrollBar* pYBScrollBarH = m_testTreeCtrl.getScrollWnd()->getScrollBarH();
	ybctrl::YBScrollBar* pYBScrollBarV = m_testTreeCtrl.getScrollWnd()->getScrollBarV();

	pYBScrollBarV->setImageIdBk( m_uImageIdScrBarBkV );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrTArrPreNor );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrTArrNor );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrTArrHov );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrTArrPre );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrBArrPreNor );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrBArrNor );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrBArrHov );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrBArrPre );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_NORMAL, m_uImageIdScrFTNor );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_HOVER, m_uImageIdScrFTHov );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_PRESS, m_uImageIdScrFTPre );
	//
	pYBScrollBarH->setImageIdBk( m_uImageIdScrBarBkH );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrTArrPreNor );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrTArrNor );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrTArrHov );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrTArrPre );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrBArrPreNor );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrBArrNor );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrBArrHov );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrBArrPre );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_NORMAL, m_uImageIdScrFTNor );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_HOVER, m_uImageIdScrFTHov );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_PRESS, m_uImageIdScrFTPre );
}

void YBPage3::loadImage() {
	TCHAR szModulePath[MAX_PATH];
	TCHAR* posTmp = NULL;
	GetModuleFileName( NULL, szModulePath, MAX_PATH );
	std::basic_string< TCHAR > tstrImageDirPath;
	std::basic_string< TCHAR > tstrImageFilePath;
	unsigned int idImage;

	posTmp = _tcsrchr( szModulePath, _T('\\') );
	if( !posTmp ){
		assert(false);
		return;
	}
	posTmp += 1;
	*posTmp = 0;

	tstrImageDirPath = szModulePath;
	tstrImageDirPath += _T("Image\\");

	tstrImageFilePath = tstrImageDirPath + _T("YBPage3Bk.png");
	setBkImage( tstrImageFilePath );
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBScrollBarBkH.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBarBkH = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBScrollBarBkV.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBarBkV = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWTArrowNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrTArrNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWTArrowHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrTArrHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWTArrowPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrTArrPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWBArrowNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBArrNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWBArrowHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBArrHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWBArrowPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBArrPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWTArrowPreNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrTArrPreNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWBArrowPreNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBArrPreNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWScrollBarNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrFTNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWScrollBarHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrFTHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWScrollBarPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrFTPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBListBoxIcon.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdMenuIcon = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBMenuItemBk.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdMenuItemBk = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBMenuItemHover.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdMenuHover = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBMenuItemDisable.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdMenuItemDisable = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWSAWMenuNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdWSAWMenuNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWSAWMenuHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdWSAWMenuHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBFSMenuHeaderBk.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdFSMenuHeaderBk = idImage;
}

HWND YBPage3::create( int x, int y, int nWidth, int nHeight, HWND hWndParent ){
	HWND hWnd;

	hWnd = YBTopMostBar::create( x, y, nWidth, nHeight, hWndParent, NULL );

	// 
	if ( hWnd ) {
		// m_btnOpenModalDlg
		m_btnOpenModalDlg.Create( 0, WS_CHILD|WS_VISIBLE|WS_TABSTOP, 0, 0, 80, 30, hWnd, (HMENU)YBCTRLID_BTN_OPENDOMODALDLG, NULL );
		::SetWindowText( m_btnOpenModalDlg.getHWND(), _T("模态对话框") );
		// m_btnOpenChildModalDlg
		m_btnOpenChildModalDlg.Create( 0, WS_CHILD|WS_VISIBLE|WS_TABSTOP, 100, 0, 80, 30, hWnd, (HMENU)YBCTRLID_BTN_OPENCHILDDOMODALDLG, NULL );
		::SetWindowText( m_btnOpenChildModalDlg.getHWND(), _T("子对话框") );
		// m_btnAddCornerNotifyDlg
		m_btnAddCornerNotifyDlg.Create( 0, WS_CHILD|WS_VISIBLE|WS_TABSTOP, 200, 0, 80, 30, hWnd, (HMENU)YBCTRLID_BTN_ADDCORNERNOTIFYDLG, NULL );
		::SetWindowText( m_btnAddCornerNotifyDlg.getHWND(), _T("角落提示框") );
		// m_btnMenuDlg
		m_btnMenuDlg.Create( 0, WS_CHILD|WS_VISIBLE|WS_TABSTOP, 300, 0, 80, 30, hWnd, (HMENU)YBCTRLID_BTN_MENUDLG, NULL );
		::SetWindowText( m_btnMenuDlg.getHWND(), _T("菜单") );
		// m_transitcenterDlg
		m_transitcenterDlg.create( 900, 350, 500, 500, hWnd );
		// m_listbox4UD
		m_listbox4UD.create( 5, 30, 500, 400, hWnd, YBCTRLID_LISTBOX4UD, NULL );
		//
		YBView4UDFolderContent* pViewContent4UDFolder = NULL;
		YBListBoxFolderItem* pListBoxItem = NULL;
		TCHAR szMsgBox[100];
		pViewContent4UDFolder = (YBView4UDFolderContent*)m_listbox4UD.getViewInner()->getViewContentObj();
		for ( int i = 0; i<16; ++i ) {
			pListBoxItem = new YBListBoxFolderItem();
			_stprintf_s( szMsgBox, _T("ROM.WANG %d"), i );
			pListBoxItem->m_tstrItemText = szMsgBox;
			pListBoxItem->m_wBtnCtrlId = 20;
			pViewContent4UDFolder->insertItem( i, pListBoxItem );
		}
		bool bNeedScrollBarV;
		bool bNeedScrollBarH;
		m_listbox4UD.checkIsNeedScrollBar( bNeedScrollBarH, bNeedScrollBarV );
		if ( bNeedScrollBarV ) {
			pViewContent4UDFolder->setWidth( 500 - 25 );
		}
		m_listbox4UD.updateStatus();
		if ( m_listbox4UD.getHWND() ) {
			::ShowWindow( m_listbox4UD.getHWND(), SW_NORMAL );
		}
		m_listbox4DD.create( 550, 20, 200, 400, hWnd, YBCTRLID_LISTBOX4DD, NULL );
		if( m_listbox4DD.getHWND() ){
			::ShowWindow( m_listbox4DD.getHWND(), SW_NORMAL );
		}
		//////////////////////////////////////////////////////////////////////////
		m_listbox4YBWSAWMenu.create( 760, 38, 300, 300, hWnd, 300, NULL );
		if ( m_listbox4YBWSAWMenu.getHWND() ) {
			YBView4WSAWMenuContent* pContentObj = (YBView4WSAWMenuContent*)m_listbox4YBWSAWMenu.getViewInner()->getViewContentObj();
			if ( pContentObj ) {
				pContentObj->setWidth( 300 );
			}
		}
		YBView4WSAWMenuContent* pViewContent4WSAWMenu = NULL;
		YBListBoxWSAWMenuItem* pListBoxMenuItem = NULL;
		TCHAR szMsgBoxMenu[100];
		unsigned int uItemId;
		pViewContent4WSAWMenu = (YBView4WSAWMenuContent*)m_listbox4YBWSAWMenu.getViewInner()->getViewContentObj();
		for ( int i = 0; i<30; ++i ) {
			pListBoxMenuItem = new YBListBoxWSAWMenuItem();
			_stprintf_s( szMsgBoxMenu, _T("%d"), i + 1 );
			pListBoxMenuItem->m_tstrItemContent = _T("Adobe Photoshop CS4");
			pListBoxMenuItem->m_uImageIdNor = m_uImageIdWSAWMenuNor;
			pListBoxMenuItem->m_uImageIdHov = m_uImageIdWSAWMenuHov;
			pViewContent4WSAWMenu->push_front( pListBoxMenuItem, uItemId );
		}
		m_listbox4YBWSAWMenu.updateStatus();
		::ShowWindow( m_listbox4YBWSAWMenu.getHWND(), SW_NORMAL );
		//////////////////////////////////////////////////////////////////////////
		m_listbox4YBWSAWMenu2.create( 1100, 20, 250, 320, hWnd, 310, NULL );
		if ( m_listbox4YBWSAWMenu2.getHWND() ) {
			YBView4WSAWMenuContent* pContentObj2 = (YBView4WSAWMenuContent*)m_listbox4YBWSAWMenu2.getViewInner()->getViewContentObj();
			if ( pContentObj2 ) {
				pContentObj2->setWidth( 250 );
			}
		}
		YBView4WSAWMenuContent* pViewContent4WSAWMenu2 = NULL;
		YBListBoxWSAWMenuItem* pListBoxMenuItem2 = NULL;
		TCHAR szMsgBoxMenu2[100];
		unsigned int uItemid2;
		pViewContent4WSAWMenu2 = (YBView4WSAWMenuContent*)m_listbox4YBWSAWMenu2.getViewInner()->getViewContentObj();
		for ( int i = 0; i<100; ++i ) {
			pListBoxMenuItem2 = new YBListBoxWSAWMenuItem();
			_stprintf_s( szMsgBoxMenu2, _T("ROM.WANG %d"), i );
			pListBoxMenuItem2->m_tstrItemContent = szMsgBoxMenu2;
			pListBoxMenuItem2->m_uImageIdNor = m_uImageIdWSAWMenuNor;
			pListBoxMenuItem2->m_uImageIdHov = m_uImageIdWSAWMenuHov;
			pViewContent4WSAWMenu2->insertItem( i, pListBoxMenuItem2, uItemid2 );
		}
		m_listbox4YBWSAWMenu2.updateStatus();
		::ShowWindow( m_listbox4YBWSAWMenu2.getHWND(), SW_NORMAL );
		//////////////////////////////////////////////////////////////////////////
		m_testTreeCtrl.create( 30, 450, 150, 300, hWnd, 320, NULL );
		YBNodeFolder* pNodeParent = NULL;
		YBNodeFolder* pNodeNew = NULL;
		for ( unsigned int uIndex = 0; uIndex < 8; ++uIndex ) {
			pNodeNew = new YBNodeFolder();
			pNodeNew->m_tstrText = _T( "1234" );
			pNodeNew->m_uFontIdTextNor = m_uFontIdMenuTxt;
			m_testTreeCtrl.insertNode( pNodeParent, 0, ( YBNodeBase* )pNodeNew, false );
		}
		YBNodeFolder* pNodeFolder = NULL;
		for ( unsigned int uIndex = 0; uIndex < 5; ++uIndex ) {
			pNodeFolder = new YBNodeFolder();
			pNodeFolder->m_tstrText = _T( "ABC" );
			pNodeFolder->m_uFontIdTextNor = m_uFontIdMenuTxt;
			m_testTreeCtrl.insertNode( pNodeNew, 0, ( YBNodeBase* )pNodeFolder, false );
		}
		YBNodeLeaf* pNodeFolder1 = NULL;
		for ( unsigned int uIndex = 0; uIndex < 3; ++uIndex ) {
			pNodeFolder1 = new YBNodeLeaf();
			pNodeFolder1->m_tstrText = _T( "5678" );
			pNodeFolder1->m_uFontIdTextNor = m_uFontIdMenuTxt;
			m_testTreeCtrl.insertNode( pNodeFolder, 0, ( YBNodeBase* )pNodeFolder1, false );
		}
		m_testTreeCtrl.calcAllNodesPos();
		m_testTreeCtrl.updateStatus();
		::ShowWindow( m_testTreeCtrl.getScrollWnd()->getHWND(), SW_NORMAL );
	}

	return hWnd;
}

void YBPage3::_onWM_RBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_RBUTTONDOWN );
	POINT ptMouse;

	ptMouse.x = LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = HIWORD( pYBCtrlWndMsg->m_lParam );

	::ClientToScreen( m_hWnd, &ptMouse );

	m_testMenu.Create( WS_EX_TOPMOST|WS_EX_TOOLWINDOW, WS_VISIBLE|WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, ptMouse.x, ptMouse.y, m_hWnd, NULL, NULL );
	//m_testAppWndMenu.trackPopupMenu( ptMouse.x, ptMouse.y, m_hWnd );
	//m_test4FSMenu.trackPopupMenu( m_hWnd );
}

void YBPage3::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );
	//////////////////////////////////////////////////////////////////////////
	unsigned int itemId;
	itemId = pYBCtrlWndMsg->m_lParam;
	switch( itemId ){
	case 23:
		{
			int a = 0;
		}	
		break;
	case 24:
		{
			int a = 0;
		}
		break; 
	default:
		break;
	}
	//////////////////////////////////////////////////////////////////////////
	switch ( wCtrlId ) {
	case YBCTRLID_BTN_ADDCORNERNOTIFYDLG:
		{
			m_testTreeCtrl.clearAll();
			YBCNMsg cnMsg;

			cnMsg.m_tstrTypeName = _T("私人聊天");
			cnMsg.m_nType = 12;
			cnMsg.m_tstrTime = _T("11月1日12:00");
			cnMsg.m_tstrContentText = _T("甘献发来的啊爱上新的聊天信息");
			m_cornerNotifyCenter.pushback( &cnMsg );
		}
		break;
	case YBCTRLID_BTN_OPENCHILDDOMODALDLG:
		{
		//	YBTestChildModalDlg childModalDlg( true, 5000 );
			YBTestChildModalDlg childModalDlg;
			int nRetCode;

			nRetCode = childModalDlg.doModal( getHWND() );

			nRetCode = 0;
		}
		break;
	case YBCTRLID_BTN_MENUDLG:
		{
			//m_testMenu.Create( 0, WS_VISIBLE|WS_CHILD, 80, 520, m_hWnd, YBCTRLID_MENU, NULL );
			//::ShowWindow( m_testMenu.getHWND(), SW_NORMAL );
		}
		break;
	case YBCTRLID_BTN_OPENDOMODALDLG:
		{
			YBView4UDFolderContent* pView4UDFolderContent = NULL;
			YBListBoxFolderItem* pListBoxItem = NULL;
			TCHAR szMsgBox[100];

			pView4UDFolderContent = (YBView4UDFolderContent*)m_listbox4UD.getViewInner()->getViewContentObj();
			if ( pView4UDFolderContent->getItemCount() ) {
				pView4UDFolderContent->clearAllItem();
				m_listbox4UD.reset();
			} else {
				m_listbox4UD.enableRedraw( false );
				CMemFuncPack mfpkEnableRedrawLB4UD( &m_listbox4UD, &YBScrollWnd::enableRedraw, true );

				for ( int i = 0; i<30; ++i ) {
					pListBoxItem = new YBListBoxFolderItem();
					_stprintf_s( szMsgBox, _T("ROM.WANG %d"), i );
					pListBoxItem->m_tstrItemText = szMsgBox;
					pListBoxItem->m_wBtnCtrlId = 20;
					pView4UDFolderContent->insertItem( i, pListBoxItem );
				}
				m_listbox4UD.updateStatus();
			}
		}
		break;
	case 300:
		{
			unsigned int uItemId;
			unsigned int uIndex;
			uIndex = pYBCtrlWndMsg->m_lParam;
			YBView4WSAWMenuContent* pViewContent4WSAWMenu = NULL;
			pViewContent4WSAWMenu = (YBView4WSAWMenuContent*)m_listbox4YBWSAWMenu.getViewInner()->getViewContentObj();
			pViewContent4WSAWMenu->index2ItemId( uIndex, uItemId );
			pViewContent4WSAWMenu->deleteMenuItemById( uItemId );
			m_listbox4YBWSAWMenu.updateStatus();

			if ( pViewContent4WSAWMenu->getItemCount()*26<320 ){
				::MoveWindow( m_listbox4YBWSAWMenu.getHWND(), 780, 20, 300, pViewContent4WSAWMenu->getItemCount()*26, TRUE );
			}

			::InvalidateRect( m_listbox4YBWSAWMenu.getViewInner()->getHWND(), NULL, false );
			::InvalidateRect( m_listbox4YBWSAWMenu.getScrollBarV()->getHWND(), NULL, false );
		}
		break;
	case 310:
		{
			int a = 0;
		}
		break;
	case 320:
		{
			unsigned int uNodeId = pYBCtrlWndMsg->m_lParam;
			int a = 0;
		}
		break;
	default:
		break;
	}
}

void YBPage3::_loadFont(){
	LOGFONT lFont;
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 20;
	m_uFontIdMenuTxt = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 27, 41, 62 ) );
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 20;
	m_uFontIdMenuDisableTxt = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 128, 128, 128 ) );

}