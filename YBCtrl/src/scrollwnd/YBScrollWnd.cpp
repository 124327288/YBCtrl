#include "stdafx.h"
#include "scrollwnd/YBScrollWnd.h"
#include "view/YBViewInner.h"
#include "scrollbar/YBScrollBar.h"
#include <assert.h>
#define YBSW_CTRLID_SBH (0x00000010)
#define YBSW_CTRLID_SBV (0x00000020)
#define YBSW_CTRLID_VIEWINNER (0x00000030)
#define CLR_YBSW_BK_DEFAULT (RGB(255,255,255))
#define YBSW_FLAG_NONE (0x00000000)
#define YBSW_FLAG_REDRAW_DISABLE (0x00000001)
USING_NAMESPACE_YBCTRL

YBScrollWnd::YBScrollWnd( YBViewInner* pViewInner, unsigned int uScrollBarThick, unsigned int uStepMove )
: m_uFlag( YBSW_FLAG_NONE )
, m_pViewInner( pViewInner )
, m_pScrollBarH( new YBScrollBar( YBScrollBar::EDIRECTION_HORIZONTAL ) )
, m_pScrollBarV( new YBScrollBar( YBScrollBar::EDIRECTION_VERTICAL ) )
, m_uScrollBarThick( ( uScrollBarThick != 0 ) ? uScrollBarThick : YBSW_SBTHICK_DEFAULT )
, m_uStepMove( ( uStepMove != 0 ) ? uStepMove : YBSW_STEPMOVE_DEFAULT )
, m_hBrushBK( ::CreateSolidBrush( CLR_YBSW_BK_DEFAULT ) )
{
	m_pViewInner->_setScrollWnd( this );
}

YBScrollWnd::~YBScrollWnd() {
	if ( m_hBrushBK ) {
		::DeleteObject( m_hBrushBK );
		m_hBrushBK = NULL;
	}
	if ( m_pScrollBarV ) {
		delete m_pScrollBarV;
		m_pScrollBarV = NULL;
	}
	if ( m_pScrollBarH ) {
		delete m_pScrollBarH;
		m_pScrollBarH = NULL;
	}
	if ( m_pViewInner ) {
		delete m_pViewInner;
		m_pViewInner = NULL;
	}
}

HWND YBScrollWnd::create( int x, int y, int nWidth, int nHeight, HWND hWndParent, WORD wCtrlID, HINSTANCE hInstance ) {
	DWORD dwExStyle;
	DWORD dwStyle;
	HWND hWndScrollWnd;
	bool bNeedScrollBarH = false;
	bool bNeedScrollBarV = false;
	RECT rcViewInner;
	RECT rcScrollBarH;
	RECT rcScrollBarV;
	YBViewContentBase* pViewContent = NULL;

	//
	dwExStyle = 0;
	dwStyle = WS_CLIPCHILDREN|WS_CHILD;
	hWndScrollWnd = _createHWND( dwExStyle, dwStyle, x, y, nWidth, nHeight, hWndParent, (HMENU)wCtrlID, hInstance, NULL );
	if ( !hWndScrollWnd ) {
		assert( false );
		return NULL;
	}

	//
	checkIsNeedScrollBar( bNeedScrollBarH, bNeedScrollBarV );
	getViewInnerRect( rcViewInner, bNeedScrollBarH, bNeedScrollBarV );
	getScrollBarHRect( rcScrollBarH, bNeedScrollBarV );
	getScrollBarVRect( rcScrollBarV, bNeedScrollBarH );
	//
	if ( !m_pViewInner->create( rcViewInner.left, rcViewInner.top, ( rcViewInner.right - rcViewInner.left ), (rcViewInner.bottom - rcViewInner.top), hWndScrollWnd, YBSW_CTRLID_VIEWINNER, hInstance ) ) {
		assert( false );
	}
	//
	pViewContent = m_pViewInner->getViewContentObj();
	// create scrollbarH
	m_pScrollBarH->create( rcScrollBarH.left, rcScrollBarH.top, ( rcScrollBarH.right - rcScrollBarH.left ), ( rcScrollBarH.bottom - rcScrollBarH.top ), m_hWnd, YBSW_CTRLID_SBH, hInstance );
	// create scrollbarV
	m_pScrollBarV->create( rcScrollBarV.left, rcScrollBarV.top, ( rcScrollBarV.right - rcScrollBarV.left ), ( rcScrollBarV.bottom - rcScrollBarV.top ), m_hWnd, YBSW_CTRLID_SBV, hInstance );
	//
	updateStatus();

	return hWndScrollWnd;
}

void YBScrollWnd::reset() {
	if ( m_pScrollBarV ) {
		m_pScrollBarV->setValRange( 0, 1000 );
		m_pScrollBarV->setCurVal( 0 );
	}
	if ( m_pScrollBarH ) {
		m_pScrollBarH->setValRange( 0, 1000 );
		m_pScrollBarH->setCurVal( 0 );
	}
	if ( m_pViewInner ) {
		m_pViewInner->setContentXPosLT( 0 );
		m_pViewInner->setContentYPosLT( 0 );
	}
	updateStatus();
}

