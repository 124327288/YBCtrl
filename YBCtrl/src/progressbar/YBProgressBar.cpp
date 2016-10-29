#include "stdafx.h"
#include "progressbar/YBProgressBar.h"
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlImageBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlFontDepository.h"
#include "frame/YBCtrlMisc.h"
#include "gdiplus.h"
#include <assert.h>

#define YBPB_DRAW_FT_NONE (0x00000000)
#define YBPB_DRAW_FT_ROLL (0x00000001)

USING_NAMESPACE_YBCTRL

YBProgressBar::YBProgressBar()
: m_uImageIdBK( YBCTRL_IMAGE_ID_INVALID )
, m_uLFixed( 5 )
, m_uRFixed( 5 )
, m_uTFixed( 0 )
, m_uBFixed( 0 )
, m_nMaxPos( 100 )
, m_nMinPos( 0 )
, m_nCurPos( 0 )
, m_uLSpaced( 0 )
, m_uTSpaced( 0 )
, m_uBSpaced( 0 )
, m_uRSpaced( 0 )
, m_uTxtSpaceT( 5 )
, m_uTxtSpaceB( 5 )
, m_uFontIdTxt( YBCTRL_FONT_ID_INVALID )
, m_uFlag( YBPB_DRAW_FT_NONE )
{

}

YBProgressBar::~YBProgressBar(){

}

YBCTRL_WNDMSGMAP_BEGIN( YBProgressBar, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
YBCTRL_WNDMSGMAP_END()

void YBProgressBar::_onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg ) {
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

	//
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
}

void YBProgressBar::_onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	HDC hPaintDC;
	RECT rcClient;
	PAINTSTRUCT ps;
	hPaintDC = ::BeginPaint( m_hWnd, &ps );

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	::BitBlt( hPaintDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top ), memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void YBProgressBar::_drawStatus( HDC hDC ) {
	_drawBk( hDC );
	_drawFT( hDC );
	_drawText( hDC );
}

void YBProgressBar::_drawFT( HDC hDC ) {
	if ( ( m_uFlag & YBPB_DRAW_FT_ROLL ) != YBPB_DRAW_FT_ROLL ) {
		_drawStaticFT( hDC );
	} else {
		_drawRollFT( hDC );
	}
}

void YBProgressBar::setImageID( unsigned int uImageIdBK, unsigned int uImageIdProBar ){
	m_uImageIdBK = uImageIdBK;
	m_uImageIdFT = uImageIdProBar;

}

HWND YBProgressBar::Create( DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance ){
	HWND hWnd;

	dwStyle |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	hWnd = _createHWND( dwExStyle, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, NULL );
	if( !hWnd ){
		assert(false);
	}

	return hWnd;
}

void YBProgressBar::_drawStaticFT( HDC hDC ){
	if( !getHWND() )
		return;
	RECT rcClient;
	int uBarCurWidth;
	int uWidthFTShow;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlImageStatic* pYBImageProBarFT = NULL;
	pYBImageProBarFT = _getImageStatic( m_uImageIdFT );
	if ( !pYBImageProBarFT ) {
		return;
	}
	//
	uBarCurWidth = (int)_calcCurFTWidth( pYBImageProBarFT->getWidth() );
	if ( pYBImageProBarFT->getWidth() == 0 ) {
		return;
	}
	uWidthFTShow = ( rcClient.right - rcClient.left - m_uLSpaced - m_uRSpaced ) * uBarCurWidth / pYBImageProBarFT->getWidth();

	Gdiplus::Graphics graphics( hDC );
	ybctrl::GdipStretchBlt( &graphics, pYBImageProBarFT->getImage(), m_uLSpaced, m_uTSpaced, uWidthFTShow, ( rcClient.bottom - rcClient.top - m_uTSpaced - m_uBSpaced )
		, 0, 0, uBarCurWidth, pYBImageProBarFT->getImage()->GetHeight() );
}

void YBProgressBar::_drawRollFT( HDC hDC ){
	if( !getHWND() )
		return;
	RECT rcClient;
	int uBarCurWidth;
	int uWidthFTShow;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlImageStatic* pYBImageProBarFT = NULL;
	pYBImageProBarFT = _getImageStatic( m_uImageIdFT );
	if ( !pYBImageProBarFT ) {
		return;
	}
	assert( ( rcClient.right - rcClient.left - m_uLSpaced - m_uRSpaced ) <= pYBImageProBarFT->getWidth() );
	
	//
	uBarCurWidth = (int)_calcCurFTWidth( pYBImageProBarFT->getWidth() );
	if ( pYBImageProBarFT->getWidth() == 0 ) {
		return;
	}
	uWidthFTShow = ( rcClient.right - rcClient.left - m_uLSpaced - m_uRSpaced ) * uBarCurWidth / pYBImageProBarFT->getWidth();

	Gdiplus::Graphics graphics( hDC );
	ybctrl::GdipStretchBlt( &graphics, pYBImageProBarFT->getImage(), m_uLSpaced, m_uTSpaced, uWidthFTShow, ( rcClient.bottom - rcClient.top - m_uTSpaced - m_uBSpaced )
		, pYBImageProBarFT->getImage()->GetWidth() - uWidthFTShow, 0, uWidthFTShow, pYBImageProBarFT->getImage()->GetHeight() );
}

__int64 YBProgressBar::_calcCurFTWidth( __int64 uFTImageWidth ) {
	if ( ( m_nMaxPos < m_nMinPos )
	|| ( m_nCurPos < m_nMinPos )
	|| ( m_nCurPos > m_nMaxPos ) ) {
		assert( false );
		return 0;
	}
	if ( ( m_nMaxPos == m_nMinPos ) ) {
		return 0;
	}
	return ( m_nCurPos - m_nMinPos ) * uFTImageWidth / ( m_nMaxPos - m_nMinPos );
}

