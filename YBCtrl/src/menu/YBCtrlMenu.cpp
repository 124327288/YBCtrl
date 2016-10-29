#include "stdafx.h"
#include "frame/YBCtrlBase.h"
#include "menu/YBCtrlMenu.h"
#include "frame/YBCtrlDefs.h"
#include <assert.h>
//////////////////////////////////////////////////////////////////////////
USING_NAMESPACE_YBCTRL

#define COLOR_YBMENU_ITEMBK_DEFAULT (RGB( 255, 255, 255 ))
#define COLOR_YBMENU_ITEMSELECT_DEFAULT (RGB( 223, 199, 199 ))

YBCtrlMenuItemBase::YBCtrlMenuItemBase()
: m_uItemId ( YBCTRL_MENU_ID_INVALID )
{

}

YBCtrlMenuItemBase::~YBCtrlMenuItemBase() {

}

unsigned int YBCtrlMenuItemBase::getItemId() const{
	assert( m_uItemId != YBCTRL_MENU_ID_INVALID );
	return m_uItemId;
}

void YBCtrlMenuItemBase::_setItemId( unsigned int uItemId ){
	assert( m_uItemId == YBCTRL_MENU_ID_INVALID );
	m_uItemId = uItemId;
}

//////////////////////////////////////////////////////////////////////////
#define YBCTRLMENU_HOVERITEM_INDEX_INVALIDATE (-1)
#define YBCTRLMENU_PRESSITEM_INDEX_INVALIDATE (-1)
//
YBCtrlMenu::YBCtrlMenu( unsigned int uWidth )
: m_uWidth( uWidth )
, m_hBrushBk( NULL )
, m_pMemDC( new YBCtrlMemDC( m_uWidth, 100 ) )
, m_uHoverItemIndex( YBCTRLMENU_HOVERITEM_INDEX_INVALIDATE )
, m_uPressItemIndexOld( YBCTRLMENU_PRESSITEM_INDEX_INVALIDATE )
{
	m_hBrushBk = ::CreateSolidBrush( COLOR_YBMENU_ITEMBK_DEFAULT );
}

YBCtrlMenu::~YBCtrlMenu(){
	if ( m_pMemDC ) {
		delete m_pMemDC;
		m_pMemDC = NULL;
	}
	if( m_hBrushBk ){
		::DeleteObject( m_hBrushBk );
		m_hBrushBk = NULL;
	}
	_clearAllItem();
}

YBCTRL_WNDMSGMAP_BEGIN( YBCtrlMenu, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
	YBCTRL_ONWNDMSG( WM_MOUSELEAVE, _onWM_MOUSELEAVE )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_NCACTIVATE, _onWM_NCACTIVATE )
	YBCTRL_ONWNDMSG( WM_DESTROY, _onWM_DESTROY )
	YBCTRL_ONWNDMSG( WM_SETCURSOR, _onWM_SETCURSOR )
YBCTRL_WNDMSGMAP_END()

void YBCtrlMenu::_onWM_SETCURSOR( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_SETCURSOR );

	POINT ptMouse;
	unsigned int uItemIndex;
	YBCtrlMenuItemBase* pMenuItem = NULL;
	::GetCursorPos( &ptMouse );
	::ScreenToClient( m_hWnd, &ptMouse );
	if ( !moveTest( ptMouse, uItemIndex ) ) {
		assert( false );
		return;
	}
	pMenuItem = m_containerItem[ uItemIndex ];
	if ( !pMenuItem ) {
		assert( false );
		return;
	}
	// 
	if ( pMenuItem->isDivide2Part() ) {
		RECT rcDividePart1;
		RECT rcDividePart2;

		if ( !_calcDividePart1Rect( uItemIndex, rcDividePart1 ) || !_calcDividePart2Rect( uItemIndex, rcDividePart2 ) )
			return;
		if ( ::PtInRect( &rcDividePart1, ptMouse ) ) {
			if( pMenuItem->isDividePara1Disable() ) {
				return;
			} else {
				pYBCtrlWndMsg->m_dwFlag = ( YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID );
				pYBCtrlWndMsg->m_lRes = TRUE;
				::SetCursor( ::LoadCursor( NULL, IDC_HAND ) );
			}
		} else if ( ::PtInRect( &rcDividePart2, ptMouse ) ) {
			if( pMenuItem->isDividePara2Disable() ) {
				return;
			} else {
				pYBCtrlWndMsg->m_dwFlag = ( YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID );
				pYBCtrlWndMsg->m_lRes = TRUE;
				::SetCursor( ::LoadCursor( NULL, IDC_HAND ) );
			}
		} else {
			return;
		}

	} else if ( pMenuItem->isDivide3Part() ) {
		//
		RECT rcDividePart1;
		RECT rcDividePart2;
		RECT rcDividePart3;

		if ( !_calcDividePart1Rect( uItemIndex, rcDividePart1 ) || !_calcDividePart2Rect( uItemIndex, rcDividePart2 ) || !_calcDividePart3Rect( uItemIndex, rcDividePart3 ) )
			return;
		if ( ::PtInRect( &rcDividePart1, ptMouse ) ) {
			if( pMenuItem->isDividePara1Disable() ) {
				return;
			} else {
				pYBCtrlWndMsg->m_dwFlag = ( YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID );
				pYBCtrlWndMsg->m_lRes = TRUE;
				::SetCursor( ::LoadCursor( NULL, IDC_HAND ) );
			}
		} else if ( ::PtInRect( &rcDividePart2, ptMouse ) ) {
			if( pMenuItem->isDividePara2Disable() ) {
				return;
			} else {
				pYBCtrlWndMsg->m_dwFlag = ( YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID );
				pYBCtrlWndMsg->m_lRes = TRUE;
				::SetCursor( ::LoadCursor( NULL, IDC_HAND ) );
			}
		} else if ( ::PtInRect( &rcDividePart3, ptMouse ) ) {
			if ( pMenuItem->isDividePara3Disable() ) {
				return;
			} else {
				pYBCtrlWndMsg->m_dwFlag = ( YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID );
				pYBCtrlWndMsg->m_lRes = TRUE;
				::SetCursor( ::LoadCursor( NULL, IDC_HAND ) );				
			}
		} else {
			return;
		}

	} else {
		if ( pMenuItem->isDisable() ) {
			return;
		}
		pYBCtrlWndMsg->m_dwFlag = ( YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID );
		pYBCtrlWndMsg->m_lRes = TRUE;
		::SetCursor( ::LoadCursor( NULL, IDC_HAND ) );
	}
}

