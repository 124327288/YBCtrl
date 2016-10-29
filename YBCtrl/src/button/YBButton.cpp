#include "stdafx.h"
#include "button\YBButton.h"
#include "frame\YBCtrlImageDepository.h"
#include "frame\YBCtrlImageStatic.h"
#include "frame\YBCtrlMisc.h"
#include "frame\YBCtrlMemDC.h"
#include "frame\YBCtrlFontDepository.h"
#include <CommCtrl.h>
#include "gdiplus.h"
#include "tooltip/YBToolTip.h"
#include <assert.h>
//
#define YBBTN_FLAG_NONE (0x00000000)
#define YBBTN_FLAG_LBTNDOWN (0x00000001)
#define YBBTN_FLAG_ADD_TOOLTIP (0x00000002)
#define YBBTN_FLAG_DRAWLICON (0x00000004)
#define YBBTN_FLAG_DRAWRICON (0x00000008)
#define YBBTN_FLAG_TOOLTIP_DISAPPERAR (0x00000010)
//
USING_NAMESPACE_YBCTRL
	//
	YBButton::YBButton() 
	: m_uBtnFlag( YBBTN_FLAG_NONE )
	, m_eBtnStatus( EBTN_STATUS_NORMAL )
	, m_uImageIDBtnNormal( YBCTRL_IMAGE_ID_INVALID )
	, m_uImageIDBtnHover( YBCTRL_IMAGE_ID_INVALID )
	, m_uImageIDBtnPress( YBCTRL_IMAGE_ID_INVALID )
	, m_uLFixed( 0 )
	, m_uTFixed( 0 )
	, m_uRFixed( 0 )
	, m_uBFixed( 0 )
	, m_uBtnSpace( 5 )
	, m_wordCtrlID( 0 )
	, m_uTxtSpaceL( 0 )
	, m_uTxtSpaceT( 0 )
	, m_uTxtSpaceB( 0 )
	, m_uTxtSpaceR( 0 )
	, m_tstrText( _T("") )
	, m_uFontId( YBCTRL_FONT_ID_INVALID )
	, m_uImageIdLIconNor( YBCTRL_IMAGE_ID_INVALID )
	, m_uImageIdLIconHov( YBCTRL_IMAGE_ID_INVALID )
	, m_uImageIdLIconPre( YBCTRL_IMAGE_ID_INVALID )
	, m_uImageIdRIconNor( YBCTRL_IMAGE_ID_INVALID )
	, m_uImageIdRIconHov( YBCTRL_IMAGE_ID_INVALID )
	, m_uImageIdRIconPre( YBCTRL_IMAGE_ID_INVALID )
	, m_uRIconWidth( 0 )
	, m_uRIconHeight( 0 )
	, m_uRIconRSpace( 0 )
	, m_uRIconTSpace( 0 )
    , m_uLIconWidth( 0 )
    , m_uLIconHeight( 0 )
    , m_uLIconLSpace( 0 )
    , m_uLIconTSpace( 0 )
	, m_bTipText( false )
	, m_tstrTipText( _T("") )
	, m_bVcenter( false )
	, m_bSetHovTxtClr( false )
	, m_rgbHovTxtClr( NULL )
	, m_bSetPreTxtClr(false)
	, m_rgbPreTxtClr(NULL)
	, m_rgbDisTxtClr( YBCOLOR_GRAY )
	, m_bSetImageIcon(false)
	, m_uLineHeight( 0 )
	, m_uIconSpaceL( 0 )
	, m_hIcon( NULL )
{
	::memset( &m_sizeIcon, 0, sizeof( SIZE ) );
}

void YBButton::_reset(){
	m_eBtnStatus = EBTN_STATUS_NORMAL;
}

YBButton::~YBButton() {

}

