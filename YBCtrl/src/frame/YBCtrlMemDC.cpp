#include "stdafx.h"
#include "frame\YBCtrlMemDC.h"
#include <assert.h>
USING_NAMESPACE_YBCTRL
YBCtrlMemDC::YBCtrlMemDC( unsigned int uWidth, unsigned int uHeight )
: m_hMemDC( NULL )
, m_hMemBitmap( NULL )
, m_hMemBitmapOld( NULL ) {
	_init( uWidth, uHeight );
}

YBCtrlMemDC::~YBCtrlMemDC() {
	_unInit();
}

void YBCtrlMemDC::resize( unsigned int uWidth, unsigned int uHeight ) {
	if ( !m_hMemDC || !m_hMemBitmap ) {
		assert( false );
		return;
	}
	if ( ( uWidth == m_uWidth ) && ( uHeight == m_uHeight ) ) {
		return;
	}
	HWND hWndDesktop;
	HDC hDCReal;
	HBITMAP hMemBitmapNew;
	HBITMAP hMemBitmapOld;

	hWndDesktop = ::GetDesktopWindow();
	hDCReal = ::GetDC( hWndDesktop );
	hMemBitmapNew = ::CreateCompatibleBitmap( hDCReal, uWidth, uHeight );
	hMemBitmapOld = (HBITMAP)::SelectObject( m_hMemDC, hMemBitmapNew );
	assert( hMemBitmapOld == m_hMemBitmap );
	::DeleteObject( hMemBitmapOld );
	m_hMemBitmap = hMemBitmapNew;
	::ReleaseDC( hWndDesktop, hDCReal );
	//
	m_uWidth = uWidth;
	m_uHeight = uHeight;
}

void YBCtrlMemDC::_init( unsigned int uWidth, unsigned int uHeight ) {
	if ( m_hMemDC || m_hMemBitmap ) {
		assert( false );
		return;
	}
	HWND hWndDesktop;
	HDC hDCReal;

	hWndDesktop = ::GetDesktopWindow();
	hDCReal = ::GetDC( hWndDesktop );
	//
	m_hMemDC = ::CreateCompatibleDC( hDCReal );
	m_hMemBitmap = ::CreateCompatibleBitmap( hDCReal, uWidth, uHeight );
	m_hMemBitmapOld = (HBITMAP)::SelectObject( m_hMemDC, m_hMemBitmap );
	//
	::ReleaseDC( hWndDesktop, hDCReal );
	//
	m_uWidth = uWidth;
	m_uHeight = uHeight;
}

void YBCtrlMemDC::_unInit() {
	if ( !m_hMemDC || !m_hMemBitmap ) {
		assert( false );
		return;
	}
	HBITMAP hBitmapTmp;

	hBitmapTmp = (HBITMAP)::SelectObject( m_hMemDC, m_hMemBitmapOld );
	//assert( hBitmapTmp == m_hMemBitmap );
	::DeleteObject( m_hMemBitmap );
	m_hMemBitmap = NULL;
	hBitmapTmp = NULL;
	//
	::DeleteDC( m_hMemDC );
	m_hMemDC = NULL;
}
