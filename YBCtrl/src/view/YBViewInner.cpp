#include "stdafx.h"
#include "view\YBViewInner.h"
#include "frame\YBCtrlMemDC.h"
#include "frame\YBCtrlDefs.h"
#include <assert.h>
//
#define YBVIEWINNER_COLOR_BK_DEFAULT ( RGB(255, 255, 255) )
#define YBVIEWINNER_FLAG_NONE (0x00000000)
#define YBVIEWINNER_FLAG_DISABLE_REDRAW (0x00000001)
USING_NAMESPACE_YBCTRL;
//
YBViewInner::YBViewInner( YBViewContentBase* pViewContent )
: m_dwFlag( YBVIEWINNER_FLAG_NONE )
, m_clrBK( YBVIEWINNER_COLOR_BK_DEFAULT )
, m_pViewContent( pViewContent )
, m_nXPosContentStart( 0 )
, m_nYPosContentStart( 0 )
, m_wCtrlId( 0 )
, m_pScrollWnd( NULL )
{

}

YBViewInner::~YBViewInner() {
	if ( m_pViewContent ) {
		delete m_pViewContent;
		m_pViewContent = NULL;
	}
}

YBCTRL_WNDMSGMAP_BEGIN( YBViewInner, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_MOUSEWHEEL, _onWM_MOUSEWHEEL )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
YBCTRL_WNDMSGMAP_END()

HWND YBViewInner::create( int x, int y, int nWidth, int nHeight, HWND hWndParent, WORD wCtrlID, HINSTANCE hInstance ) {
	DWORD dwExStyle;
	DWORD dwStyle;

	dwExStyle = WS_EX_RIGHTSCROLLBAR;
	dwStyle = WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_CHILD|WS_VISIBLE;
	m_wCtrlId = wCtrlID;
	return _createHWND( dwExStyle, dwStyle, x, y, nWidth, nHeight, hWndParent, (HMENU)wCtrlID, hInstance, NULL );
}

void YBViewInner::setBkClr( COLORREF clrBK ) {
	m_clrBK = clrBK;
}

void YBViewInner::_onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	HDC hPaintDC;
	PAINTSTRUCT ps;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top ), memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void YBViewInner::_onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	RECT rcClient;
	//
	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = 1;
	//
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	::BitBlt( hDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top ), memDC.getMemDC(), 0, 0, SRCCOPY );
}

void YBViewInner::_onWM_MOUSEMOVE( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	::SetFocus( m_hWnd );

}

void YBViewInner::_onWM_MOUSEWHEEL( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEWHEEL );
	__int16 i16WheelDelta;
 
	i16WheelDelta = HIWORD( pYBCtrlWndMsg->m_wParam );
 	switch( i16WheelDelta ){
	case 120:
		{
			WPARAM wParam = (WPARAM)MAKELONG( m_wCtrlId, YBSB_CMDEVT_UPROLL );
			LPARAM lParam = (LPARAM)m_hWnd;
			HWND Hh = ::GetParent( m_hWnd );
			::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
			//::PostMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
		}
		break;
	case -120:
		{
			WPARAM wParam = (WPARAM)MAKELONG( m_wCtrlId, YBSB_CMDEVT_DOWNROLL );
			LPARAM lParam = (LPARAM)m_hWnd;
			::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
			//::PostMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
		}
		break;
	default:
		break;
 	}

}

void YBViewInner::_drawStatus( HDC hDC ) {
	if ( !enableRedraw() )
		return;
	//
	_drawBK( hDC );
	_drawContent( hDC );
}

void YBViewInner::_drawBK( HDC hDC ) {
	assert( ::IsWindow( m_hWnd ) );
	RECT rcClient;
	HBRUSH hBrushBK;

	::GetClientRect( m_hWnd, &rcClient );
	hBrushBK = ::CreateSolidBrush( m_clrBK );
	::FillRect( hDC, &rcClient, hBrushBK );
	::DeleteObject( hBrushBK );
}

