#include "stdafx.h"
#include "button\YBButtonX.h"
#include "frame\YBCtrlImageDepository.h"
#include "frame\YBCtrlImageStatic.h"
#include "frame\YBCtrlMisc.h"
#include "frame\YBCtrlMemDC.h"
#include "frame\YBCtrlDefs.h"
#include "frame\YBCtrlFontDepository.h"
#include "gdiplus.h"
#include "tooltip/YBToolTip.h"
#include <assert.h>
//
#define YBBTNX_BTN_FLAG_NONE (0x00000000)
#define YBBTNX_BTN_FLAG_LBTNDOWN (0x00000001)
#define YBBTNX_BTN_FLAG_TOOLTIP_HIDE (0x00000002)
#define YBBTNX_BTN_FLAG_MOVETEST (0x00000004)
#define YBBTNX_BTN_FLAG_TOOLTIP_DISAPEAR (0x00000008)
//
#define YBBTNX_BTNX_FLAG_NONE (0x00000000)
#define YBBTNX_BTNX_FLAG_LBTNDOWN (0x00000001)
#define YBBTNX_BTNX_FLAG_HIDE (0x00000002)
//
USING_NAMESPACE_YBCTRL
//
YBButtonX::YBButtonX()
: m_uBtnFlag( YBBTNX_BTN_FLAG_NONE )
, m_uBtnXFlag( YBBTNX_BTNX_FLAG_NONE )
, m_eBtnStatus( EBTN_STATUS_NORMAL )
, m_eBtnXStatus( EBTNX_STATUS_NORMAL )
, m_uImageIDBtnNormal( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIDBtnHover( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIDBtnPress( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIDBtnXNormal( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIDBtnXHover( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIDBtnXPress( YBCTRL_IMAGE_ID_INVALID )
, m_uLFixed( 0 )
, m_uTFixed( 0 )
, m_uRFixed( 0 )
, m_uBFixed( 0 )
, m_uBtnXRSpace( 5 )
, m_uBtnXTSpace( 5 )
, m_wordCtrlID( 0 )
, m_uTxtSpaceL( 0 )
, m_uTxtSpaceT( 0 )
, m_uTxtSpaceR( 0 )
, m_uTxtSpaceB( 0 )
, m_tstrText( _T("") )
, m_tstrBtnXTip( _T("¹Ø ±Õ"))
, m_uFontId( YBCTRL_FONT_ID_INVALID )
, m_hIcon( NULL )
, m_uIconSpaceL( 0 )
, m_bCenter( false )
{
	::memset( &m_sizeIcon, 0, sizeof(SIZE) );
}
	
YBButtonX::~YBButtonX() {}

YBCTRL_WNDMSGMAP_BEGIN( YBButtonX, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
	YBCTRL_ONWNDMSG( WM_MOUSELEAVE, _onWM_MOUSELEAVE )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_LBUTTONDBLCLK, _onWM_LBUTTONDBLCLK )
	YBCTRL_ONWNDMSG( WM_LBUTTONUP, _onWM_LBUTTONUP )
	YBCTRL_ONWNDMSG( WM_DESTROY, _onWM_DESTROY )
YBCTRL_WNDMSGMAP_END()

void YBButtonX::_onWM_DESTROY( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_DESTROY );
	//
	YBToolTip::getInstance()->hide();
}

HWND YBButtonX::Create( DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance ) {
	HWND hWnd;

	dwStyle |= WS_CLIPSIBLINGS;
	hWnd = _createHWND( dwExStyle, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, NULL );
	m_wordCtrlID = (unsigned int)hMenu;

	return hWnd;
}

void YBButtonX::_onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	
	HDC hPaintDC;
	PAINTSTRUCT ps;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eBtnStatus, m_eBtnXStatus );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top )
		, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void YBButtonX::_onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	RECT rcClient;

	//
	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = 1;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eBtnStatus, m_eBtnXStatus );

	::BitBlt( hDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top )
		, memDC.getMemDC(), 0, 0, SRCCOPY );
}

