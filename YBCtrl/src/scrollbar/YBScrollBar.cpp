#include "stdafx.h"
#include "scrollbar/YBScrollBar.h"
#include "frame/YBCtrlImageDepository.h"
#include <assert.h>
#include "frame/YBCtrlMisc.h"
#include "frame/YBCtrlMemDC.h"

#define YBSB_FLAG_NONE (0x00000000)
#define YBSB_FLAG_STARTBTNDOWN (0x00000001)
#define YBSB_FLAG_ENDBTNDOWN (0x00000002)
#define YBSB_FLAG_THUMBBTNDOWN (0x00000004)

USING_NAMESPACE_YBCTRL

YBScrollBar::YBScrollBar( ENUMDIRECTIONTYPE eDirectType, unsigned int uLenStartBtn, unsigned int uLenEndBtn )
: m_eDirectType( eDirectType )
, m_eStartArrowStatus( EARROW_STATUS_NORMAL )
, m_eEndArrowStatus( EARROW_STATUS_NORMAL )
, m_eThumbStatus( ETHUMB_STATUS_NORMAL )
, m_uImageIdBk( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdThumbNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdThumbHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdThumbPre( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdStartArrNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdStartArrOutHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdStartArrInHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdStartArrPre( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdEndArrNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdEndArrOutHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdEndArrInHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdEndArrPre( YBCTRL_IMAGE_ID_INVALID )
, m_uLenThumbBtn( 30 )
, m_uLenStartBtn( uLenStartBtn )
, m_uLenEndBtn( uLenEndBtn )
, m_uFlag( YBSB_FLAG_NONE )
, m_nValMin( 0 )
, m_nValMax( 100 )
, m_nValCur( 0 )
, m_nValLBtnDownOnThumb( 0 )
{
	::memset( &m_ptLBtnDownOnThumb, 0, sizeof( POINT ) );
}

YBScrollBar::~YBScrollBar() {

}

YBCTRL_WNDMSGMAP_BEGIN( YBScrollBar, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
	YBCTRL_ONWNDMSG( WM_MOUSELEAVE, _onWM_MOUSELEAVE )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_LBUTTONUP, _onWM_LBUTTONUP )
YBCTRL_WNDMSGMAP_END()

HWND YBScrollBar::create( int x, int y, int nWidth, int nHeight, HWND hWndParent, WORD wCtrlID, HINSTANCE hInstance ) {
	DWORD dwExStyle;
	DWORD dwStyle;

	dwExStyle = 0;
	dwStyle = WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	m_wordCtrlId = wCtrlID;

	return _createHWND( dwExStyle, dwStyle, x, y, nWidth, nHeight, hWndParent, (HMENU)wCtrlID, hInstance, NULL );
}

void YBScrollBar::setValRange( int nValMin, int nValMax ) {
	m_nValMin = nValMin;
	m_nValMax = nValMax;
}

void YBScrollBar::getValRange( int& nValMin, int& nValMax ) {
	nValMin = m_nValMin;
	nValMax = m_nValMax;
}

int YBScrollBar::getTotalLen() const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	switch ( m_eDirectType ) {
	case YBScrollBar::EDIRECTION_HORIZONTAL:
		{
			if ( ( rcClient.right - rcClient.left ) < (int)( m_uLenStartBtn + m_uLenEndBtn ) ) {
				return 0;
			}
			return ( rcClient.right - rcClient.left ) - ( m_uLenStartBtn + m_uLenEndBtn );
		}
		break;
	case YBScrollBar::EDIRECTION_VERTICAL:
		{
			if ( ( rcClient.bottom - rcClient.top ) < (int)( m_uLenStartBtn + m_uLenEndBtn ) ) {
				return 0;
			}
			return ( rcClient.bottom - rcClient.top ) - ( m_uLenStartBtn + m_uLenEndBtn );
		}
		break;
	default:
		break;
	}

	assert( false );
	return 0;
}

int YBScrollBar::getPosRange() const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	switch ( m_eDirectType ) {
	case YBScrollBar::EDIRECTION_HORIZONTAL:
		{
			if ( ( rcClient.right - rcClient.left ) < (int)( m_uLenStartBtn + m_uLenEndBtn + m_uLenThumbBtn ) ) {
				return 0;
			}
			return ( rcClient.right - rcClient.left ) - ( m_uLenStartBtn + m_uLenEndBtn + m_uLenThumbBtn );
		}
		break;
	case YBScrollBar::EDIRECTION_VERTICAL:
		{
			if ( ( rcClient.bottom - rcClient.top ) < (int)( m_uLenStartBtn + m_uLenEndBtn + m_uLenThumbBtn ) ) {
				return 0;
			}
			return ( rcClient.bottom - rcClient.top ) - ( m_uLenStartBtn + m_uLenEndBtn + m_uLenThumbBtn );
		}
		break;
	default:
		break;
	}

	assert( false );
	return 0;
}

void YBScrollBar::_onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg ) {
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

void YBScrollBar::_onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	RECT rcClient;
	HDC hPaintDC;
	PAINTSTRUCT ps;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void YBScrollBar::_onWM_MOUSEMOVE( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	RECT rcClient;
	RECT rcBtnStart;
	RECT rcBtnEnd;
	RECT rcBtnThumb;
	HDC hDC;
	POINT ptMouse;
	TRACKMOUSEEVENT trackMouseEvt;

	::GetClientRect( m_hWnd, &rcClient );
	ptMouse.x = (__int16)LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = (__int16)HIWORD( pYBCtrlWndMsg->m_lParam );
	if ( !_getStartBtnRect( rcBtnStart ) ) {
		assert( false );
		return;
	}
	if ( !_getEndBtnRect( rcBtnEnd ) ) {
		assert( false );
		return;
	}
	if ( !_getThumbBtnRect( rcBtnThumb ) ) {
		assert( false );
		return;
	}

	if( m_uFlag & YBSB_FLAG_THUMBBTNDOWN ){
		m_eStartArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eEndArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eThumbStatus = ETHUMB_STATUS_PRESS;
		//
		_moveThumbBtn( ptMouse );
		//
		WPARAM wParam = (WPARAM)MAKELONG( m_wordCtrlId, YBSB_CMDEVT_THUMBMOVE );
		LPARAM lParam = (LPARAM)m_hWnd;
		::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );

	}else{
		//
		if( ::PtInRect( &rcBtnStart, ptMouse ) ){
			if( m_uFlag & YBSB_FLAG_STARTBTNDOWN ){
				m_eStartArrowStatus = EARROW_STATUS_PRESS;
				m_eEndArrowStatus = EARROW_STATUS_OUTHOVER;
			}else{
				m_eStartArrowStatus = EARROW_STATUS_INHOVER;
				m_eEndArrowStatus = EARROW_STATUS_OUTHOVER;
			}
			m_eThumbStatus = ETHUMB_STATUS_NORMAL;
		}else if( ::PtInRect( &rcBtnEnd, ptMouse )){
			if( m_uFlag & YBSB_FLAG_ENDBTNDOWN ){
				m_eStartArrowStatus = EARROW_STATUS_OUTHOVER;
				m_eEndArrowStatus = EARROW_STATUS_PRESS;
			}else{
				m_eStartArrowStatus = EARROW_STATUS_OUTHOVER;
				m_eEndArrowStatus = EARROW_STATUS_INHOVER;
			}
			m_eThumbStatus = ETHUMB_STATUS_NORMAL;
		}else if( ::PtInRect( &rcBtnThumb, ptMouse ) ){
			m_eStartArrowStatus = EARROW_STATUS_OUTHOVER;
			m_eEndArrowStatus = EARROW_STATUS_OUTHOVER;
			m_eThumbStatus = ETHUMB_STATUS_HOVER;
		}else{
				m_eStartArrowStatus = EARROW_STATUS_OUTHOVER;
				m_eEndArrowStatus = EARROW_STATUS_OUTHOVER;
				m_eThumbStatus = ETHUMB_STATUS_NORMAL;
		}
	}

	trackMouseEvt.cbSize = sizeof( TRACKMOUSEEVENT );
	trackMouseEvt.dwFlags = TME_LEAVE;
	trackMouseEvt.hwndTrack = pYBCtrlWndMsg->m_hWnd;
	trackMouseEvt.dwHoverTime = HOVER_DEFAULT;
	::TrackMouseEvent( &trackMouseEvt );
	//
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
}