void YBCtrlMenu::_onWM_DESTROY( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_DESTROY );

	m_uHoverItemIndex = -1;
}

void YBCtrlMenu::_onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	HDC hPaintDC;
	PAINTSTRUCT ps;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	_drawStatus( m_pMemDC->getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, m_pMemDC->getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void YBCtrlMenu::_onWM_MOUSEMOVE( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	POINT ptMouse;
	unsigned int uItemIndexMove;
	RECT rcClient;
	HDC hDC;
	TRACKMOUSEEVENT trackMouseEvt;

	ptMouse.x = LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = HIWORD( pYBCtrlWndMsg->m_lParam );
	//
	trackMouseEvt.cbSize = sizeof( TRACKMOUSEEVENT );
	trackMouseEvt.dwFlags = TME_LEAVE;
	trackMouseEvt.hwndTrack = pYBCtrlWndMsg->m_hWnd;
	trackMouseEvt.dwHoverTime = HOVER_DEFAULT;
	//
	::TrackMouseEvent ( &trackMouseEvt );
	moveTest( ptMouse, uItemIndexMove );
	// 判断某一行的Item是否分区 
	setDivideHoverItem( ptMouse, uItemIndexMove );
	// 如果不分区...
	//setHoverItem( ptMouse, uItemIndexMove );
	setHoverItem( uItemIndexMove );

	::GetClientRect( m_hWnd, &rcClient );
	_drawStatus( m_pMemDC->getMemDC() );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, m_pMemDC->getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
}

void YBCtrlMenu::_onWM_MOUSELEAVE( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSELEAVE );
	RECT rcClient;
	HDC hDC;

	::GetClientRect( m_hWnd, &rcClient );
	for ( unsigned int Index = 0; Index < m_containerItem.size(); ++Index ){
		m_containerItem[ Index ]->setMouseHoverFlag( false );
		m_containerItem[ Index ]->setMouseHoverInPart1Flag( false );
		m_containerItem[ Index ]->setMouseHoverInPart2Flag( false );
		m_containerItem[ Index ]->setMouseHoverInPart3Flag( false );
	}
	_drawStatus( m_pMemDC->getMemDC() );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, m_pMemDC->getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
}

void YBCtrlMenu::_onWM_NCACTIVATE( YBCtrlWndMsg* pYBCtrlWndMsg ) {
 	YBCTRLMSG_ENTER_CHECK( WM_NCACTIVATE );
	if ( pYBCtrlWndMsg->m_wParam == FALSE ) { // inactive
		::PostMessage( m_hWnd, WM_CLOSE, 0, 0 );
		return;
	}
}

