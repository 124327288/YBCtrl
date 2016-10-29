#include "stdafx.h"
#include "splitter\YBSplitter.h"
#include "frame\YBCtrlDefs.h"
#include "frame\YBCtrlMemDC.h"
#include <assert.h>

#define YBSPLITTER_FLAG_NONE (0x00000000)
#define YBSPLITTER_FLAG_BTNDOWN (0x00000001)

#define YBSPLITTER_FLAG_MOVING (0x00000002)

USING_NAMESPACE_YBCTRL

YBSplitter::YBSplitter( ESPLITTERTYPE eSplitterType )
: m_uFlag( YBSPLITTER_FLAG_NONE )
, m_eSplitterType( eSplitterType )
, m_hWndRecv( NULL )
, m_hWndSetCaptureOld( NULL )
, m_wordCtrlID( 0 )
, m_nMinSpace1( 0 )
, m_nMinSpace2( 0 )
, m_nStarMoveSpace( 0 )
, m_nEndMoveSpace( 0 )
, m_b1stMoving( true )
, m_nXPos( 0 )
, m_nYPos( 0 )
{
	m_hBrushBk = ::CreateSolidBrush( RGB( 255, 255, 255 ));
	m_hBrushMoveBk = ::CreateSolidBrush( RGB( 255, 0, 0 ));
}

YBSplitter::~YBSplitter() {
	if ( m_hBrushBk ) {
		::DeleteObject( m_hBrushBk );
		m_hBrushBk = NULL;
	}
	if ( m_hBrushMoveBk ) {
		::DeleteObject( m_hBrushMoveBk );
		m_hBrushMoveBk = NULL;
	}
}

HWND YBSplitter::Create( HWND hWndRecv, int x, int y, int nWidth, int nHeight, int nMinSpace1, int nMinSpace2, WORD wCtrlId ) {
	HWND hWnd;
	//
	m_hWndRecv = hWndRecv;
	m_nXPos = x;
	m_nYPos = y;
	m_nMinSpace1 = nMinSpace1;
	m_nMinSpace2 = nMinSpace2;
	m_wordCtrlID = wCtrlId;
	hWnd = _createHWND( WS_EX_TOPMOST, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, x, y, nWidth, nHeight, hWndRecv, (HMENU)wCtrlId, NULL, NULL );
	if ( !hWnd ) {
		assert( false );
		return NULL;
	}
	//
	::BringWindowToTop( m_hWnd );
	return hWnd;
}

YBCTRL_WNDMSGMAP_BEGIN( YBSplitter, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
	YBCTRL_ONWNDMSG( WM_MOUSELEAVE, _onWM_MOUSELEAVE )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_LBUTTONUP, _onWM_LBUTTONUP )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
YBCTRL_WNDMSGMAP_END()

void YBSplitter::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
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

void YBSplitter::_onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	RECT rcClient;
	unsigned int uWidth;
	unsigned int uHeight;

	pYBCtrlWndMsg->m_dwFlag = ( YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID );
	pYBCtrlWndMsg->m_lRes = 1;

	::GetClientRect( m_hWnd, &rcClient );
	uWidth = rcClient.right - rcClient.left;
	uHeight = rcClient.bottom - rcClient.top;
	YBCtrlMemDC memDC( uWidth, uHeight );
	_drawStatus( memDC.getMemDC() );

	::BitBlt( hDC, 0, 0, uWidth, uHeight, memDC.getMemDC(), 0, 0, SRCCOPY );
}

void YBSplitter::_onWM_MOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	TRACKMOUSEEVENT trackMouseEvt;
	POINT ptMouse;
	POINT ptClient;
	RECT rcClient;
	RECT rcRecv;
	RECT rcWnd;
	TCHAR szMsg[ 250 ];
	unsigned int uWidth;
	unsigned int uHeight;
	unsigned int uWidthRecv;
	unsigned int uHieghtRecv;

	::GetCursorPos( &ptMouse );
	::GetClientRect( m_hWnd, &rcClient );
	::ScreenToClient( m_hWndRecv, &ptMouse );
	ptClient = ptMouse;
	uWidth = rcClient.right - rcClient.left;
	uHeight = rcClient.bottom - rcClient.top;
	::GetClientRect( m_hWndRecv, &rcRecv );
	uWidthRecv = rcRecv.right - rcRecv.left;
	uHieghtRecv = rcRecv.bottom - rcRecv.top;

	_stprintf_s( szMsg, _T( "WM_MOUSEMOVE y:%d\n" ), ptClient.y );
	::OutputDebugString( szMsg );
	//
	trackMouseEvt.cbSize = sizeof( TRACKMOUSEEVENT );
	trackMouseEvt.dwFlags = TME_LEAVE;
	trackMouseEvt.hwndTrack = pYBCtrlWndMsg->m_hWnd;
	trackMouseEvt.dwHoverTime = HOVER_DEFAULT;
	::TrackMouseEvent( &trackMouseEvt );

	if ( m_eSplitterType == ESPLITTER_HORIZONTAL ) {
		::SetCursor( ::LoadCursor( NULL, IDC_SIZEWE ) );
	} else {
		::SetCursor( ::LoadCursor( NULL, IDC_SIZENS ) );
	}

	if ( m_uFlag & YBSPLITTER_FLAG_BTNDOWN ) {
		if ( m_b1stMoving ) {
			::GetWindowRect( m_hWnd, &rcClient );
			::GetWindowRect( m_hWndRecv, &rcWnd );
			m_b1stMoving = false;
			if ( m_eSplitterType == YBSplitter::ESPLITTER_HORIZONTAL ) {
				m_nXPos = rcClient.left - rcWnd.left;
			} else {
				//m_nYPos = rcClient.top - rcWnd.top;
				m_nYPos = ptClient.y;
			}
		}
		if ( m_eSplitterType == ESPLITTER_HORIZONTAL ) {
			::MoveWindow( m_hWnd, ( ptClient.x < m_nXPos ) ? max( ptClient.x, m_nMinSpace1 ) : min( ptClient.x, ( int )uWidthRecv - m_nMinSpace2 ), m_nYPos, uWidth, uHeight, TRUE );
		} else {
			::MoveWindow( m_hWnd, m_nXPos, ( ptClient.y < m_nYPos ) ? max( ptClient.y, m_nMinSpace1 ) : min( ptClient.y, ( int )uHieghtRecv - m_nMinSpace2 ), uWidth, uHeight, TRUE );
		}
	}
	//
	m_uFlag |= YBSPLITTER_FLAG_MOVING;
	//
	drawStatus();
}