//void YBButtonX::drawStatus() {
//	if ( !getHWND() )
//		return; 
//	if ( !::IsWindowVisible( m_hWnd ) )
//		return;
//	HDC hDC;
//	RECT rcClient;
//
//	::GetClientRect( m_hWnd, &rcClient );
//	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
//	_drawStatus( memDC.getMemDC(), m_eBtnStatus, m_eBtnXStatus );
//	//
//	hDC = ::GetDC( m_hWnd );
//	::BitBlt( hDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top )
//		, memDC.getMemDC(), 0, 0, SRCCOPY );
//	::ReleaseDC( m_hWnd, hDC );
//}

void YBButtonX::drawStatus() {
	if ( !getHWND() )
		return; 
	if ( !::IsWindowVisible( m_hWnd ) )
		return;
	::InvalidateRect( m_hWnd, NULL, TRUE );
}

void YBButtonX::_drawStatus( HDC hDC, ENUMBTNSTATUS eBtnStatus, ENUMBTNXSTATUS eBtnXStatus ) {
	if ( !getHWND() )
		return;
	RECT rcClient;
	RECT rcBtnX;
	RECT rcDrawText;
	RECT rcDrawIcon;
	YBCtrlImageStatic* pYBImageBtn = NULL;
	YBCtrlImageStatic* pYBImageBtnX = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	HFONT hFontOld = NULL;
	int iBkModeOld;
	COLORREF clrTxtOld;
	
	::GetClientRect( m_hWnd, &rcClient );
	pYBImageBtn = _getYBImageStatic( eBtnStatus );
	if ( !pYBImageBtn ) {
		assert( false );
		return;
	}
	pYBImageBtnX = _getYBImageStatic( eBtnXStatus );
	if ( !pYBImageBtnX ) {
		if ( ( ( YBBTNX_BTNX_FLAG_HIDE & m_uBtnXFlag ) != YBBTNX_BTNX_FLAG_HIDE ) ) {
			assert( false );
			return;
		}
	}
	// draw back ground 
	BackGroundBitBlt( hDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top )
		, pYBImageBtn->getMemDC(), 0, 0, pYBImageBtn->getWidth(), pYBImageBtn->getHeight()
		, m_uLFixed, m_uTFixed, m_uRFixed, m_uBFixed );

	// draw text.
	if ( _getDrawTextRect( rcDrawText ) 
		&& ( rcDrawText.right - rcDrawText.left )
		&& ( rcDrawText.bottom - rcDrawText.top )
		&& ( m_tstrText.length() > 0 ) ) {
		pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontId );
		if ( pYBCtrlFont ) {
			hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
			iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
			if ( m_eBtnStatus == YBButtonX::EBTN_STATUS_DISABLE ) {
				clrTxtOld = ::SetTextColor( hDC, YBCOLOR_GRAY );
			}
			else {
				if (m_bSetHovTxtClr  && m_eBtnStatus == EBTN_STATUS_HOVER) {
					clrTxtOld = ::SetTextColor(hDC, m_rgbHovTxtClr);
				}
				else if (m_bSetPreTxtClr && m_eBtnStatus == EBTN_STATUS_PRESS) {
					clrTxtOld = ::SetTextColor(hDC, m_rgbPreTxtClr);
				}
				else {
					clrTxtOld = ::SetTextColor(hDC, pYBCtrlFont->m_clrTxt);
				}
			}
			if ( m_bCenter ) {
				::DrawText( hDC, m_tstrText.c_str(), m_tstrText.length(), &rcDrawText, DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS|DT_SINGLELINE );
			} else {
				::DrawText( hDC, m_tstrText.c_str(), m_tstrText.length(), &rcDrawText, DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS|DT_SINGLELINE );
			}
			::SetTextColor( hDC, clrTxtOld );
			::SetBkMode( hDC, iBkModeOld );
			::SelectObject( hDC, hFontOld );
		}
	}

	// draw x button ( sometimes is close button. )
	if ( ( eBtnStatus != EBTN_STATUS_NORMAL )
		&& ( eBtnStatus != EBTN_STATUS_DISABLE ) ) {
		if ( !( m_uBtnXFlag & YBBTNX_BTNX_FLAG_HIDE ) ) {
			Gdiplus::Graphics graphics( hDC );
			if ( _getBtnXRect( rcBtnX ) ) {
				GdipBitBlt( &graphics, pYBImageBtnX->getImage(), rcBtnX.left, rcBtnX.top, (rcBtnX.right - rcBtnX.left), (rcBtnX.bottom - rcBtnX.top)
					, 0, 0, pYBImageBtnX->getWidth(), pYBImageBtnX->getHeight() );
			}
		}
	}

	// draw icons
	if ( m_hIcon ) {
		if (m_eBtnStatus != YBButtonX::EBTN_STATUS_DISABLE)
		{
			_getDrawIconRect(rcDrawIcon);
			::DrawIconEx(hDC, rcDrawIcon.left, rcDrawIcon.top, m_hIcon, m_sizeIcon.cx, m_sizeIcon.cy, 0, NULL, DI_NORMAL);
		}
		
	}
}