HWND YBButton::Create( DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance ) {
	HWND hWnd;

	_reset();
	//
	dwStyle |= (WS_CHILD|WS_CLIPSIBLINGS|WS_TABSTOP);
	hWnd = _createHWND( dwExStyle, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, NULL );
	m_wordCtrlID = unsigned int( hMenu );
	
	return hWnd;
}

YBCTRL_WNDMSGMAP_BEGIN( YBButton, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_LBUTTONDBLCLK, _onWM_LBUTTONDBLCLK )
	YBCTRL_ONWNDMSG( WM_LBUTTONUP, _onWM_LBUTTONUP )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
	YBCTRL_ONWNDMSG( WM_MOUSELEAVE, _onWM_MOUSELEAVE)
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_KEYDOWN, _onWM_KEYDOWN )
	YBCTRL_ONWNDMSG( WM_DESTROY, _onWM_DESTROY )
YBCTRL_WNDMSGMAP_END()

void YBButton::_onWM_DESTROY( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_DESTROY );
	//
	YBToolTip::getInstance()->hide();
}

void YBButton::_onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );

	HDC hPaintDC;
	PAINTSTRUCT ps;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left , rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eBtnStatus );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top )
		, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );

}

void YBButton::_onWM_KEYDOWN( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_KEYDOWN );
	if ( pYBCtrlWndMsg->m_wParam == VK_TAB ) {
		::SendMessage( m_hWnd, WM_NEXTDLGCTL, 0, 0 );
	}
}

void YBButton::_onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	RECT rcClient;

	//
	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = 1;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eBtnStatus );

	::BitBlt( hDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top )
		, memDC.getMemDC(), 0, 0, SRCCOPY );

}

void YBButton::_onWM_LBUTTONDBLCLK( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDBLCLK );
	WPARAM wParam;
	LPARAM lParam;
	
	if ( m_eBtnStatus == EBTN_STATUS_DISABLE ) {
		return;
	}
	//
	wParam = MAKELONG( m_wordCtrlID, YBBTN_CMDEVT_DBLCLK );
	lParam = ( LPARAM )m_hWnd;
	::SendMessage( ::GetParent( m_hWnd ), YBWM_BTN_DBLCLK, wParam, lParam );

}

void YBButton::_onWM_LBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
	HDC hDC;
	RECT rcClient;
//	POINT ptMouse;
	unsigned int uBtnFlagOld;

	YBToolTip::getInstance()->hide();
	if ( m_eBtnStatus == EBTN_STATUS_DISABLE ) {
		return; 
	}

	uBtnFlagOld = m_uBtnFlag;
	m_uBtnFlag |= YBBTN_FLAG_LBTNDOWN;
	m_uBtnFlag |= YBBTN_FLAG_TOOLTIP_DISAPPERAR;
	m_eBtnStatus = EBTN_STATUS_PRESS;
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eBtnStatus );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), (rcClient.bottom - rcClient.top), memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
	::SetFocus( m_hWnd );
	// send command message to parent window.
// 	if ( ::PtInRect( &rcClient, ptMouse ) ) {
// 		WPARAM wParam;
// 		LPARAM lParam;
// 
// 		wParam = MAKELONG( m_wordCtrlID, YBBTNX_CMDEVT_BTNXDOWN );
// 		lParam = (LPARAM)m_hWnd;
// 		::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
// 	}
	
}

void YBButton::_onWM_LBUTTONUP( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONUP );
	HDC hDC;
	RECT rcClient;
	POINT ptMouse;
	unsigned int uBtnFlagOld;
	// Save last state flag
	uBtnFlagOld = m_uBtnFlag;

	if ( m_eBtnStatus == EBTN_STATUS_DISABLE ) {
		return; 
	}

	m_uBtnFlag &= ~YBBTN_FLAG_LBTNDOWN;
	m_eBtnStatus = EBTN_STATUS_HOVER;
	ptMouse.x = (__int16)LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = (__int16)HIWORD( pYBCtrlWndMsg->m_lParam );

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eBtnStatus );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), (rcClient.bottom - rcClient.top), memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );

	// send command message to parent window.
	if ( ::PtInRect( &rcClient, ptMouse ) ) {
		WPARAM wParam;
		LPARAM lParam;
		if( ( uBtnFlagOld & YBBTN_FLAG_LBTNDOWN ) == YBBTN_FLAG_LBTNDOWN ){

			wParam = MAKELONG( m_wordCtrlID, YBBTNX_CMDEVT_BTNDOWN );
			lParam = (LPARAM)m_hWnd;
			::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
		}
	}
}

