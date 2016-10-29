#include "stdafx.h"
#include "YBTransparentWnd.h"
#include "frame/YBCtrlMemDC.h"
#include <assert.h>

USING_NAMESPACE_YBCTRL

YBTransparentWnd::YBTransparentWnd() {
	m_hBrushBk = ::CreateSolidBrush( RGB( 0, 0, 0 ) );
}

YBTransparentWnd::~YBTransparentWnd() {
	if ( m_hBrushBk ) {
		::DeleteObject( m_hBrushBk );
		m_hBrushBk = NULL;
	}
}

HWND YBTransparentWnd::create( unsigned int x, unsigned int y, int nWidth, int nHeight ) {
	HWND hWnd;
	//DWORD dwExStyle = WS_EX_TOPMOST|WS_EX_TOOLWINDOW|WS_EX_LAYERED|WS_EX_TRANSPARENT; // 透明且能处理窗口后面的消息.
	DWORD dwExStyle = WS_EX_TOPMOST|WS_EX_TOOLWINDOW|WS_EX_LAYERED;				 // 透明且不能触发窗口后面的消息.(窗口会进行拉伸)
	DWORD dwStyle = WS_VISIBLE|WS_CLIPSIBLINGS|WS_POPUP|WS_CLIPCHILDREN;

	hWnd = _createHWND( dwExStyle, dwStyle, x, y, nWidth, nHeight, NULL, (HMENU)NULL, NULL, NULL );
	if ( !hWnd ) {
		assert( false );
		return NULL;
	}
	_setTransparent( x, y );

	return hWnd;
}

void YBTransparentWnd::_drawBk( HDC hDC ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	::FillRect( hDC, &rcClient, m_hBrushBk );
}

void YBTransparentWnd::_setTransparent( unsigned int uPtDstX, unsigned int uPtDstY ) {
	HDC hDC;
	RECT rcClient;
	::GetClientRect( m_hWnd, &rcClient );
	POINT ptSrc = {0, 0};
	POINT ptDst = {uPtDstX, uPtDstY};

	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawBk( memDC.getMemDC() );

	hDC = ::GetDC( m_hWnd );
	//
	SIZE wndSize = { rcClient.right - rcClient.left, rcClient.bottom - rcClient.top };
	BLENDFUNCTION blendFunction;
	//blendFunction.AlphaFormat = AC_SRC_ALPHA;
	blendFunction.AlphaFormat = AC_SRC_OVER;
	blendFunction.BlendFlags = 0;
	blendFunction.BlendOp = AC_SRC_OVER;
	blendFunction.SourceConstantAlpha = 100;
	UpdateLayeredWindow( m_hWnd, hDC, &ptDst, &wndSize, memDC.getMemDC(), &ptSrc, 0, &blendFunction, ULW_ALPHA );
	//
	::ReleaseDC( m_hWnd, hDC );
}