Gdiplus::Image* YBButtonX::_getGPImage( ENUMBTNSTATUS eBtnStatus ) {
	YBCtrlImageStatic* pYBImageStatic = NULL;

	pYBImageStatic = _getYBImageStatic( eBtnStatus );
	if ( !pYBImageStatic )
		return NULL;

	return pYBImageStatic->getImage();
}

Gdiplus::Image* YBButtonX::_getGPImage( ENUMBTNXSTATUS eBtnXStatus ) {
	YBCtrlImageStatic* pYBImageStatic = NULL;

	pYBImageStatic = _getYBImageStatic( eBtnXStatus );
	if ( !pYBImageStatic )
		return NULL;

	return pYBImageStatic->getImage();
}

YBCtrlImageStatic* YBButtonX::_getYBImageStatic( ENUMBTNSTATUS eBtnStatus ) {
	YBCtrlImageBase* pYBImageBase = NULL;

	pYBImageBase = _getYBImageBase( eBtnStatus );
	if ( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

YBCtrlImageStatic* YBButtonX::_getYBImageStatic( ENUMBTNXSTATUS eBtnXStatus ) {
	YBCtrlImageBase* pYBImageBase = NULL;

	pYBImageBase = _getYBImageBase( eBtnXStatus );
	if ( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;	
}

YBCtrlImageBase* YBButtonX::_getYBImageBase( ENUMBTNSTATUS eBtnStatus ) {
	YBCtrlImageBase* pYBImage = NULL;

	if ( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( _getImageID( eBtnStatus ), &pYBImage ) )
		return NULL;

	return pYBImage;
}

YBCtrlImageBase* YBButtonX::_getYBImageBase( ENUMBTNXSTATUS eBtnXStatus ) {
	YBCtrlImageBase* pYBImage = NULL;

	if ( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( _getImageID( eBtnXStatus ), &pYBImage ) )
		return NULL;

	return pYBImage;
}

void YBButtonX::setImage( ENUMBTNSTATUS eBtnStatus, unsigned int uImageID ) {
	switch ( eBtnStatus ) {
	case EBTN_STATUS_NORMAL:
		{
			m_uImageIDBtnNormal = uImageID;
		}
		break;
	case EBTN_STATUS_HOVER:
		{
			m_uImageIDBtnHover = uImageID;
		}
		break;
	case EBTN_STATUS_PRESS:
		{
			m_uImageIDBtnPress = uImageID;
		}
		break;
	case EBTN_STATUS_DISABLE:
		{
			m_uImageIDBtnDisable = uImageID;
		}
		break;
	default:
		{
			assert( false );
		}
		break;
	}
}

void YBButtonX::setImage( ENUMBTNXSTATUS eBtnXStatus, unsigned int uImageID ) {
	switch ( eBtnXStatus ) 
	{
	case EBTNX_STATUS_NORMAL:
		{
			m_uImageIDBtnXNormal = uImageID;
		}
		break;
	case EBTNX_STATUS_HOVER:
		{
			m_uImageIDBtnXHover = uImageID;
		}
		break;
	case EBTNX_STATUS_PRESS:
		{
			m_uImageIDBtnXPress = uImageID;
		}
		break;
	default:
		{
			assert( false );
		}
		break;
	}
}

unsigned int YBButtonX::_getImageID( ENUMBTNSTATUS eBtnStatus ) {
	switch ( eBtnStatus ) {
	case EBTN_STATUS_NORMAL:
		{
			return m_uImageIDBtnNormal;
		}
		break;
	case EBTN_STATUS_HOVER:
		{
			return m_uImageIDBtnHover;
		}
		break;
	case EBTN_STATUS_PRESS:
		{
			return m_uImageIDBtnPress;
		}
		break;
	case EBTN_STATUS_DISABLE:
		{
			return m_uImageIDBtnDisable;
		}
		break;
	default:
		{
			assert( false );
		}
		break;
	}

	assert( false );
	return YBCTRL_IMAGE_ID_INVALID;
}

unsigned int YBButtonX::_getImageID( ENUMBTNXSTATUS eBtnXStatus ) {
	switch ( eBtnXStatus ) {
	case EBTNX_STATUS_NORMAL:
		{
			return m_uImageIDBtnXNormal;
		}
		break;
	case EBTNX_STATUS_HOVER:
		{
			return m_uImageIDBtnXHover;
		}
		break;
	case EBTNX_STATUS_PRESS:
		{
			return m_uImageIDBtnXPress;
		}
		break;
	default:
		{
			assert( false );
		}
		break;
	}

	assert( false );
	return YBCTRL_IMAGE_ID_INVALID;
}

void YBButtonX::setFixedSpace( unsigned int uLFixed, unsigned int uTFixed, unsigned int uRFixed, unsigned int uBFixed ) {
	m_uLFixed = uLFixed;
	m_uTFixed = uTFixed;
	m_uRFixed = uRFixed;
	m_uBFixed = uBFixed;
}

void YBButtonX::_onWM_MOUSELEAVE( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSELEAVE );
	HDC hDC;
	RECT rcClient;

	if ( m_eBtnStatus == EBTN_STATUS_DISABLE ) {
		return; 
	}

	// set button background status
	if ( ( m_uBtnFlag & YBBTNX_BTN_FLAG_LBTNDOWN ) != YBBTNX_BTN_FLAG_LBTNDOWN ) {
		m_eBtnStatus = EBTN_STATUS_NORMAL;
	}
	// set button X status
	m_uBtnXFlag &= ~YBBTNX_BTNX_FLAG_LBTNDOWN;
	m_uBtnFlag &= ~YBBTNX_BTN_FLAG_TOOLTIP_DISAPEAR;
	m_eBtnXStatus = EBTNX_STATUS_NORMAL;
	
	//
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eBtnStatus, m_eBtnXStatus );
	
	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), (rcClient.bottom - rcClient.top), memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
	//
	if ( ( m_uBtnFlag & YBBTNX_BTN_FLAG_TOOLTIP_HIDE ) != YBBTNX_BTN_FLAG_TOOLTIP_HIDE ) {
		YBToolTip::getInstance()->hide();
	}
}