void YBScrollBar::_moveThumbBtn( POINT ptMouseNow ) {
	assert( m_uFlag & YBSB_FLAG_THUMBBTNDOWN );
	switch ( m_eDirectType ) {
	case EDIRECTION_HORIZONTAL:
		{
			int nPosMove = ptMouseNow.x - m_ptLBtnDownOnThumb.x;
			int nPosRange = getPosRange();
			if ( nPosRange == 0 ) {
				m_nValCur = m_nValMin;
			} else {
				m_nValCur = m_nValLBtnDownOnThumb + ( m_nValMax - m_nValMin ) * nPosMove / nPosRange;
			}
			// 
			m_nValCur = max( m_nValMin, min( m_nValCur, m_nValMax ) );
			return;
		}
		break;
	case EDIRECTION_VERTICAL:
		{
			int nPosMove = ptMouseNow.y - m_ptLBtnDownOnThumb.y;
			int nPosRange = getPosRange();
			if ( nPosRange == 0 ) {
				m_nValCur = m_nValMin;
			} else {
				m_nValCur = m_nValLBtnDownOnThumb + ( m_nValMax - m_nValMin ) * nPosMove / nPosRange;
			}

			if ( m_nValCur > 10000 ) {
				int a  = 0;
			}
			{
				TCHAR szMsg[200];
				_stprintf_s( szMsg, _T("m_nValMin:%d, m_nValMax:%d, m_nValCur:%d, nPosRange:%d, ptMouseNow.y:%d, m_ptLBtnDownOnThumb.y:%d\n"), 
					m_nValMin, m_nValMax, m_nValCur, nPosRange, ptMouseNow.y, m_ptLBtnDownOnThumb.y );
				::OutputDebugString( szMsg );
			}

			// 
			m_nValCur = max( m_nValMin, min( m_nValCur, m_nValMax ) );

			return;
		}
		break;
	default:
		break;
	}

	assert( false );
}

