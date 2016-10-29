#include "stdafx.h"
#include "tooltip/YBToolTip.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlImageBase.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlFontDepository.h"
#include "frame/YBCtrlMisc.h"
#include "frame/YBCtrlMemDC.h"
#include <assert.h>
#include "loader/YBImageLoader.h"

#define YBTOOLTIP_TIMERID_AUTOEXIT (0x00000300)
#define YBTOOLTIP_TIME_SLEEP (400)

USING_NAMESPACE_YBCTRL

YBToolTip::YBToolTip() 
: m_uLineHeight( 0 )
, m_uHeight( 0 )
, m_uExitTimeMS( 4000 )
, m_pMemDC( new YBCtrlMemDC( 100, 100 ) )
, m_uWidth( 0 )
, m_uFontId( YBCTRL_FONT_ID_INVALID )
, m_uImageBk( YBCTRL_IMAGE_ID_INVALID )
, m_tstrText( _T("") )
, m_uCharWidth( 0 )
{
	//m_hPen = ::CreatePen( PS_SOLID, 1, RGB(100, 100, 100) );
	//m_hBrushBk = ::CreateSolidBrush( RGB(255, 255, 225) );
	m_hPen = ::CreatePen( PS_SOLID, 1, RGB(118, 118, 118) );
	m_hBrushBk = ::CreateSolidBrush( RGB(247, 247, 250) );
	_loadFont();
	_createWnd();
}

YBToolTip* YBToolTip::getInstance() {
	static YBToolTip s_instance;
	return &s_instance;
}

YBToolTip::~YBToolTip() {
	if ( m_pMemDC ) {
		delete m_pMemDC;
		m_pMemDC = NULL;
	}
	if ( m_hPen ) {
		::DeleteObject( m_hPen );
		m_hPen = NULL;
	}
	if ( m_hBrushBk ) {
		::DeleteObject( m_hBrushBk );
		m_hBrushBk = NULL;
	}
	//
	m_containerCharWidth.clear();
}

void YBToolTip::_calcWndSpace( int pointx, int pointy, int& newpointx, int& newpointy ) const {
	int uRSpace;
	int uBSpace;
	int uArrowHeight = 20;
	int nScreenWidth;
	int nScreenHeight;
	
	nScreenWidth = ::GetSystemMetrics( SM_CXSCREEN );
	nScreenHeight = ::GetSystemMetrics( SM_CYSCREEN );
	uRSpace = nScreenWidth - pointx;
	uBSpace = nScreenHeight - pointy - uArrowHeight;
	
	if ( uRSpace <  (int)m_uWidth ) {
		newpointx = nScreenWidth - m_uWidth;
	} else {
		newpointx = pointx;
	}

	if ( uBSpace < (int)m_uHeight ) {
		newpointy = pointy - m_uHeight;
	} else {
		newpointy = pointy + uArrowHeight;
	}
}

void YBToolTip::_movewnd( int pointx, int pointy ) {
	int newpointx;
	int newpointy;
	m_uCharWidth = 0;
	
	if ( !_calcWidth() )
		return ;
	if ( !_calcHeight() )
		return ;
	//
	_calcWndSpace( pointx, pointy, newpointx, newpointy );

	::MoveWindow( m_hWnd, newpointx, newpointy, m_uWidth, m_uHeight, TRUE );
	//::Sleep( YBTOOLTIP_TIME_SLEEP );
	//::ShowWindow( m_hWnd, SW_NORMAL|SWP_NOACTIVATE );

	::SetWindowPos( m_hWnd, NULL, newpointx, newpointy, m_uWidth, m_uHeight, SWP_NOACTIVATE|SWP_NOSIZE|SWP_SHOWWINDOW );

	::SetTimer( m_hWnd, YBTOOLTIP_TIMERID_AUTOEXIT, m_uExitTimeMS, NULL );
}