void YBButtonX::_onWM_MOUSEMOVE( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	HDC hDC;
	RECT rcClient;
	RECT rcBtnX;
	TRACKMOUSEEVENT trackMouseEvt;
	POINT ptMouse;

	if ( m_eBtnStatus == EBTN_STATUS_DISABLE ) {
		YBToolTip::getInstance()->hide();
		return; 
	}

	if ( m_uBtnFlag & YBBTNX_BTN_FLAG_LBTNDOWN ) {
		m_eBtnStatus = EBTN_STATUS_PRESS;
	} else {
		m_eBtnStatus = EBTN_STATUS_HOVER;
	}
	//
	ptMouse.x = (__int16)LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = (__int16)HIWORD( pYBCtrlWndMsg->m_lParam );
	if ( ( m_uBtnXFlag & YBBTNX_BTNX_FLAG_HIDE ) != YBBTNX_BTNX_FLAG_HIDE ) {
		if ( _getBtnXRect( rcBtnX ) ) {
			if ( ::PtInRect( &rcBtnX, ptMouse ) ) {
				if ( m_uBtnXFlag & YBBTNX_BTNX_FLAG_LBTNDOWN ) {
					m_eBtnXStatus = EBTNX_STATUS_PRESS;
				} else {
					m_eBtnXStatus = EBTNX_STATUS_HOVER;
					// Add tooltip to BtnX.
					if ( ( m_uBtnFlag & YBBTNX_BTN_FLAG_TOOLTIP_DISAPEAR ) != YBBTNX_BTN_FLAG_TOOLTIP_DISAPEAR ) {
						if ( ( m_uBtnFlag & YBBTNX_BTN_FLAG_TOOLTIP_HIDE ) != YBBTNX_BTN_FLAG_TOOLTIP_HIDE ) {
							//YBToolTip::getInstance()->hide();
							YBToolTip::getInstance()->updateTipText( m_tstrBtnXTip, m_hWnd );
						}
					}
				}
			} else {
				m_eBtnXStatus = EBTNX_STATUS_NORMAL;
				if ( ( m_uBtnFlag & YBBTNX_BTN_FLAG_TOOLTIP_DISAPEAR ) != YBBTNX_BTN_FLAG_TOOLTIP_DISAPEAR ) {
					if ( ( m_uBtnFlag & YBBTNX_BTN_FLAG_TOOLTIP_HIDE ) != YBBTNX_BTN_FLAG_TOOLTIP_HIDE ) {
						YBToolTip::getInstance()->updateTipText( m_tstrBtnTip, m_hWnd );
					}
				}
			}
		}
	} else {
		if ( ( m_uBtnFlag & YBBTNX_BTN_FLAG_TOOLTIP_DISAPEAR ) != YBBTNX_BTN_FLAG_TOOLTIP_DISAPEAR ) {
			if ( ( m_uBtnFlag & YBBTNX_BTN_FLAG_TOOLTIP_HIDE ) != YBBTNX_BTN_FLAG_TOOLTIP_HIDE ) {
				YBToolTip::getInstance()->updateTipText( m_tstrBtnTip, m_hWnd );
			}
		}
	}
	//
	trackMouseEvt.cbSize = sizeof( TRACKMOUSEEVENT );
	trackMouseEvt.dwFlags = TME_LEAVE;
	trackMouseEvt.hwndTrack = pYBCtrlWndMsg->m_hWnd;
	trackMouseEvt.dwHoverTime = HOVER_DEFAULT;
	::TrackMouseEvent ( &trackMouseEvt );

	//
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eBtnStatus, m_eBtnXStatus );
	
	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), (rcClient.bottom - rcClient.top), memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );

	//
	//YBWin32Tooltip::getInstance()->updateTipText( m_tstrText.c_str(), m_hWnd, 1 );
	//
	if ( ( m_uBtnFlag & YBBTNX_BTN_FLAG_MOVETEST ) == YBBTNX_BTN_FLAG_MOVETEST ) {
		WPARAM wParam;
		LPARAM lParam;
		wParam = MAKELONG( m_wordCtrlID, YBBTNX_CMDEVT_BTNMOVE );
		lParam = ( LPARAM )m_hWnd;
		::SendMessage( ::GetParent( m_hWnd ), YBWM_MOVEHITTEST, wParam, lParam );
	}
}