void YBButton::_onWM_MOUSEMOVE( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	HDC hDC;
	RECT rcClient;
	TRACKMOUSEEVENT trackMouseEvt;

	if ( m_eBtnStatus == EBTN_STATUS_DISABLE ) {
		YBToolTip::getInstance()->hide();
		return; 
	}

	if ( m_uBtnFlag & YBBTN_FLAG_LBTNDOWN ) {
		m_eBtnStatus = EBTN_STATUS_PRESS;
	} else {
		m_eBtnStatus = EBTN_STATUS_HOVER;
		//
		if ( ( m_uBtnFlag & YBBTN_FLAG_TOOLTIP_DISAPPERAR ) != YBBTN_FLAG_TOOLTIP_DISAPPERAR ) {
			if ( ( m_uBtnFlag & YBBTN_FLAG_ADD_TOOLTIP ) != YBBTN_FLAG_ADD_TOOLTIP ) {
				if ( m_bTipText ) {
					YBToolTip::getInstance()->updateTipText( m_tstrTipText, m_hWnd );
				} else {
					YBToolTip::getInstance()->updateTipText( m_tstrText, m_hWnd );
				}
			}
		}
	}
	trackMouseEvt.cbSize = sizeof( TRACKMOUSEEVENT );
	trackMouseEvt.dwFlags = TME_LEAVE;
	trackMouseEvt.hwndTrack = pYBCtrlWndMsg->m_hWnd;
	trackMouseEvt.dwHoverTime = HOVER_DEFAULT;
	::TrackMouseEvent ( &trackMouseEvt );
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eBtnStatus );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), (rcClient.bottom - rcClient.top), memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
}

void YBButton::_onWM_MOUSELEAVE( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSELEAVE );
	HDC hDC;
	RECT rcClient;

	if ( m_eBtnStatus == EBTN_STATUS_DISABLE ) {
		return; 
	}

	m_uBtnFlag &= ~YBBTN_FLAG_LBTNDOWN;
	m_eBtnStatus = EBTN_STATUS_NORMAL;
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eBtnStatus );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), (rcClient.bottom - rcClient.top), memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );

	m_uBtnFlag &= ~YBBTN_FLAG_TOOLTIP_DISAPPERAR;
	if ( ( m_uBtnFlag & YBBTN_FLAG_ADD_TOOLTIP ) != YBBTN_FLAG_ADD_TOOLTIP ) {
		YBToolTip::getInstance()->hide();
	}
}