void YBCtrlMenu::_onWM_LBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
	RECT rcClient;
	HDC hDC;
	POINT ptMouse;
	unsigned int uHitItemIndex;
	YBCtrlMenuItemBase* pYBMenuItem = NULL;
	YBCtrlMenuItemBase* pYBMenuItemOld = NULL;
	WPARAM wParam;
	LPARAM lParam;

	::GetClientRect( m_hWnd, &rcClient );
	ptMouse.x = LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = HIWORD( pYBCtrlWndMsg->m_lParam );
	
	moveTest( ptMouse, uHitItemIndex );
	pYBMenuItem = m_containerItem[ uHitItemIndex ];
	if ( !pYBMenuItem ) {
		return;
	}
	// 响应后把Hover标志设回去.
	//pYBMenuItem->setMouseHoverFlag( false );

	_drawStatus( m_pMemDC->getMemDC() );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, m_pMemDC->getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
	
	// 对于全屏里面的某个Item项分成两个Item而加的一个标志 isDivide (一行两个Item)
	if ( pYBMenuItem->isDivide2Part() ) {
		RECT rcDividePart1;
		RECT rcDividePart2;

		if ( !_calcDividePart1Rect( uHitItemIndex, rcDividePart1 ) || !_calcDividePart2Rect( uHitItemIndex, rcDividePart2 ) )
			return;
		if ( ::PtInRect( &rcDividePart1, ptMouse ) ) {
			if ( pYBMenuItem->isDividePara1Disable() )
				return;
			if ( pYBMenuItem->CanClickFlagInPart1() ) {
				wParam = MAKELONG( ::GetDlgCtrlID( m_hWnd ), YBMENU_CMDEVT_DIVIDE2PART1DOWN );
				lParam = (LPARAM)( pYBMenuItem->getItemId() );
				::SendMessage( m_hWndMsgRecv, WM_COMMAND, wParam, lParam );// send message if have been click.
			}
		} else if ( ::PtInRect( &rcDividePart2, ptMouse ) ) {
			if ( pYBMenuItem->isDividePara2Disable() )
				return;
			if ( pYBMenuItem->CanClickFlagInPart2() ) {
				wParam = MAKELONG( ::GetDlgCtrlID( m_hWnd ), YBMENU_CMDEVT_DIVIDE2PART2DOWN );
				lParam = (LPARAM)( pYBMenuItem->getItemId() );
				::SendMessage( m_hWndMsgRecv, WM_COMMAND, wParam, lParam );// send message if have been click.
			}
		}
	} else if ( pYBMenuItem->isDivide3Part() ) {
		RECT rcDividePart1;
		RECT rcDividePart2;
		RECT rcDividePart3;

		if ( !_calcDividePart1Rect( uHitItemIndex, rcDividePart1 ) || !_calcDividePart2Rect( uHitItemIndex, rcDividePart2 ) || !_calcDividePart3Rect( uHitItemIndex, rcDividePart3 ) )
			return;
		if ( ::PtInRect( &rcDividePart1, ptMouse ) ) {
			if ( pYBMenuItem->isDividePara1Disable() )
				return;
			if ( pYBMenuItem->CanClickFlagInPart1() ) {
				wParam = MAKELONG( ::GetDlgCtrlID( m_hWnd ), YBMENU_CMDEVT_DIVIDE3PART1DOWN );
				lParam = (LPARAM)( pYBMenuItem->getItemId() );
				::SendMessage( m_hWndMsgRecv, WM_COMMAND, wParam, lParam );// send message if have been click.
			}
		} else if ( ::PtInRect( &rcDividePart2, ptMouse ) ) {
			if ( pYBMenuItem->isDividePara2Disable() )
				return;
			if ( pYBMenuItem->CanClickFlagInPart2() ) {
				wParam = MAKELONG( ::GetDlgCtrlID( m_hWnd ), YBMENU_CMDEVT_DIVIDE3PART2DOWN );
				lParam = (LPARAM)( pYBMenuItem->getItemId() );
				::SendMessage( m_hWndMsgRecv, WM_COMMAND, wParam, lParam );// send message if have been click.
			}
		} else if ( ::PtInRect( &rcDividePart3, ptMouse ) ) {
			if ( pYBMenuItem->isDividePara3Disable() )
				return;
			if ( pYBMenuItem->CanClickFlagInPart3() ) {
				wParam = MAKELONG( ::GetDlgCtrlID( m_hWnd ), YBMENU_CMDEVT_DIVIDE3PART3DOWN );
				lParam = (LPARAM)( pYBMenuItem->getItemId() );
				::SendMessage( m_hWndMsgRecv, WM_COMMAND, wParam, lParam );// send message if have been click.
			}
		}
	} else {
		// 一行一个Item
		if ( pYBMenuItem->isDisable() ) {  // if disable then return;
			return;
		} else {
			if ( pYBMenuItem->canClick() ){
				if ( pYBMenuItem->isDrawCheckIcon() ) {
					if ( m_uPressItemIndexOld == uHitItemIndex ) {
						pYBMenuItem->setFlagDrawCheckIcon( true );
					} else {
						if ( m_uPressItemIndexOld == YBCTRLMENU_PRESSITEM_INDEX_INVALIDATE ) {
							m_uPressItemIndexOld = uHitItemIndex;
							pYBMenuItem->setFlagDrawCheckIcon( true );
						} else {
							pYBMenuItemOld = m_containerItem[ m_uPressItemIndexOld ];
							if ( pYBMenuItemOld ) {
								pYBMenuItemOld->setFlagDrawCheckIcon( false );
								pYBMenuItem->setFlagDrawCheckIcon( true );
							}
						}
					}
					::InvalidateRect( m_hWnd, NULL, TRUE );
				}
				wParam = MAKELONG( ::GetDlgCtrlID( m_hWnd ), YBMENU_CMDEVT_ITEMDOWN );
				lParam = (LPARAM)( pYBMenuItem->getItemId() );
				::SendMessage( m_hWndMsgRecv, WM_COMMAND, wParam, lParam );// send message if have been click.
			}
		}
	}
}

void YBCtrlMenu::_onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	RECT rcClient;
	//
	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = 1;
	//
	::GetClientRect( m_hWnd, &rcClient );
	_drawStatus( m_pMemDC->getMemDC() );

	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, m_pMemDC->getMemDC(), 0, 0, SRCCOPY );
}