bool YBButtonX::_isHideBtnX() const {
	return ( m_uBtnXFlag & YBBTNX_BTNX_FLAG_HIDE ) == YBBTNX_BTNX_FLAG_HIDE;
}

void YBButtonX::_onWM_LBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
	HDC hDC;
	RECT rcClient;
	RECT rcBtnX;
	POINT ptMouse;
	unsigned int uBtnFlagOld;

	if ( m_eBtnStatus ==  EBTN_STATUS_DISABLE ) {
		return; 
	}

	m_uBtnFlag |= YBBTNX_BTN_FLAG_TOOLTIP_DISAPEAR;
	uBtnFlagOld = m_uBtnFlag;
	ptMouse.x = (__int16)LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = (__int16)HIWORD( pYBCtrlWndMsg->m_lParam );
	//
	YBToolTip::getInstance()->hide();
	//
	if ( !_isHideBtnX() && _getBtnXRect( rcBtnX ) ) {
		if ( ::PtInRect( &rcBtnX, ptMouse ) ) {
			// set button X back status.
			m_uBtnXFlag |= YBBTNX_BTNX_FLAG_LBTNDOWN;
			m_eBtnXStatus = EBTNX_STATUS_PRESS;
		} else {
			// set button back status.
			m_uBtnFlag |= YBBTNX_BTN_FLAG_LBTNDOWN;
			m_eBtnStatus = EBTN_STATUS_PRESS;
		}
	} else {
		// set button back status.
		m_uBtnFlag |= YBBTNX_BTN_FLAG_LBTNDOWN;
		m_eBtnStatus = EBTN_STATUS_PRESS;
	}
	
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eBtnStatus, m_eBtnXStatus );
	
	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), (rcClient.bottom - rcClient.top), memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );

	// send command message to parent window.
	if ( !( m_uBtnXFlag & YBBTNX_BTNX_FLAG_HIDE ) && ::PtInRect( &rcBtnX, ptMouse ) ) {
		WPARAM wParam;
		LPARAM lParam;

		wParam = MAKELONG( m_wordCtrlID, YBBTNX_CMDEVT_BTNXDOWN )  ;
		lParam = (LPARAM)m_hWnd;
		::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
	} else {
		WPARAM wParam;
		LPARAM lParam;

		if ( ( uBtnFlagOld & YBBTNX_BTNX_FLAG_LBTNDOWN ) != YBBTNX_BTNX_FLAG_LBTNDOWN ) {
			wParam = MAKELONG( m_wordCtrlID, YBBTNX_CMDEVT_BTNDOWN );
			lParam = (LPARAM)m_hWnd;
			::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
		}
	}
	
}

