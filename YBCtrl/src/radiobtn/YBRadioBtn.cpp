#include "stdafx.h"
#include "radiobtn/YBRadioBtn.h"
#include "frame/YBCtrlImageBase.h"
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlImageBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlMisc.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlFontDepository.h"
#include "frame/YBCtrlMemDC.h"
#include "gdiplus.h"
#include <assert.h>

#define YBRDBTN_FLAG_NONE (0x00000000)
#define YBRDBTN_FLAG_LBTDOWN (0x00000001)

USING_NAMESPACE_YBCTRL

YBRadioBtn::YBRadioBtn()
: m_uNoRdBtnIIdNormal(YBCTRL_IMAGE_ID_INVALID)
, m_uNoRdBtnIIdHover(YBCTRL_IMAGE_ID_INVALID)
, m_uNoRdBtnIIdPress(YBCTRL_IMAGE_ID_INVALID)
, m_uRdBtnIIdNormal(YBCTRL_IMAGE_ID_INVALID)
, m_uRdBtnIIdHover(YBCTRL_IMAGE_ID_INVALID)
, m_uRdBtnIIdPress(YBCTRL_IMAGE_ID_INVALID)
, m_uRdBtnIIdBK(YBCTRL_IMAGE_ID_INVALID)
, m_uRdBtnFlag(YBRDBTN_FLAG_NONE)
, m_eRadioBtnStatus(ENORADIOBTN_STATUS_NORMAL)
, m_uNoRdBtnIIdDisable( YBCTRL_IMAGE_ID_INVALID )
, m_uRdBtnIIdDisable( YBCTRL_IMAGE_ID_INVALID )
, m_uLFixed(5)
, m_uTFixed(5)
, m_uRFixed(5)
, m_uBFixed(5)
, m_uRdBtnIconSL(15)
, m_uRdBtnIconWidth(0)
, m_uRdBtnIconHeight(0)
, m_uTxtSpaceL( 0 )
, m_uTxtSpaceT( 0 )
, m_uTxtSpaceB( 0 )
, m_uTxtSpaceR( 0 )
, m_tstrText( _T("") )
, m_uFontId( YBCTRL_FONT_ID_INVALID )
, m_wordCtrlID(0)
{

}

YBRadioBtn::~YBRadioBtn(){

}

YBCTRL_WNDMSGMAP_BEGIN( YBRadioBtn, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_LBUTTONUP, _onWM_LBUTTONUP )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
	YBCTRL_ONWNDMSG( WM_MOUSELEAVE, _onWM_MOUSELEAVE )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
YBCTRL_WNDMSGMAP_END()

void YBRadioBtn::_onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg ) {
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

	void YBRadioBtn::_onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg ){
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

void YBRadioBtn::_onWM_LBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
	unsigned int uRdBtnFlagOld;

	uRdBtnFlagOld = m_uRdBtnFlag;
	m_uRdBtnFlag |= YBRDBTN_FLAG_LBTDOWN;
	//
	if ( ( m_eRadioBtnStatus == ERADIOBTN_STATUS_DISABLE ) || ( m_eRadioBtnStatus == ENORADIOBTN_STATUS_DISABLE ) )
		return;
	//
	if( m_eRadioBtnStatus == ENORADIOBTN_STATUS_HOVER ){
		m_eRadioBtnStatus = ENORADIOBTN_STATUS_PRESS;
	}else if( m_eRadioBtnStatus == ERADIOBTN_STATUS_HOVER ){
		m_eRadioBtnStatus = ERADIOBTN_STATUS_PRESS;
	}else{
		assert(false);
	}
	HDC hDC = NULL;

	hDC = ::GetDC( m_hWnd );
	_drawStatus( hDC );
	::ReleaseDC( m_hWnd, hDC );
	// send command message to parent window
	if ( ( uRdBtnFlagOld & YBRDBTN_FLAG_LBTDOWN ) != YBRDBTN_FLAG_LBTDOWN ) {
		WPARAM wParam;
		LPARAM lParam;

		wParam = MAKELONG( m_wordCtrlID, YBBTNX_CMDEVT_BTNXDOWN );
		lParam = (LPARAM)m_hWnd;
		::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
	}
}

