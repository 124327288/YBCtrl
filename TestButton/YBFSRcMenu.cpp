#include "stdafx.h"
#include "YBFSRcMenu.h"
#include "frame/YBCtrlImageBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlMisc.h"
#include "frame/YBCtrlFontDepository.h"
#include "frame/YBCtrlImageDepository.h"
#include "loader/YBImageLoader.h"
#include "YBFSRcMenuItem.h"

USING_NAMESPACE_YBCTRL

YBFSRcMenu::YBFSRcMenu( unsigned int uMenuWidth )
: m_uWidth( uMenuWidth )
, m_menu4FS( uMenuWidth - 2 )
, m_hWndMsgRecv( NULL )
, m_uImageIdBk( YBCTRL_IMAGE_ID_INVALID)
, m_uImageIdHeaderBk( YBCTRL_IMAGE_ID_INVALID)
, m_uHeight( 0 )
, m_uFontIdFSRCMTxtNor( YBCTRL_FONT_ID_INVALID )
, m_uFontIdFSRCMTxtHov( YBCTRL_FONT_ID_INVALID )
, m_uFontIdFSRCMTxtDis( YBCTRL_FONT_ID_INVALID )
, m_uFontIdFSRCMIconTxt( YBCTRL_FONT_ID_INVALID )
, m_uFontIdFSRCMNumTxt( YBCTRL_FONT_ID_INVALID )
, m_uImageIdMenuIconNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdMenuIconHor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdMenuIconDis( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdMenuItemBk( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdMenuHover( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdMenuItemDisable( YBCTRL_IMAGE_ID_INVALID )
, m_hPenLine( NULL )
, m_uImageIdFSRCMHov_Bk( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdFSRCMBk_Hov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdFSRCMBk_Dis( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdFSRCMDis_Bk( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdFSRCMDis_Dis( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdFSRCMDis_Hov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdFSRCMHov_Dis( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdFSRCMBk_Bk( YBCTRL_IMAGE_ID_INVALID )
, m_uItemId1( YBCTRL_IMAGE_ID_INVALID )
, m_uItemId2( YBCTRL_IMAGE_ID_INVALID )
, m_uItemId3( YBCTRL_IMAGE_ID_INVALID )
, m_uItemId4( YBCTRL_IMAGE_ID_INVALID )
, m_uItemId5( YBCTRL_IMAGE_ID_INVALID )
, m_uItemId6( YBCTRL_IMAGE_ID_INVALID )
, m_uItemId7( YBCTRL_IMAGE_ID_INVALID )
, m_uItemId8( YBCTRL_IMAGE_ID_INVALID )
, m_uItemId9( YBCTRL_IMAGE_ID_INVALID )
, m_uItemId10( YBCTRL_IMAGE_ID_INVALID )
, m_uItemId11( YBCTRL_IMAGE_ID_INVALID )
{
	_loadFont();
	_loadImage();
	_init();
	m_hPenLine = ::CreatePen( PS_SOLID, 2, RGB(6, 49, 91) );
}

YBFSRcMenu::~YBFSRcMenu() {
	if ( m_hPenLine ) {
		::DeleteObject( m_hPenLine );
		m_hPenLine = NULL;
	}
}

bool YBFSRcMenu::trackPopupMenu( HWND hWndParent ) {
	m_hWndMsgRecv = hWndParent;
	HWND hWnd;
	DWORD dwExStyle = WS_EX_TOPMOST|WS_EX_TOOLWINDOW;
	DWORD dwStyle = WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	if ( ::IsWindow( m_hWnd ) ) {
		_destorySelf();
		assert( m_hWnd == 0 );
	}
	//
	hWnd = _createHWND( dwExStyle, dwStyle, 0, 0, m_uWidth, 0, hWndParent, (HMENU)NULL, NULL, NULL );
	if ( hWnd ) {
		//
		m_menu4FS.Create( 0, WS_CHILD|WS_VISIBLE, 1, YBFSRCMENU_HEADER_HEIGHT - 2, hWnd, YBFSRCMENUID_MENU4FS, NULL );
		// get Height 4 YBFSRcMenu
		m_uHeight = YBFSRCMENU_HEADER_HEIGHT + m_menu4FS.getHeight();
		//
		_movewnd();
		_clipWnd();
		//
		return true;
	}

	return false;
}

void YBFSRcMenu::_clipWnd() {
	HRGN hRgn;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	hRgn = ::CreateRoundRectRgn( 0, 0, rcClient.right + 1, rcClient.bottom + 1, 5, 5 );

	::SetWindowRgn( m_hWnd, hRgn, TRUE );
	::DeleteObject( hRgn );
}

void YBFSRcMenu::_movewnd() {
	if ( !m_hWnd ) {
		assert( false );
		return;
	}
	POINT ptMouse;
	unsigned int uScreenWidth;
	unsigned int uScreenHeight;
	unsigned int uRSpace;
	unsigned int uBSpace;
	unsigned int uTSpace;
	unsigned int uLSpace;

	::GetCursorPos( &ptMouse );
	uScreenWidth = ::GetSystemMetrics( SM_CXSCREEN );
	uScreenHeight = ::GetSystemMetrics( SM_CYSCREEN );
	uRSpace = uScreenWidth - ptMouse.x - RIGHTBAR_RSPACE;
	uBSpace = uScreenHeight - ptMouse.y - BOTTOMBAR_BSPACE;
	uTSpace = ptMouse.y - TOPBAR_TSPACE;
	uLSpace = ptMouse.x - LEFTBAR_LSPACE;
	//
	assert( LEFTBAR_LSPACE < uScreenWidth );
	assert( RIGHTBAR_RSPACE < uScreenWidth );
	assert( ( LEFTBAR_LSPACE + RIGHTBAR_RSPACE ) < uScreenWidth );
	assert( ( LEFTBAR_LSPACE + RIGHTBAR_RSPACE + m_uWidth ) < uScreenWidth );
	assert( BOTTOMBAR_BSPACE < uScreenHeight );
	assert( TOPBAR_TSPACE < uScreenHeight );
	assert( ( TOPBAR_TSPACE + BOTTOMBAR_BSPACE ) < uScreenHeight );
	assert( ( BOTTOMBAR_BSPACE + TOPBAR_TSPACE + m_uHeight ) < uScreenHeight );

	if ( ( ptMouse.x > int( uScreenWidth - RIGHTBAR_RSPACE ) )
		|| ( ptMouse.x < LEFTBAR_LSPACE )
		|| ( ptMouse.y > int( uScreenHeight - BOTTOMBAR_BSPACE )
		|| ( ptMouse.y < TOPBAR_TSPACE ) ) ) {
			return;
	}

	if ( ( m_uWidth == 0 ) || ( m_uHeight == 0 ) || ( m_uWidth > uScreenWidth ) || ( m_uHeight > uScreenHeight ) ) {
		assert( false );
		return;
	} else if ( ( uRSpace >= m_uWidth ) && ( uBSpace < m_uHeight ) && ( uTSpace >= m_uHeight ) ) {  // Bottom right corner1
		::MoveWindow( m_hWnd, ptMouse.x, ptMouse.y - m_uHeight, m_uWidth, m_uHeight, TRUE );
	} else if ( ( uRSpace >= m_uWidth ) && ( uBSpace < m_uHeight ) && ( uTSpace < m_uHeight ) ) {
		::MoveWindow( m_hWnd, ptMouse.x, TOPBAR_TSPACE, m_uWidth, m_uHeight, TRUE );
	} else if ( ( uBSpace >= m_uHeight ) && ( uRSpace < m_uWidth ) && ( uLSpace >= m_uWidth ) ) {  // Bottom right corner2
		::MoveWindow( m_hWnd, ptMouse.x - m_uWidth, ptMouse.y, m_uWidth, m_uHeight, TRUE );
	} else if ( ( uBSpace >= m_uHeight ) && ( uRSpace < m_uWidth ) && ( uLSpace < m_uWidth ) ) {
		::MoveWindow( m_hWnd, LEFTBAR_LSPACE, ptMouse.y, m_uWidth, m_uHeight, TRUE );
	} else if ( ( uRSpace < m_uWidth ) && ( uBSpace < m_uHeight ) && ( uLSpace >= m_uWidth ) && ( uTSpace >= m_uHeight ) ) { // Bottom right corner3
		::MoveWindow( m_hWnd, ptMouse.x - m_uWidth, ptMouse.y - m_uHeight, m_uWidth, m_uHeight, TRUE );
	} else if ( ( uRSpace < m_uWidth ) && ( uBSpace < m_uHeight ) && ( uLSpace < m_uWidth ) && ( uTSpace >= m_uHeight ) ) {
		::MoveWindow( m_hWnd, LEFTBAR_LSPACE, ptMouse.y - m_uHeight, m_uWidth, m_uHeight, TRUE  );
	} else if ( ( uRSpace < m_uWidth ) && ( uBSpace < m_uHeight ) && ( uLSpace >= m_uWidth ) && ( uTSpace < m_uHeight ) ) {
		::MoveWindow( m_hWnd, ptMouse.x - m_uWidth, TOPBAR_TSPACE, m_uWidth, m_uHeight, TRUE );
	} else if ( ( uRSpace < m_uWidth ) && ( uBSpace < m_uHeight ) && ( uLSpace < m_uWidth ) && ( uTSpace < m_uHeight ) ) {
		::MoveWindow( m_hWnd, LEFTBAR_LSPACE, TOPBAR_TSPACE, m_uWidth, m_uHeight, TRUE );
	} else if ( ( uRSpace >= m_uWidth ) && ( uBSpace < m_uHeight ) && ( uTSpace >= m_uHeight ) ) { // Bottom left corner
		::MoveWindow( m_hWnd, ptMouse.x, ptMouse.y - m_uHeight, m_uWidth, m_uHeight, TRUE );
	} else if ( ( uRSpace >= m_uWidth ) && ( uBSpace < m_uHeight ) && ( uTSpace < m_uHeight ) ) {
		::MoveWindow( m_hWnd, ptMouse.x, TOPBAR_TSPACE, m_uWidth, m_uHeight, TRUE  );
	} else if ( ( uRSpace < m_uWidth ) && ( uBSpace >= m_uHeight ) && ( uLSpace >= m_uWidth ) ) { // the top right corner
		::MoveWindow( m_hWnd, ptMouse.x - m_uWidth, ptMouse.y, m_uWidth, m_uHeight, TRUE );
	} else if ( ( uRSpace < m_uWidth ) && ( uBSpace >= m_uHeight ) && ( uLSpace < m_uWidth ) ) {
		::MoveWindow( m_hWnd, LEFTBAR_LSPACE, ptMouse.y, m_uWidth, m_uHeight, TRUE );
	} else {
		::MoveWindow( m_hWnd, ptMouse.x, ptMouse.y, m_uWidth, m_uHeight, TRUE );
	}

	::SetWindowPos( m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOSIZE );
}

void YBFSRcMenu::_destorySelf() {
	if ( m_hWnd == 0 )
		return;
	if ( ::IsWindow( m_hWnd ) ) {
		::DestroyWindow( m_hWnd );
	} else {
		assert( false );
	}
	m_hWnd = NULL;
}

YBCTRL_WNDMSGMAP_BEGIN( YBFSRcMenu, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_NCACTIVATE, _onWM_NCACTIVATE )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
YBCTRL_WNDMSGMAP_END()

void YBFSRcMenu::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );
	//////////////////////////////////////////////////////////////////////////
	unsigned int uItemId;
	uItemId = pYBCtrlWndMsg->m_lParam;
	WPARAM wParam;
	LPARAM lParam;
	lParam = (LPARAM)m_hWnd;

	switch ( wCtrlId ) {
	case YBFSRCMENUID_MENU4FS:
		{
			switch( wEvtCode ) {
			case YBMENU_CMDEVT_DIVIDE3PART1DOWN:
				{
					int a = 0;
				}
				break;
			case YBMENU_CMDEVT_DIVIDE3PART2DOWN:
				{
					int a = 0;
				}
				break;
			case YBMENU_CMDEVT_DIVIDE3PART3DOWN:
				{
					int a = 0;
				}
				break;
			case YBMENU_CMDEVT_DIVIDE2PART1DOWN:
				{
					if ( m_uItemId1 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN1;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA1_EVT, wParam, lParam );
					} else if ( m_uItemId2 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN2;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA1_EVT, wParam, lParam );
					} else if ( m_uItemId3 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN3;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA1_EVT, wParam, lParam );
					} else if ( m_uItemId4 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN4;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA1_EVT, wParam, lParam );
					} else if ( m_uItemId5 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN5;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA1_EVT, wParam, lParam );
					} else if ( m_uItemId6 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN6;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA1_EVT, wParam, lParam );
					} else if ( m_uItemId7 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN7;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA1_EVT, wParam, lParam );
					} else if ( m_uItemId8 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN8;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA1_EVT, wParam, lParam );
					} else if ( m_uItemId9 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN9;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA1_EVT, wParam, lParam );
					} else if ( m_uItemId10 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN10;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA1_EVT, wParam, lParam );
					} else if ( m_uItemId11 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN11;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA1_EVT, wParam, lParam );
					} else {
						assert( false );
						return;
					}
				}
				break;
			case YBMENU_CMDEVT_DIVIDE2PART2DOWN:
				{
					if ( m_uItemId1 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN1;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA2_EVT, wParam, lParam );
					} else if ( m_uItemId2 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN2;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA2_EVT, wParam, lParam );
					} else if ( m_uItemId3 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN3;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA2_EVT, wParam, lParam );
					} else if ( m_uItemId4 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN4;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA2_EVT, wParam, lParam );
					} else if ( m_uItemId5 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN5;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA2_EVT, wParam, lParam );
					} else if ( m_uItemId6 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN6;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA2_EVT, wParam, lParam );
					} else if ( m_uItemId7 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN7;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA2_EVT, wParam, lParam );
					} else if ( m_uItemId8 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN8;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA2_EVT, wParam, lParam );
					} else if ( m_uItemId9 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN9;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA2_EVT, wParam, lParam );
					} else if ( m_uItemId10 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN10;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA2_EVT, wParam, lParam );
					} else if ( m_uItemId11 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN11;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_DIVIDEPARA2_EVT, wParam, lParam );
					} else {
						assert( false );
						return;
					}
				}
				break;
			case YBMENU_CMDEVT_ITEMDOWN:
				{
					if ( m_uItemId1 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN1;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_EVT, wParam, lParam );
					} else if ( m_uItemId2 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN2;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_EVT, wParam, lParam );
					} else if ( m_uItemId3 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN3;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_EVT, wParam, lParam );
					} else if ( m_uItemId4 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN4;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_EVT, wParam, lParam );
					} else if ( m_uItemId5 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN5;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_EVT, wParam, lParam );
					} else if ( m_uItemId6 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN6;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_EVT, wParam, lParam );
					} else if ( m_uItemId7 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN7;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_EVT, wParam, lParam );
					} else if ( m_uItemId8 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN8;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_EVT, wParam, lParam );
					} else if ( m_uItemId9 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN9;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_EVT, wParam, lParam );
					} else if ( m_uItemId10 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN10;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_EVT, wParam, lParam );
					} else if ( m_uItemId11 == uItemId ) {
						wParam = YBFSRCMENU_CMDEVT_ITEMDOWN11;
						::SendMessage( m_hWndMsgRecv, WM_YBFSRCMENU_CLICK_EVT, wParam, lParam );
					} else {
						assert( false );
						return;
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	
}