void YBButtonX::_onWM_LBUTTONUP( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONUP );
	HDC hDC;
	RECT rcClient;
	RECT rcBtnX;
	POINT ptMouse;

	if ( m_eBtnStatus == EBTN_STATUS_DISABLE ) {
		return; 
	}
	//m_uBtnFlag &= ~YBBTNX_BTNX_FLAG_LBTNDOWN;
	//m_eBtnStatus = EBTN_STATUS_HOVER;
	
	m_uBtnXFlag &= ~YBBTNX_BTNX_FLAG_LBTNDOWN;
	if ( ( m_uBtnXFlag & YBBTNX_BTNX_FLAG_HIDE ) != YBBTNX_BTNX_FLAG_HIDE ) {
		_getBtnXRect( rcBtnX );
		ptMouse.x = (__int16)LOWORD( pYBCtrlWndMsg->m_lParam );
		ptMouse.y = (__int16)HIWORD( pYBCtrlWndMsg->m_lParam );
		if ( ::PtInRect( &rcBtnX, ptMouse ) ) {
			m_eBtnXStatus = EBTNX_STATUS_HOVER;
		} else {
			m_eBtnXStatus = EBTNX_STATUS_NORMAL;
		}
	}

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eBtnStatus, m_eBtnXStatus );
	
	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), (rcClient.bottom - rcClient.top), memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
}

void YBButtonX::_onWM_LBUTTONDBLCLK( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDBLCLK );
	HDC hDC;
	RECT rcClient;
	RECT rcBtnX;
	POINT ptMouse;
	//
	if ( m_eBtnStatus == EBTN_STATUS_DISABLE ) {
		return;
	}
	m_uBtnFlag |= YBBTNX_BTN_FLAG_TOOLTIP_DISAPEAR;
	ptMouse.x = ( __int16 )LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = ( __int16 )HIWORD( pYBCtrlWndMsg->m_lParam );
	//
	YBToolTip::getInstance()->hide();
	//
	if ( !_getBtnXRect( rcBtnX ) )
		return;
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eBtnStatus, m_eBtnXStatus );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top ), memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );

	// send command message to parent window.
	if ( !( m_uBtnXFlag & YBBTNX_BTNX_FLAG_HIDE ) && ::PtInRect( &rcBtnX, ptMouse ) ) {
		WPARAM wParam;
		LPARAM lParam;

		wParam = MAKELONG( m_wordCtrlID, YBBTNX_CMDEVT_BTNXDOWN );
		lParam = ( LPARAM )m_hWnd;
		::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
	}
}

void YBButtonX::pushDown() {
	//HDC hDC;
	RECT rcClient;
	RECT rcBtnX;
	POINT ptInScreen;
	POINT ptInClient;

	if ( m_eBtnStatus == EBTN_STATUS_DISABLE ) {
		return; 
	}
	//
	m_uBtnFlag |= YBBTNX_BTN_FLAG_LBTNDOWN;
	m_eBtnStatus = EBTN_STATUS_PRESS;
	//
	::GetClientRect( m_hWnd, &rcClient );
	::GetCursorPos( &ptInScreen );
	ptInClient = ptInScreen;
	::ScreenToClient( m_hWnd, &ptInClient );
	if ( _getBtnXRect( rcBtnX ) && ::PtInRect( &rcBtnX, ptInClient ) ) {
		m_eBtnXStatus = EBTNX_STATUS_HOVER;
	} else {
		m_eBtnXStatus = EBTNX_STATUS_NORMAL;
	}

	drawStatus();
// 	::GetClientRect( m_hWnd, &rcClient );
// 	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
// 	_drawStatus( memDC.getMemDC(), m_eBtnStatus, m_eBtnXStatus );
// 
// 	hDC = ::GetDC( m_hWnd );
// 	::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), (rcClient.bottom - rcClient.top), memDC.getMemDC(), 0, 0, SRCCOPY );
// 	::ReleaseDC( m_hWnd, hDC );
}

