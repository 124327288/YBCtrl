#include "stdafx.h"
#include "progressbar/YBProgressCtrl.h"
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlImageBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlMisc.h"
#include <assert.h>
//
#define YBCTRL_PROGROSSCTRL_BUTTON (258)
#define YBPC_FLAG_NONE (0x00000000)
#define YBPC_FLAG_LBTNDOWN (0x00000001)
USING_NAMESPACE_YBCTRL
	
YBProgressCtrl::YBProgressCtrl()
: m_uFlag( YBPC_FLAG_NONE )
, m_wordCtrlID(0)
, m_uRFixed(0)
, m_uLFixed(0)
, m_uTFixed(2)
, m_uBFixed(2)
, m_unForeGndPtX(0)
, m_nMinPos(0)
, m_nMaxPos(100)
, m_hWndSetCaptureOld( NULL )
, m_uThumbWidth( 16 )
, m_uLSpaced( 0 )
, m_uTSpaced( 0 )
, m_uRSpaced( 0 )
, m_uBSpaced( 0 )
{

}

YBProgressCtrl::~YBProgressCtrl(){

}

YBCTRL_WNDMSGMAP_BEGIN( YBProgressCtrl, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_LBUTTONUP, _onWM_LBUTTONUP )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
	YBCTRL_ONWNDMSG( WM_MOUSELEAVE, _onWM_MOUSELEAVE )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_SETCURSOR, _onWM_SETCURSOR )
	YBCTRL_WNDMSGMAP_END()

	void YBProgressCtrl::_onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg ){
		YBCTRLMSG_ENTER_CHECK( WM_PAINT );
		HDC hPaintDC;
		RECT rcClient;
		PAINTSTRUCT ps;

		hPaintDC = ::BeginPaint( m_hWnd, &ps );
		::GetClientRect( m_hWnd, &rcClient );

		YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
		_drawStatus( memDC.getMemDC() );

		::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
		::EndPaint( m_hWnd, &ps );

}

void YBProgressCtrl::_onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );

}

void YBProgressCtrl::_onWM_SETCURSOR( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_SETCURSOR );

	::SetCursor( ::LoadCursor( NULL, IDC_HAND ) );
	
	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = TRUE;

}

void YBProgressCtrl::_onWM_MOUSEMOVE( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	HDC hDC;
	RECT rcClient;
	TRACKMOUSEEVENT trackMouseEvt;

	hDC = ::GetDC( m_hWnd );
	::GetClientRect( m_hWnd, &rcClient );

	__int16 xPos = (__int16)LOWORD(pYBCtrlWndMsg->m_lParam);

	if ( m_uFlag & YBPC_FLAG_LBTNDOWN ) {
		if( xPos > int(m_uLSpaced) ){
			if( xPos <__int16( rcClient.right - rcClient.left - m_uRSpaced ) ){
				m_unForeGndPtX = xPos - m_uLSpaced;
			}else{
				m_unForeGndPtX = rcClient.right - rcClient.left - m_uRSpaced - m_uLSpaced;
			}
		}else{
			m_unForeGndPtX = 0;
		}
		//
		WPARAM wParam;
		LPARAM lParam;

		wParam = MAKELONG( m_wordCtrlID, YBBTNX_CMDEVT_BTNDOWN );
		lParam = (LPARAM)m_hWnd;
		::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
	}else{
		m_eThumbStatus = ETHUMB_STATUS_HOVER;
	}

	trackMouseEvt.cbSize = sizeof( TRACKMOUSEEVENT );
	trackMouseEvt.dwFlags = TME_LEAVE;
	trackMouseEvt.hwndTrack = pYBCtrlWndMsg->m_hWnd;
	trackMouseEvt.dwHoverTime = HOVER_DEFAULT;
	::TrackMouseEvent ( &trackMouseEvt );

	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	
	_drawStatus( memDC.getMemDC() );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );

}

void YBProgressCtrl::_onWM_MOUSELEAVE( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSELEAVE );
	HDC hdc;
	RECT rcClient;

	m_uFlag &= ~YBPC_FLAG_LBTNDOWN;
	m_eThumbStatus = ETHUMB_STATUS_NORMAL;

	hdc = ::GetDC( m_hWnd );
	::GetClientRect( m_hWnd, &rcClient );

	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );
	::BitBlt( hdc, 0, 0, rcClient.right - rcClient.left, ( rcClient.bottom - rcClient.top ), memDC.getMemDC(), 0, 0, SRCCOPY );

	::ReleaseDC( m_hWnd, hdc );
}