void YBRadioBtn::_onWM_LBUTTONUP( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONUP );
	unsigned int uRdBtnFlagOld;
	
	uRdBtnFlagOld = m_uRdBtnFlag;
	m_uRdBtnFlag &= ~YBRDBTN_FLAG_LBTDOWN;
	//
	if ( ( m_eRadioBtnStatus == ERADIOBTN_STATUS_DISABLE ) || ( m_eRadioBtnStatus == ENORADIOBTN_STATUS_DISABLE ) )
		return;
	//

	if ( ( uRdBtnFlagOld & YBRDBTN_FLAG_LBTDOWN ) == YBRDBTN_FLAG_LBTDOWN ) {
		if( ( m_eRadioBtnStatus == ENORADIOBTN_STATUS_PRESS ) || ( m_eRadioBtnStatus == ENORADIOBTN_STATUS_HOVER ) ){
			m_eRadioBtnStatus = ERADIOBTN_STATUS_HOVER;
		}else if( ( m_eRadioBtnStatus == ERADIOBTN_STATUS_PRESS ) || ( m_eRadioBtnStatus == ERADIOBTN_STATUS_HOVER ) ){
			m_eRadioBtnStatus = ERADIOBTN_STATUS_HOVER;
		}else{
			assert(false);
		}
	} else {
		if ( m_eRadioBtnStatus == ENORADIOBTN_STATUS_HOVER ) {
			m_eRadioBtnStatus = ENORADIOBTN_STATUS_HOVER;
		}
	}

	HDC hDC = NULL;

	hDC = ::GetDC( m_hWnd );
	_drawStatus( hDC );
	::ReleaseDC( m_hWnd, hDC );

}

void YBRadioBtn::_onWM_MOUSEMOVE( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	TRACKMOUSEEVENT trackMouseEvent;
	HDC hDC = NULL;

	switch( m_eRadioBtnStatus )
	{
	case ENORADIOBTN_STATUS_DISABLE:
		{
			return;
		}
		break;
	case ERADIOBTN_STATUS_DISABLE:
		{
			return;
		}
		break;
	case ENORADIOBTN_STATUS_NORMAL:
		{
			if( m_uRdBtnFlag & YBRDBTN_FLAG_LBTDOWN ){
				m_eRadioBtnStatus = ENORADIOBTN_STATUS_PRESS;
			}else{
				m_eRadioBtnStatus = ENORADIOBTN_STATUS_HOVER;
			}
		}
		break;
	case ENORADIOBTN_STATUS_HOVER:
		{
			if( m_uRdBtnFlag & YBRDBTN_FLAG_LBTDOWN ){
				m_eRadioBtnStatus = ENORADIOBTN_STATUS_PRESS;
			}else{
				m_eRadioBtnStatus = ENORADIOBTN_STATUS_HOVER;
			}
		}
		break;
	case ENORADIOBTN_STATUS_PRESS:
		{
			if( m_uRdBtnFlag & YBRDBTN_FLAG_LBTDOWN ){
				m_eRadioBtnStatus = ENORADIOBTN_STATUS_PRESS;
			}else{
				m_eRadioBtnStatus = ERADIOBTN_STATUS_HOVER;
			}
		}
		break;
	case ERADIOBTN_STATUS_NORMAL:
		{
			if( m_uRdBtnFlag & YBRDBTN_FLAG_LBTDOWN ){
				m_eRadioBtnStatus = ERADIOBTN_STATUS_PRESS;
			}else{
				m_eRadioBtnStatus = ERADIOBTN_STATUS_HOVER;
			}
		}
		break;
	case ERADIOBTN_STATUS_HOVER:
		{
			if( m_uRdBtnFlag & YBRDBTN_FLAG_LBTDOWN ){
				m_eRadioBtnStatus = ERADIOBTN_STATUS_PRESS;
			}else{
				m_eRadioBtnStatus = ERADIOBTN_STATUS_HOVER;
			}
		}
		break;
	case ERADIOBTN_STATUS_PRESS:
		{
			if( m_uRdBtnFlag & YBRDBTN_FLAG_LBTDOWN ){
				m_eRadioBtnStatus = ERADIOBTN_STATUS_PRESS;
			}else{
				m_eRadioBtnStatus = ERADIOBTN_STATUS_HOVER;
			}
		}
		break;
	default:
		{
			assert( false );
			return;
		}
		break;
	}

	trackMouseEvent.cbSize = sizeof( TRACKMOUSEEVENT );
	trackMouseEvent.dwFlags = TME_LEAVE;
	trackMouseEvent.hwndTrack = pYBCtrlWndMsg->m_hWnd;
	trackMouseEvent.dwHoverTime = HOVER_DEFAULT;
	::TrackMouseEvent( &trackMouseEvent );
	//
	hDC = ::GetDC( m_hWnd );
	_drawStatus( hDC );
	::ReleaseDC( m_hWnd, hDC );

}