void YBButtonX::popUp() {
	//HDC hDC;
	RECT rcClient;
	RECT rcBtnX;
	POINT ptInScreen;
	POINT ptInClient;

	if ( m_eBtnStatus == YBButtonX::EBTN_STATUS_DISABLE ) {
		return;
	}

	::GetCursorPos( &ptInScreen );
	::GetClientRect( m_hWnd, &rcClient );
	ptInClient = ptInScreen;
	::ScreenToClient( m_hWnd, &ptInClient );

	m_uBtnXFlag &= ~YBBTNX_BTNX_FLAG_LBTNDOWN;
	m_uBtnFlag &= ~YBBTNX_BTN_FLAG_LBTNDOWN;

	if ( _getBtnXRect( rcBtnX ) && ::PtInRect( &rcBtnX, ptInClient ) ) {
		m_eBtnXStatus = EBTNX_STATUS_HOVER;
		m_eBtnStatus = EBTN_STATUS_HOVER;
	} else {
		m_eBtnXStatus = EBTNX_STATUS_NORMAL;
		if ( ::PtInRect( &rcClient, ptInClient ) ) {
			m_eBtnStatus = EBTN_STATUS_HOVER;
		} else {
			m_eBtnStatus = EBTN_STATUS_NORMAL;
		}
	}
	
	drawStatus();
	//YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	//_drawStatus( memDC.getMemDC(), m_eBtnStatus, m_eBtnXStatus );
	//
	//hDC = ::GetDC( m_hWnd );
	//::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), (rcClient.bottom - rcClient.top), memDC.getMemDC(), 0, 0, SRCCOPY );
	//::ReleaseDC( m_hWnd, hDC );
}

bool YBButtonX::_getBtnXRect( RECT& rcBtnX ) {
	RECT rcClient;
	YBCtrlImageBase* pYBImageBtnX = NULL;

	::GetClientRect( m_hWnd, &rcClient );
	pYBImageBtnX = _getYBImageBase( m_eBtnXStatus );
	if ( !pYBImageBtnX ) {
		assert( false );
		return false;
	}
	rcBtnX.left = rcClient.right - m_uBtnXRSpace - pYBImageBtnX->getWidth();
	rcBtnX.right = rcClient.right - m_uBtnXRSpace;
	rcBtnX.top = rcClient.top + m_uBtnXTSpace;
	rcBtnX.bottom = rcBtnX.top + pYBImageBtnX->getHeight();
	return ( ( rcBtnX.right > rcBtnX.left ) && ( rcBtnX.bottom > rcBtnX.top ) );
}

void YBButtonX::setBtnXSpace( unsigned int uBtnXRSpace, unsigned int uBtnYTSpace ) {
	m_uBtnXRSpace = uBtnXRSpace;
	m_uBtnXTSpace = uBtnYTSpace;
}

bool YBButtonX::_getDrawTextRect( RECT& rcDrawText ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );

	rcDrawText.left = rcClient.left + m_uTxtSpaceL + m_sizeIcon.cx;
	rcDrawText.top = rcClient.top + m_uTxtSpaceT;
	rcDrawText.right = rcClient.right - m_uTxtSpaceR;
	rcDrawText.bottom = rcClient.bottom - m_uTxtSpaceB;

	return ( rcDrawText.right >= rcDrawText.left ) && ( rcDrawText.bottom >= rcDrawText.top );
}

void YBButtonX::_getDrawIconRect( RECT& rcDrawIcon ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcDrawIcon.left = rcClient.left + m_uIconSpaceL;
	rcDrawIcon.right = rcDrawIcon.left + m_sizeIcon.cx;
	rcDrawIcon.top = rcClient.top + ( rcClient.bottom - rcClient.top - m_sizeIcon.cy ) / 2;
	rcDrawIcon.bottom = rcDrawIcon.top + m_sizeIcon.cy;
}