void YBButton::setImage( ENUMBTNSTATUS eBtnXStatus, unsigned int uImageID ) {
	switch ( eBtnXStatus ) 
	{
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

unsigned int YBButton::_getImageID( ENUMBTNSTATUS eBtnStatus ) {
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
	default:
		{
			assert( false );
		}
		break;
	}

	assert( false );
	return YBCTRL_IMAGE_ID_INVALID;
}

void YBButton::setFixedSpace( unsigned int uLFixed, unsigned int uTFixed, unsigned int uRFixed, unsigned int uBFixed ) {
	m_uLFixed = uLFixed;
	m_uTFixed = uTFixed;
	m_uRFixed = uRFixed;
	m_uBFixed = uBFixed;
}

Gdiplus::Image* YBButton::_getGPImage( ENUMBTNSTATUS eBtnStatus ) {
	YBCtrlImageStatic* pYBImageStatic = NULL;

	pYBImageStatic = _getYBImageStatic( eBtnStatus );
	if ( !pYBImageStatic )
		return NULL;

	return pYBImageStatic->getImage();
}

YBCtrlImageStatic* YBButton::_getYBImageStatic( ENUMBTNSTATUS eBtnStatus ) {
	YBCtrlImageBase* pYBImageBase = NULL;

	pYBImageBase = _getYBImageBase( eBtnStatus );
	if ( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

YBCtrlImageStatic* YBButton::_getImageStatic( unsigned int uImageId ) const{
	YBCtrlImageBase* pImageBase = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pImageBase ) )
		return NULL;
	if( !pImageBase || ( pImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pImageBase;
}

YBCtrlImageBase* YBButton::_getYBImageBase( ENUMBTNSTATUS eBtnStatus ) {
	YBCtrlImageBase* pYBImage = NULL;

	if ( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( _getImageID( eBtnStatus ), &pYBImage ) )
		return NULL;

	return pYBImage;
}

void YBButton::_drawStatus( HDC hDC, ENUMBTNSTATUS eBtnStatus ) {
	if ( m_uImageIDBtnNormal == 77 ) {
		int a = 0;
	}
	if ( !getHWND() )
		return;
	RECT rcClient;
	YBCtrlImageStatic* pYBImageBtn = NULL;
	HDC hMemDC;

	::GetClientRect( m_hWnd, &rcClient );
	pYBImageBtn = _getYBImageStatic( eBtnStatus );

	// draw back ground 
	if ( !pYBImageBtn ) {
		return;
	}
	hMemDC = pYBImageBtn->getMemDC();
	BackGroundBitBlt( hDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top )
		, hMemDC, 0, 0, pYBImageBtn->getWidth(), pYBImageBtn->getHeight()
		, m_uLFixed, m_uTFixed, m_uRFixed, m_uBFixed );
	// draw Text
	_drawText( hDC );
	// drawLIcon.
	_drawLIcon( hDC );
	// drawRIcon.
	_drawRIcon( hDC );
	// drawIcon.
	_drawIcon( hDC );
}

void YBButton::_drawText( HDC hDC ) {
	//
	RECT rcDrawText;
	RECT rcCalc;
	const YBCtrlFont* pYBCtrlFont = NULL;
	HFONT hFontOld = NULL;
	int iBkModeOld;
	COLORREF clrTxtOld;
	unsigned int uLineTop = 0;
	tstring_type tstrLineData;

	if ( _getDrawTextRect( rcDrawText )
		&& ( rcDrawText.right - rcDrawText.left )
		&& ( rcDrawText.bottom - rcDrawText.top )
		&& ( m_tstrText.length() > 0 ) ) {
		pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontId );
		if ( pYBCtrlFont ) {
			hFontOld = ( HFONT )::SelectObject( hDC, pYBCtrlFont->m_hFont );
			iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
			if ( m_eBtnStatus == EBTN_STATUS_DISABLE ) {	//是否设置 EBTN_STATUS_DISABLE 状态时字体的眼色
				clrTxtOld = ::SetTextColor( hDC, m_rgbDisTxtClr );
			} else {			//是否设置 EBTN_STATUS_HOVER 状态时字体的眼色
				if ( m_bSetHovTxtClr  && m_eBtnStatus == EBTN_STATUS_HOVER ) {
					clrTxtOld = ::SetTextColor( hDC, m_rgbHovTxtClr );
				} else if ( m_bSetPreTxtClr && m_eBtnStatus == EBTN_STATUS_PRESS ) {
					clrTxtOld = ::SetTextColor( hDC, m_rgbPreTxtClr );
				} else {
					clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
				}
			}
			if ( m_containerTxt.size() == 1 ) {     // 单行
				if ( m_bVcenter ) {
					::DrawText( hDC, m_tstrText.c_str(), m_tstrText.length(), &rcDrawText, DT_SINGLELINE|DT_END_ELLIPSIS|DT_CENTER|DT_VCENTER );
				} else {
					::DrawText( hDC, m_tstrText.c_str(), m_tstrText.length(), &rcDrawText, DT_EDITCONTROL|DT_WORDBREAK|DT_CENTER );
				}
			} else {								// 多行
				for ( unsigned int uIndex = 0; uIndex != m_containerTxt.size(); ++uIndex ) {
					uLineTop = rcDrawText.top + m_uLineHeight*uIndex;
					tstrLineData = m_containerTxt[ uIndex ];
					::SetRect( &rcCalc, rcDrawText.left, uLineTop, rcDrawText.right, uLineTop + m_uLineHeight );
					if ( m_bVcenter ) {
						::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_END_ELLIPSIS|DT_CENTER|DT_VCENTER );
					} else {
						::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT|DT_CENTER );
					}
				}
			}
			::SetTextColor( hDC, clrTxtOld );
			::SetBkMode( hDC, iBkModeOld );
			::SelectObject( hDC, hFontOld );
		}
	}
}