void YBScrollBar::_onWM_MOUSELEAVE( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSELEAVE );
	RECT rcClient;
	HDC hDC;
	
	::GetClientRect( m_hWnd, &rcClient );
	if ( m_uFlag & YBSB_FLAG_THUMBBTNDOWN ) {
		m_eStartArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eEndArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eThumbStatus = ETHUMB_STATUS_PRESS;
	} else if ( m_uFlag & YBSB_FLAG_STARTBTNDOWN ) {
		m_eStartArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eEndArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eThumbStatus = ETHUMB_STATUS_NORMAL;
		//
		m_uFlag &= ~( YBSB_FLAG_STARTBTNDOWN|YBSB_FLAG_ENDBTNDOWN|YBSB_FLAG_THUMBBTNDOWN );
	} else if ( m_uFlag & YBSB_FLAG_ENDBTNDOWN ) {
		m_eStartArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eEndArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eThumbStatus = ETHUMB_STATUS_NORMAL;
		//
		m_uFlag &= ~( YBSB_FLAG_STARTBTNDOWN|YBSB_FLAG_ENDBTNDOWN|YBSB_FLAG_THUMBBTNDOWN );
	} else {
		m_eStartArrowStatus = EARROW_STATUS_NORMAL;
		m_eEndArrowStatus = EARROW_STATUS_NORMAL;
		m_eThumbStatus = ETHUMB_STATUS_NORMAL;
		//
		m_uFlag &= ~( YBSB_FLAG_STARTBTNDOWN|YBSB_FLAG_ENDBTNDOWN|YBSB_FLAG_THUMBBTNDOWN );
	}
	
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
}