void YBFSRcMenu::_onWM_NCACTIVATE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_NCACTIVATE );
	if ( pYBCtrlWndMsg->m_wParam == FALSE ) { // inactive
		::PostMessage( m_hWnd, WM_CLOSE, 0, 0 );
		return;
	}
}

void YBFSRcMenu::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	PAINTSTRUCT ps;
	HDC hPaintDC;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void YBFSRcMenu::_onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	pYBCtrlWndMsg->m_dwFlag = (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = 1;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
}

void YBFSRcMenu::_drawStatus( HDC hDC ) {
	_drawBk( hDC );
	_drawHeaderBk( hDC );
	//_drawLine( hDC );
}

void YBFSRcMenu::_drawLine( HDC hDC ) {
	HPEN hPenOld = NULL;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	hPenOld = (HPEN)::SelectObject( hDC, m_hPenLine );
	//
	::MoveToEx( hDC, rcClient.left, rcClient.bottom - 2, NULL );
	::LineTo( hDC, rcClient.right, rcClient.bottom - 2 );
	::SelectObject( hDC, hPenOld );
}

void YBFSRcMenu::_drawBk( HDC hDC ) {
	YBCtrlImageStatic* pYBImage = NULL;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	pYBImage = _getImageStatic( m_uImageIdBk );
	if ( !pYBImage ) {
		return;
	}

	BackGroundBitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pYBImage->getMemDC(),
		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0 );
}