HWND YBCtrlMenu::Create( DWORD dwExStyle, DWORD dwStyle, int x, int y, HWND hWndMsgRecv, unsigned int uCtrlId, HINSTANCE hInstance ){
	HWND hWnd;
	int nHeight = 0;
	//DWORD dwExStyle = WS_EX_TOPMOST|WS_EX_TOOLWINDOW;
	//DWORD dwStyle = (WS_VISIBLE|WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);

	if ( ::IsWindow( m_hWnd ) ) {
		_destorySelf();
		assert( m_hWnd == 0 );
	}

	if( m_containerItem.size() == 0 ){
		assert(false);
		return NULL;
	}
	//
	item_container_type::const_iterator citItem, ciendItem;
	ciendItem = m_containerItem.end();
	YBCtrlMenuItemBase* pYBMenuItem = NULL;
	for ( citItem = m_containerItem.begin(); citItem!=ciendItem; ++citItem ) {
		nHeight += ( (*citItem)->getHeight() );
		pYBMenuItem = (*citItem);
		pYBMenuItem->setMouseHoverFlag( false );
		pYBMenuItem->setMouseHoverInPart1Flag( false );
		pYBMenuItem->setMouseHoverInPart2Flag( false );
		pYBMenuItem->setMouseHoverInPart3Flag( false );
	}
	//
	hWnd = _createHWND( dwExStyle, dwStyle, x, y, m_uWidth, nHeight, hWndMsgRecv, (HMENU)uCtrlId, NULL, NULL );
	if( !hWnd ){
		assert(false);
	}

	m_hWndMsgRecv = hWndMsgRecv;
	return hWnd;
}

unsigned int YBCtrlMenu::getHeight() {
	unsigned int nHeight = 0;

	item_container_type::const_iterator citItem, ciendItem;
	ciendItem = m_containerItem.end();
	for ( citItem = m_containerItem.begin(); citItem!=ciendItem; ++citItem ) {
		nHeight += ( (*citItem)->getHeight() );
	}

	return nHeight;
}

void YBCtrlMenu::_destorySelf() {
	if ( m_hWnd == 0 )
		return;
	if ( ::IsWindow( m_hWnd ) ) {
		::DestroyWindow( m_hWnd );
	} else {
		assert( false );
	}
	m_hWnd = NULL;
}

bool YBCtrlMenu::insertItem( unsigned int uItemIndex, YBCtrlMenuItemBase* pItem, item_id_type& itemId ){
	if( !pItem ){
		assert(false);
		return false;
	}
	RECT rcWnd;
	unsigned int uAllItemHeight = 0;
	YBCtrlMenuItemBase* pYBMenuItem = NULL;

	itemId = _allocalItemId();
	pItem->_setItemId( itemId );
	m_containerItem.insert( m_containerItem.begin() + uItemIndex, pItem );
	//
	if( !::IsWindow( m_hWnd ) ){
		return false;
	}
	//
	if( !::GetWindowRect( m_hWnd, &rcWnd ) ){
		assert(false);
		return false;
	}

	for( unsigned int uIndex = 0; uIndex <= uItemIndex; ++uIndex ){
		pYBMenuItem = m_containerItem[ uIndex ];
		uAllItemHeight += ( pYBMenuItem->getHeight() );
	}
	::MoveWindow( m_hWnd, rcWnd.left, rcWnd.top, rcWnd.right - rcWnd.left, uAllItemHeight, TRUE );

	::ShowWindow( m_hWnd, SW_NORMAL );
	return true;
}

bool YBCtrlMenu::deleteItemById( unsigned int uItemId ) {
	if ( m_containerItem.size() == 0 )
		return false;
	YBCtrlMenuItemBase* pLBItem = NULL;
	item_container_type::iterator itItem, iendItem;

	pLBItem = getItemById( uItemId );
	if ( !pLBItem ) {
		assert( false );
		return false;
	}

	iendItem = m_containerItem.end();
	for ( itItem = m_containerItem.begin(); itItem != iendItem; ++itItem ) {
		if ( pLBItem == ( *itItem ) ) {
			m_containerItem.erase( itItem );
			delete pLBItem;
			pLBItem = NULL;
			return true;
		}
	}
	//
	return false;
}

void YBCtrlMenu::clearAllItem() {
	if ( m_containerItem.size() == 0 )
		return;
	//
	return _clearAllItem();
}

void YBCtrlMenu::_clearAllItem(){
	item_container_type containerItem_type;
	item_container_type::iterator itItem, iendItem;
	YBCtrlMenuItemBase* pLBItem = NULL;

	containerItem_type = m_containerItem;
	m_containerItem.clear();
	//
	iendItem = containerItem_type.end();
	for ( itItem = containerItem_type.begin(); itItem!=iendItem; ++itItem) {
		pLBItem = (*itItem);
		pLBItem->setMouseHoverFlag( false );
		delete pLBItem;
		pLBItem = NULL;
	}
}

YBCtrlMenuItemBase* YBCtrlMenu::getItemByIndex( unsigned int uIndex ) {
	YBCtrlMenuItemBase* pYBItem = NULL;

	pYBItem = m_containerItem[ uIndex ];
	
	return pYBItem;
}

YBCtrlMenuItemBase* YBCtrlMenu::getItemById( item_id_type itItem ) {
	unsigned int uItemIndex;
	YBCtrlMenuItemBase* pYBItem = NULL;

	if ( !itemId2Index(itItem, uItemIndex) )
		return NULL;
	pYBItem = m_containerItem[ uItemIndex ];
	return pYBItem;
}