void YBViewInner::_drawContent( HDC hDC ) {
	if ( m_pViewContent ) {
		m_pViewContent->_drawStatus( hDC, this );
	}
}

bool YBViewInner::PointContent2View( const POINT& ptInC, POINT& ptInV ) const {
	ptInV.x = ptInC.x - m_nXPosContentStart;
	ptInV.y = ptInC.y - m_nYPosContentStart;
	return true;
}

bool YBViewInner::PointView2Content( const POINT& ptInV, POINT& ptInC ) const {
	ptInC.x = ptInV.x + m_nXPosContentStart;
	ptInC.y = ptInV.y + m_nYPosContentStart;
	return true;
}

bool YBViewInner::getRectContent2View( const RECT& rcInContent, RECT& rcInView ) {
	return _getRectContent2View( rcInContent, rcInView );
}

bool YBViewInner::getRectView2Content( const RECT& rcInView, RECT& rcInContent ) {
	return _getRectView2Content( rcInView, rcInContent );
}

bool YBViewInner::_getRectContent2View( const RECT& rcInContent, RECT& rcInView ) {
	rcInView = rcInContent;
	::OffsetRect( &rcInView, -m_nXPosContentStart, -m_nYPosContentStart );
	return true;
}

bool YBViewInner::_getRectView2Content( const RECT& rcInView, RECT& rcInContent ) {
	rcInContent = rcInView;
	::OffsetRect( &rcInContent, m_nXPosContentStart, m_nYPosContentStart );
	return true;
}

bool YBViewInner::getContentUpdateRect( RECT& rcDestInContent, RECT& rcDestInView ) {
	if ( !m_pViewContent || !::IsWindow( m_hWnd ) )
		return false;
	RECT rcCInC; // Content 在Content坐标系下的坐标
	RECT rcCInV; // Content 在View坐标系下的坐标
	RECT rcVInV; // View 在View坐标系下的坐标
	RECT rcVInC; // View 在Content坐标系下的坐标
	RECT rcDInC; // Dest 在Content坐标系下的坐标
	RECT rcDInV; // Dest 在View坐标系下的坐标

	::SetRect( &rcCInC, 0, 0, m_pViewContent->getWidth(), m_pViewContent->getHeight() );
	if ( !_getRectContent2View( rcCInC, rcCInV ) )
		return false;
	::GetClientRect( m_hWnd, &rcVInV );
	if ( !_getRectView2Content( rcVInV, rcVInC ) )
		return false;
	//
	if ( !::IntersectRect( &rcDInC, &rcCInC, &rcVInC ) )
		return false;
	rcDestInContent = rcDInC;
	if ( !::IntersectRect( &rcDInV, &rcCInV, &rcVInV ) )
		return false;
	rcDestInView = rcDInV;

	return true;
}

void YBViewInner::setContentXPosLT( int nXPosLT ) {
	m_nXPosContentStart = nXPosLT;
}

void YBViewInner::setContentYPosLT( int nYPosLT ) {
	m_nYPosContentStart = nYPosLT;
}

void YBViewInner::enableRedraw( bool bEnable ) {
	if ( bEnable ) {
		m_dwFlag &= ~YBVIEWINNER_FLAG_DISABLE_REDRAW;
		if ( ::IsWindow( m_hWnd ) ) {
			::InvalidateRect( m_hWnd, NULL, TRUE );
		}
	} else {
		m_dwFlag |= YBVIEWINNER_FLAG_DISABLE_REDRAW;
	}
}

bool YBViewInner::enableRedraw() {
	return (m_dwFlag & YBVIEWINNER_FLAG_DISABLE_REDRAW) != YBVIEWINNER_FLAG_DISABLE_REDRAW;
}

void YBViewInner::_setScrollWnd( YBScrollWnd* pScrollWnd ) {
	m_pScrollWnd = pScrollWnd;
}