void YBScrollBar::_onWM_LBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
	RECT rcClient;
	RECT rcBtnStart;
	RECT rcBtnEnd;
	RECT rcBtnThumb;
	RECT rcStartSpace;
	RECT rcEndSpace;
	POINT ptMouse;
	HDC hDC;
	WPARAM wParam;
	LPARAM lParam;

	::GetClientRect( m_hWnd, &rcClient );
	ptMouse.x = (__int16)LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = (__int16)HIWORD( pYBCtrlWndMsg->m_lParam );
	if ( !_getStartBtnRect( rcBtnStart ) ) {
		assert( false );
		return;
	}
	if ( !_getEndBtnRect( rcBtnEnd ) ) {
		assert( false );
		return;
	}
	if ( !_getThumbBtnRect( rcBtnThumb ) ) {
		assert( false );
		return;
	}
	if( !_getStartSpaceRect( rcStartSpace ) ){
		assert(false);
		return;
	}
	if( !_getEndSpaceRect( rcEndSpace ) ){
		assert(false);
		return;
	}
	//
	if( ::PtInRect( &rcBtnStart, ptMouse ) ){
		m_eStartArrowStatus = EARROW_STATUS_PRESS;
		m_eEndArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eThumbStatus = ETHUMB_STATUS_NORMAL;
		m_uFlag |= YBSB_FLAG_STARTBTNDOWN;
		//
		wParam = (WPARAM)MAKELONG( m_wordCtrlId, YBSB_CMDEVT_STARTBTNDOWN );
		lParam = (LPARAM)m_hWnd;
		::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );

	}else if( ::PtInRect( &rcBtnEnd, ptMouse ) ){
		m_eStartArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eEndArrowStatus = EARROW_STATUS_PRESS;
		m_eThumbStatus = ETHUMB_STATUS_NORMAL;
		m_uFlag |= YBSB_FLAG_ENDBTNDOWN;
		//
		wParam = (WPARAM)MAKELONG( m_wordCtrlId, YBSB_CMDEVT_ENDTBTNDOWN );
		lParam = (LPARAM)m_hWnd;
		::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
	}else if( ::PtInRect( &rcBtnThumb, ptMouse ) ){
		m_eStartArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eEndArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eThumbStatus = ETHUMB_STATUS_PRESS;
		m_uFlag |= YBSB_FLAG_THUMBBTNDOWN;
		//
		::SetCapture( m_hWnd );
		m_ptLBtnDownOnThumb = ptMouse;
		m_nValLBtnDownOnThumb = m_nValCur;
	}else if( ::PtInRect( &rcStartSpace, ptMouse )){
		m_eStartArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eEndArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eThumbStatus = ETHUMB_STATUS_NORMAL;

		wParam = (WPARAM)MAKELONG( m_wordCtrlId, YBSB_CMDEVT_STARTSPACEDOWN );
		lParam = (LPARAM)m_hWnd;
		::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
	}else{
		m_eStartArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eEndArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eThumbStatus = ETHUMB_STATUS_NORMAL;

		wParam = (WPARAM)MAKELONG( m_wordCtrlId, YBSB_CMDEVT_ENDSPACEDOWN );
		lParam = (LPARAM)m_hWnd;
		::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
	}

	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
}