void YBButton::_drawIcon( HDC hDC ) {
	RECT rcDrawIcon;
	//
	if ( m_hIcon ) {
		if ( m_eBtnStatus != YBButton::EBTN_STATUS_DISABLE )
		{
			_getDrawIconRect( rcDrawIcon );
			::DrawIconEx( hDC, rcDrawIcon.left, rcDrawIcon.top, m_hIcon, m_sizeIcon.cx, m_sizeIcon.cy, 0, NULL, DI_NORMAL );
		}

	}
}

void YBButton::_drawLIcon( HDC hDC ) {
	if ( ( m_uBtnFlag & YBBTN_FLAG_DRAWLICON ) != YBBTN_FLAG_DRAWLICON )
		return;
	YBCtrlImageStatic* pYBIcon = NULL;
	if ( m_eBtnStatus == EBTN_STATUS_HOVER ) {
		pYBIcon = _getImageStatic( m_uImageIdLIconHov );
	} else if ( m_eBtnStatus == EBTN_STATUS_PRESS ) {
		pYBIcon = _getImageStatic( m_uImageIdLIconPre );
	} else {
		pYBIcon = _getImageStatic( m_uImageIdLIconNor );
	}
	if ( !pYBIcon )
		return;
	//
	Gdiplus::Graphics graphics( hDC );
	GdipBitBlt( &graphics, pYBIcon->getImage(), m_uLIconLSpace, m_uLIconTSpace, m_uLIconWidth, m_uLIconHeight
		, 0, 0, pYBIcon->getWidth(), pYBIcon->getHeight() );
}

void YBButton::_drawRIcon( HDC hDC ) {
	if ( ( m_uBtnFlag & YBBTN_FLAG_DRAWRICON ) != YBBTN_FLAG_DRAWRICON )
		return;
	RECT rcClient;
	YBCtrlImageStatic* pYBIcon = NULL;

	::GetClientRect( m_hWnd, &rcClient );
	if ( m_eBtnStatus == EBTN_STATUS_HOVER ) {
		pYBIcon = _getImageStatic( m_uImageIdRIconHov );
	} else if ( m_eBtnStatus == EBTN_STATUS_PRESS ) {
		pYBIcon = _getImageStatic( m_uImageIdRIconPre );
	} else {
		pYBIcon = _getImageStatic( m_uImageIdRIconNor );
	}
	if ( !pYBIcon )
		return;
	//
	Gdiplus::Graphics graphics( hDC );
	GdipBitBlt( &graphics, pYBIcon->getImage(), rcClient.right - rcClient.left - m_uRIconRSpace, m_uRIconTSpace, m_uRIconWidth, m_uRIconHeight
		, 0, 0, pYBIcon->getWidth(), pYBIcon->getHeight() );
}