bool YBScrollWnd::enableRedraw() {
	return ( m_uFlag & YBSW_FLAG_REDRAW_DISABLE ) != YBSW_FLAG_REDRAW_DISABLE;
}

void YBScrollWnd::enableRedraw( bool bReDraw ) {
	if ( ::IsWindow( getHWND() ) ) {
		::SendMessage( getHWND(), WM_SETREDRAW, (bReDraw ? TRUE : FALSE), 0 );
	}

	//
	m_pViewInner->enableRedraw( bReDraw );
	//
	if ( bReDraw ) {
		m_uFlag &= ~YBSW_FLAG_REDRAW_DISABLE;
		if ( ::IsWindow( getHWND() ) ) {
			::InvalidateRect( getHWND(), NULL, FALSE );
		}
		if ( ::IsWindow( m_pScrollBarH->getHWND() ) && ::IsWindowVisible( m_pScrollBarH->getHWND() ) ) {
			::InvalidateRect( m_pScrollBarH->getHWND(), NULL, FALSE );
		}
		if ( ::IsWindow( m_pScrollBarV->getHWND() ) && ::IsWindowVisible( m_pScrollBarV->getHWND() ) ) {
			::InvalidateRect( m_pScrollBarV->getHWND(), NULL, FALSE );
		}
	} else {
		m_uFlag |= YBSW_FLAG_REDRAW_DISABLE;
	}
}

void YBScrollWnd::updateStatus() {
	YBViewInner* pViewInner = NULL;
	bool bNeedScrollBarH = false;
	bool bNeedScrollBarV = false;
	RECT rcViewInner;
	RECT rcScrollBarH;
	RECT rcScrollBarV;
	
	checkIsNeedScrollBar( bNeedScrollBarH, bNeedScrollBarV );
	getViewInnerRect( rcViewInner, bNeedScrollBarH, bNeedScrollBarV );
	// adjust m_pViewInner
	::MoveWindow( m_pViewInner->getHWND(), rcViewInner.left, rcViewInner.top, 
		( rcViewInner.right - rcViewInner.left ), ( rcViewInner.bottom - rcViewInner.top ), FALSE );
	::InvalidateRect( m_pViewInner->getHWND(), NULL, FALSE );
	// check again, after move m_pViewInner.
	checkIsNeedScrollBar( bNeedScrollBarH, bNeedScrollBarV );
	getScrollBarHRect( rcScrollBarH, bNeedScrollBarV );
	getScrollBarVRect( rcScrollBarV, bNeedScrollBarH );
	// adjust m_pScrollBarH
	if ( bNeedScrollBarH ) {
		if ( m_pScrollBarH && ::IsWindow( m_pScrollBarH->getHWND() ) ) {
			::MoveWindow( m_pScrollBarH->getHWND(), rcScrollBarH.left, rcScrollBarH.top,
				( rcScrollBarH.right - rcScrollBarH.left ), ( rcScrollBarH.bottom - rcScrollBarH.top ), TRUE );
			adjustSBThumbLenH();
			::ShowWindow( m_pScrollBarH->getHWND(), SW_NORMAL );
		}
	} else {
		if ( m_pScrollBarH && ::IsWindow( m_pScrollBarH->getHWND() ) ) {
			adjustSBThumbLenH();
			::ShowWindow( m_pScrollBarH->getHWND(), SW_HIDE );
		}
	}
	// adjust m_pScrollBarV
	if ( bNeedScrollBarV ) {
		if ( m_pScrollBarV && ::IsWindow( m_pScrollBarV->getHWND() ) ) {
			::MoveWindow( m_pScrollBarV->getHWND(), rcScrollBarV.left, rcScrollBarV.top,
				( rcScrollBarV.right - rcScrollBarV.left ), ( rcScrollBarV.bottom - rcScrollBarV.top ), TRUE );	
			adjustSBThumbLenV();
			::ShowWindow( m_pScrollBarV->getHWND(), SW_NORMAL );
		}
	} else {
		if ( m_pScrollBarV && ::IsWindow( m_pScrollBarV->getHWND() ) ) {
			adjustSBThumbLenV();
			::ShowWindow( m_pScrollBarV->getHWND(), SW_HIDE );
		}
	}

	// adjust left-top positon of content in view.
	adjustContentLTPos();

}