void YBFSRcMenu::_drawHeaderBk( HDC hDC ) {
	YBCtrlImageStatic* pYBImage = NULL;
	RECT rcHeader;
	
	if( !_getHeaderRect( rcHeader ) ) {
		assert( false );
		return;
	}
	pYBImage = _getImageStatic( m_uImageIdHeaderBk );
	if ( !pYBImage )
		return;

	Gdiplus::Graphics graphics( hDC );
	GdipBitBlt( &graphics, pYBImage->getImage(), rcHeader.left, rcHeader.top, ( rcHeader.right - rcHeader.left ), ( rcHeader.bottom - rcHeader.top ),
		0, 0, pYBImage->getWidth(), pYBImage->getHeight() );

}

bool YBFSRcMenu::_getHeaderRect( RECT& rcHeader ) const {
	RECT rcClient;
	YBCtrlImageStatic* pYBImage = NULL;

	pYBImage = _getImageStatic( m_uImageIdHeaderBk );
	if ( !pYBImage )
		return false;
	::GetClientRect( m_hWnd, &rcClient );
	rcHeader.left = ( rcClient.right - rcClient.left - pYBImage->getWidth() )/2;
	rcHeader.right = rcHeader.left + pYBImage->getWidth();
	rcHeader.top = rcClient.top + YBFSRCMENU_HEADER_TSPACE;
	rcHeader.bottom = rcHeader.top + pYBImage->getHeight();

	return ( ( rcHeader.bottom > rcHeader.top ) && ( rcHeader.right > rcHeader.left ) && ( rcHeader.bottom <= YBFSRCMENU_HEADER_HEIGHT ) );
}