bool YBCtrlMenu::itemId2Index( item_id_type idItem, unsigned int& uItemIndex ) const {
	if ( m_containerItem.size() == 0 ) {
		return false;
	}
	item_container_type::const_iterator citItem, ciendItem;

	ciendItem = m_containerItem.end();
	for ( citItem = m_containerItem.begin(); citItem!=ciendItem; ++citItem ) {
		if ( (*citItem)->getItemId() == idItem ) {
			uItemIndex = citItem - m_containerItem.begin();
			return true;
		}
	}

	return false;
}

bool YBCtrlMenu::hasItemId( unsigned int uItemId ) const {
	return _hasItemId( uItemId );
}

bool YBCtrlMenu::_hasItemId( unsigned int uItemId ) const {
	item_container_type::const_iterator citItem, ciendItem;

	ciendItem = m_containerItem.end();
	for( citItem = m_containerItem.begin(); citItem!=ciendItem; ++citItem ) {
		if ( (*citItem)->getItemId() == uItemId ) {
			return true;
		}
	}

	return false;
}

YBCtrlMenu::item_id_type YBCtrlMenu::_allocalItemId() const {
	static item_id_type s_idBase = YBCTRL_MENU_ID_BASE;
	return ++s_idBase;
}

void YBCtrlMenu::_drawBk( HDC hDC ){
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	::FillRect( hDC, &rcClient, m_hBrushBk );
}

bool YBCtrlMenu::_calcDividePart1Rect( unsigned int uItemIndex, RECT& rcDividePart1 ) {
	if( ( uItemIndex > m_containerItem.size() ) || ( uItemIndex < 0 ) ){
		assert(false);
		return false;
	}
	//
	YBCtrlMenuItemBase* pYBMenuItem = NULL;
	RECT rcClient;
	unsigned int uItemTop = 0;
	::GetClientRect( m_hWnd, &rcClient );
	for( unsigned int uIndex = 0; uIndex < uItemIndex; ++uIndex ){
		pYBMenuItem = m_containerItem[ uIndex ];
		uItemTop += pYBMenuItem->getHeight();
	}

	pYBMenuItem = m_containerItem[ uItemIndex ];
	if ( !pYBMenuItem ){
		return false;
	}
	//
	rcDividePart1.left = rcClient.left + 34;
	if ( pYBMenuItem->isDivide2Part() ) {
		if ( pYBMenuItem->isDivide2PartCenter() ) {
			rcDividePart1.right = rcDividePart1.left + ( rcClient.right - 34 )/2;
		} else {
			rcDividePart1.right = rcDividePart1.left + 82;
		}
	} else if ( pYBMenuItem->isDivide3Part() ) {
		rcDividePart1.right = rcDividePart1.left + 82;
	} else {
		assert( false );
		return false;
	}
	rcDividePart1.top = uItemTop;
	rcDividePart1.bottom = rcDividePart1.top + pYBMenuItem->getHeight();

	return ( ( rcDividePart1.right > rcDividePart1.left ) && ( rcDividePart1.bottom > rcDividePart1.top ) );
}

bool YBCtrlMenu::_calcDividePart2Rect( unsigned int uItemIndex, RECT& rcDividePart2 ) {
	if( ( uItemIndex > m_containerItem.size() ) || ( uItemIndex < 0 ) ){
		assert(false);
		return false;
	}
	//
	YBCtrlMenuItemBase* pYBMenuItem = NULL;
	RECT rcClient;
	unsigned int uItemTop = 0;
	
	::GetClientRect( m_hWnd, &rcClient );
	for( unsigned int uIndex = 0; uIndex < uItemIndex; ++uIndex ){
		pYBMenuItem = m_containerItem[ uIndex ];
		uItemTop += pYBMenuItem->getHeight();
	}
	
	pYBMenuItem = m_containerItem[ uItemIndex ];
	if ( !pYBMenuItem ) {
		return false;
	}
	if ( pYBMenuItem->isDivide2Part() ) {
		if ( pYBMenuItem->isDivide2PartCenter() ) {
			rcDividePart2.left = 34 + ( rcClient.right - 34 )/2 + rcClient.left;
		} else {
			rcDividePart2.left = 34 + 82 + rcClient.left;
		}
		rcDividePart2.right = rcClient.right;
	} else if ( pYBMenuItem->isDivide3Part() ) {
		rcDividePart2.left = rcClient.left + 34 + 82;
		rcDividePart2.right = rcDividePart2.left + 82;
	} else {
		assert( false );
		return false;
	}
	rcDividePart2.top = uItemTop;
	rcDividePart2.bottom = rcDividePart2.top + pYBMenuItem->getHeight();

	return ( ( rcDividePart2.right > rcDividePart2.left ) && ( rcDividePart2.bottom > rcDividePart2.top ) );
}

bool YBCtrlMenu::_calcDividePart3Rect( unsigned int uItemIndex, RECT& rcDividePart3 ) {
	if( ( uItemIndex > m_containerItem.size() ) || ( uItemIndex < 0 ) ){
		assert(false);
		return false;
	}
	//
	YBCtrlMenuItemBase* pYBMenuItem = NULL;
	RECT rcClient;
	unsigned int uItemTop = 0;
	::GetClientRect( m_hWnd, &rcClient );
	for( unsigned int uIndex = 0; uIndex < uItemIndex; ++uIndex ){
		pYBMenuItem = m_containerItem[ uIndex ];
		uItemTop += pYBMenuItem->getHeight();
	}
	pYBMenuItem = m_containerItem[ uItemIndex ];
	if ( !pYBMenuItem ) {
		return false;
	}
	if ( pYBMenuItem->isDivide3Part() ) {
		rcDividePart3.left = rcClient.left + 34 + 82 +82;
		rcDividePart3.right = rcClient.right;
	} else {
		assert( false );
		return false;
	}
	rcDividePart3.top = uItemTop;
	rcDividePart3.bottom = rcDividePart3.top + pYBMenuItem->getHeight();

	return ( ( rcDividePart3.right > rcDividePart3.left ) && ( rcDividePart3.bottom > rcDividePart3.top ) );
}