void YBRadioBtn::_onWM_MOUSELEAVE( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSELEAVE );
	HDC hDC = NULL;

	m_uRdBtnFlag &= ~YBRDBTN_FLAG_LBTDOWN;

	if ( ( m_eRadioBtnStatus == ERADIOBTN_STATUS_DISABLE ) || ( m_eRadioBtnStatus == ERADIOBTN_STATUS_DISABLE ) )
		return;

	if( ( m_eRadioBtnStatus == ENORADIOBTN_STATUS_PRESS ) || ( m_eRadioBtnStatus == ENORADIOBTN_STATUS_HOVER ) ){
		m_eRadioBtnStatus = ENORADIOBTN_STATUS_NORMAL;
	}else if( ( m_eRadioBtnStatus == ERADIOBTN_STATUS_HOVER ) || ( m_eRadioBtnStatus == ERADIOBTN_STATUS_PRESS ) ){
		m_eRadioBtnStatus = ERADIOBTN_STATUS_NORMAL;
	}else{
		assert(false);
	}

	hDC = ::GetDC( m_hWnd );
	_drawStatus( hDC );
	::ReleaseDC( m_hWnd, hDC );

}

HWND YBRadioBtn::Create( DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance ){
	HWND hWnd;

	dwStyle |= WS_CLIPSIBLINGS;
	hWnd = _createHWND( 0, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, NULL );
	if( !hWnd ){
		assert(false);
	}
	m_wordCtrlID = (unsigned int)hMenu;
	return hWnd;
}

bool YBRadioBtn::_getImageID( ERADIOBTNSTATUS eRadioBtnStatus, unsigned int& uImageId ){
	switch( eRadioBtnStatus ){

	case ERADIOBTN_STATUS_NORMAL:
		{
			uImageId = m_uRdBtnIIdNormal;
		}
		break;
	case ERADIOBTN_STATUS_HOVER:
		{
			uImageId = m_uRdBtnIIdHover;
		}
		break;
	case ERADIOBTN_STATUS_PRESS:
		{
			uImageId = m_uRdBtnIIdPress;
		}
		break;
	case ENORADIOBTN_STATUS_NORMAL:
		{
			uImageId = m_uNoRdBtnIIdNormal;
		}
		break;
	case ENORADIOBTN_STATUS_HOVER:
		{
			uImageId = m_uNoRdBtnIIdHover;
		}
		break;
	case ENORADIOBTN_STATUS_PRESS:
		{
			uImageId = m_uNoRdBtnIIdPress;
		}
		break;
	case ENORADIOBTN_STATUS_DISABLE:
		{
			uImageId = m_uNoRdBtnIIdDisable;
		}
		break;
	case ERADIOBTN_STATUS_DISABLE:
		{
			uImageId = m_uRdBtnIIdDisable;
		}
		break;
	default:
		{
			assert(false);
			return false;
		}
		break;
	
	}
	return true;
}

Gdiplus::Image* YBRadioBtn::_getBKGPImage( unsigned int uImageIdBK ){
	YBCtrlImageStatic* pYBImageStatic = NULL;

	pYBImageStatic = _getBKImageStatic( uImageIdBK );
	if( !pYBImageStatic )
		return NULL;

	return pYBImageStatic->getImage();
}

//void YBRadioBtn::drawStatus( HDC hDC ) {
//	_drawStatus( hDC );
//}

void YBRadioBtn::_drawStatus( HDC hDC ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawBK( memDC.getMemDC() );
	_drawRadio( memDC.getMemDC() );
	_drawText( memDC.getMemDC() );

	::BitBlt( hDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top ), memDC.getMemDC(), 0, 0, SRCCOPY );

}

void YBRadioBtn::_drawRadio( HDC hDC ){
	if( !getHWND() )
		return;
	RECT rcRdBtnClient;
	unsigned int uImageIdStatus;

	YBCtrlImageStatic* pYBImage = NULL;
	if( !_getImageID( m_eRadioBtnStatus, uImageIdStatus ) ){
		return;
	}
	pYBImage = _getBKImageStatic( uImageIdStatus );
	if( !pYBImage )
		return;
	m_uRdBtnIconWidth = pYBImage->getWidth();
	m_uRdBtnIconHeight = pYBImage->getHeight();

	Gdiplus::Graphics graphics( hDC );
	_getRadioBtnRect( rcRdBtnClient );
	//
	GdipBitBlt( &graphics, pYBImage->getImage(), rcRdBtnClient.left, rcRdBtnClient.top, ( rcRdBtnClient.right - rcRdBtnClient.left ),
		( rcRdBtnClient.bottom - rcRdBtnClient.top ), 0, 0, m_uRdBtnIconWidth, m_uRdBtnIconHeight );
}