bool YBButton::_getDrawTextRect( RECT& rcDrawText ){
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcDrawText.left = rcClient.left + m_uTxtSpaceL;
	rcDrawText.top = rcClient.top + m_uTxtSpaceT;
	rcDrawText.right = rcClient.right - m_uTxtSpaceR;
	rcDrawText.bottom = rcClient.bottom - m_uTxtSpaceB;

	return ( rcDrawText.right >= rcDrawText.left ) && ( rcDrawText.bottom >= rcDrawText.top );
}

void YBButton::setFontId( unsigned int uFontId ){
	if( !YBCtrlFontDepository::getInstance()->hasYBFont( uFontId ) ){
		return;
	}
	m_uFontId = uFontId;
	_calcLineHeight();
}

void YBButton::setTextSpace( unsigned int uTxtSpaceL,unsigned int uTxtSpaceT, unsigned int uTxtSpaceR, unsigned int uTxtSpaceB ){
	m_uTxtSpaceL = uTxtSpaceL;
	m_uTxtSpaceB = uTxtSpaceB;
	m_uTxtSpaceR = uTxtSpaceR;
	m_uTxtSpaceT = uTxtSpaceT;
}

void YBButton::setText( const tstring_type& tstrText ){
	_calcEveryPartTxt();
	m_tstrText = tstrText;
}

void YBButton::_calcLineHeight() {
	const YBCtrlFont* pYBFont = NULL;

	pYBFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontId );
	if ( !pYBFont ) {
		assert( false );
		return;
	}
	if ( pYBFont->m_logFont.lfHeight > 0 ) {
		m_uLineHeight = pYBFont->m_logFont.lfHeight;
	} else {
		m_uLineHeight = -( pYBFont->m_logFont.lfHeight );
	}
}

void YBButton::_calcEveryPartTxt() {
	m_containerTxt.clear();
	//
	TCHAR tchData;
	tstring_type tstrLineData;
	unsigned int uLenStrData;
	unsigned int uCharIndex = 0;
	//
	uLenStrData = m_tstrText.length();
	for ( unsigned int uIndex = 0; uIndex < uLenStrData; ++uIndex ) {
		tchData = m_tstrText[ uIndex ];
		if ( tchData == '\n' ) {
			tstrLineData = m_tstrText.substr( uCharIndex, uIndex - uCharIndex );
			m_containerTxt.push_back( tstrLineData );
			uCharIndex = uIndex;
		}
	}
	tstrLineData = m_tstrText.substr( uCharIndex, uLenStrData - uCharIndex );
	m_containerTxt.push_back( tstrLineData );
}

void YBButton::enable( bool bEnable ) {
	if ( ( m_eBtnStatus != EBTN_STATUS_DISABLE ) == bEnable ) {
		return; // need to nothing.
	}

	//
	if ( bEnable ) {
		m_eBtnStatus = YBButton::EBTN_STATUS_NORMAL;
	} else {
		m_eBtnStatus = YBButton::EBTN_STATUS_DISABLE;
	}

    updateStatus();
}

void YBButton::setLIconSize( unsigned int uWidth, unsigned int uHeight ) {
	m_uLIconWidth = uWidth;
	m_uLIconHeight = uHeight;
}

void YBButton::setLIconSpace( unsigned int uLSpace, unsigned int uTSpace ) {
	m_uLIconLSpace = uLSpace;
	m_uLIconTSpace = uTSpace;
}

void YBButton::setImageLIcon( unsigned int uLIconIdNor, unsigned int uLIconIdHov, unsigned int uLIconIdPre ) {
	m_uImageIdLIconNor = uLIconIdNor;
	m_uImageIdLIconHov = uLIconIdHov;
	m_uImageIdLIconPre = uLIconIdPre;
}

void YBButton::updateStatus() {
	if ( !m_hWnd )
		return;
	if ( !IsWindow( m_hWnd ) ) {
		assert(false);
		return;
	}

	::InvalidateRect( m_hWnd, NULL, TRUE );
}