void YBScrollWnd::adjustContentLTPos() {
	if ( !m_pViewInner || !::IsWindow( m_pViewInner->getHWND() ) ) {
		return ;
	}
	ybctrl::YBViewContentBase* pViewContentObj = NULL;
	unsigned int uXPosLT;
	unsigned int uYPosLT;
	unsigned int uXPosLTNew;
	unsigned int uYPosLTNew;
	RECT rcViewInner;
	unsigned int uWidthViewInner;
	unsigned int uHeightViewInner;
	int nValMin;
	int nValMax;
	int nValCur;

	pViewContentObj = m_pViewInner->getViewContentObj();
	if ( !pViewContentObj ) {
		return;
	}
	uXPosLT = m_pViewInner->getContentXPosLT();
	uYPosLT = m_pViewInner->getContentYPosLT();
	if ( !::GetClientRect( m_pViewInner->getHWND(), &rcViewInner ) ) {
		return;
	}
	//
	uWidthViewInner = rcViewInner.right - rcViewInner.left;
	if ( uXPosLT + uWidthViewInner > pViewContentObj->getWidth() ) {
		if ( pViewContentObj->getWidth() > uWidthViewInner ) {
			m_pViewInner->setContentXPosLT( pViewContentObj->getWidth() - uWidthViewInner );
		} else {
			m_pViewInner->setContentXPosLT( 0 );
		}
	} else {
		if ( m_pScrollBarH ) {
			m_pScrollBarH->getValRange( nValMin, nValMax );
			nValCur = m_pScrollBarH->getCurVal();
			if ( nValMax == nValMin ) {
				uXPosLTNew = 0;
			} else {
				uXPosLTNew = ( pViewContentObj->getWidth() - uWidthViewInner ) * nValCur / ( nValMax - nValMin );
			}
			m_pViewInner->setContentXPosLT( uXPosLTNew );
		}
	}
	//
	uHeightViewInner = rcViewInner.bottom - rcViewInner.top;
	if ( uYPosLT + uHeightViewInner > pViewContentObj->getHeight() ) {
		if ( pViewContentObj->getHeight() > uHeightViewInner ) {
			m_pViewInner->setContentYPosLT( pViewContentObj->getHeight() - uHeightViewInner );
		} else {
			m_pViewInner->setContentYPosLT( 0 );
		}
	} else {
		if ( m_pScrollBarV ) {
			m_pScrollBarV->getValRange( nValMin, nValMax );
			nValCur = m_pScrollBarV->getCurVal();
			if ( nValMax == nValMin ) {
				uYPosLTNew = 0;
			} else {
				uYPosLTNew = ( pViewContentObj->getHeight() - uHeightViewInner ) * nValCur / ( nValMax - nValMin );
			}
			m_pViewInner->setContentYPosLT( uYPosLTNew );
		}
	}

}

void YBScrollWnd::adjustSBThumbLenH() {
	if ( !m_pViewInner || !::IsWindow( m_pViewInner->getHWND() ) 
		|| !m_pScrollBarH || !::IsWindow( m_pScrollBarH->getHWND() ) ) {
		assert( false );
		return;
	}
	YBViewContentBase* pViewContentObj = NULL;
	RECT rcView;
	int nLenSBTotal;
	int nLenThumbBtn;

	pViewContentObj = m_pViewInner->getViewContentObj();
	::GetClientRect( m_pViewInner->getHWND(), &rcView );
	nLenSBTotal = m_pScrollBarH->getTotalLen();
	if ( pViewContentObj->getWidth() == 0 ) {
		nLenThumbBtn = 0;
	} else if ( ( rcView.right - rcView.left ) >= (int)pViewContentObj->getWidth() ) {
		nLenThumbBtn = 0;
	} else {
		nLenThumbBtn = nLenSBTotal*( rcView.right - rcView.left )/( pViewContentObj->getWidth() );
	}
	if ( nLenSBTotal < 0 ) {
		assert( false );
		return;
	}
	m_pScrollBarH->setLenThumbBtn( (unsigned int)nLenThumbBtn );
	::InvalidateRect( m_pScrollBarH->getHWND(), NULL, TRUE );
}

void YBScrollWnd::adjustSBThumbLenV() {
	if ( !m_pViewInner || !::IsWindow( m_pViewInner->getHWND() ) 
		|| !m_pScrollBarV || !::IsWindow( m_pScrollBarV->getHWND() ) ) {
			assert( false );
			return;
	}
	YBViewContentBase* pViewContentObj = NULL;
	RECT rcView;
	int nLenSBTotal;
	int nLenThumbBtn;

	pViewContentObj = m_pViewInner->getViewContentObj();
	::GetClientRect( m_pViewInner->getHWND(), &rcView );
	nLenSBTotal = m_pScrollBarV->getTotalLen();
	if ( pViewContentObj->getHeight() == 0 ) {
		nLenThumbBtn = 0;
	} else if ( ( rcView.bottom - rcView.top ) >= (int)pViewContentObj->getHeight() ) {
		nLenThumbBtn = 0;
	} else {
		nLenThumbBtn = nLenSBTotal*( rcView.bottom - rcView.top )/( pViewContentObj->getHeight() );
	}
	if ( nLenSBTotal < 0 ) {
		assert( false );
		return;
	}
	m_pScrollBarV->setLenThumbBtn( (unsigned int)nLenThumbBtn );
	::InvalidateRect( m_pScrollBarV->getHWND(), NULL, TRUE );
}

