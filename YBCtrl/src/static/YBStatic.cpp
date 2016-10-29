#include "stdafx.h"
#include "static/YBStatic.h"
#include "frame/YBCtrlImageBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlImageDepository.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlFontDepository.h"
#include "frame/YBCtrlMisc.h"
#include "gdiplus.h"
#include  <assert.h>

#define DT_YBSTATIC_FMT (DT_SINGLELINE|DT_LEFT)

USING_NAMESPACE_YBCTRL

YBStatic::YBStatic()
: m_uImageId(YBCTRL_IMAGE_ID_INVALID)
, m_uFontId(YBCTRL_FONT_ID_INVALID)
, m_tstrText(_T(""))
, m_uLineHeight( 12 )
, m_uLineSpace( 0 ) {

}

YBStatic::~YBStatic() {

}

YBCTRL_WNDMSGMAP_BEGIN( YBStatic, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
YBCTRL_WNDMSGMAP_END()

void YBStatic::_onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	HDC hPaintDC;
	RECT rcClient;
	PAINTSTRUCT ps;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawBk( memDC.getMemDC() );
	_drawText( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );

}

void YBStatic::_onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = 1;

	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	RECT rcClient;

	::GetClientRect( pYBCtrlWndMsg->m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawBk( memDC.getMemDC() );
	_drawText( memDC.getMemDC() );
	//
	::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), ( rcClient.bottom - rcClient.top ),
		memDC.getMemDC(), 0, 0, SRCCOPY );

}

HWND YBStatic::Create( DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance ){
	HWND hWnd;

	hWnd = _createHWND( 0, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, NULL );
	if( !hWnd ){
		assert(false);
	}
	//
	_calcAllCharWidth();
	//
	return hWnd;
}

YBCtrlImageBase* YBStatic::_getImageBase( unsigned int uImageId ){
	YBCtrlImageBase* pYBImage = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( m_uImageId, &pYBImage ) )
		return NULL;

	return pYBImage;

}

YBCtrlImageStatic* YBStatic::_getImageStatic( unsigned int uImageId ){
	YBCtrlImageBase* pYBImageBase = NULL;
	pYBImageBase = _getImageBase( uImageId );

	if ( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;

}

void YBStatic::setImage( unsigned int uImageId ){
	m_uImageId = uImageId;

}

void YBStatic::setFontId( unsigned int uFontId ){
	
	m_uFontId = uFontId;

	_calcLineHeight();
	 	//
 	_calcAllCharWidth();

}

void YBStatic::_drawBk( HDC hdc ){
	if( !getHWND() )
		return;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlImageStatic* pYBImage = NULL;
	pYBImage = _getImageStatic( m_uImageId );
	if ( !pYBImage ) {
		return;
	}

	BackGroundBitBlt( hdc, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		pYBImage->getMemDC(), 0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0 );

}

void YBStatic::setText( const tstring_type& tstrText ){
	m_tstrText = tstrText;
	_calcAllCharWidth();
}

void YBStatic::_calcLineHeight() {
	const ybctrl::YBCtrlFont* pYBCtrlFont = NULL;

	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontId );
	if ( !pYBCtrlFont ) {
		assert( false );
		return;
	}
	if ( pYBCtrlFont->m_logFont.lfHeight > 0 ) {
		m_uLineHeight = pYBCtrlFont->m_logFont.lfHeight;
	} else {
		m_uLineHeight = -(pYBCtrlFont->m_logFont.lfHeight);
	}

}

void YBStatic::_calcAllCharWidth() {
	m_containerCharWidth.clear();
	YBCtrlMemDC memDC( 100, 100 );
	unsigned int uLenStrData;
	TCHAR tchData;
	TCHAR tszData[ 2 ];
	RECT rcTextCalc;
	RECT rcClient;
	const YBCtrlFont* pYBCtrlFont = NULL;
	HFONT hFontOld;

	if ( !m_hWnd )
		return;
	::GetClientRect( m_hWnd, &rcClient );
 	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontId );
 	if( !pYBCtrlFont )
 		return ;
 	hFontOld = (HFONT)::SelectObject( memDC.getMemDC(), pYBCtrlFont->m_hFont );
	uLenStrData = m_tstrText.length();
	for ( unsigned int uIndex = 0; uIndex<uLenStrData; ++uIndex ) {
		tchData = m_tstrText[ uIndex ];
		tszData[ 0 ] = tchData;
		tszData[ 1 ] = 0;
		
		::SetRect( &rcTextCalc, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom );
		::DrawText( memDC.getMemDC(), tszData, 1, &rcTextCalc, DT_YBSTATIC_FMT|DT_CALCRECT );
		 m_containerCharWidth.push_back( rcTextCalc.right - rcTextCalc.left );
	}
	::SelectObject( memDC.getMemDC(), hFontOld );
	
	assert( m_tstrText.length() == m_containerCharWidth.size() );
}

void YBStatic::_drawText( HDC hDC ) {
	RECT rcCalc;
	RECT rcClient;
	unsigned int uWidth;
	unsigned int uCharIndex = 0;
	unsigned int uCharCountCalc;
	unsigned int uLineIndex;
	unsigned int uLineTop;
	tstring_type tstrLineData;
	const YBCtrlFont* pYBCtrlFont = NULL;
	HFONT hFontOld;

	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontId );
	if( !pYBCtrlFont )
		return ;
	hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
	::GetClientRect( m_hWnd, &rcClient );
	uWidth = rcClient.right - rcClient.left;
	uLineTop = 0;
	uCharIndex = 0;
	uLineIndex = 0;
	int iBkModeOld;
	COLORREF clrTxtOld;

	iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
	clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
	while ( uCharIndex < m_tstrText.length() ) {
		if ( _calcCharCount( uCharIndex, uWidth, uCharCountCalc ) ) {
			//
			if ( uLineIndex == 0 ) {
				uLineTop = uLineIndex*m_uLineHeight;
			} else {
				uLineTop = uLineIndex*m_uLineHeight + uLineIndex*m_uLineSpace;
			}
			::SetRect( &rcCalc, 0, uLineTop, uWidth, uLineTop + m_uLineHeight );
			//
			tstrLineData = m_tstrText.substr( uCharIndex, uCharCountCalc );
			//
			::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_YBSTATIC_FMT );
			//
			uCharIndex += uCharCountCalc;
			++uLineIndex;
		} else {
			assert( false );
			break;
		}
	}
	::SetTextColor( hDC, clrTxtOld );
	::SetBkMode( hDC, iBkModeOld );
	::SelectObject( hDC, hFontOld );
}

bool YBStatic::_calcCharCount( unsigned int uCharIndexBegin, unsigned int uWidth, unsigned int& uCharCountCalc ) {
	assert( m_tstrText.length() == m_containerCharWidth.size() );
	if ( uCharIndexBegin >= m_containerCharWidth.size() ) {
		assert ( false );
		return false;
	}
	unsigned int uWidthSum = 0;
	uWidthSum = 0;
	uCharCountCalc = 0;
	for ( unsigned int uIndex = uCharIndexBegin; uIndex < m_containerCharWidth.size(); ++uIndex ) {
		uWidthSum += m_containerCharWidth[ uIndex ];
		if ( uWidthSum > uWidth ) {
			break;
		}
		uCharCountCalc++;
	}
	return true;
}

void YBStatic::setLineSpace( unsigned int uLineSpace ) {
	m_uLineSpace = uLineSpace;
}