void YBScrollBar::_onWM_LBUTTONUP( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONUP );
	RECT rcClient;
	RECT rcBtnStart;
	RECT rcBtnEnd;
	RECT rcBtnThumb;
	HDC hDC;
	POINT ptMouse;

	::ReleaseCapture();
	::GetClientRect( m_hWnd, &rcClient );
	ptMouse.x = (__int16)LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = (__int16)HIWORD( pYBCtrlWndMsg->m_lParam );
	if ( !_getStartBtnRect( rcBtnStart ) ) {
		assert( false );
		return;
	}
	if ( !_getEndBtnRect( rcBtnEnd ) ) {
		assert( false );
		return;
	}
	if ( !_getThumbBtnRect( rcBtnThumb ) ) {
		assert( false );
		return;
	}
	// 清除之前的按下的标志
	m_uFlag &= ~( YBSB_FLAG_STARTBTNDOWN | YBSB_FLAG_ENDBTNDOWN | YBSB_FLAG_THUMBBTNDOWN );
	// 
	if( ::PtInRect( &rcBtnStart, ptMouse ) ){
		m_eStartArrowStatus = EARROW_STATUS_INHOVER;
		m_eEndArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eThumbStatus = ETHUMB_STATUS_NORMAL;
	}else if( ::PtInRect( &rcBtnEnd, ptMouse )){
		m_eStartArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eEndArrowStatus = EARROW_STATUS_INHOVER;
		m_eThumbStatus = ETHUMB_STATUS_NORMAL;
	}else if( ::PtInRect( &rcBtnThumb, ptMouse )){
		m_eStartArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eEndArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eThumbStatus = ETHUMB_STATUS_HOVER;
	}else{
		m_eStartArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eEndArrowStatus = EARROW_STATUS_OUTHOVER;
		m_eThumbStatus = ETHUMB_STATUS_NORMAL;
	}

	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );

	//::SetCapture( m_hWndSetCapture );
}

void YBScrollBar::_drawStatus( HDC hDC ){
	_drawBk( hDC );
	_drawStartArrow( hDC );
	_drawEndArrow( hDC );
	_drawThumb( hDC );
}

void YBScrollBar::_drawBk( HDC hDC ){
	RECT rcClient;
	
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlImageStatic* pYBImageBk = NULL;
	pYBImageBk = _getImageStatic( m_uImageIdBk );
	if ( !pYBImageBk ) {
		return;
	}
	//
	BackGroundBitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pYBImageBk->getMemDC(),
		0, 0, pYBImageBk->getWidth(), pYBImageBk->getHeight(), 0, 0, 0, 0 );
}

void YBScrollBar::_drawStartArrow( HDC hDC ){
	unsigned int uImageId;
	RECT rcStartArrow;
	YBCtrlImageStatic* pYBImage = NULL;

	if( !_getImageIdStartArrow( m_eStartArrowStatus, uImageId ) ){
		assert(false);
		return;
	}
	pYBImage = _getImageStatic( uImageId );
	if ( !pYBImage ) {
		return;
	}

	if ( !_getStartBtnRect( rcStartArrow ) ){
		assert(false);
		return;
	}

	BackGroundBitBlt( hDC, rcStartArrow.left, rcStartArrow.top, rcStartArrow.right - rcStartArrow.left,
		rcStartArrow.bottom - rcStartArrow.top, pYBImage->getMemDC(), 0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0 );
}

void YBScrollBar::_drawEndArrow( HDC hDC ){
	unsigned int uImageId;
	RECT rcEndArrow;
	YBCtrlImageStatic* pYBImage = NULL;

	if( !_getImageIdEndArrow( m_eEndArrowStatus, uImageId ) ){
		assert(false);
		return;
	}
	pYBImage = _getImageStatic( uImageId );
	if ( !pYBImage ) {
		return;
	}
	if ( !_getEndBtnRect( rcEndArrow ) ){
		assert(false);
		return;
	}

	BackGroundBitBlt( hDC, rcEndArrow.left, rcEndArrow.top, rcEndArrow.right - rcEndArrow.left, rcEndArrow.bottom - rcEndArrow.top,
		pYBImage->getMemDC(), 0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0 );
}