void YBScrollWnd::getViewInnerRect( RECT& rcViewInner, bool bNeedScrollBarH, bool bNeedScrollBarV ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcViewInner = rcClient;
	if ( bNeedScrollBarH ) {
		rcViewInner.bottom -= m_uScrollBarThick;
	}
	if ( bNeedScrollBarV ) {
		rcViewInner.right -= m_uScrollBarThick;
	}
}

void YBScrollWnd::getScrollBarHRect( RECT& rcScrollBarH, bool bNeedScrollBarV ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcScrollBarH = rcClient;
	rcScrollBarH.top = rcScrollBarH.bottom - m_uScrollBarThick;
	if ( bNeedScrollBarV ) {
		rcScrollBarH.right -= m_uScrollBarThick;
	}
}

void YBScrollWnd::getScrollBarVRect( RECT& rcScrollBarV, bool bNeedScrollBarH ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcScrollBarV = rcClient;
	rcScrollBarV.left = rcScrollBarV.right - m_uScrollBarThick;
	if ( bNeedScrollBarH ) {
		rcScrollBarV.bottom -= m_uScrollBarThick;
	}
}

void YBScrollWnd::checkIsNeedScrollBar( bool& bNeedScrollBarH, bool& bNeedScrollBarV ) {
	ybctrl::YBViewContentBase* pYBViewContent = NULL;
	RECT rcClient;
	unsigned int uThickScrollBar = 0;

	//
	bNeedScrollBarH = false;
	bNeedScrollBarV = false;
	//
	::GetClientRect( m_hWnd, &rcClient );
	//
	pYBViewContent = m_pViewInner->getViewContentObj();
	if ( !pYBViewContent ) {
		return;
	}

	// get bNeedScrollBarV
	if ( (int)pYBViewContent->getHeight() > ( rcClient.bottom - rcClient.top ) ) {
		bNeedScrollBarV = true;
	}
	// get bNeedScrollBarH
	if ( !m_pScrollBarV->getThick( uThickScrollBar ) ) {
		uThickScrollBar = 0;
	}
	if ( ( int )pYBViewContent->getWidth() + (int)( bNeedScrollBarV ? uThickScrollBar : 0 ) > ( rcClient.right - rcClient.left ) ) {
		bNeedScrollBarH = true;
	}
	// get bNeedScrollBarV again.
	if ( !m_pScrollBarH->getThick( uThickScrollBar ) ) {
		uThickScrollBar = 0;
	}
	if ( ( int )pYBViewContent->getHeight() + ( int )( bNeedScrollBarH ? uThickScrollBar : 0 ) > ( rcClient.bottom - rcClient.top ) ) {
		bNeedScrollBarV = true;
	}
}

YBCTRL_WNDMSGMAP_BEGIN( YBScrollWnd, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_SIZE, _onWM_SIZE )
YBCTRL_WNDMSGMAP_END()

void YBScrollWnd::_onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	HDC hPaintDC;
	PAINTSTRUCT ps;
	RECT rcClient;

	assert( ::IsWindowVisible( pYBCtrlWndMsg->m_hWnd ) );
	::GetClientRect( m_hWnd, &rcClient );
	hPaintDC = ::BeginPaint( m_hWnd, &ps );

	::FillRect( hPaintDC, &rcClient, m_hBrushBK );

	::EndPaint( m_hWnd, &ps );
}

void YBScrollWnd::_onWM_SIZE( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_SIZE );
	bool bNeedSBH = false;
	bool bNeedSBV = false;
	RECT rcViewInner;
	RECT rcSBH;
	RECT rcSBV;

	checkIsNeedScrollBar( bNeedSBH, bNeedSBV );
	getViewInnerRect( rcViewInner, bNeedSBH, bNeedSBV );
	::MoveWindow( m_pViewInner->getHWND(), rcViewInner.left, rcViewInner.top
		, rcViewInner.right - rcViewInner.left, rcViewInner.bottom - rcViewInner.top, TRUE );
	//
	getScrollBarHRect( rcSBH, bNeedSBV );
	::MoveWindow( m_pScrollBarH->getHWND(), rcSBH.left, rcSBH.top, ( rcSBH.right - rcSBH.left ), ( rcSBH.bottom - rcSBH.top ), TRUE );
	//
	getScrollBarVRect( rcSBV, bNeedSBH );
	::MoveWindow( m_pScrollBarV->getHWND(), rcSBV.left, rcSBV.top, ( rcSBV.right - rcSBV.left ), ( rcSBV.bottom - rcSBV.top ), TRUE );
	//
	updateStatus();
}

void YBScrollWnd::_onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	HDC hDC;
	RECT rcClient;

	assert( ::IsWindowVisible( pYBCtrlWndMsg->m_hWnd ) );
	hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	::GetClientRect( m_hWnd, &rcClient );
	::FillRect( hDC, &rcClient, m_hBrushBK );

}