void YBButtonX::setTextSpace( unsigned int uTxtSpaceL, unsigned int uTxtSpaceT, unsigned int uTxtSpaceR, unsigned int uTxtSpaceB ) {
	m_uTxtSpaceL = uTxtSpaceL;
	m_uTxtSpaceT = uTxtSpaceT;
	m_uTxtSpaceR = uTxtSpaceR;
	m_uTxtSpaceB = uTxtSpaceB;
}

void YBButtonX::setText( const tstring_type& tstrText ) {
	m_tstrText = tstrText;
	m_tstrBtnTip = tstrText;
}

void YBButtonX::setFontId( unsigned int uFontId ) {
	if ( !YBCtrlFontDepository::getInstance()->hasYBFont( uFontId ) ) {
		return;
	}

	m_uFontId = uFontId;
}

void YBButtonX::setHovTxtClr(COLORREF clrTxt /*= RGB( 143, 70, 1 )*/) {
	m_bSetHovTxtClr = true;
	m_rgbHovTxtClr = clrTxt;
}

void YBButtonX::setPreTxtClr(COLORREF clrTxt /*= RGB(143, 70, 1)*/)
{
	m_bSetPreTxtClr = true;
	m_rgbPreTxtClr = clrTxt;
}

void YBButtonX::enableBtnX( bool bEnable ) {
	if ( bEnable ) {
		m_uBtnXFlag &= ~YBBTNX_BTNX_FLAG_HIDE;
	} else {
		m_uBtnXFlag |= YBBTNX_BTNX_FLAG_HIDE;
	}
}

void YBButtonX::enableBtn( bool bEnable ) {
	bool bStatusChange = false;
	//RECT rcClient;

	if ( bEnable ) {
		if ( m_eBtnStatus != YBButtonX::EBTN_STATUS_NORMAL ) {
			bStatusChange = true;
		}
		m_eBtnStatus = YBButtonX::EBTN_STATUS_NORMAL;
	} else {
		if ( m_eBtnStatus != YBButtonX::EBTN_STATUS_DISABLE ) {
			bStatusChange = true;
		}
		m_eBtnStatus = YBButtonX::EBTN_STATUS_DISABLE;
	}

	if ( bStatusChange && m_hWnd ) {
// 		::GetClientRect( m_hWnd, &rcClient );
// 		YBCtrlMemDC memDC( rcClient.right - rcClient.left , rcClient.bottom - rcClient.top );
// 		_drawStatus( memDC.getMemDC(), m_eBtnStatus, m_eBtnXStatus );
// 
// 		HDC hDC = ::GetDC( m_hWnd );
// 		::BitBlt( hDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top )
// 			, memDC.getMemDC(), 0, 0, SRCCOPY );
// 		::ReleaseDC( m_hWnd, hDC );
		drawStatus();
	}
}

void YBButtonX::addToolTip( bool bEnable ) {
	if ( bEnable ) {
		m_uBtnFlag &= ~YBBTNX_BTN_FLAG_TOOLTIP_HIDE;
	} else {
		m_uBtnFlag |= YBBTNX_BTN_FLAG_TOOLTIP_HIDE;
	}
}

void YBButtonX::setBtnXTipTxt( const tstring_type& tstrBtnXTip ) {
	m_tstrBtnXTip = tstrBtnXTip;
}

void YBButtonX::setBtnTipTxt( const tstring_type& tstrBtnTip ) {
	m_tstrBtnTip = tstrBtnTip;
}

void YBButtonX::setTextCenter( bool bCenter ) {
	if ( bCenter ) {
		m_bCenter = true;
	} else {
		m_bCenter = false;
	}
}

void YBButtonX::setMoveTest( bool bMoveTest ) {
	if ( bMoveTest ) {
		m_uBtnFlag |= YBBTNX_BTN_FLAG_MOVETEST;
	} else {
		m_uBtnFlag &= ~YBBTNX_BTN_FLAG_MOVETEST;
	}
}

void YBButtonX::setIcon(HICON hIcon, unsigned int uWidth, unsigned int uHeight)
{
	m_hIcon = hIcon;
	m_sizeIcon.cx = uWidth;
	m_sizeIcon.cy = uHeight;
}

void YBButtonX::updateStatus() {
	if (!m_hWnd)
		return;
	if (!IsWindow(m_hWnd)) {
		assert(false);
		return;
	}

	::InvalidateRect(m_hWnd, NULL, TRUE);
}