void YBRadioBtn::_drawBK( HDC hDC ){
	if( !getHWND() )
		return;
	RECT rcClient;
	
	YBCtrlImageStatic* pYBImageRadioBK = NULL;

	::GetClientRect( m_hWnd, &rcClient );
	pYBImageRadioBK = _getBKImageStatic( m_uRdBtnIIdBK );
	if ( !pYBImageRadioBK )
		return;

	BackGroundBitBlt( hDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top ), pYBImageRadioBK->getMemDC(),
		0, 0, pYBImageRadioBK->getWidth(), pYBImageRadioBK->getHeight(), m_uLFixed, m_uTFixed, m_uRFixed, m_uBFixed );

}

void YBRadioBtn::_drawText( HDC hDC ){
	RECT rcDrawText;
	int iBkModeOld;
	COLORREF clrTxtOld;
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	// draw text
	if ( _getDrawTextRect( rcDrawText ) 
		&& ( rcDrawText.right - rcDrawText.left )
		&& ( rcDrawText.bottom - rcDrawText.top )
		&& ( m_tstrText.length() > 0 ) ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontId );
			if ( pYBCtrlFont ) {
				hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
				iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
				//clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
				if ( m_eRadioBtnStatus == ERADIOBTN_STATUS_DISABLE || m_eRadioBtnStatus == ENORADIOBTN_STATUS_DISABLE )
				{
					clrTxtOld = ::SetTextColor( hDC, ::GetSysColor(COLOR_GRAYTEXT));
				}
				else
				{
					clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
				}
				::DrawText( hDC, m_tstrText.c_str(), m_tstrText.length(), &rcDrawText, DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS|DT_SINGLELINE );
				::SetTextColor( hDC, clrTxtOld );
				::SetBkMode( hDC, iBkModeOld );
				::SelectObject( hDC, hFontOld );
			}
	}
}

void YBRadioBtn::_getRadioBtnRect( RECT& rcRdBtnClient ){
	RECT rcClient;
	::GetClientRect( m_hWnd, &rcClient );

	rcRdBtnClient.left = rcClient.left + m_uRdBtnIconSL;
	rcRdBtnClient.top = rcClient.top + ( rcClient.bottom - rcClient.top - m_uRdBtnIconHeight )/2;
	rcRdBtnClient.right = rcRdBtnClient.left + m_uRdBtnIconWidth;
	rcRdBtnClient.bottom = rcRdBtnClient.top + m_uRdBtnIconHeight;

}

void YBRadioBtn::setImageIdBk( unsigned int uImageIdBK ){
	m_uRdBtnIIdBK = uImageIdBK;
}

YBCtrlImageBase* YBRadioBtn::_getBKImageBase( unsigned int uImageIdBK ){
	YBCtrlImageBase* pYBImage = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageIdBK, &pYBImage ) )
		return NULL;

	return pYBImage;
}