void YBScrollWnd::_onWM_COMMAND( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtlEvt = HIWORD( pYBCtrlWndMsg->m_wParam );
	WORD wCtlID = LOWORD( pYBCtrlWndMsg->m_wParam );

	switch ( wCtlID ) {
	case YBSW_CTRLID_SBH:
		{
			onScrollBarEventH( wCtlEvt, pYBCtrlWndMsg->m_lParam );
		}
		break;
	case YBSW_CTRLID_SBV:
		{
			onScrollBarEventV( wCtlEvt, pYBCtrlWndMsg->m_lParam );
		}
		break;
	case YBSW_CTRLID_VIEWINNER:
		{
			onViewInnerEvent( wCtlEvt, pYBCtrlWndMsg->m_lParam );
		}
	default:
		break;
	}

	
//#define YBSW_CTRLID_SBH (0x00000010)
//#define YBSW_CTRLID_SBV (0x00000020)
}

void YBScrollWnd::onScrollBarEventH( WORD wCtlEvt, LPARAM lParam ) {
	switch ( wCtlEvt ) {
	case YBSB_CMDEVT_STARTBTNDOWN: // start button down.
		{
			unsigned int nContentWidth;
			RECT rcInner;
			int nXPosLTOld;
			int nXPosLTNew;
			int nStepPos;

			YBViewContentBase* pViewContent = NULL;
			nXPosLTOld = m_pViewInner->getContentXPosLT();
			::GetClientRect( m_pViewInner->getHWND(), &rcInner );
			pViewContent = m_pViewInner->getViewContentObj();
			nContentWidth = pViewContent->getWidth();
			nStepPos = m_uStepMove;
			if( (int)nContentWidth <= nStepPos ){
				nXPosLTNew = 0;
			}else{
				nXPosLTNew = nXPosLTOld - nStepPos;
			}
			if( nXPosLTNew < 0 ){
				nXPosLTNew = 0;
			}
			m_pViewInner->setContentXPosLT( nXPosLTNew );
			m_pScrollBarH->setCurVal( _calcSBValH( nXPosLTNew ) );
			//
			::InvalidateRect( m_pViewInner->getHWND(), &rcInner, false );
		}
		break;
	case YBSB_CMDEVT_ENDTBTNDOWN: // end button down.
		{
			unsigned int nContentWidth;
			RECT rcInner;
			int nXPosLTOld;
			int nXPosLTNew;
			int nStepPos;

			YBViewContentBase* pViewContent = NULL;
			nXPosLTOld = m_pViewInner->getContentXPosLT();
			::GetClientRect( m_pViewInner->getHWND(), &rcInner );
			pViewContent = m_pViewInner->getViewContentObj();
			nContentWidth = pViewContent->getWidth();
			nStepPos = m_uStepMove;
			if( (int)nContentWidth <= nStepPos ){
				nXPosLTNew = 0;
			}else{
				nXPosLTNew = nXPosLTOld + nStepPos;
			}
			if( int( nContentWidth - nXPosLTNew ) < ( rcInner.right - rcInner.left ) ){
				nXPosLTNew = nContentWidth - ( rcInner.right - rcInner.left );
			}
			m_pViewInner->setContentXPosLT( nXPosLTNew );
			m_pScrollBarH->setCurVal( _calcSBValH( nXPosLTNew ) );
			//
			::InvalidateRect( m_pViewInner->getHWND(), &rcInner, false );
			
		}
		break;
	case YBSB_CMDEVT_THUMBMOVE: // thumb button move.
		{
			RECT rcInner;
			unsigned int uContentWidth;
			int nValMin;
			int nValMax;
			int nValCur;
			YBViewContentBase* pViewContent = NULL;
			int nXPosLTNew;

			pViewContent = m_pViewInner->getViewContentObj();
			::GetClientRect( m_pViewInner->getHWND(), &rcInner );
			if ( rcInner.right < rcInner.left )
				return;
			uContentWidth = pViewContent->getWidth();
			m_pScrollBarH->getValRange( nValMin, nValMax );
			nValCur = m_pScrollBarH->getCurVal();
			//
			if ( (int)uContentWidth < ( rcInner.right - rcInner.left ) ) {
				nXPosLTNew = 0;
			} else {
				if ( nValMax == nValMin ) {
					nXPosLTNew = 0;
				} else {
					nXPosLTNew = ( uContentWidth - ( rcInner.right - rcInner.left ) ) * ( nValCur - nValMin ) / ( nValMax - nValMin );
				}
			}
			m_pViewInner->setContentXPosLT( nXPosLTNew );
			//
			::InvalidateRect( m_pViewInner->getHWND(), &rcInner, false );
		}
		break;
	case YBSB_CMDEVT_STARTSPACEDOWN: // start space down.
		{
			unsigned int nContentWidth;
			RECT rcInner;
			int nXPosLTOld;
			int nXPosLTNew;
			int nStepPos;

			YBViewContentBase* pViewContent = NULL;
			nXPosLTOld = m_pViewInner->getContentXPosLT();
			::GetClientRect( m_pViewInner->getHWND(), &rcInner );
			pViewContent = m_pViewInner->getViewContentObj();
			nContentWidth = pViewContent->getWidth();
			nStepPos = rcInner.right - rcInner.left;
			if( (int)nContentWidth <= nStepPos ){
				nXPosLTNew = 0;
			}else{
				nXPosLTNew = nXPosLTOld - nStepPos;
			}
			if( nXPosLTNew < 0 ){
				nXPosLTNew = 0;
			}
			m_pViewInner->setContentXPosLT( nXPosLTNew );
			m_pScrollBarH->setCurVal( _calcSBValH( nXPosLTNew ) );
			//
			::InvalidateRect( m_pViewInner->getHWND(), &rcInner, false );
		}
		break;
	case YBSB_CMDEVT_ENDSPACEDOWN: // end space down.
		{
			unsigned int nContentWidth;
			RECT rcInner;
			int nXPosLTOld;
			int nXPosLTNew;
			int nStepPos;

			YBViewContentBase* pViewContent = NULL;
			nXPosLTOld = m_pViewInner->getContentXPosLT();
			::GetClientRect( m_pViewInner->getHWND(), &rcInner );
			pViewContent = m_pViewInner->getViewContentObj();
			nContentWidth = pViewContent->getWidth();
			nStepPos = rcInner.right - rcInner.left;
			if( (int)nContentWidth <= nStepPos ){
				nXPosLTNew = 0;
			}else{
				nXPosLTNew = nXPosLTOld + nStepPos;
			}
			if( int( nContentWidth - nXPosLTNew ) < ( rcInner.right - rcInner.left ) ){
				nXPosLTNew = nContentWidth - ( rcInner.right - rcInner.left );
			}
			m_pViewInner->setContentXPosLT( nXPosLTNew );
			m_pScrollBarH->setCurVal( _calcSBValH( nXPosLTNew ) );
			//
			::InvalidateRect( m_pViewInner->getHWND(), &rcInner, false );
		}
		break;
	default:
		break;
	}
}