void YBToolTip::hide() {
	if ( ::IsWindow( m_hWnd ) && ( ::IsWindowVisible( m_hWnd )) ) {
		//::Sleep( YBTOOLTIP_TIME_SLEEP );
		::ShowWindow( m_hWnd, SW_HIDE );
		::KillTimer( m_hWnd, YBTOOLTIP_TIMERID_AUTOEXIT );
	}
}

void YBToolTip::updateTipText( const tstring_type& tstrText, HWND hWndDest ) {
	assert( ::IsWindow( m_hWnd ) );
	if ( ::IsWindowVisible( m_hWnd ) ) {
		return;
	}
	if ( tstrText.length() == 0 ) {
		return;
	}
	if ( !::IsWindow( ::GetParent( hWndDest ) ) || !hWndDest || !::GetParent( hWndDest ) ) {
		int a = 0;
		return;
	}

	POINT ptMouse;
	m_tstrText = tstrText;
	//
	_calcAllCharWidth();
	//
	::GetCursorPos( &ptMouse );
	_movewnd( ptMouse.x, ptMouse.y );
}

HWND YBToolTip::_createWnd() {
	HWND hWnd;
	DWORD dwExStyle = WS_EX_TOPMOST|WS_EX_TOOLWINDOW|WS_EX_LTRREADING;
	DWORD dwStyle = WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	hWnd = _createHWND( dwExStyle, dwStyle, 0, 0, 100, 20, NULL, (HMENU)NULL, NULL, NULL );
	if ( !hWnd ) {
		assert( false );
	}

//	::SetWindowPos( hWnd, NULL, 0,0,0,0, SWP_NOACTIVATE|SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE );
	//
	::SetWindowPos( m_hWnd, NULL, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE );
	return hWnd;
}

YBCTRL_WNDMSGMAP_BEGIN( YBToolTip, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_TIMER, _onWM_TIMER )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
YBCTRL_WNDMSGMAP_END()

void YBToolTip::_onWM_MOUSEMOVE( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	RECT rcClient;
	POINT ptMouse;

	::GetClientRect( m_hWnd, &rcClient );
	ptMouse.x = LOWORD(pYBCtrlWndMsg->m_lParam);
	ptMouse.y = HIWORD(pYBCtrlWndMsg->m_lParam);
	if( ::PtInRect( &rcClient, ptMouse ) ) {
		hide();
	}
}

void YBToolTip::_onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	RECT rcClient;

	//
	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = 1;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	::BitBlt( hDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top )
		, memDC.getMemDC(), 0, 0, SRCCOPY );

}

void YBToolTip::_onWM_TIMER( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_TIMER );
	switch( pYBCtrlWndMsg->m_wParam ) {
	case YBTOOLTIP_TIMERID_AUTOEXIT:
		{
			::KillTimer( m_hWnd, YBTOOLTIP_TIMERID_AUTOEXIT );
			hide();
		}
		break;
	default:
		break;
	}
}

