#include "stdafx.h"
#include "YBFSMenuHeader.h"
#include "frame/YBCtrlDefs.h"
#include "frame/YBCtrlImageBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlMisc.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlFontDepository.h"
#include <assert.h>

USING_NAMESPACE_YBCTRL

YBFSMenuHeader::YBFSMenuHeader()
: m_uImageIdBk( YBCTRL_IMAGE_ID_INVALID )
, m_uFontIdTxt( YBCTRL_FONT_ID_INVALID )
, m_tstrFront( _T("序号") )
, m_tstrMiddle( _T("本地系统名称") )
, m_tstrLast( _T("点击") )
{
	m_hPen = ::CreatePen( PS_SOLID, 1, RGB( 111, 191, 241 ) );
}

YBFSMenuHeader::~YBFSMenuHeader() {
	if ( m_hPen ) {
		::DeleteObject( m_hPen );
		m_hPen = NULL;
	}
}

YBCTRL_WNDMSGMAP_BEGIN( YBFSMenuHeader, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
YBCTRL_WNDMSGMAP_END()

void YBFSMenuHeader::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	RECT rcClient;
	PAINTSTRUCT ps;
	HDC hPaintDC;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

HWND YBFSMenuHeader::create( int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance ) {
	HWND hWnd;
	DWORD dwExStyle = 0;
	DWORD dwStyle = WS_VISIBLE|WS_CHILD;

	hWnd = _createHWND( dwExStyle, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, NULL );

	if ( !hWnd ){
		assert(false);
		return NULL;
	}
	return hWnd;
}

ybctrl::YBCtrlImageStatic* YBFSMenuHeader::_getImageStatic( unsigned int uImageId ) const {
	YBCtrlImageBase* pImageBase = NULL;
	YBCtrlImageStatic* pImageStatic = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pImageBase ) )
		return NULL;
	if( !pImageBase || ( pImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pImageBase;
}

void YBFSMenuHeader::_drawStatus( HDC hDC ) {
	_drawBk( hDC );
	_drawFrontTxt( hDC );
	_drawMiddleTxt( hDC );
	_drawLastTxt( hDC );
	_drawLine( hDC );
}

void YBFSMenuHeader::_drawBk( HDC hDC ) {
	RECT rcClient;
	ybctrl::YBCtrlImageStatic* pYBImage = NULL;

	::GetClientRect( m_hWnd, &rcClient );
	pYBImage = _getImageStatic( m_uImageIdBk );

	BackGroundBitBlt( hDC, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pYBImage->getMemDC(),
		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0 );
}

void YBFSMenuHeader::_drawFrontTxt( HDC hDC ) {
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcFrontTxt;

	if ( !_getFrontTxtRect( rcFrontTxt ) )
		return;

	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdTxt );
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

		::DrawText( hDC, m_tstrFront.c_str(), m_tstrFront.length(), &rcFrontTxt, DT_CENTER|DT_VCENTER );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

void YBFSMenuHeader::_drawMiddleTxt( HDC hDC ) {
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcMiddleTxt;

	if ( !_getMiddleTxtRect( rcMiddleTxt ) )
		return;

	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdTxt );
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

		::DrawText( hDC, m_tstrMiddle.c_str(), m_tstrMiddle.length(), &rcMiddleTxt, DT_CENTER|DT_VCENTER );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

void YBFSMenuHeader::_drawLastTxt( HDC hDC ) {
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcLastTxt;

	if ( !_getLastTxtRect( rcLastTxt ) )
		return;

	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdTxt );
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

		::DrawText( hDC, m_tstrLast.c_str(), m_tstrLast.length(), &rcLastTxt, DT_CENTER|DT_VCENTER );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

bool YBFSMenuHeader::_getFrontTxtRect( RECT& rcFrontTxt ) {
	RECT rcClient;
	unsigned int uWidthHeader;

	::GetClientRect( m_hWnd, &rcClient );
	uWidthHeader = rcClient.right - rcClient.left;

	rcFrontTxt.left = rcClient.left;
	rcFrontTxt.right = rcClient.left + uWidthHeader/5;
	rcFrontTxt.top = rcClient.top + 4;
	rcFrontTxt.bottom = rcClient.bottom;

	return ( ( rcFrontTxt.right > rcFrontTxt.left ) && ( rcFrontTxt.bottom > rcFrontTxt.top ) );
}

bool YBFSMenuHeader::_getMiddleTxtRect( RECT& rcMiddleTxt ) {
	RECT rcClient;
	unsigned int uWidthHeader;

	::GetClientRect( m_hWnd, &rcClient );
	uWidthHeader = rcClient.right - rcClient.left;

	rcMiddleTxt.left = rcClient.left + uWidthHeader/5;
	rcMiddleTxt.right = rcClient.left + uWidthHeader*4/5;
	rcMiddleTxt.top = rcClient.top + 4;
	rcMiddleTxt.bottom = rcClient.bottom;

	return ( ( rcMiddleTxt.right > rcMiddleTxt.left ) && ( rcMiddleTxt.bottom > rcMiddleTxt.top ) );
}

bool YBFSMenuHeader::_getLastTxtRect( RECT& rcLastTxt ) {
	RECT rcClient;
	unsigned int uWidthHeader;

	::GetClientRect( m_hWnd, &rcClient );
	uWidthHeader = rcClient.right - rcClient.left;

	rcLastTxt.left = rcClient.left + uWidthHeader*4/5;
	rcLastTxt.right = rcClient.right;
	rcLastTxt.top = rcClient.top + 4;
	rcLastTxt.bottom = rcClient.bottom;

	return ( ( rcLastTxt.right > rcLastTxt.left ) && ( rcLastTxt.bottom > rcLastTxt.top ) );
}

void YBFSMenuHeader::_drawLine( HDC hDC ) {
	RECT rcClient;
	HPEN hPenOld;

	::GetClientRect( m_hWnd, &rcClient );
	hPenOld = (HPEN)::SelectObject( hDC, m_hPen );
	::MoveToEx( hDC, ( rcClient.right - rcClient.left )/5, rcClient.top, NULL );
	::LineTo( hDC, ( rcClient.right - rcClient.left )/5, rcClient.bottom );
	//
	::MoveToEx( hDC, ( rcClient.right - rcClient.left )*4/5, rcClient.top, NULL );
	::LineTo( hDC, ( rcClient.right - rcClient.left )*4/5, rcClient.bottom );

	::SelectObject( hDC, hPenOld );
}

void YBFSMenuHeader::setHeaderImageId( unsigned int uImageHeaderId ) {
	m_uImageIdBk = uImageHeaderId;
}

void YBFSMenuHeader::setFontId( unsigned int uFontId ) {
	m_uFontIdTxt = uFontId;
}