void YBProgressBar::getRange( __int64& nMinPos, __int64& nMaxPos ) const {
	nMinPos = m_nMinPos;
	nMaxPos = m_nMaxPos;
}

void YBProgressBar::setPos( __int64 nPos ){
	if ( ( nPos < m_nMinPos ) || ( nPos > m_nMaxPos ) ) {
		assert( false );
		return;
	}
	m_nCurPos = nPos;
	if ( ( m_nCurPos >= m_nMinPos ) && ( m_nCurPos <= m_nMaxPos ) ) {
		if ( ::IsWindow( m_hWnd ) && ::IsWindowVisible( m_hWnd ) ) {
			HDC hDC;
			RECT rcClient;

			::GetClientRect( m_hWnd, &rcClient );
			YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
			_drawStatus( memDC.getMemDC() );

			hDC = ::GetDC( m_hWnd );
			::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 
				memDC.getMemDC(), 0, 0, SRCCOPY );
			::ReleaseDC( m_hWnd, hDC );
		}
	}
	return;
}

void YBProgressBar::setRange( __int64 nMinPos, __int64 nMaxPos ){
	m_nMinPos = nMinPos;
	m_nMaxPos = nMaxPos;
}

__int64 YBProgressBar::getPos() const {
	return m_nCurPos;
}

void YBProgressBar::SetProgressBarSpaced( unsigned int uLSpaced, unsigned int uTSpaced, unsigned int uRSpaced, unsigned int uBSpaced ){
	m_uLSpaced = uLSpaced;
	m_uTSpaced = uTSpaced;
	m_uRSpaced = uRSpaced;
	m_uBSpaced = uBSpaced;
}

YBCtrlImageStatic* YBProgressBar::_getImageStatic( unsigned int uImageId ) {
	YBCtrlImageBase* pYBImageBase = NULL;
	YBCtrlImageStatic* pImageStatic = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pYBImageBase ) )
		return NULL;

	if( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

void YBProgressBar::_drawBk( HDC hDC ){
	if( !getHWND() )
		return;
	RECT rcClient;
	YBCtrlImageStatic* pYBImage = NULL;
	pYBImage = _getImageStatic( m_uImageIdBK );
	//
	if ( !pYBImage ) {
		return;
	}

	::GetClientRect( m_hWnd, &rcClient );

	BackGroundBitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pYBImage->getMemDC(), 0, 0,
		pYBImage->getWidth(), pYBImage->getHeight(), m_uLFixed, m_uTFixed, m_uRFixed, m_uBFixed );
}

bool YBProgressBar::_getDrawText( RECT& rcDrawText ){
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );

	rcDrawText.left = rcClient.left;
	rcDrawText.right = rcClient.right;
	rcDrawText.bottom = rcClient.bottom - m_uTxtSpaceB;
	rcDrawText.top = rcClient.top + m_uTxtSpaceT;

	return ( rcDrawText.right > rcDrawText.left ) && ( rcDrawText.bottom > rcDrawText.top );
}

void YBProgressBar::_getCurText( tstring_type& tstrCurText ) {
	float fPercent = 0;
	__int64 i64CalcPos;
	TCHAR szText[ 100 ];

	if ( m_nCurPos < m_nMinPos ) {
		i64CalcPos = m_nMinPos;
	} else if ( m_nCurPos > m_nMaxPos ) {
		i64CalcPos = m_nMaxPos;
	} else {
		i64CalcPos = m_nCurPos;
	}

	if ( m_nMinPos == m_nMaxPos ) {
		fPercent = 0;
	} else {
		fPercent = (float)( (int)( ( i64CalcPos - m_nMinPos ) * 1000 / ( m_nMaxPos - m_nMinPos ) ) ) / 10;
		//fPercent = (float)( (float)( ( i64CalcPos - m_nMinPos ) * 100 ) / (float)( m_nMaxPos - m_nMinPos ) );
	}
	_stprintf_s( szText, _T("%0.1f%%"), fPercent );
	tstrCurText = szText;
}

void YBProgressBar::_drawText( HDC hDC ){
	if( !getHWND() )
		return;

	RECT rcDrawText;
	HFONT hFontOld = NULL;
	int iBkModeOld;
	COLORREF clrTxtOld;
	tstring_type tstrPercent;

	_getCurText( tstrPercent );
	const YBCtrlFont* pYBFont = NULL;
	if( _getDrawText( rcDrawText ) ){
		pYBFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdTxt );
		if( pYBFont ){
			hFontOld = (HFONT)::SelectObject( hDC, pYBFont->m_hFont );
			iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
			clrTxtOld = ::SetTextColor( hDC, pYBFont->m_clrTxt );

			::DrawText( hDC, tstrPercent.c_str(), tstrPercent.length(), &rcDrawText, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );

			::SetTextColor( hDC, clrTxtOld );
			::SetBkMode( hDC, iBkModeOld );
			::SelectObject( hDC, hFontOld );
		}
	}

}

void YBProgressBar::setTextAttri( unsigned int uFontId, unsigned int uTxtSpaceT, unsigned int uTxtSpaceB ){
	m_uFontIdTxt = uFontId;
	m_uTxtSpaceB = uTxtSpaceB;
	m_uTxtSpaceT = uTxtSpaceT;
}

void YBProgressBar::getCurText( tstring_type& tstrCurText ) {
	_getCurText( tstrCurText );
}

void YBProgressBar::setBitBltFTMode( bool bRoll ) {
	if ( bRoll ) {
		m_uFlag |= YBPB_DRAW_FT_ROLL;
	} else {
		m_uFlag &= ~YBPB_DRAW_FT_ROLL;
	}
}