void YBToolTip::_onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	PAINTSTRUCT ps;
	HDC hPaintDC;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	m_pMemDC->resize( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( m_pMemDC->getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, m_pMemDC->getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void YBToolTip::_drawStatus( HDC hDC ) {
	_drawBk( hDC );
	_drawText( hDC );
}

YBCtrlImageStatic* YBToolTip::_getImageStatic( unsigned int uImageId ) {
	YBCtrlImageBase* pYBImage = NULL;

	if ( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pYBImage ) )
		return NULL;
	if ( !pYBImage || ( pYBImage->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImage;
}

void YBToolTip::_drawBk( HDC hDC ) {
	YBCtrlImageStatic* pYBImage = NULL;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	pYBImage = _getImageStatic( m_uImageBk );
	if ( !pYBImage ) {
		// default Bk
		_drawDefaultBk( hDC );
		return;
	}

	BackGroundBitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pYBImage->getMemDC(), 0, 0,
		pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0 );
}

void YBToolTip::_drawDefaultBk( HDC hDC ) {
	HPEN hPenOld = NULL;
	HBRUSH hBrushOld = NULL;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	hBrushOld = (HBRUSH)::SelectObject( hDC, m_hBrushBk );
	hPenOld = (HPEN)::SelectObject( hDC, m_hPen );
	//
	::FillRect( hDC, &rcClient, hBrushOld );
	//
	::MoveToEx( hDC, rcClient.left, rcClient.top, NULL );
	::LineTo( hDC, rcClient.right, rcClient.top );
	//
	::MoveToEx( hDC, rcClient.right - 1, rcClient.top, NULL );
	::LineTo( hDC, rcClient.right - 1, rcClient.bottom );
	//
	::MoveToEx( hDC, rcClient.left, rcClient.bottom - 1, NULL );
	::LineTo( hDC, rcClient.right, rcClient.bottom - 1 );
	//
	::MoveToEx( hDC, rcClient.left, rcClient.top, NULL );
	::LineTo( hDC, rcClient.left, rcClient.bottom );

	::SelectObject( hDC, hPenOld );
	::SelectObject( hDC, hBrushOld );
}

bool YBToolTip::_calcHeight() {
	const YBCtrlFont* pYBFont = NULL;

	pYBFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontId );
	if ( !pYBFont ) {
		assert( false );
		return false;
	}
	if ( pYBFont->m_logFont.lfHeight > 0 ) {
		m_uLineHeight = pYBFont->m_logFont.lfHeight;
	} else {
		m_uLineHeight = -( pYBFont->m_logFont.lfHeight );
	}
	//
	unsigned int nScreenWidth;
	unsigned int uCharIndex = 0;
	unsigned int uLineIndex = 0;
	unsigned int uCharCountCalc;
	unsigned int uLineTop;
	unsigned int uWidthPart;
	unsigned int uCountPart = m_containerBrCharIndex.size() + 1;

	tstring_type tstrLineData;
	nScreenWidth = ::GetSystemMetrics( SM_CXSCREEN );

	if ( m_containerBrCharIndex.size() == 0 ) {
		//
		if ( m_uCharWidth > nScreenWidth ) {
			while ( uCharIndex < m_tstrText.length() ) {
				if ( _calcCharCount( uCharIndex, nScreenWidth, uCharCountCalc ) ) {
					//
					uLineTop = uLineIndex*m_uLineHeight;
					tstrLineData = m_tstrText.substr( uCharIndex, uCharCountCalc );
					//
					uCharIndex += uCharCountCalc;
					++uLineIndex;
				} else {
					assert( false );
					break;
				}
				// 字体距离上下距离各位2像素
				m_uHeight = uLineIndex*( m_uLineHeight + 4 );
			}
		} else {
			m_uHeight = m_uLineHeight + 4;
		}
	} else {
		for ( unsigned int uIndexPart = 0; uIndexPart != uCountPart; ++uIndexPart ) {
			uWidthPart = _calcEveryPartWidth( uIndexPart );
			if ( uWidthPart > nScreenWidth ) {
				++uLineIndex;
			}
			++uLineIndex;
		}
		// 字体距离上下距离各位2像素
		m_uHeight = uLineIndex*( m_uLineHeight + 4 );
	}

	return true;
}

bool YBToolTip::_calcWidth() {
	if ( m_containerCharWidth.size() != m_tstrText.length() ) {
		assert( false );
		return false;
	}
	unsigned int nScreenWidth;

	nScreenWidth = ::GetSystemMetrics( SM_CXSCREEN );
	// 是否有换行符.
	if ( m_containerBrCharIndex.size() == 0 ) {
		for ( unsigned int uIndex = 0; uIndex < m_containerCharWidth.size(); ++uIndex ) {
			m_uCharWidth += m_containerCharWidth[ uIndex ];
		}
		// 字体距离前后边框距离各位5像素
		m_uWidth = min( m_uCharWidth + 10, nScreenWidth );
	} else {
		unsigned int uCountDivPart = m_containerBrCharIndex.size() + 1;
		unsigned int uPartWidth = 0;
		unsigned int uPrePartWidth = 0;

		for ( unsigned int uIndex = 0; uIndex != uCountDivPart; ++uIndex ) {
			uPartWidth = _calcEveryPartWidth( uIndex );
			m_uCharWidth = max( uPartWidth, uPrePartWidth );
			uPrePartWidth = uPartWidth;
		}
		// 字体距离前后边框距离各位5像素
		m_uWidth = min( m_uCharWidth + 10, nScreenWidth );
	}
	return true;
}

unsigned int YBToolTip::_calcEveryPartCharCount( unsigned int uIndexPart ) {
	unsigned int uCountBr = m_containerBrCharIndex.size();
	unsigned int uIndexStart = 0;
	unsigned int uIndexEnd;
	unsigned int uCountPartChar;

	if ( uIndexPart != 0 ) {
		uIndexStart = m_containerBrCharIndex[ uIndexPart - 1 ];
	}
	if ( uIndexPart == uCountBr ) {
		uIndexEnd = m_containerCharWidth.size();
	} else {
		uIndexEnd = m_containerBrCharIndex[ uIndexPart ];
	}
	uCountPartChar = uIndexEnd - uIndexStart;
	
	return uCountPartChar;
}

unsigned int YBToolTip::_calcEveryPartWidth( unsigned int uIndexPart ) {
	unsigned int uWidthPart = 0;
	unsigned int uIndexStart = 0;
	unsigned int uIndexEnd;
	unsigned int uCountBr = m_containerBrCharIndex.size();

	if ( uIndexPart != 0 ) {
		uIndexStart = m_containerBrCharIndex[ uIndexPart - 1 ];
	}
	if ( uIndexPart == uCountBr ) {
		uIndexEnd = m_containerCharWidth.size();
	} else {
		uIndexEnd = m_containerBrCharIndex[ uIndexPart ];
	}
	for ( uIndexStart; uIndexStart != uIndexEnd; ++uIndexStart ) {
		uWidthPart += m_containerCharWidth[ uIndexStart ];
	}

	return uWidthPart;
}

void YBToolTip::_drawText( HDC hDC ) {
	RECT rcCalc;
	RECT rcClient;
	unsigned int uWidth;
	unsigned int uLineTop;
	unsigned int uCharIndex = 0;
	unsigned int uLineIndex = 0;
	unsigned int uCharCountCalc;
	tstring_type tstrLineData;
	const YBCtrlFont* pYBFont = NULL;
	HFONT hFontOld = NULL;
	COLORREF clrTxtOld;
	int iBkMode;

	pYBFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontId );
	if ( !pYBFont ) {
		assert(false);
		return;
	}
	::GetClientRect( m_hWnd, &rcClient );
	uWidth = rcClient.right - rcClient.left;
	hFontOld = (HFONT)::SelectObject( hDC, pYBFont->m_hFont );
	iBkMode = ::SetBkMode( hDC, TRANSPARENT );
	clrTxtOld = ::SetTextColor( hDC, pYBFont->m_clrTxt );

	if ( m_containerBrCharIndex.size() == 0 ) {  //没有换行符.
		// 
		while ( uCharIndex < m_tstrText.length() ) {
			if ( _calcCharCount( uCharIndex, uWidth, uCharCountCalc ) ) {
				//
				uLineTop = m_uLineHeight*uLineIndex;
				::SetRect( &rcCalc, 5, uLineTop + 2, uWidth, uLineTop + m_uLineHeight );
				//
				tstrLineData = m_tstrText.substr( uCharIndex, uCharCountCalc );
				::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_LEFT|DT_SINGLELINE );
				//
				uCharIndex += uCharCountCalc;
				++uLineIndex;
			} else {
				assert( false );
				break;
			}
		}
	} else {    //有换行符.
		unsigned int uWidthPart;
		unsigned int uCountPart = m_containerBrCharIndex.size() + 1;
		unsigned int nScreenWidth = ::GetSystemMetrics( SM_CXSCREEN );
		for ( unsigned int uIndexPart = 0; uIndexPart != uCountPart; ++uIndexPart ) {
			if ( uIndexPart == 0 ) {
				uCharIndex = 0;
			} else {
				uCharIndex = m_containerBrCharIndex[ uIndexPart - 1 ];
			}
			uWidthPart = _calcEveryPartWidth( uIndexPart );
			if ( uWidthPart > nScreenWidth ) {   //当每部分的长度超过最大屏幕宽度时.
				// 
				while ( uCharIndex < m_tstrText.length() ) {
					if ( _calcCharCount( uCharIndex, uWidth, uCharCountCalc ) ) {
						//
						uLineTop = m_uLineHeight*uLineIndex;
						::SetRect( &rcCalc, 5, uLineTop + 2, uWidth, uLineTop + m_uLineHeight );
						//
						tstrLineData = m_tstrText.substr( uCharIndex, uCharCountCalc );
						::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_LEFT | DT_SINGLELINE );
						//
						uCharIndex += uCharCountCalc;
						++uLineIndex;
					} else {
						assert( false );
						break;
					}
				}
			} else {
				uLineTop = m_uLineHeight*uLineIndex;
				uCharCountCalc = _calcEveryPartCharCount( uIndexPart );
				::SetRect( &rcCalc, 5, uLineTop + 2, uWidth, uLineTop + m_uLineHeight );
				//
				tstrLineData = m_tstrText.substr( uCharIndex, uCharCountCalc );
				::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_LEFT | DT_SINGLELINE );
				++uLineIndex;
			}
		}
	}
	//
	::SetTextColor( hDC, clrTxtOld );
	::SetBkMode( hDC, iBkMode );
	::SelectObject( hDC, hFontOld );
}