bool YBCtrlMenu::_calcItemRect( unsigned int uItemIndex, RECT& rcItem ){
	if( ( uItemIndex > m_containerItem.size() ) || ( uItemIndex < 0 ) ){
		assert(false);
		return false;
	}
	//
	YBCtrlMenuItemBase* pYBMenuItem = NULL;
	RECT rcClient;
	unsigned int uItemTop = 0;
	::GetClientRect( m_hWnd, &rcClient );

	for( unsigned int uIndex = 0; uIndex < uItemIndex; ++uIndex ){
		pYBMenuItem = m_containerItem[ uIndex ];
		uItemTop += pYBMenuItem->getHeight();
	}

	rcItem.left = rcClient.left;
	rcItem.right = rcClient.right;
	rcItem.top = uItemTop;
	pYBMenuItem = m_containerItem[ uItemIndex ];
	rcItem.bottom = rcItem.top + pYBMenuItem->getHeight();
	
	return ( ( rcItem.right > rcItem.left ) && ( rcItem.bottom > rcItem.top ) );
}

void YBCtrlMenu::_drawAllItem( HDC hDC ){
	if( m_containerItem.size() == 0 ) 
		return;
	item_container_type::iterator itMenuItem, iendMenuItem;
	RECT rcMenuItem;

	iendMenuItem = m_containerItem.end();
	for ( itMenuItem = m_containerItem.begin(); itMenuItem!=iendMenuItem; ++itMenuItem ) {
		if ( !_calcItemRect( ( itMenuItem - m_containerItem.begin() ), rcMenuItem ) ) {
			continue;
		}
		(*itMenuItem)->drawStatus( hDC, rcMenuItem );
	}
}

void YBCtrlMenu::_drawStatus( HDC hDC ){
	RECT rcClient;
	unsigned int uHeight = 0;

	//{
	//	TCHAR szMsg[100];
	//	_stprintf_s( szMsg, _T("_drawStatus|  m_uHoverItemIndex:%d\n"), m_uHoverItemIndex );
	//	::OutputDebugString( szMsg );
	//}

	item_container_type::const_iterator citItem, ciendItem;
	ciendItem = m_containerItem.end();
	for ( citItem = m_containerItem.begin(); citItem!=ciendItem; ++citItem ) {
		uHeight += ( (*citItem)->getHeight() );
	}

	::GetClientRect( m_hWnd, &rcClient );
	m_pMemDC->resize( rcClient.right - rcClient.left, uHeight );
	_drawBk( m_pMemDC->getMemDC() );
	_drawAllItem( m_pMemDC->getMemDC() );

	::BitBlt( hDC, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, m_pMemDC->getMemDC(), 0, 0, SRCCOPY );
}

bool YBCtrlMenu::moveTest( const POINT& ptMouse, unsigned int& uIndexItem ) {
	int nItemCount = m_containerItem.size();
	RECT rcItem;

	for( int i= 0; i != nItemCount; ++i ) {
		if( !_calcItemRect( i, rcItem ) )
			return false;
		if( ::PtInRect( &rcItem, ptMouse ) ) {
			uIndexItem = i;
			return true;
		}
	}
	return false;
}

void YBCtrlMenu::setDivideHoverItem( const POINT& ptMouse, unsigned int uIndexItem ) {
	YBCtrlMenuItemBase* pYBMenuItem = NULL;

	pYBMenuItem = m_containerItem[ uIndexItem ];
	if ( !pYBMenuItem )
		return;

	if ( pYBMenuItem->isDivide2Part() ) {
		_setDividePart2Flag( ptMouse, uIndexItem );
		return;
	}
	if ( pYBMenuItem->isDivide3Part() ) {
		_setDividePart3Flag( ptMouse, uIndexItem );
		return;
	}
}