void YBScrollWnd::onViewInnerEvent ( WORD wCtrlEvt, LPARAM lParam ){
	switch ( wCtrlEvt ){
	case YBSB_CMDEVT_DOWNROLL:
		{
			unsigned int nContentHeight;
			RECT rcInner;
			int nYPosLTOld;
			int nYPosLTNew;
			int nStepPos;

			YBViewContentBase* pViewContent = NULL;
			nYPosLTOld = m_pViewInner->getContentYPosLT();
			::GetClientRect( m_pViewInner->getHWND(), &rcInner );
			pViewContent = m_pViewInner->getViewContentObj();
			nContentHeight = pViewContent->getHeight();
			if( nContentHeight <= unsigned int( rcInner.bottom - rcInner.top ) )
				return;
			nStepPos = 3*m_uStepMove;
			if( (int)nContentHeight <= nStepPos ){
				nYPosLTNew = 0;
			}else{
				nYPosLTNew = nYPosLTOld + nStepPos;
			}
			if( int( nContentHeight - nYPosLTNew ) < ( rcInner.bottom - rcInner.top ) ){
				nYPosLTNew = nContentHeight - ( rcInner.bottom - rcInner.top );
			}
			m_pViewInner->setContentYPosLT( nYPosLTNew );
			m_pScrollBarV->setCurVal( _calcSBValV( nYPosLTNew ) );
			//
			::InvalidateRect( m_pViewInner->getHWND(), &rcInner, false );
			::InvalidateRect( m_pScrollBarV->getHWND(), &rcInner, false );
		}
		break;
	case YBSB_CMDEVT_UPROLL:
		{
			unsigned int nContentHeight;
			RECT rcInner;
			int nYPosLTOld;
			int nYPosLTNew;
			int nStepPos;

			YBViewContentBase* pViewContent = NULL;
			nYPosLTOld = m_pViewInner->getContentYPosLT();
			::GetClientRect( m_pViewInner->getHWND(), &rcInner );
			pViewContent = m_pViewInner->getViewContentObj();
			nContentHeight = pViewContent->getHeight();
			if( nContentHeight <= unsigned int( rcInner.bottom - rcInner.top ) )
				return;
			nStepPos = 3*m_uStepMove;
			if( (int)nContentHeight <= nStepPos ){
				nYPosLTNew = 0;
			}else{
				nYPosLTNew = nYPosLTOld - nStepPos;
			}
			if( nYPosLTNew < 0 ){
				nYPosLTNew = 0;
			}
			m_pViewInner->setContentYPosLT( nYPosLTNew );
			m_pScrollBarV->setCurVal( _calcSBValV( nYPosLTNew ) );
			//
			::InvalidateRect( m_pViewInner->getHWND(), &rcInner, false );
			::InvalidateRect( m_pScrollBarV->getHWND(), &rcInner, false );
		}
		break;
	default:
		break;
	}
}