void YBToolTip::_calcAllCharWidth() {
	m_containerBrCharIndex.clear();
	m_containerCharWidth.clear();
	RECT rcTextCalc;

	TCHAR tchData;
	TCHAR tszData[2];
	unsigned int uLenStrData;
	int nScreenWidth;
	int nScreenHeight;
	const YBCtrlFont* pYBCtrlFont = NULL;
	HFONT hFontOld;

	nScreenWidth = GetSystemMetrics( SM_CXSCREEN );
	nScreenHeight = GetSystemMetrics( SM_CYSCREEN );
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontId );
	if ( !pYBCtrlFont ) {
		assert( false );
		return;
	}
	hFontOld = (HFONT)::SelectObject( m_pMemDC->getMemDC(), pYBCtrlFont->m_hFont );
	uLenStrData = m_tstrText.length();
	for ( unsigned int uIndex = 0; uIndex < uLenStrData; ++uIndex ) {
		tchData = m_tstrText[ uIndex ];
		tszData[ 0 ] = tchData;
		tszData[ 1 ] = 0;

		if ( tchData == '\n' ) {
			m_containerBrCharIndex.push_back( uIndex );
		} 
		::SetRect( &rcTextCalc, 0, 0, nScreenWidth, nScreenHeight );
		::DrawText( m_pMemDC->getMemDC(), tszData, 1, &rcTextCalc, DT_CALCRECT|DT_SINGLELINE|DT_LEFT );
		m_containerCharWidth.push_back( rcTextCalc.right - rcTextCalc.left );
	}
	::SelectObject( m_pMemDC->getMemDC(), hFontOld );

	assert( m_containerCharWidth.size() == m_tstrText.length() );
}

bool YBToolTip::_calcCharCount( unsigned int uCharIndexBegin, unsigned int uWidth, unsigned int& uCharCountCalc ) {
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

void YBToolTip::_loadFont() {
	LOGFONT lFont;
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 18;
	m_uFontId = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(100, 100, 100) );
}