void YBProgressCtrl::_drawStatus(HDC hDC) {

	_drawBK( hDC );
	_drawForeGnd( hDC );
	_drawThumbBtn( hDC );
}

void YBProgressCtrl::_onWM_LBUTTONUP( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONUP );
	RECT rcClient;
	HDC hdc;
	POINT pt;

	pt.x = (__int16)LOWORD( pYBCtrlWndMsg->m_lParam );
	pt.y = HIWORD( pYBCtrlWndMsg->m_lParam );

	::GetClientRect( m_hWnd, &rcClient );
	m_uFlag &= ~YBPC_FLAG_LBTNDOWN;
	if ( ::PtInRect( &rcClient, pt ) ){
		m_eThumbStatus = ETHUMB_STATUS_HOVER;
	}else{
		m_eThumbStatus = ETHUMB_STATUS_NORMAL;
	}
	::SetCapture( m_hWndSetCaptureOld );

	hdc = ::GetDC( m_hWnd );
	::GetClientRect( m_hWnd, &rcClient );

	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );
	::BitBlt( hdc, 0, 0, rcClient.right - rcClient.left, ( rcClient.bottom - rcClient.top ), memDC.getMemDC(), 0, 0, SRCCOPY );

	::ReleaseDC( m_hWnd, hdc );
	::ReleaseCapture();
}

void YBProgressCtrl::_onWM_LBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
	HDC hdc;
	RECT rcClient;
	//
	m_uFlag |= YBPC_FLAG_LBTNDOWN;
	m_eThumbStatus = ETHUMB_STATUS_PRESS;
	//

	::GetClientRect( m_hWnd, &rcClient );

	__int16 xPos = (__int16)LOWORD( pYBCtrlWndMsg->m_lParam );
	if( xPos > __int16(m_uLSpaced) ){
		if( xPos < __int16( rcClient.right - rcClient.left - m_uRSpaced ) ){
			m_unForeGndPtX = xPos - m_uLSpaced;
		}else{
			m_unForeGndPtX = rcClient.right - rcClient.left - m_uLSpaced - m_uRSpaced;
		}
	}else{
		m_unForeGndPtX = 0;
	}
	//
	hdc = ::GetDC( m_hWnd );

	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );
	::BitBlt( hdc, 0, 0, rcClient.right - rcClient.left, ( rcClient.bottom - rcClient.top ), memDC.getMemDC(), 0, 0, SRCCOPY );
	
	::ReleaseDC( m_hWnd, hdc );

	WPARAM wParam;
	LPARAM lParam;

	wParam = MAKELONG( m_wordCtrlID, YBBTNX_CMDEVT_BTNDOWN );
	lParam = (LPARAM)m_hWnd;
	::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );

	m_hWndSetCaptureOld = ::SetCapture( m_hWnd );
}

void YBProgressCtrl::_drawThumbBtn( HDC hDC ){
	if( !getHWND() )
		return;
	RECT rcClient;
	unsigned int uImageIdThumb;
	unsigned int uProCtrlWidth;
	YBCtrlImageStatic* pYBImage = NULL;
	//
	::GetClientRect( m_hWnd, &rcClient );

	if ( !_getImageId( m_eThumbStatus, uImageIdThumb ) ) {
		return;
	}
	pYBImage = _getImageStatic( uImageIdThumb );
	if ( !pYBImage )
		return;
	// thumb width
	m_uThumbWidth = pYBImage->getWidth();
	if( m_unForeGndPtX < m_uThumbWidth/2 ){
		uProCtrlWidth = m_uLSpaced;
	} else if( m_unForeGndPtX > ( rcClient.right - rcClient.left - m_uLSpaced - m_uRSpaced - m_uThumbWidth/2 ) ) {
		uProCtrlWidth = rcClient.right - rcClient.left - m_uLSpaced - m_uRSpaced - m_uThumbWidth;
	}else{
		uProCtrlWidth = m_unForeGndPtX + m_uLSpaced - m_uThumbWidth/2;
	}
	BackGroundBitBlt( hDC, uProCtrlWidth, m_uTSpaced, m_uThumbWidth, ( rcClient.bottom - rcClient.top - m_uTSpaced - m_uBSpaced ), pYBImage->getMemDC(),
		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), m_uLFixed, m_uTFixed, m_uRFixed, m_uBFixed );

}