void YBScrollWnd::onScrollBarEventV( WORD wCtlEvt, LPARAM lParam ) {
	switch ( wCtlEvt ) {
	case YBSB_CMDEVT_STARTBTNDOWN: // start button down.
		{
			unsigned int nContentHeight;
			RECT rcInner;
			int nYPosLTOld;
			int nYPosLTNew;
			int nStepPos;

			YBViewContentBase* pViewContent = NULL;
			nYPosLTOld = m_pViewInner->getContentYPosLT();
			::GetClientRect( m_pViewInner->getHWND(), &rcInner );
			pViewContent = m_pViewInner->getViewContentObj();
			nContentHeight = pViewContent->getWidth();
			nStepPos = m_uStepMove;
			if( (int)nContentHeight <= nStepPos ){
				nYPosLTNew = 0;
			}else{
				nYPosLTNew = nYPosLTOld - nStepPos;
			}
			if( nYPosLTNew < 0 ){
				nYPosLTNew = 0;
			}
			m_pViewInner->setContentYPosLT( nYPosLTNew );
			m_pScrollBarV->setCurVal( _calcSBValV( nYPosLTNew ) );
			//
			::InvalidateRect( m_pViewInner->getHWND(), &rcInner, false );
		}
		break;
	case YBSB_CMDEVT_ENDTBTNDOWN: // end button down.
		{
			unsigned int nContentHeight;
			RECT rcInner;
			int nYPosLTOld;
			int nYPosLTNew;
			int nStepPos;

			YBViewContentBase* pViewContent = NULL;
			nYPosLTOld = m_pViewInner->getContentYPosLT();
			::GetClientRect( m_pViewInner->getHWND(), &rcInner );
			pViewContent = m_pViewInner->getViewContentObj();
			nContentHeight = pViewContent->getHeight();
			nStepPos = m_uStepMove;
			if( (int)nContentHeight <= nStepPos ){
				nYPosLTNew = 0;
			}else{
				nYPosLTNew = nYPosLTOld + nStepPos;
			}
			if( int( nContentHeight - nYPosLTNew ) < ( rcInner.bottom - rcInner.top ) ){
				nYPosLTNew = nContentHeight - ( rcInner.bottom - rcInner.top );
			}
			m_pViewInner->setContentYPosLT( nYPosLTNew );
			m_pScrollBarV->setCurVal( _calcSBValV( nYPosLTNew ) );
			//
			::InvalidateRect( m_pViewInner->getHWND(), &rcInner, false );
		}
		break;
	case YBSB_CMDEVT_THUMBMOVE: // thumb button move.
		{
			RECT rcInner;
			unsigned int uContentHeight;
			int nValMin;
			int nValMax;
			int nValCur;
			YBViewContentBase* pViewContent = NULL;
			int nYPosLTNew;

			pViewContent = m_pViewInner->getViewContentObj();
			::GetClientRect( m_pViewInner->getHWND(), &rcInner );
			if ( rcInner.bottom < rcInner.top )
				return;
			uContentHeight = pViewContent->getHeight();
			m_pScrollBarV->getValRange( nValMin, nValMax );
			nValCur = m_pScrollBarV->getCurVal();
			//
			if ( (int)uContentHeight < ( rcInner.bottom - rcInner.top ) ) {
				nYPosLTNew = 0;
			} else {
				if ( nValMax == nValMin ) {
					nYPosLTNew = 0;
				} else {
					nYPosLTNew = ( uContentHeight - ( rcInner.bottom - rcInner.top ) ) * ( nValCur - nValMin ) / ( nValMax - nValMin );
				}
			}
			m_pViewInner->setContentYPosLT( nYPosLTNew );
			//
			::InvalidateRect( m_pViewInner->getHWND(), &rcInner, false );

		}
		break;
	case YBSB_CMDEVT_STARTSPACEDOWN: // start space down.
		{
			unsigned int nContentHeight;
			RECT rcInner;
			int nYPosLTOld;
			int nYPosLTNew;
			int nStepPos;

			YBViewContentBase* pViewContent = NULL;
			nYPosLTOld = m_pViewInner->getContentYPosLT();
			::GetClientRect( m_pViewInner->getHWND(), &rcInner );
			pViewContent = m_pViewInner->getViewContentObj();
			nContentHeight = pViewContent->getHeight();
			nStepPos = rcInner.bottom - rcInner.top;
			if( (int)nContentHeight <= nStepPos ){
				nYPosLTNew = 0;
			}else{
				nYPosLTNew = nYPosLTOld - nStepPos;
			}
			if( nYPosLTNew < 0 ){
				nYPosLTNew = 0;
			}
			m_pViewInner->setContentYPosLT( nYPosLTNew );
			m_pScrollBarV->setCurVal( _calcSBValV( nYPosLTNew ) );
			//
			::InvalidateRect( m_pViewInner->getHWND(), &rcInner, false );
		}
		break;
	case YBSB_CMDEVT_ENDSPACEDOWN: // end space down.
		{
			unsigned int nContentHeight;
			RECT rcInner;
			int nYPosLTOld;
			int nYPosLTNew;
			int nStepPos;

			YBViewContentBase* pViewContent = NULL;
			nYPosLTOld = m_pViewInner->getContentYPosLT();
			::GetClientRect( m_pViewInner->getHWND(), &rcInner );
			pViewContent = m_pViewInner->getViewContentObj();
			nContentHeight = pViewContent->getHeight();
			nStepPos = rcInner.bottom - rcInner.top;
			if( (int)nContentHeight <= nStepPos ){
				nYPosLTNew = 0;
			}else{
				nYPosLTNew = nYPosLTOld + nStepPos;
			}
			if( int( nContentHeight - nYPosLTNew ) < ( rcInner.bottom - rcInner.top ) ){
				nYPosLTNew = nContentHeight - ( rcInner.bottom - rcInner.top );
			}
			m_pViewInner->setContentYPosLT( nYPosLTNew );
			m_pScrollBarV->setCurVal( _calcSBValV( nYPosLTNew ) );
			//
			::InvalidateRect( m_pViewInner->getHWND(), &rcInner, false );
		}
		break;
	default:
		break;
	}	
}

