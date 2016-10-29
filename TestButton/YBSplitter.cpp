#include "stdafx.h"
#include "YBSplitter.h"
#include "frame\YBCtrlDefs.h"
#include "frame\YBCtrlMemDC.h"

#define YBSPLITTER_FLAG_NONE (0x00000000)
#define YBSPLITTER_FLAG_BTNDOWN (0x00000001)

USING_NAMESPACE_YBCTRL

YBSplitter::YBSplitter( ESPLITTERTYPE eSplitterType )
: m_uFlag( YBSPLITTER_FLAG_NONE )
, m_eSplitterType( eSplitterType )
, m_hWndRecv( NULL )
, m_hWndSetCaptureOld( NULL )
, m_wordCtrlID( 0 )
, m_nMinSpace1( 0 )
, m_nMinSpace2( 0 )
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

HWND YBSplitter::create( HWND hWndRecv, int x, int y, int nWidth, int nHeight, int nMinSpace1, int nMinSpace2, WORD wCtrlId ) {
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
	TCHAR szMsg[ 50 ];
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

	_stprintf_s( szMsg, _T( "WM_MOUSEMOVE x:%d, y:%d\n" ), ptClient.x, ptClient.y );
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
		if ( m_eSplitterType == ESPLITTER_HORIZONTAL ) {
			::MoveWindow( m_hWnd, ( ptClient.x < m_nXPos ) ? max( ptClient.x, m_nMinSpace1 ) : min( ptClient.x, ( int )uWidthRecv - m_nMinSpace2 ), m_nYPos, uWidth, uHeight, TRUE );
		} else {
			::MoveWindow( m_hWnd, m_nXPos, ( ptClient.y < m_nYPos ) ? max( ptClient.y, m_nMinSpace1 ) : min( ptClient.y, (int)uHieghtRecv - m_nMinSpace2 ), uWidth, uHeight, TRUE );
		}
		//
		YBCtrlMemDC memDC( uWidth, uHeight );
		_drawMoveBk( memDC.getMemDC() );

		HDC hDC = ::GetDC( m_hWnd );
		::BitBlt( hDC, 0, 0, uWidth, uHeight, memDC.getMemDC(), 0, 0, SRCCOPY );
		::ReleaseDC( m_hWnd, hDC );
		return;
	}
	//
	drawStatus();
}

void YBSplitter::_onWM_MOUSELEAVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSELEAVE );
	POINT ptMouse;
	RECT rcClient;

	m_uFlag &= ~YBSPLITTER_FLAG_BTNDOWN;

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
	
	m_uFlag |= YBSPLITTER_FLAG_BTNDOWN;

	if ( m_eSplitterType == ESPLITTER_HORIZONTAL ) {
		::SetCursor( ::LoadCursor( NULL, IDC_SIZEWE ) );
	} else {
		::SetCursor( ::LoadCursor( NULL, IDC_SIZENS ) );
	}
	//
	drawStatus();
	m_hWndSetCaptureOld = ::SetCapture( m_hWnd );
}

void YBSplitter::_onWM_LBUTTONUP( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONUP );
	POINT ptMouse;
	RECT rcClient;
	WPARAM wParam;
	LPARAM lParam;

	m_uFlag &= ~YBSPLITTER_FLAG_BTNDOWN;

	::SetCursor( ::LoadCursor( NULL, IDC_ARROW ) );
	::SetCapture( m_hWndSetCaptureOld );
	//
	::GetCursorPos( &ptMouse );
	::GetClientRect( m_hWnd, &rcClient );
	::ScreenToClient( m_hWndRecv, &ptMouse );

	wParam = MAKELONG( m_wordCtrlID, YBSPLITTER_CMDEVT_DOWN );
	if ( m_eSplitterType == ESPLITTER_HORIZONTAL ) {
		lParam = (LPARAM)ptMouse.x;
	} else {
		lParam = (LPARAM)ptMouse.y;
	}
	::SendMessage( m_hWndRecv, WM_COMMAND, wParam, lParam );

	drawStatus();
	::ReleaseCapture();
}

void YBSplitter::_drawStatus( HDC hDC ) {
	_drawBk( hDC );
}

void YBSplitter::_drawBk( HDC hDC ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	::FillRect( hDC, &rcClient, m_hBrushBk );
}

void YBSplitter::_drawMoveBk( HDC hDC ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	::FillRect( hDC, &rcClient, m_hBrushMoveBk );
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