ybctrl::YBCtrlImageStatic* YBFSRcMenu::_getImageStatic( unsigned int uImageId ) const {
	YBCtrlImageBase* pYBImage = NULL;

	if ( !YBCtrlImageDepository::getInstance()->getImage( uImageId, &pYBImage ) )
		return NULL;
	if ( !pYBImage || ( pYBImage->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImage;
}

void YBFSRcMenu::_loadFont() {
	LOGFONT lFont;
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 19;
	m_uFontIdFSRCMTxtNor = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 6, 49, 91 ) );
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 19;
	m_uFontIdFSRCMTxtHov = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 190, 40, 0 ) );
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 19;
	m_uFontIdFSRCMTxtDis = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 102, 102, 102 ) );
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 19;
	m_uFontIdFSRCMIconTxt = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 19;
	m_uFontIdFSRCMNumTxt = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 6, 49, 91 ) );
}

#define BINDING_IMAGE_FNAME2ID( filename, imageid ) \
	do { \
	tstring_type tstrImageDirPath; \
	tstring_type tstrImageFilePath; \
	TCHAR szModulePath[MAX_PATH]; \
	TCHAR* posTmp = NULL; \
	::GetModuleFileName( NULL, szModulePath, MAX_PATH ); \
	posTmp = _tcsrchr( szModulePath, _T('\\')); \
	if ( !posTmp ) { \
	assert( false ); \
	return; \
	} \
	posTmp += 1; \
	*posTmp = 0; \
	tstrImageDirPath = szModulePath; \
	tstrImageDirPath += _T("Image\\"); \
	tstrImageFilePath = tstrImageDirPath; \
	tstrImageFilePath += filename; \
	if ( !ybctrl::YBImageLoader::getInstance()->loadImage( tstrImageFilePath, imageid ) ) { \
	assert( false ); \
	return; \
	} \
	} while( 0 )

void YBFSRcMenu::_loadImage() {
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuBk.png"), m_uImageIdBk );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuHeaderBk.png"), m_uImageIdHeaderBk );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuIconNor.png"), m_uImageIdMenuIconNor );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuIconHor.png"), m_uImageIdMenuIconHor );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuIconDis.png"), m_uImageIdMenuIconDis );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemBkNor.png"), m_uImageIdMenuItemBk );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemBkHov.png"), m_uImageIdMenuHover );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemBkDis.png"), m_uImageIdMenuItemDisable );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemBk_Bk.png"), m_uImageIdFSRCMBk_Bk );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemBk_Dis.png"), m_uImageIdFSRCMBk_Dis );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemBk_Hov.png"), m_uImageIdFSRCMBk_Hov );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemDis_Bk.png"), m_uImageIdFSRCMDis_Bk );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemDis_Dis.png"), m_uImageIdFSRCMDis_Dis );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemDis_Hov.png"), m_uImageIdFSRCMDis_Hov );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemHov_Bk.png"), m_uImageIdFSRCMHov_Bk );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemHov_Dis.png"), m_uImageIdFSRCMHov_Dis );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemHov_Bk_Bk.png"), m_uImageIdFSRCMHov_Bk_Bk );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemBk_Bk_Hov.png"), m_uImageIdFSRCMBk_Bk_Hov );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemBk_Hov_Bk.png"), m_uImageIdFSRCMBk_Hov_Bk );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemBk_Bk_Bk.png"), m_uImageIdFSRCMBk_Bk_Bk );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemDis_Bk_Hov.png"), m_uImageIdFSRCMDis_Bk_Hov );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemDis_Bk_Bk.png"), m_uImageIdFSRCMDis_Bk_Bk );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemDis_Hov_Bk.png"), m_uImageIdFSRCMDis_Hov_Bk );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemBk_Dis_Bk.png"), m_uImageIdFSRCMBk_Dis_Bk );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemBk_Dis_Hov.png"), m_uImageIdFSRCMBk_Dis_Hov );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemHov_Dis_Bk.png"), m_uImageIdFSRCMHov_Dis_Bk );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemBk_Bk_Dis.png"), m_uImageIdFSRCMBk_Bk_Dis );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemBk_Hov_Dis.png"), m_uImageIdFSRCMBk_Hov_Dis );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemHov_Bk_Dis.png"), m_uImageIdFSRCMHov_Bk_Dis );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemDis_Dis_Hov.png"), m_uImageIdFSRCMDis_Dis_Hov );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemDis_Dis_Bk.png"), m_uImageIdFSRCMDis_Dis_Bk );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemDis_Hov_Dis.png"), m_uImageIdFSRCMDis_Hov_Dis );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemDis_Bk_Dis.png"), m_uImageIdFSRCMDis_Bk_Dis );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemBk_Dis_Dis.png"), m_uImageIdFSRCMBk_Dis_Dis );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemHov_Dis_Dis.png"), m_uImageIdFSRCMHov_Dis_Dis );
	BINDING_IMAGE_FNAME2ID( _T("YBFSRcMenuItemDis_Dis_Dis.png"), m_uImageIdFSRCMDis_Dis_Dis );
}

void YBFSRcMenu::setItemStatus( unsigned int uItemId, tstring_type tstrTxt, bool bDisable ) {
	YBFSRcMenuItem* pYBFSRcMenuItem = NULL;
	pYBFSRcMenuItem = (YBFSRcMenuItem*)m_menu4FS.getItemById( uItemId );
	if ( !pYBFSRcMenuItem ) {
		assert( false );
		return;
	}
	pYBFSRcMenuItem->m_tstrTxt = tstrTxt;
	pYBFSRcMenuItem->setDisableFlag( bDisable );
}