int YBScrollWnd::_calcSBValH( int nXPosLT ){
	int nValMin;
	int nValMax;
	int nValCur;
	RECT rcInner;
	YBViewContentBase* pViewContentObj = NULL;

	pViewContentObj = m_pViewInner->getViewContentObj();
	::GetClientRect( m_pViewInner->getHWND(), &rcInner );
	m_pScrollBarH->getValRange( nValMin, nValMax );
	if ( pViewContentObj->getWidth() == ( rcInner.right - rcInner.left ) ) {
		nValCur = 0;
	} else {
		nValCur = nValMin + ( nValMax - nValMin ) * nXPosLT/( pViewContentObj->getWidth() - ( rcInner.right - rcInner.left ) );
		nValCur = max( nValMin, min( nValCur, nValMax ) );
	}

	return nValCur;
}

int YBScrollWnd::_calcSBValV( int nYPosLT ){
	int nValMin;
	int nValMax;
	int nValCur;
	RECT rcInner;
	YBViewContentBase* pViewContentObj = NULL;

	pViewContentObj = m_pViewInner->getViewContentObj();
	::GetClientRect( m_pViewInner->getHWND(), &rcInner );
	m_pScrollBarV->getValRange( nValMin, nValMax );
	if ( pViewContentObj->getHeight() == ( rcInner.bottom - rcInner.top ) ) {
		nValCur = 0;
	} else {
		nValCur = nValMin + nYPosLT*( nValMax - nValMin )/( pViewContentObj->getHeight() - ( rcInner.bottom - rcInner.top ) );
		nValCur = max( nValMin, min( nValCur, nValMax ) );
	}

	return nValCur;
}

// int YBScrollWnd::setScrollBarPos(){
// 	int nValMin;
// 	int nValMax;
// 	int nPosMove;
// 	unsigned int nContentHeight;
// 	YBViewContentBase* pViewContent = NULL;
// 	pViewContent = m_pViewInner->getViewContentObj();
// 	m_pScrollBarV->getValRange( nValMin, nValMax );
// 
// 	nPosMove = m_pViewInner->getContentYPosLT();
// 	nContentHeight = pViewContent->getHeight();
// 	return ( nValMax - nValMin )*nPosMove/nContentHeight;
// }

//void YBScrollWnd::_onWM_TIMER( YBCtrlWndMsg* pYBCtrlWndMsg ) {
//	YBCTRLMSG_ENTER_CHECK( WM_TIMER );
//	ybctrl::YBViewContentBase* pViewContent = NULL;
//	RECT rcViewClient;
//	unsigned int uViewWndHeight;
//	unsigned int uStep = 10;
//
//	::GetClientRect( m_pViewInner->getHWND(), &rcViewClient );
//	uViewWndHeight = rcViewClient.bottom - rcViewClient.top;
//	pViewContent = m_pViewInner->getViewContentObj();
//	//
//	if ( m_pViewInner->getContentYPosLT() == pViewContent->getHeight() ) {
//		m_pViewInner->setContentYPosLT( 0 );
//	} else if ( m_pViewInner->getContentYPosLT() + uStep >= pViewContent->getHeight() ) {
//		m_pViewInner->setContentYPosLT( pViewContent->getHeight() );
//	} else {
//		m_pViewInner->setContentYPosLT( m_pViewInner->getContentYPosLT() + uStep );
//	}
//	
//	InvalidateRect( m_pViewInner->getHWND(), NULL, TRUE );
//}