void YBButton::addToolTip( bool bEnable ) {
	if ( bEnable ) {
		m_uBtnFlag &= ~YBBTN_FLAG_ADD_TOOLTIP;
	} else {
		m_uBtnFlag |= YBBTN_FLAG_ADD_TOOLTIP;
	}
}

void YBButton::setTipText( bool bTipText, const tstring_type& tstrTipText ) {
	if ( bTipText ) {
		m_bTipText = true;
		m_tstrTipText = tstrTipText;
	} else {
		m_bTipText = false;
		return;
	}
}

void YBButton::setTxtVcenter( bool bVcenter ) {
	if( bVcenter ) {
		m_bVcenter = true;
	} else {
		m_bVcenter = false;
	}
}

void YBButton::setHovTxtClr( COLORREF clrTxt /*= RGB( 143, 70, 1 )*/ ) {
	m_bSetHovTxtClr = true;
	m_rgbHovTxtClr = clrTxt;
}

void YBButton::setPreTxtClr(COLORREF clrTxt /*= RGB(143, 70, 1)*/)
{
	m_bSetPreTxtClr = true;
	m_rgbPreTxtClr = clrTxt;
}

void YBButton::setDisTxtClr( COLORREF clrTxt /* = YBCOLOR_GRAY*/ ) {
	m_rgbDisTxtClr = clrTxt;
}

void YBButton::popUp() {
	if ( !::IsWindow( m_hWnd ) )
		return;
	if ( m_eBtnStatus == EBTN_STATUS_DISABLE )
		return;

	m_eBtnStatus = EBTN_STATUS_NORMAL;
	//
	updateStatus();
}

void YBButton::pushDown() {
	if ( !::IsWindow( m_hWnd ) )
		return;
	if ( m_eBtnStatus == EBTN_STATUS_DISABLE )
		return;

	m_eBtnStatus = EBTN_STATUS_PRESS;
	//
	updateStatus();
}

void YBButton::setImageRIcon( unsigned int uRIconIdNor, unsigned int uRIconIdHov, unsigned int uRIconIdPre ) {
	m_uImageIdRIconNor = uRIconIdNor;
	m_uImageIdRIconHov = uRIconIdHov;
	m_uImageIdRIconPre = uRIconIdPre;
}

void YBButton::setRIconSize( unsigned int uWidth, unsigned int uHeight ) {
	m_uRIconWidth = uWidth;
	m_uRIconHeight = uHeight;
}

void YBButton::setRIconSpace( unsigned int uRSpace, unsigned int uTSpace ) {
	m_uRIconRSpace = uRSpace;
	m_uRIconTSpace = uTSpace;
}

void YBButton::bDrawLIcon( bool bDrawLIcon ) {
	if ( bDrawLIcon ) {
		m_uBtnFlag |= YBBTN_FLAG_DRAWLICON;
	} else {
		m_uBtnFlag &= ~YBBTN_FLAG_DRAWLICON;
	}
}

void YBButton::bDrawRIcon( bool bDrawRIcon ) {
	if ( bDrawRIcon ) {
		m_uBtnFlag |= YBBTN_FLAG_DRAWRICON;
	} else {
		m_uBtnFlag &= ~YBBTN_FLAG_DRAWRICON;
	}
}

void YBButton::setIcon( HICON hIcon, unsigned int uWidth, unsigned int uHeight )
{
	m_hIcon = hIcon;
	m_sizeIcon.cx = uWidth;
	m_sizeIcon.cy = uHeight;
}

void YBButton::_getDrawIconRect( RECT& rcDrawIcon ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcDrawIcon.left = rcClient.left + m_uIconSpaceL;
	rcDrawIcon.right = rcDrawIcon.left + m_sizeIcon.cx;
	rcDrawIcon.top = rcClient.top + ( rcClient.bottom - rcClient.top - m_sizeIcon.cy ) / 2;
	rcDrawIcon.bottom = rcDrawIcon.top + m_sizeIcon.cy;
}