void YBCtrlMenu::_setDividePart3Flag( const POINT& ptMouse, unsigned int uIndexItem ) {
	RECT rcDividePart1;
	RECT rcDividePart2;
	RECT rcDividePart3;
	YBCtrlMenuItemBase* pYBMenuItem = NULL;
	pYBMenuItem = m_containerItem[ uIndexItem ];

	if ( !_calcDividePart1Rect( uIndexItem, rcDividePart1 ) || !_calcDividePart2Rect( uIndexItem, rcDividePart2 ) || !_calcDividePart3Rect( uIndexItem, rcDividePart3 ) )
		return;

	if ( m_uHoverItemIndex == uIndexItem ) {
		if ( ::PtInRect( &rcDividePart1, ptMouse ) ) {
			m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( true );
			m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
			m_containerItem[ uIndexItem ]->setMouseHoverInPart3Flag( false );
		} else if ( ::PtInRect( &rcDividePart2, ptMouse ) ) {
			m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( true );
			m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
			m_containerItem[ uIndexItem ]->setMouseHoverInPart3Flag( false );
		} else if ( ::PtInRect( &rcDividePart3, ptMouse ) ) {
			m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( true );
			m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
			m_containerItem[ uIndexItem ]->setMouseHoverInPart3Flag( false );
		} else {
			m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
			m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
			m_containerItem[ uIndexItem ]->setMouseHoverInPart3Flag( false );
		}
	}
	//
	if ( m_uHoverItemIndex != YBCTRLMENU_HOVERITEM_INDEX_INVALIDATE ) {
		m_containerItem[ m_uHoverItemIndex ]->setMouseHoverInPart1Flag( false );
		m_containerItem[ m_uHoverItemIndex ]->setMouseHoverInPart2Flag( false );
		m_containerItem[ m_uHoverItemIndex ]->setMouseHoverInPart3Flag( false );
		m_containerItem[ m_uHoverItemIndex ]->setMouseHoverFlag( false );
	}
	//
	m_uHoverItemIndex = uIndexItem;
	//
	if ( ::PtInRect( &rcDividePart1, ptMouse ) ) {
		m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( true );
		m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
		m_containerItem[ uIndexItem ]->setMouseHoverInPart3Flag( false );
	} else if ( ::PtInRect( &rcDividePart2, ptMouse ) ) {
		m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( true );
		m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
		m_containerItem[ uIndexItem ]->setMouseHoverInPart3Flag( false );
	} else if ( ::PtInRect( &rcDividePart3, ptMouse ) ) {
		m_containerItem[ uIndexItem ]->setMouseHoverInPart3Flag( true );
		m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
		m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
	} else {
		m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
		m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
		m_containerItem[ uIndexItem ]->setMouseHoverInPart3Flag( false );
	}

	if ( ::IsWindow( m_hWnd ) ) {
		::InvalidateRect( m_hWnd, NULL, FALSE );
	}
}

void YBCtrlMenu::_setDividePart2Flag( const POINT& ptMouse, unsigned int uIndexItem ) {
	RECT rcDividePart1;
	RECT rcDividePart2;
	YBCtrlMenuItemBase* pYBMenuItem = NULL;
	pYBMenuItem = m_containerItem[ uIndexItem ];

	if ( !_calcDividePart1Rect( uIndexItem, rcDividePart1 ) || !_calcDividePart2Rect( uIndexItem, rcDividePart2 ) )
		return;
	
	if ( m_uHoverItemIndex == uIndexItem ) {
		if ( ::PtInRect( &rcDividePart1, ptMouse ) ) {
			m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( true );
			m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
		} else if ( ::PtInRect( &rcDividePart2, ptMouse ) ) {
			m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( true );
			m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
		} else {
			m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
			m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
		}
	}
	//
	if ( m_uHoverItemIndex != YBCTRLMENU_HOVERITEM_INDEX_INVALIDATE ) {
		m_containerItem[ m_uHoverItemIndex ]->setMouseHoverInPart1Flag( false );
		m_containerItem[ m_uHoverItemIndex ]->setMouseHoverInPart2Flag( false );
		m_containerItem[ m_uHoverItemIndex ]->setMouseHoverInPart3Flag( false );
		m_containerItem[ m_uHoverItemIndex ]->setMouseHoverFlag( false );
	}
	//
	m_uHoverItemIndex = uIndexItem;
	//
	if ( ::PtInRect( &rcDividePart1, ptMouse ) ) {
		m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( true );
		m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
	} else if ( ::PtInRect( &rcDividePart2, ptMouse ) ) {
		m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( true );
		m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
	} else {
		m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
		m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
	}
	
	if ( ::IsWindow( m_hWnd ) ) {
		::InvalidateRect( m_hWnd, NULL, FALSE );
	}
}

bool YBCtrlMenu::setHoverItem( unsigned int uIndexItem ) {
	if ( uIndexItem >= m_containerItem.size() ) {
		assert(false);
		return false;
	}
	YBCtrlMenuItemBase* pYBMenuItemBase = NULL;
	pYBMenuItemBase = m_containerItem[ uIndexItem ];
	if ( pYBMenuItemBase->isDivide2Part() ) {
		return false;
	}
	if ( pYBMenuItemBase->isDivide3Part() ) {
		return false;
	}
 	if ( m_uHoverItemIndex == uIndexItem ) {
 		m_containerItem[ m_uHoverItemIndex ]->setMouseHoverFlag( true );
  		return true;
  	}
	//
	if ( m_uHoverItemIndex != YBCTRLMENU_HOVERITEM_INDEX_INVALIDATE ) {
		m_containerItem[ m_uHoverItemIndex ]->setMouseHoverFlag( false );
		m_containerItem[ m_uHoverItemIndex ]->setMouseHoverInPart1Flag( false );
		m_containerItem[ m_uHoverItemIndex ]->setMouseHoverInPart2Flag( false );
		m_containerItem[ m_uHoverItemIndex ]->setMouseHoverInPart3Flag( false );
	}
	//
	m_uHoverItemIndex = uIndexItem;
	//
	m_containerItem[ uIndexItem ]->setMouseHoverFlag( true );

	if( ::IsWindow( m_hWnd ) ) {
		::InvalidateRect( m_hWnd, NULL, FALSE );
	}
	return true;
}