void YBSplitter::_onWM_MOUSELEAVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSELEAVE );
	POINT ptMouse;
	RECT rcClient;

	m_b1stMoving = true;
	m_uFlag &= ~YBSPLITTER_FLAG_BTNDOWN;
	m_uFlag &= ~YBSPLITTER_FLAG_MOVING;

	::GetCursorPos( &ptMouse );
	::GetClientRect( m_hWnd, &rcClient );
	::ScreenToClient( m_hWndRecv, &ptMouse );
	
	TCHAR szMsg[ 50 ];
	_stprintf_s( szMsg, _T( "WM_MOUSELEAVE x:%d, y:%d\n" ), ptMouse.x, ptMouse.y );
	::OutputDebugString( szMsg );

	drawStatus();
}

void YBSplitter::_onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
	RECT rcClient;
	RECT rcWnd;

	m_uFlag |= YBSPLITTER_FLAG_BTNDOWN;
	m_uFlag |= YBSPLITTER_FLAG_MOVING;

	::GetWindowRect( m_hWnd, &rcClient );
	::GetWindowRect( m_hWndRecv, &rcWnd );
	if ( m_eSplitterType == ESPLITTER_HORIZONTAL ) {
		::SetCursor( ::LoadCursor( NULL, IDC_SIZEWE ) );
		m_nStarMoveSpace = rcClient.left - rcWnd.left;
	} else {
		::SetCursor( ::LoadCursor( NULL, IDC_SIZENS ) );
		m_nStarMoveSpace = rcClient.top - rcWnd.top;
	}
	//
	drawStatus();
	//
	m_hWndSetCaptureOld = ::SetCapture( m_hWnd );
}

void YBSplitter::_onWM_LBUTTONUP( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONUP );
	RECT rcClient;
	RECT rcWnd;
	WPARAM wParam;
	int nMoveSpace;
	 
	m_b1stMoving = true;
	m_uFlag &= ~YBSPLITTER_FLAG_BTNDOWN;
	m_uFlag &= ~YBSPLITTER_FLAG_MOVING;

	::GetWindowRect( m_hWnd, &rcClient );
	::GetWindowRect( m_hWndRecv, &rcWnd );
	::SetCursor( ::LoadCursor( NULL, IDC_ARROW ) );
	::SetCapture( m_hWndSetCaptureOld );
	//

	wParam = MAKELONG( m_wordCtrlID, YBSPLITTER_CMDEVT_DOWN );
	if ( m_eSplitterType == ESPLITTER_HORIZONTAL ) {
		m_nEndMoveSpace = rcClient.left - rcWnd.left;
	} else {
		m_nEndMoveSpace = rcClient.top - rcWnd.top;
	}
	nMoveSpace = ( m_nEndMoveSpace - m_nStarMoveSpace );
// 	if ( m_b1stMoving ) {
// 		nMoveSpace = 0;
// 	} else {
// 		nMoveSpace = ( m_nEndMoveSpace - m_nStarMoveSpace );
// 		m_b1stMoving = false;
// 	}
	TCHAR tszData[ 50 ];
	_stprintf_s( tszData, _T( "最开始移动为： %d, 最后移动为：%d, 移动距离为：%d \n" ), m_nStarMoveSpace, m_nEndMoveSpace, nMoveSpace );
	::OutputDebugString(tszData);
	::SendMessage( m_hWndRecv, WM_COMMAND, wParam, nMoveSpace );

	drawStatus();
	::ReleaseCapture();
}

void YBSplitter::_drawStatus( HDC hDC ) {
	_drawBk( hDC );
}

void YBSplitter::_drawBk( HDC hDC ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	if ( m_uFlag & YBSPLITTER_FLAG_MOVING ) {
		::FillRect( hDC, &rcClient, m_hBrushMoveBk );
	} else {
		::FillRect( hDC, &rcClient, m_hBrushBk );
	}
}


void YBSplitter::drawStatus() {
	HDC hDC;
	RECT rcClient;
	unsigned int uWidth;
	unsigned int uHeight;

	::GetClientRect( m_hWnd, &rcClient );
	uWidth = rcClient.right - rcClient.left;
	uHeight = rcClient.bottom - rcClient.top;
	YBCtrlMemDC memDC( uWidth, uHeight );
	_drawStatus( memDC.getMemDC() );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, uWidth, uHeight, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
}

void YBSplitter::setBkClr( COLORREF clrBk ) {
	m_hBrushBk = ::CreateSolidBrush( clrBk );
}

void YBSplitter::setMoveBkClr( COLORREF clrMoveBk ) {
	m_hBrushMoveBk = ::CreateSolidBrush( clrMoveBk );
}