void YBScrollBar::_drawThumb( HDC hDC ){
	unsigned int uImageId;
	YBCtrlImageStatic* pYBImage = NULL;
	RECT rcClient;
	RECT rcThumbBtn;

	::GetClientRect( m_hWnd, &rcClient );
	if ( !_getImageIdThumb( m_eThumbStatus, uImageId ) ) {
		return;
	}
	pYBImage = _getImageStatic( uImageId );
	if ( !pYBImage ) {
		return;
	}
	if ( !_getThumbBtnRect( rcThumbBtn ) ) {
		return;
	}

	BackGroundBitBlt( hDC, rcThumbBtn.left, rcThumbBtn.top, rcThumbBtn.right - rcThumbBtn.left, rcThumbBtn.bottom - rcThumbBtn.top,
		pYBImage->getMemDC(), 0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 3, 3, 3, 3 );
}

bool YBScrollBar::_getThumbBtnRect( RECT& rcThumbBtn ){
	RECT rcClient;
	int nPosRange;

	nPosRange = getPosRange();
	::GetClientRect( m_hWnd, &rcClient );
	switch ( m_eDirectType ) {
	case YBScrollBar::EDIRECTION_HORIZONTAL:
		{
			rcThumbBtn.top = rcClient.top;
			rcThumbBtn.bottom = rcClient.bottom;
			if ( m_nValMax == m_nValMin  ) {
				rcThumbBtn.left = rcClient.left + m_uLenStartBtn;
			} else {
				rcThumbBtn.left = rcClient.left + m_uLenStartBtn + nPosRange * ( m_nValCur - m_nValMin ) / ( m_nValMax - m_nValMin );
			}
			rcThumbBtn.right = rcThumbBtn.left + m_uLenThumbBtn;
			return true;
		}
		break;
	case YBScrollBar::EDIRECTION_VERTICAL:
		{
			rcThumbBtn.left = rcClient.left;
			rcThumbBtn.right = rcClient.right;
			if ( m_nValMax == m_nValMin ) {
				rcThumbBtn.top = rcClient.top + m_uLenStartBtn;
			} else {
				rcThumbBtn.top = rcClient.top + m_uLenStartBtn + nPosRange * ( m_nValCur - m_nValMin ) / ( m_nValMax - m_nValMin );
			}
			rcThumbBtn.bottom = rcThumbBtn.top + m_uLenThumbBtn;
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

bool YBScrollBar::_getStartBtnRect( RECT& rcStartBtn ){
	RECT rcClient;
	
	::GetClientRect( m_hWnd, &rcClient );
	switch( m_eDirectType ){
	case EDIRECTION_HORIZONTAL:
		{
			rcStartBtn.left = rcClient.left;
			rcStartBtn.right = rcClient.left + m_uLenStartBtn;
			rcStartBtn.top = rcClient.top;
			rcStartBtn.bottom = rcClient.bottom;
		}
		break;
	case EDIRECTION_VERTICAL:
		{
			rcStartBtn.left = rcClient.left;
			rcStartBtn.right = rcClient.right;
			rcStartBtn.top = rcClient.top;
			rcStartBtn.bottom = rcClient.top + m_uLenStartBtn;
		}
		break;
	default:
		{
			assert(false);
			return false;
		}
	}
	return ( ( rcStartBtn.right >= rcStartBtn.left ) && ( rcStartBtn.bottom >= rcStartBtn.top ) );
}

bool YBScrollBar::_getStartSpaceRect( RECT& rcStartSpace ){
	RECT rcStartBtn;
	RECT rcThumbBtn;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	if( !_getStartBtnRect( rcStartBtn ) ){
		assert(false);
		return false;
	}
	if( !_getThumbBtnRect( rcThumbBtn ) ){
		assert(false);
		return false;
	}
	switch( m_eDirectType ){
	case EDIRECTION_HORIZONTAL:
		{
			rcStartSpace.left = rcStartBtn.right;
			rcStartSpace.right = rcThumbBtn.left;
			rcStartSpace.top = rcClient.top;
			rcStartSpace.bottom = rcClient.bottom;
		}
		break;
	case EDIRECTION_VERTICAL:
		{
			rcStartSpace.left = rcClient.left;
			rcStartSpace.right = rcClient.right;
			rcStartSpace.top = rcStartBtn.bottom;
			rcStartSpace.bottom = rcThumbBtn.top;
		}
		break;
	default:
		{
			assert(false);
			return false;
		}
	}
	return ( ( rcStartSpace.right >= rcStartSpace.left ) && ( rcStartSpace.bottom >= rcStartSpace.top ) );
}

bool YBScrollBar::_getEndSpaceRect( RECT& rcEndSpace ){
	RECT rcClient;
	RECT rcThumbBtn;
	RECT rcEndBtn;

	::GetClientRect( m_hWnd, &rcClient );
	if( !_getThumbBtnRect( rcThumbBtn ) ){
		assert(false);
		return false;
	}
	if( !_getEndBtnRect( rcEndBtn ) ){
		assert(false);
		return false;
	}
	switch( m_eDirectType ){
	case EDIRECTION_HORIZONTAL:
		{
			rcEndSpace.left = rcThumbBtn.right;
			rcEndSpace.right = rcEndBtn.left;
			rcEndSpace.top = rcClient.top;
			rcEndSpace.bottom = rcClient.bottom;
		}
		break;
	case EDIRECTION_VERTICAL:
		{
			rcEndSpace.left = rcClient.left;
			rcEndSpace.right = rcClient.right;
			rcEndSpace.top = rcThumbBtn.bottom;
			rcEndSpace.bottom = rcEndBtn.top;
		}
		break;
	default:
		{
			assert(false);
			return false;
		}
	}
	return ( ( rcEndSpace.right >= rcEndSpace.left ) && ( rcEndSpace.bottom >= rcEndSpace.top ) );
	
}

bool YBScrollBar::_getEndBtnRect( RECT& rcEndBtn ){
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	switch( m_eDirectType ){
	case EDIRECTION_HORIZONTAL:
		{
			rcEndBtn.left = rcClient.right - m_uLenEndBtn;
			rcEndBtn.right = rcClient.right;
			rcEndBtn.top = rcClient.top;
			rcEndBtn.bottom = rcClient.bottom;
		}
		break;
	case EDIRECTION_VERTICAL:
		{
			rcEndBtn.left = rcClient.left;
			rcEndBtn.right = rcClient.right;
			rcEndBtn.top = rcClient.bottom - m_uLenEndBtn;
			rcEndBtn.bottom = rcClient.bottom;
		}
		break;
	default:
		{
			assert(false);
			return false;
		}
	}
	return ( ( rcEndBtn.right >= rcEndBtn.left ) && ( rcEndBtn.bottom >= rcEndBtn.top ) );
}

void YBScrollBar::setImageIdBk( unsigned int uImageId ){
	m_uImageIdBk = uImageId;
}

void YBScrollBar::setImageIdStartArrow( EARROWSTATUS eArrowStatus, unsigned int uImageId ){
	switch( eArrowStatus ){
	case EARROW_STATUS_NORMAL:
		{
			m_uImageIdStartArrNor = uImageId;
		}
		break;
	case EARROW_STATUS_OUTHOVER:
		{
			m_uImageIdStartArrOutHov = uImageId;
		}
		break;
	case EARROW_STATUS_INHOVER:
		{
			m_uImageIdStartArrInHov = uImageId;
		}
		break;
	case EARROW_STATUS_PRESS:
		{
			m_uImageIdStartArrPre = uImageId;
		}
		break;
	default:
		break;
	}
}

bool YBScrollBar::_getImageIdStartArrow( EARROWSTATUS eArrowStatus, unsigned int& uImageId ){
	switch( eArrowStatus ){
	case EARROW_STATUS_NORMAL:
		{
			uImageId = m_uImageIdStartArrNor;
		}
		break;
	case EARROW_STATUS_OUTHOVER:
		{
			uImageId = m_uImageIdStartArrOutHov;
		}
		break;
	case EARROW_STATUS_INHOVER:
		{
			uImageId = m_uImageIdStartArrInHov;
		}
		break;
	case EARROW_STATUS_PRESS:
		{
			uImageId = m_uImageIdStartArrPre;
		}
		break;
	default:
		{
			assert(false);
			return false;
		}
	}
	return true;
}

void YBScrollBar::setImageIdEndArrow( EARROWSTATUS eArrowStatus, unsigned int uImageId ){
	switch( eArrowStatus ){
	case EARROW_STATUS_NORMAL:
		{
			m_uImageIdEndArrNor = uImageId;
		}
		break;
	case EARROW_STATUS_OUTHOVER:
		{
			m_uImageIdEndArrOutHov = uImageId;
		}
		break;
	case EARROW_STATUS_INHOVER:
		{
			m_uImageIdEndArrInHov = uImageId;
		}
		break;
	case EARROW_STATUS_PRESS:
		{
			m_uImageIdEndArrPre = uImageId;
		}
		break;
	default:
		break;
	}
}

bool YBScrollBar::_getImageIdEndArrow( EARROWSTATUS eArrowStatus, unsigned int& uImageId ){
	switch( eArrowStatus ){
	case EARROW_STATUS_NORMAL:
		{
			uImageId = m_uImageIdEndArrNor;
		}
		break;
	case EARROW_STATUS_OUTHOVER:
		{
			uImageId = m_uImageIdEndArrOutHov;
		}
		break;
	case EARROW_STATUS_INHOVER:
		{
			uImageId = m_uImageIdEndArrInHov;
		}
		break;
	case EARROW_STATUS_PRESS:
		{
			uImageId = m_uImageIdEndArrPre;
		}
		break;
	default:
		{
			assert(false);
			return false;
		}
	}
	return true;
}

void YBScrollBar::setImageIdThumb( ENUMTHUMBSTATUS eThumbStatus, unsigned int uImageId ){
	switch( eThumbStatus ){
	case ETHUMB_STATUS_NORMAL:
		{
			m_uImageIdThumbNor = uImageId;
		}
		break;
	case ETHUMB_STATUS_HOVER:
		{
			m_uImageIdThumbHov = uImageId;
		}
		break;
	case ETHUMB_STATUS_PRESS:
		{
			m_uImageIdThumbPre = uImageId;
		}
		break;
	default:
		break;
	}
}

bool YBScrollBar::_getImageIdThumb( ENUMTHUMBSTATUS eThumbStatus, unsigned int& uImageId ){
	switch( eThumbStatus ){
	case ETHUMB_STATUS_NORMAL:
		{
			uImageId = m_uImageIdThumbNor;
		}
		break;
	case ETHUMB_STATUS_HOVER:
		{
			uImageId = m_uImageIdThumbHov;
		}
		break;
	case ETHUMB_STATUS_PRESS:
		{
			uImageId = m_uImageIdThumbPre;
		}
		break;
	default:
		{
			assert(false);
			return false;
		}
	}
	return true;
}

YBCtrlImageStatic* YBScrollBar::_getImageStatic( unsigned int uImageId ){
	YBCtrlImageBase* pImageBase = NULL;
	YBCtrlImageStatic* pImageStatic = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pImageBase ) )
		return NULL;
	if( !pImageBase || ( pImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pImageBase;
}

bool YBScrollBar::getThick( unsigned int& uScrollBarThick ) const {
	if ( !::IsWindow( m_hWnd ) )
		return false;
	RECT rcWnd;

	::GetWindowRect( m_hWnd, &rcWnd );
	if ( m_eDirectType == EDIRECTION_HORIZONTAL ) {
		uScrollBarThick = rcWnd.bottom - rcWnd.top;
		return true;
	} else if ( m_eDirectType == EDIRECTION_VERTICAL ) {
		uScrollBarThick = rcWnd.right - rcWnd.left;
		return true;
	} 

	assert( false );
	return false;
}