void YBCtrlMenu::getHoverItemIndex( unsigned int& uHoverItemIndex ) const{
	uHoverItemIndex = m_uHoverItemIndex;
}

void YBCtrlMenu::setWidth( unsigned int uWidth ) {
	if ( m_uWidth == uWidth )
		return;
	m_uWidth = uWidth;
}

//////////////////////////////////////////////////////////////////////////
//bool YBCtrlMenu::setHoverItem( const POINT& ptMouse, unsigned int uIndexItem ) {
//	if ( uIndexItem >= m_containerItem.size() ) {
//		assert(false);
//		return false;
//	}
//	YBCtrlMenuItemBase* pYBMenuItemBase = NULL;
//	pYBMenuItemBase = m_containerItem[ uIndexItem ];
//	RECT rcDividePart1;
//	RECT rcDividePart2;
//
//	if ( !_calcDividePart1Rect( uIndexItem, rcDividePart1 ) || !_calcDividePart2Rect( uIndexItem, rcDividePart2 ) )
//		assert( false );
//	
//	if ( m_uHoverItemIndex == uIndexItem ) {
//		if ( pYBMenuItemBase->isDivide() ) {
//			if ( ::PtInRect( &rcDividePart1, ptMouse ) ) {
//				m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( true );
//				m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
//			} else if ( ::PtInRect( &rcDividePart2, ptMouse ) ) {
//				m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( true );
//				m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
//			} else {
//				m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
//				m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
//			}
//			return true;
//		} else {
//			m_containerItem[ m_uHoverItemIndex ]->setMouseHoverFlag( true );
//			return true;
//		}
//	}
//	//
//	if ( m_uHoverItemIndex != YBCTRLMENU_HOVERITEM_INDEX_INVALIDATE ) {
//		if ( pYBMenuItemBase->isDivide() ) {
//			if ( ::PtInRect( &rcDividePart1, ptMouse ) ) {
//				m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( true );
//				m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
//			} else if ( ::PtInRect( &rcDividePart2, ptMouse ) ) {
//				m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( true );
//				m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
//			} else {
//				m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
//				m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
//			}
//
//		} else {
//			m_containerItem[ m_uHoverItemIndex ]->setMouseHoverFlag( false );
//		}
//	}
//	//
//	m_uHoverItemIndex = uIndexItem;
//	//
//	if ( pYBMenuItemBase->isDivide() ) {
//		if ( ::PtInRect( &rcDividePart1, ptMouse ) ) {
//			m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( true );
//			m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
//		} else if ( ::PtInRect( &rcDividePart2, ptMouse ) ) {
//			m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( true );
//			m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
//		} else {
//			m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
//			m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
//		}
//	} else {
//		m_containerItem[ uIndexItem ]->setMouseHoverFlag( true );
//	}
//	
//	if( ::IsWindow( m_hWnd ) ) {
//		::InvalidateRect( m_hWnd, NULL, FALSE );
//	}
//	return true;
//}

//bool YBCtrlMenu::setHoverItem( const POINT& ptMouse, unsigned int uIndexItem ) {
//	if ( uIndexItem >= m_containerItem.size() ) {
//		assert(false);
//		return false;
//	}
//	RECT rcDividePart1;
//	RECT rcDividePart2;
//
//	if ( !_calcDividePart1Rect( uIndexItem, rcDividePart1 ) || !_calcDividePart2Rect( uIndexItem, rcDividePart2 ) )
//		assert( false );
//	YBCtrlMenuItemBase* pYBMenuItemBase = NULL;
//	pYBMenuItemBase = m_containerItem[ uIndexItem ];
//	if ( m_uHoverItemIndex == uIndexItem ) {
//		m_containerItem[ m_uHoverItemIndex ]->setMouseHoverFlag( true );
//		if ( pYBMenuItemBase->isDivide() ) {
//			if ( ::PtInRect( &rcDividePart1, ptMouse ) ) {
//				m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( true );
//				m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
//			} else if ( ::PtInRect( &rcDividePart2, ptMouse ) ) {
//				m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( true );
//				m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
//			} else {
//				m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
//				m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
//			}
//			return true;
//		}
//		return true;
//	}
//	//
//	if ( m_uHoverItemIndex != YBCTRLMENU_HOVERITEM_INDEX_INVALIDATE ) {
//		m_containerItem[ m_uHoverItemIndex ]->setMouseHoverFlag( false );
//		if ( pYBMenuItemBase->isDivide() ) {
//			m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
//			m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
//		}
//	}
//	//
//	m_uHoverItemIndex = uIndexItem;
//	//
//	m_containerItem[ uIndexItem ]->setMouseHoverFlag( true );
//	//
//	if ( ::PtInRect( &rcDividePart1, ptMouse ) ) {
//		m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( true );
//		m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
//	} else if ( ::PtInRect( &rcDividePart2, ptMouse ) ) {
//		m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( true );
//		m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
//	} else {
//		m_containerItem[ uIndexItem ]->setMouseHoverInPart1Flag( false );
//		m_containerItem[ uIndexItem ]->setMouseHoverInPart2Flag( false );
//	}
//	//
//	if( ::IsWindow( m_hWnd ) ) {
//		::InvalidateRect( m_hWnd, NULL, FALSE );
//	}
//	return true;
//}