void YBProgressCtrl::_drawForeGnd( HDC hDC ){
	if( !getHWND() )
		return;

	RECT rcClient;
	YBCtrlImageStatic* pYBImage = NULL;
	::GetClientRect( m_hWnd, &rcClient );

	pYBImage = _getImageStatic( m_uImageIdFG );
	if ( !pYBImage ) {
		return;
	}

	BackGroundBitBlt( hDC, m_uLSpaced, m_uTSpaced, m_unForeGndPtX, ( rcClient.bottom - rcClient.top - m_uTSpaced - m_uBSpaced ), pYBImage->getMemDC(),
		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), m_uLFixed, m_uTFixed, m_uRFixed, m_uBFixed );

}

HWND YBProgressCtrl::Create( DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance ){
	HWND hWnd;

	dwStyle|=WS_CLIPCHILDREN;
	hWnd = _createHWND( 0, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, NULL );
	if( !hWnd ){
		assert(false);
	}
	
	m_wordCtrlID = (unsigned int)hMenu;
	return hWnd;
}

void YBProgressCtrl::SetImageId( unsigned int uImageIdBK, unsigned int uForGndImageId ){
	m_uImageIdBK = uImageIdBK;
	m_uImageIdFG  = uForGndImageId;

}

void YBProgressCtrl::SetImageId( ENUMTHUMBSTATUS eThumbStatus, unsigned int uImageId ){
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

bool YBProgressCtrl::_getImageId( ENUMTHUMBSTATUS eThumbStatus, unsigned int& uImageId ) {
	switch ( eThumbStatus ) 
	{
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
			assert( false );
			return false;
		}
		break;
	}

	return true;
}

YBCtrlImageBase* YBProgressCtrl::_getImageBase( unsigned int uImageId ){
	YBCtrlImageBase* pYBImage = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pYBImage ) )
		return NULL;

	return pYBImage;
}

YBCtrlImageStatic* YBProgressCtrl::_getImageStatic( unsigned int uImageId ){
	YBCtrlImageBase* pYBImageBase = NULL;

	pYBImageBase = _getImageBase( uImageId );
	if( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;
	return (YBCtrlImageStatic*)pYBImageBase;

}

void YBProgressCtrl::_drawBK( HDC hDC){
	if( !getHWND() )
		return;

	RECT rcClient;
	YBCtrlImageStatic* pYBImageBK = NULL;
	::GetClientRect( m_hWnd, &rcClient );

	pYBImageBK = _getImageStatic( m_uImageIdBK );
	if ( !pYBImageBK ) {
		return;
	}
	//
	BackGroundBitBlt( hDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top ), pYBImageBK->getMemDC(),
		0, 0, pYBImageBK->getWidth(), pYBImageBK->getHeight(), m_uLFixed, m_uTFixed, m_uRFixed, m_uBFixed );

}

void YBProgressCtrl::setPos( unsigned int nPos ){
	if( !getHWND() ) {
		assert(false);
		return;
	}

	RECT rcClient;
	::GetClientRect( m_hWnd, &rcClient );

	double Fraction = (double)( nPos  - m_nMinPos )/( (double)( m_nMaxPos - m_nMinPos ) );
	int nBarWidth = (int)( Fraction*( rcClient.right - rcClient.left - m_uLSpaced - m_uRSpaced ) );
	if( nBarWidth != m_unForeGndPtX ){
		m_unForeGndPtX = nBarWidth;
	}
	return;
}

void YBProgressCtrl::setRange( int nLower, int nUpper ){
	m_nMinPos = nLower;
	m_nMaxPos = nUpper;
}

unsigned int YBProgressCtrl::GetPos(){
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	unsigned int uForeGndPtX = m_unForeGndPtX*100/(rcClient.right - rcClient.left - m_uLSpaced - m_uRSpaced );
	if(uForeGndPtX > (unsigned int)m_nMaxPos) {
		uForeGndPtX = m_nMaxPos;
	} else if(uForeGndPtX < (unsigned int)m_nMinPos) {
		uForeGndPtX = m_nMinPos;
	}

	return uForeGndPtX;
}

void YBProgressCtrl::SetProgressCtrlSpaced( unsigned int uLSpaced, unsigned int uTSpaced, unsigned int uRSpaced, unsigned int uBSpaced ){
	m_uLSpaced = uLSpaced;
	m_uTSpaced = uTSpaced;
	m_uRSpaced = uRSpaced;
	m_uBSpaced = uBSpaced;
}