void YBFSRcMenu::_init() {
	unsigned int uItemHeight = 29;
	//
	YBFSRcMenuItem* pYBMenuItem1 = NULL;
	unsigned int index = 0;
	pYBMenuItem1 = new YBFSRcMenuItem();
	pYBMenuItem1->m_tstrTxt = _T("外网收藏图片至相册编辑");
	pYBMenuItem1->m_tstrNumber = _T("1");
	pYBMenuItem1->m_uFontIdNorTxt = m_uFontIdFSRCMTxtNor;
	pYBMenuItem1->m_uFontIdHovTxt = m_uFontIdFSRCMTxtHov;
	pYBMenuItem1->m_uFontIdDisTxt = m_uFontIdFSRCMTxtDis;
	pYBMenuItem1->m_uFontIdIconTxt = m_uFontIdFSRCMIconTxt;
	pYBMenuItem1->m_uFontIdNumTxt = m_uFontIdFSRCMNumTxt;
	pYBMenuItem1->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem1->m_uImageIdIconNor = m_uImageIdMenuIconNor;
	pYBMenuItem1->m_uImageIdIconHor = m_uImageIdMenuIconHor;
	pYBMenuItem1->m_uImageIdIconDisable = m_uImageIdMenuIconDis;
	pYBMenuItem1->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem1->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem1->m_uItemHeight = uItemHeight;
	pYBMenuItem1->setCanClickFlag( true );
	pYBMenuItem1->setDisableFlag( true );
	pYBMenuItem1->m_bDrawDivideLine = true;
	m_menu4FS.insertItem( index, pYBMenuItem1, m_uItemId1 );
	//
	YBFSRcMenuItem* pYBMenuItem2 = NULL;
	unsigned int index1 = 1;
	pYBMenuItem2 = new YBFSRcMenuItem();
	pYBMenuItem2->m_tstrTxt = _T("外网收藏图片至聊天共享");
	pYBMenuItem2->m_tstrNumber = _T("2");
	pYBMenuItem2->m_uFontIdNorTxt = m_uFontIdFSRCMTxtNor;
	pYBMenuItem2->m_uFontIdHovTxt = m_uFontIdFSRCMTxtHov;
	pYBMenuItem2->m_uFontIdDisTxt = m_uFontIdFSRCMTxtDis;
	pYBMenuItem2->m_uFontIdIconTxt = m_uFontIdFSRCMIconTxt;
	pYBMenuItem2->m_uFontIdNumTxt = m_uFontIdFSRCMNumTxt;
	pYBMenuItem2->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem2->m_uImageIdIconNor = m_uImageIdMenuIconNor;
	pYBMenuItem2->m_uImageIdIconHor = m_uImageIdMenuIconHor;
	pYBMenuItem2->m_uImageIdIconDisable = m_uImageIdMenuIconDis;
	pYBMenuItem2->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem2->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem2->m_uItemHeight = uItemHeight;
	pYBMenuItem2->setCanClickFlag( true );
	pYBMenuItem2->setDisableFlag( true );
	pYBMenuItem2->m_bDrawDivideLine = true;
	m_menu4FS.insertItem( index1, pYBMenuItem2, m_uItemId2 );
	//
	YBFSRcMenuItem* pYBMenuItem3 = NULL;
	unsigned int index2 = 2;
	pYBMenuItem3 = new YBFSRcMenuItem();
	pYBMenuItem3->m_tstrTxt = _T("外网收藏图片至图片相册");
	pYBMenuItem3->m_tstrNumber = _T("3");
	pYBMenuItem3->m_uFontIdNorTxt = m_uFontIdFSRCMTxtNor;
	pYBMenuItem3->m_uFontIdHovTxt = m_uFontIdFSRCMTxtHov;
	pYBMenuItem3->m_uFontIdDisTxt = m_uFontIdFSRCMTxtDis;
	pYBMenuItem3->m_uFontIdIconTxt = m_uFontIdFSRCMIconTxt;
	pYBMenuItem3->m_uFontIdNumTxt = m_uFontIdFSRCMNumTxt;
	pYBMenuItem3->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem3->m_uImageIdIconNor = m_uImageIdMenuIconNor;
	pYBMenuItem3->m_uImageIdIconHor = m_uImageIdMenuIconHor;
	pYBMenuItem3->m_uImageIdIconDisable = m_uImageIdMenuIconDis;
	pYBMenuItem3->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem3->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem3->m_uItemHeight = uItemHeight;
	pYBMenuItem3->setCanClickFlag( true );
	pYBMenuItem3->setDisableFlag( true );
	pYBMenuItem3->m_bDrawDivideLine = true;
	m_menu4FS.insertItem( index2, pYBMenuItem3, m_uItemId3 );
	//
	YBFSRcMenuItem* pYBMenuItem4 = NULL;
	unsigned int index3 = 3;
	pYBMenuItem4 = new YBFSRcMenuItem();
	pYBMenuItem4->m_tstrTxt = _T("外网文件添加至邮件附件");
	pYBMenuItem4->m_tstrNumber = _T("4");
	pYBMenuItem4->m_uFontIdNorTxt = m_uFontIdFSRCMTxtNor;
	pYBMenuItem4->m_uFontIdHovTxt = m_uFontIdFSRCMTxtHov;
	pYBMenuItem4->m_uFontIdDisTxt = m_uFontIdFSRCMTxtDis;
	pYBMenuItem4->m_uFontIdIconTxt = m_uFontIdFSRCMIconTxt;
	pYBMenuItem4->m_uFontIdNumTxt = m_uFontIdFSRCMNumTxt;
	pYBMenuItem4->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem4->m_uImageIdIconNor = m_uImageIdMenuIconNor;
	pYBMenuItem4->m_uImageIdIconHor = m_uImageIdMenuIconHor;
	pYBMenuItem4->m_uImageIdIconDisable = m_uImageIdMenuIconDis;
	pYBMenuItem4->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem4->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem4->m_uItemHeight = uItemHeight;
	pYBMenuItem4->setCanClickFlag( true );
	pYBMenuItem4->setDisableFlag( true );
	pYBMenuItem4->m_bDrawDivideLine = true;
	m_menu4FS.insertItem( index3, pYBMenuItem4, m_uItemId4 );
	//
	YBFSRcMenuItem* pYBMenuItem5 = NULL;
	unsigned int index4 = 4;
	pYBMenuItem5 = new YBFSRcMenuItem();
	pYBMenuItem5->m_tstrTxt = _T("外网文件收藏至在线编辑器");
	pYBMenuItem5->m_tstrNumber = _T("5");
	pYBMenuItem5->m_bDrawDivideLine = true;
	pYBMenuItem5->m_uFontIdNorTxt = m_uFontIdFSRCMTxtNor;
	pYBMenuItem5->m_uFontIdHovTxt = m_uFontIdFSRCMTxtHov;
	pYBMenuItem5->m_uFontIdDisTxt = m_uFontIdFSRCMTxtDis;
	pYBMenuItem5->m_uFontIdIconTxt = m_uFontIdFSRCMIconTxt;
	pYBMenuItem5->m_uFontIdNumTxt = m_uFontIdFSRCMNumTxt;
	pYBMenuItem5->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem5->m_uImageIdIconNor = m_uImageIdMenuIconNor;
	pYBMenuItem5->m_uImageIdIconHor = m_uImageIdMenuIconHor;
	pYBMenuItem5->m_uImageIdIconDisable = m_uImageIdMenuIconDis;
	pYBMenuItem5->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem5->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem5->m_uItemHeight = uItemHeight;
	pYBMenuItem5->setCanClickFlag( true );
	pYBMenuItem5->setDisableFlag( true );
	pYBMenuItem5->m_bDrawDivideLine = true;
	m_menu4FS.insertItem( index4, pYBMenuItem5, m_uItemId5 );
	//
	YBFSRcMenuItem* pYBMenuItem6 = NULL;
	unsigned int index5 = 5;
	pYBMenuItem6 = new YBFSRcMenuItem();
	pYBMenuItem6->m_tstrDividePart1Txt = _T("复制");
	pYBMenuItem6->m_tstrDividePart2Txt = _T("粘贴");
	pYBMenuItem6->m_tstrDividePart3Txt = _T("返回上级");
	pYBMenuItem6->setCanClickInPart1Flag( true );
	pYBMenuItem6->setCanClickInPart2Flag( true );
	pYBMenuItem6->m_uImageIdBk_Hov = m_uImageIdFSRCMBk_Hov;
	pYBMenuItem6->m_uImageIdHov_Bk = m_uImageIdFSRCMHov_Bk;
	pYBMenuItem6->m_uImageIdDis_Bk = m_uImageIdFSRCMDis_Bk;
	pYBMenuItem6->m_uImageIdBk_Dis = m_uImageIdFSRCMBk_Dis;
	pYBMenuItem6->m_uImageIdDis_Dis = m_uImageIdFSRCMDis_Dis;
	pYBMenuItem6->m_uImageIdHov_Dis = m_uImageIdFSRCMHov_Dis;
	pYBMenuItem6->m_uImageIdDis_Hov = m_uImageIdFSRCMDis_Hov;
	pYBMenuItem6->m_uImageIdHov_Bk_Bk = m_uImageIdFSRCMHov_Bk_Bk;
	pYBMenuItem6->m_uImageIdBk_Bk_Hov = m_uImageIdFSRCMBk_Bk_Hov;
	pYBMenuItem6->m_uImageIdBk_Hov_Bk = m_uImageIdFSRCMBk_Hov_Bk;
	pYBMenuItem6->m_uImageIdBk_Bk_Bk = m_uImageIdFSRCMBk_Bk_Bk;
	pYBMenuItem6->m_uImageIdDis_Bk_Hov = m_uImageIdFSRCMDis_Bk_Hov;
	pYBMenuItem6->m_uImageIdDis_Bk_Bk = m_uImageIdFSRCMDis_Bk_Bk;
	pYBMenuItem6->m_uImageIdDis_Hov_Bk = m_uImageIdFSRCMDis_Hov_Bk;
	pYBMenuItem6->m_uImageIdBk_Dis_Bk = m_uImageIdFSRCMBk_Dis_Bk;
	pYBMenuItem6->m_uImageIdBk_Dis_Hov = m_uImageIdFSRCMBk_Dis_Hov;
	pYBMenuItem6->m_uImageIdHov_Dis_Bk = m_uImageIdFSRCMHov_Dis_Bk;
	pYBMenuItem6->m_uImageIdBk_Bk_Dis = m_uImageIdFSRCMBk_Bk_Dis;
	pYBMenuItem6->m_uImageIdBk_Hov_Dis = m_uImageIdFSRCMBk_Hov_Dis;
	pYBMenuItem6->m_uImageIdHov_Bk_Dis = m_uImageIdFSRCMHov_Bk_Dis;
	pYBMenuItem6->m_uImageIdDis_Dis_Hov = m_uImageIdFSRCMDis_Dis_Hov;
	pYBMenuItem6->m_uImageIdDis_Dis_Bk = m_uImageIdFSRCMDis_Dis_Bk;
	pYBMenuItem6->m_uImageIdDis_Hov_Dis = m_uImageIdFSRCMDis_Hov_Dis;
	pYBMenuItem6->m_uImageIdDis_Bk_Dis = m_uImageIdFSRCMDis_Bk_Dis;
	pYBMenuItem6->m_uImageIdBk_Dis_Dis = m_uImageIdFSRCMBk_Dis_Dis;
	pYBMenuItem6->m_uImageIdHov_Dis_Dis = m_uImageIdFSRCMHov_Dis_Dis;
	pYBMenuItem6->m_uImageIdDis_Dis_Dis = m_uImageIdFSRCMDis_Dis_Dis;
	pYBMenuItem6->m_tstrNumber = _T("6");
	pYBMenuItem6->setDividePart3Flag( true );
	pYBMenuItem6->setDisableInPart1Flag( true );
	pYBMenuItem6->setDisableInPart2Flag( true );
	pYBMenuItem6->setDisableInPart3Flag( true );
	pYBMenuItem6->m_uFontIdNorTxt = m_uFontIdFSRCMTxtNor;
	pYBMenuItem6->m_uFontIdHovTxt = m_uFontIdFSRCMTxtHov;
	pYBMenuItem6->m_uFontIdDisTxt = m_uFontIdFSRCMTxtDis;
	pYBMenuItem6->m_uFontIdIconTxt = m_uFontIdFSRCMIconTxt;
	pYBMenuItem6->m_uFontIdNumTxt = m_uFontIdFSRCMNumTxt;
	pYBMenuItem6->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem6->m_uImageIdIconNor = m_uImageIdMenuIconNor;
	pYBMenuItem6->m_uImageIdIconHor = m_uImageIdMenuIconHor;
	pYBMenuItem6->m_uImageIdIconDisable = m_uImageIdMenuIconDis;
	pYBMenuItem6->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem6->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem6->m_uItemHeight = uItemHeight;
	pYBMenuItem6->m_bDrawDivideLine = true;
	m_menu4FS.insertItem( index5, pYBMenuItem6, m_uItemId6 );
	//
	YBFSRcMenuItem* pYBMenuItem7 = NULL;
	unsigned int index6 = 6;
	pYBMenuItem7 = new YBFSRcMenuItem();
	pYBMenuItem7->setDividePart2Flag( true );
	pYBMenuItem7->m_tstrNumber = _T("7");
	pYBMenuItem7->m_tstrDividePart1Txt = _T("实时刷新");
	pYBMenuItem7->m_tstrDividePart2Txt = _T("关闭电脑");
	pYBMenuItem7->setCanClickInPart1Flag( true );
	pYBMenuItem7->setCanClickInPart2Flag( true );
	pYBMenuItem7->m_uImageIdBk_Hov = m_uImageIdFSRCMBk_Hov;
	pYBMenuItem7->m_uImageIdHov_Bk = m_uImageIdFSRCMHov_Bk;
	pYBMenuItem7->m_uImageIdDis_Bk = m_uImageIdFSRCMDis_Bk;
	pYBMenuItem7->m_uImageIdBk_Dis = m_uImageIdFSRCMBk_Dis;
	pYBMenuItem7->m_uImageIdDis_Dis = m_uImageIdFSRCMDis_Dis;
	pYBMenuItem7->m_uImageIdHov_Dis = m_uImageIdFSRCMHov_Dis;
	pYBMenuItem7->m_uImageIdDis_Hov = m_uImageIdFSRCMDis_Hov;
	pYBMenuItem7->m_uFontIdNorTxt = m_uFontIdFSRCMTxtNor;
	pYBMenuItem7->m_uFontIdHovTxt = m_uFontIdFSRCMTxtHov;
	pYBMenuItem7->m_uFontIdDisTxt = m_uFontIdFSRCMTxtDis;
	pYBMenuItem7->m_uFontIdIconTxt = m_uFontIdFSRCMIconTxt;
	pYBMenuItem7->m_uFontIdNumTxt = m_uFontIdFSRCMNumTxt;
	pYBMenuItem7->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem7->m_uImageIdIconNor = m_uImageIdMenuIconNor;
	pYBMenuItem7->m_uImageIdIconHor = m_uImageIdMenuIconHor;
	pYBMenuItem7->m_uImageIdIconDisable = m_uImageIdMenuIconDis;
	pYBMenuItem7->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem7->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem7->m_uItemHeight = uItemHeight;
	pYBMenuItem7->m_bDrawDivideLine = true;
	m_menu4FS.insertItem( index6, pYBMenuItem7, m_uItemId7 );
	//
	YBFSRcMenuItem* pYBMenuItem8 = NULL;
	unsigned int index7 = 7;
	pYBMenuItem8 = new YBFSRcMenuItem();
	pYBMenuItem8->m_tstrDividePart1Txt = _T("重启电脑");
	pYBMenuItem8->m_tstrDividePart2Txt = _T("功能说明");
	pYBMenuItem8->m_tstrNumber = _T("8");
	pYBMenuItem8->setDividePart2Flag( true );
	pYBMenuItem8->setCanClickInPart1Flag( true );
	pYBMenuItem8->setCanClickInPart2Flag( true );
	pYBMenuItem8->m_uImageIdBk_Hov = m_uImageIdFSRCMBk_Hov;
	pYBMenuItem8->m_uImageIdHov_Bk = m_uImageIdFSRCMHov_Bk;
	pYBMenuItem8->m_uImageIdDis_Bk = m_uImageIdFSRCMDis_Bk;
	pYBMenuItem8->m_uImageIdBk_Dis = m_uImageIdFSRCMBk_Dis;
	pYBMenuItem8->m_uImageIdDis_Dis = m_uImageIdFSRCMDis_Dis;
	pYBMenuItem8->m_uImageIdHov_Dis = m_uImageIdFSRCMHov_Dis;
	pYBMenuItem8->m_uImageIdDis_Hov = m_uImageIdFSRCMDis_Hov;
	pYBMenuItem8->m_uFontIdNorTxt = m_uFontIdFSRCMTxtNor;
	pYBMenuItem8->m_uFontIdHovTxt = m_uFontIdFSRCMTxtHov;
	pYBMenuItem8->m_uFontIdDisTxt = m_uFontIdFSRCMTxtDis;
	pYBMenuItem8->m_uFontIdIconTxt = m_uFontIdFSRCMIconTxt;
	pYBMenuItem8->m_uFontIdNumTxt = m_uFontIdFSRCMNumTxt;
	pYBMenuItem8->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem8->m_uImageIdIconNor = m_uImageIdMenuIconNor;
	pYBMenuItem8->m_uImageIdIconHor = m_uImageIdMenuIconHor;
	pYBMenuItem8->m_uImageIdIconDisable = m_uImageIdMenuIconDis;
	pYBMenuItem8->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem8->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem8->m_uItemHeight = uItemHeight;
	pYBMenuItem8->m_bDrawDivideLine = true;
	m_menu4FS.insertItem( index7, pYBMenuItem8, m_uItemId8 );
	//
	YBFSRcMenuItem* pYBMenuItem9 = NULL;
	unsigned int index8 = 8;
	pYBMenuItem9 = new YBFSRcMenuItem();
	pYBMenuItem9->m_tstrTxt = _T("关闭电脑");
	pYBMenuItem9->m_tstrNumber = _T("9");
	pYBMenuItem9->setDividePart2Flag( true );
	pYBMenuItem9->m_tstrDividePart1Txt = _T("任添网站");
	pYBMenuItem9->m_tstrDividePart2Txt = _T("关联容器");
	pYBMenuItem9->setCanClickInPart1Flag( true );
	pYBMenuItem9->setCanClickInPart2Flag( true );
	pYBMenuItem9->m_uImageIdBk_Hov = m_uImageIdFSRCMBk_Hov;
	pYBMenuItem9->m_uImageIdHov_Bk = m_uImageIdFSRCMHov_Bk;
	pYBMenuItem9->m_uImageIdDis_Bk = m_uImageIdFSRCMDis_Bk;
	pYBMenuItem9->m_uImageIdBk_Dis = m_uImageIdFSRCMBk_Dis;
	pYBMenuItem9->m_uImageIdDis_Dis = m_uImageIdFSRCMDis_Dis;
	pYBMenuItem9->m_uImageIdHov_Dis = m_uImageIdFSRCMHov_Dis;
	pYBMenuItem9->m_uImageIdDis_Hov = m_uImageIdFSRCMDis_Hov;
	pYBMenuItem9->m_uFontIdNorTxt = m_uFontIdFSRCMTxtNor;
	pYBMenuItem9->m_uFontIdHovTxt = m_uFontIdFSRCMTxtHov;
	pYBMenuItem9->m_uFontIdDisTxt = m_uFontIdFSRCMTxtDis;
	pYBMenuItem9->m_uFontIdIconTxt = m_uFontIdFSRCMIconTxt;
	pYBMenuItem9->m_uFontIdNumTxt = m_uFontIdFSRCMNumTxt;
	pYBMenuItem9->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem9->m_uImageIdIconNor = m_uImageIdMenuIconNor;
	pYBMenuItem9->m_uImageIdIconHor = m_uImageIdMenuIconHor;
	pYBMenuItem9->m_uImageIdIconDisable = m_uImageIdMenuIconDis;
	pYBMenuItem9->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem9->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem9->m_uItemHeight = uItemHeight;
	pYBMenuItem9->m_bDrawDivideLine = true;
	m_menu4FS.insertItem( index8, pYBMenuItem9, m_uItemId9 );
	//
	YBFSRcMenuItem* pYBMenuItem10 = NULL;
	unsigned int index9 = 9;
	pYBMenuItem10 = new YBFSRcMenuItem();
	pYBMenuItem10->m_tstrTxt = _T("显示版本信息");
	pYBMenuItem10->m_tstrNumber = _T("10");
	pYBMenuItem10->m_uFontIdNorTxt = m_uFontIdFSRCMTxtNor;
	pYBMenuItem10->m_uFontIdHovTxt = m_uFontIdFSRCMTxtHov;
	pYBMenuItem10->m_uFontIdDisTxt = m_uFontIdFSRCMTxtDis;
	pYBMenuItem10->m_uFontIdIconTxt = m_uFontIdFSRCMIconTxt;
	pYBMenuItem10->m_uFontIdNumTxt = m_uFontIdFSRCMNumTxt;
	pYBMenuItem10->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem10->m_uImageIdIconNor = m_uImageIdMenuIconNor;
	pYBMenuItem10->m_uImageIdIconHor = m_uImageIdMenuIconHor;
	pYBMenuItem10->m_uImageIdIconDisable = m_uImageIdMenuIconDis;
	pYBMenuItem10->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem10->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem10->m_uItemHeight = uItemHeight;
	pYBMenuItem10->setCanClickFlag( true );
	pYBMenuItem10->setDisableFlag( false );
	pYBMenuItem10->m_bDrawDivideLine = true;
	m_menu4FS.insertItem( index9, pYBMenuItem10, m_uItemId10 );
	//
	YBFSRcMenuItem* pYBMenuItem11 = NULL;
	unsigned int index10 = 10;
	pYBMenuItem11 = new YBFSRcMenuItem();
	pYBMenuItem11->m_tstrTxt = _T("查看并复制网页网址(URL)");
	pYBMenuItem11->m_tstrNumber = _T("11");
	pYBMenuItem11->m_uFontIdNorTxt = m_uFontIdFSRCMTxtNor;
	pYBMenuItem11->m_uFontIdHovTxt = m_uFontIdFSRCMTxtHov;
	pYBMenuItem11->m_uFontIdDisTxt = m_uFontIdFSRCMTxtDis;
	pYBMenuItem11->m_uFontIdIconTxt = m_uFontIdFSRCMIconTxt;
	pYBMenuItem11->m_uFontIdNumTxt = m_uFontIdFSRCMNumTxt;
	pYBMenuItem11->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem11->m_uImageIdIconNor = m_uImageIdMenuIconNor;
	pYBMenuItem11->m_uImageIdIconHor = m_uImageIdMenuIconHor;
	pYBMenuItem11->m_uImageIdIconDisable = m_uImageIdMenuIconDis;
	pYBMenuItem11->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem11->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem11->m_uItemHeight = uItemHeight;
	pYBMenuItem11->setCanClickFlag( true );
	pYBMenuItem11->setDisableFlag( false );
	pYBMenuItem11->m_bDrawDivideLine = true;
	m_menu4FS.insertItem( index10, pYBMenuItem11, m_uItemId11 );
}