YBCtrlImageStatic* YBRadioBtn::_getBKImageStatic( unsigned int uImageBK ){
	YBCtrlImageBase* pYBImageBase = NULL;

	pYBImageBase = _getBKImageBase( uImageBK );
	if( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

void YBRadioBtn::setImage( ERADIOBTNSTATUS eRadioBtnStatus, unsigned int uImageId ){
	switch( eRadioBtnStatus )
	{
	case ERADIOBTN_STATUS_NORMAL:
		{
			m_uRdBtnIIdNormal = uImageId;
		}
		break;
	case ERADIOBTN_STATUS_HOVER:
		{
			m_uRdBtnIIdHover = uImageId;
		}
		break;
	case ERADIOBTN_STATUS_PRESS:
		{
			m_uRdBtnIIdPress = uImageId;
		}
		break;
	case ENORADIOBTN_STATUS_HOVER:
		{
			m_uNoRdBtnIIdHover = uImageId;
		}
		break;
	case ENORADIOBTN_STATUS_NORMAL:
		{
			m_uNoRdBtnIIdNormal = uImageId;
		}
		break;
	case ENORADIOBTN_STATUS_PRESS:
		{
			m_uNoRdBtnIIdPress = uImageId;
		}
		break;
	case ERADIOBTN_STATUS_DISABLE:
		{
			m_uRdBtnIIdDisable = uImageId;
		}
		break;
	case ENORADIOBTN_STATUS_DISABLE:
		{
			m_uNoRdBtnIIdDisable = uImageId;
		}
		break;
	default:
		break;
	}
}

void YBRadioBtn::setText( const tstring_type& tstrText ){
	m_tstrText = tstrText;
}

void YBRadioBtn::setTextSpace( unsigned int uTxtSpaceL,unsigned int uTxtSpaceT, unsigned int uTxtSpaceR, unsigned int uTxtSpaceB ){
	m_uTxtSpaceL = uTxtSpaceL;
	m_uTxtSpaceB = uTxtSpaceB;
	m_uTxtSpaceR = uTxtSpaceR;
	m_uTxtSpaceT = uTxtSpaceT;
}

void YBRadioBtn::setFontId( unsigned int uFontId ){
	if( !YBCtrlFontDepository::getInstance()->hasYBFont( uFontId ) ){
		return;
	}
	m_uFontId = uFontId;
}

bool YBRadioBtn::_getDrawTextRect( RECT& rcDrawText ){
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcDrawText.left = rcClient.left + m_uTxtSpaceL + m_uRdBtnIconSL + m_uRdBtnIconWidth;
	rcDrawText.top = rcClient.top + m_uTxtSpaceT;
	rcDrawText.right = rcClient.right - m_uTxtSpaceR;
	rcDrawText.bottom = rcClient.bottom - m_uTxtSpaceB;

	return ( rcDrawText.right >= rcDrawText.left ) && ( rcDrawText.bottom >= rcDrawText.top );
}

bool YBRadioBtn::isSelect(){

	return ( ( m_eRadioBtnStatus == ERADIOBTN_STATUS_NORMAL ) || ( m_eRadioBtnStatus == ERADIOBTN_STATUS_HOVER ) ||
		( m_eRadioBtnStatus == ERADIOBTN_STATUS_PRESS) || ( m_eRadioBtnStatus == ERADIOBTN_STATUS_DISABLE ) );
}
// 
//void YBRadioBtn::InitialState(){
//	HDC hDC = NULL;
//	if ( ( m_eRadioBtnStatus == ERADIOBTN_STATUS_DISABLE ) || ( m_eRadioBtnStatus == ENORADIOBTN_STATUS_DISABLE )) {
//		return;
//	}else {
//		m_eRadioBtnStatus = ENORADIOBTN_STATUS_NORMAL;
//	}
//
//	hDC = ::GetDC( m_hWnd );
//	_drawStatus( hDC );
//	::ReleaseDC( m_hWnd, hDC );
//}

void YBRadioBtn::InitialState(){
	HDC hDC = NULL;
	if ( ( m_eRadioBtnStatus == ERADIOBTN_STATUS_DISABLE ) || ( m_eRadioBtnStatus == ENORADIOBTN_STATUS_DISABLE )) {
		return;
	}else {
		m_eRadioBtnStatus = ENORADIOBTN_STATUS_NORMAL;
	}
	//
	::InvalidateRect( m_hWnd, NULL, TRUE );
}

//void YBRadioBtn::setbeSelect( bool bSelected ){
//	HDC hDC = NULL;
//
//	if( bSelected ){
//		m_eRadioBtnStatus = ERADIOBTN_STATUS_NORMAL;
//	}else{
//		m_eRadioBtnStatus = ENORADIOBTN_STATUS_NORMAL;
//	}
//	hDC = ::GetDC( m_hWnd );
//	_drawStatus( hDC );
//	::ReleaseDC( m_hWnd, hDC );
//}

void YBRadioBtn::setbeSelect( bool bSelected ){
	HDC hDC = NULL;

	if( bSelected ){
		m_eRadioBtnStatus = ERADIOBTN_STATUS_NORMAL;
	}else{
		m_eRadioBtnStatus = ENORADIOBTN_STATUS_NORMAL;
	}
	//
	::InvalidateRect( m_hWnd, NULL, TRUE );
}

void YBRadioBtn::setRadioBtnLSpace( unsigned int uRdBtnIconSL ){
	m_uRdBtnIconSL = uRdBtnIconSL;
}

void YBRadioBtn::setEnable( bool bEnable ) {
	if( bEnable )
	{
		if ( isSelect() )
		{
			m_eRadioBtnStatus = ERADIOBTN_STATUS_NORMAL;
		}
		else{
			m_eRadioBtnStatus = ENORADIOBTN_STATUS_NORMAL;
		}
	}
	else
	{
		m_eRadioBtnStatus = ENORADIOBTN_STATUS_DISABLE;
	}
}

void YBRadioBtn::updateStatus() {
	if ( !m_hWnd )
		return;
	if ( !IsWindow( m_hWnd ) ) {
		assert(false);
		return;
	}

	::InvalidateRect( m_hWnd, NULL, TRUE );

}