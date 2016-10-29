#include "stdafx.h"
#include "frame/YBCtrlBase.h"
#include "checkbox/YBCheckBox.h"
#include "frame/YBCtrlImageBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlMisc.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlFontDepository.h"
#include "gdiplus.h"
#include "tooltip/YBToolTip.h"
#include  <assert.h>

#define YBCHECK_FLAG_NONE (0x00000000)
#define YBCHECK_FLAG_LBTDOWN (0x00000001)
#define YBCHECK_FLAG_TOOLTIP (0x00000002)
#define YBCHECK_FLAG_TOOLTIP_SETTEXT (0x00000004)

USING_NAMESPACE_YBCTRL

YBCheckBox::YBCheckBox()
: m_uFlag(YBCHECK_FLAG_NONE)
, m_uImageIdBK( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdCNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdCHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdCPre( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdNCNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdNCHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdNCPre( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdNCDisable( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdCDisable( YBCTRL_IMAGE_ID_INVALID )
, m_eCheckStatus( ENOCHECK_STATUS_NORMAL )
, m_wordCtrlId(0)
, m_uIconHeight(0)
, m_uIconWidth(0)
, m_uIconOriginL(12)
, m_uIconOriginT(12)
, m_uLFixed(5)
, m_uTFixed(5)
, m_uRFixed(5)
, m_uBFixed(5)
, m_uTxtSpaceL( 0 )
, m_uTxtSpaceT( 0 )
, m_uTxtSpaceB( 0 )
, m_uTxtSpaceR( 0 )
, m_tstrText( _T("") )
, m_tstrToolTipTxt( _T("") )
, m_bTipText( false )
, m_uFontId( YBCTRL_FONT_ID_INVALID )
{

}

YBCheckBox::~YBCheckBox(){

}

YBCTRL_WNDMSGMAP_BEGIN( YBCheckBox, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_LBUTTONUP, _onWM_LBUTTONUP )
	YBCTRL_ONWNDMSG( WM_MOUSELEAVE, _onWM_MOUSELEAVE )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_DESTROY, _onWM_DESTROY )
YBCTRL_WNDMSGMAP_END()

void YBCheckBox::_onWM_DESTROY( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_DESTROY );
	//
	YBToolTip::getInstance()->hide();
}

void YBCheckBox::_onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );

	HDC hPaintDC;
	PAINTSTRUCT ps;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eCheckStatus );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0,( rcClient.right - rcClient.left ),( rcClient.bottom - rcClient.top ), memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void YBCheckBox::_onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	pYBCtrlWndMsg->m_dwFlag |= ( YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID );
	pYBCtrlWndMsg->m_lRes = 1;

	RECT rcClient;
	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;

	::GetClientRect( pYBCtrlWndMsg->m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eCheckStatus );

	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );

}

void YBCheckBox::_onWM_LBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
	HDC hDC;
	RECT rcClient;
	RECT rcCheckBox;
	
	_getBoxRect( rcCheckBox );

	if( m_eCheckStatus == ECHECK_STATUS_DISABLE )
		return;

	if( m_eCheckStatus == ENOCHECK_STATUS_DISABLE )
		return;

	m_uFlag |= YBCHECK_FLAG_LBTDOWN;
	if( m_eCheckStatus == ECHECK_STATUS_HOVER ){
		m_eCheckStatus = ECHECK_STATUS_PRESS;
	}else if ( m_eCheckStatus == ENOCHECK_STATUS_HOVER ) {
		m_eCheckStatus = ENOCHECK_STATUS_PRESS;
	}else if ( m_eCheckStatus == ECHECK_STATUS_NORMAL ) {
		m_eCheckStatus = ECHECK_STATUS_PRESS;
	}else if ( m_eCheckStatus == ENOCHECK_STATUS_NORMAL ) {
		m_eCheckStatus = ENOCHECK_STATUS_PRESS;
	}else {
		assert( false );
	}

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eCheckStatus );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0,( rcClient.right - rcClient.left ),( rcClient.bottom - rcClient.top ), memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
}

void YBCheckBox::_onWM_LBUTTONUP( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONUP );
	HDC hDC;
	RECT rcClient;
	RECT rcCheckBox;
	unsigned int uFlagOld;

	uFlagOld = m_uFlag;
	_getBoxRect( rcCheckBox );
	//
	WPARAM wParam;
	LPARAM lParam;

	if( m_eCheckStatus == ECHECK_STATUS_DISABLE )
		return;

	if( m_eCheckStatus == ENOCHECK_STATUS_DISABLE )
		return;

	m_uFlag &= ~YBCHECK_FLAG_LBTDOWN;
	if(( m_eCheckStatus == ECHECK_STATUS_PRESS ) || ( m_eCheckStatus == ENOCHECK_STATUS_HOVER ) || ( m_eCheckStatus == ENOCHECK_STATUS_NORMAL ) ){
		m_eCheckStatus = ENOCHECK_STATUS_HOVER;
		//
		lParam = (LPARAM)isCheck();
	}else if(( m_eCheckStatus == ENOCHECK_STATUS_PRESS ) || ( m_eCheckStatus == ECHECK_STATUS_HOVER ) || ( m_eCheckStatus == ECHECK_STATUS_NORMAL ) ){
		m_eCheckStatus = ECHECK_STATUS_HOVER;
		//
		lParam = (LPARAM)isCheck();
	}else{
		assert(false);
	}
	//
	if ( ( uFlagOld & YBCHECK_FLAG_LBTDOWN ) == YBCHECK_FLAG_LBTDOWN ) {
		wParam = MAKELONG( m_wordCtrlId, YBCHECKBOX_CMDEVT_DOWN );
		::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
	}

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eCheckStatus );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0,( rcClient.right - rcClient.left ),( rcClient.bottom - rcClient.top ), memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
}

void YBCheckBox::_onWM_MOUSEMOVE( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	HDC hDC;
	RECT rcClient;
	RECT rcCheckBox;
	TRACKMOUSEEVENT trackMouseEvt;

	_getBoxRect( rcCheckBox );

	if( m_eCheckStatus == ECHECK_STATUS_DISABLE )
		return;
	if( m_eCheckStatus == ENOCHECK_STATUS_DISABLE )
		return;

	switch ( m_eCheckStatus ) {
	case ENOCHECK_STATUS_NORMAL:
		{
			if ( m_uFlag & YBCHECK_FLAG_LBTDOWN ) {
				m_eCheckStatus = ENOCHECK_STATUS_PRESS;
			} else {
				m_eCheckStatus = ENOCHECK_STATUS_HOVER;
			}
		}
		break;
	case ENOCHECK_STATUS_HOVER:
		{
			if ( m_uFlag & YBCHECK_FLAG_LBTDOWN ) {
				m_eCheckStatus = ENOCHECK_STATUS_PRESS;
			} else {
				m_eCheckStatus = ENOCHECK_STATUS_HOVER;
			}
		}
		break;
	case ENOCHECK_STATUS_PRESS:
		{
			if ( m_uFlag & YBCHECK_FLAG_LBTDOWN ) {
				m_eCheckStatus = ENOCHECK_STATUS_PRESS;
			} else {
				m_eCheckStatus = ENOCHECK_STATUS_HOVER;
			}
		}
		break;
	case ECHECK_STATUS_NORMAL:
		{
			if ( m_uFlag & YBCHECK_FLAG_LBTDOWN ) {
				m_eCheckStatus = ECHECK_STATUS_PRESS;
			} else {
				m_eCheckStatus = ECHECK_STATUS_HOVER;
			}
		}
		break;
	case ECHECK_STATUS_HOVER:
		{
			if ( m_uFlag & YBCHECK_FLAG_LBTDOWN ) {
				m_eCheckStatus = ECHECK_STATUS_PRESS;
			} else {
				m_eCheckStatus = ECHECK_STATUS_HOVER;
			}
		}
		break;
	case ECHECK_STATUS_PRESS:
		{
			if ( m_uFlag & YBCHECK_FLAG_LBTDOWN ) {
				m_eCheckStatus = ECHECK_STATUS_PRESS;
			} else {
				m_eCheckStatus = ECHECK_STATUS_HOVER;
			}
		}
		break;
	default:
		break;
	}
	
	trackMouseEvt.cbSize = sizeof( TRACKMOUSEEVENT );
	trackMouseEvt.dwFlags = TME_LEAVE;
	trackMouseEvt.hwndTrack = pYBCtrlWndMsg->m_hWnd;
	trackMouseEvt.dwHoverTime = HOVER_DEFAULT;
	::TrackMouseEvent ( &trackMouseEvt );
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eCheckStatus );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), (rcClient.bottom - rcClient.top), memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );

	if ( ( m_uFlag & YBCHECK_FLAG_TOOLTIP ) == YBCHECK_FLAG_TOOLTIP ) {
		if ( m_bTipText ) {
			YBToolTip::getInstance()->updateTipText( m_tstrToolTipTxt, m_hWnd );
		} else {
			YBToolTip::getInstance()->updateTipText( m_tstrText, m_hWnd );
		}
	}
}

void YBCheckBox::_onWM_MOUSELEAVE( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSELEAVE );
	HDC hDC;
	RECT rcClient;

	if( m_eCheckStatus == ECHECK_STATUS_DISABLE )
		return;

	if( m_eCheckStatus == ENOCHECK_STATUS_DISABLE )
		return;

	m_uFlag &= ~YBCHECK_FLAG_LBTDOWN;
	if( m_eCheckStatus == ECHECK_STATUS_NORMAL ){
		m_eCheckStatus = ECHECK_STATUS_HOVER;
	}else if ( m_eCheckStatus == ENOCHECK_STATUS_NORMAL ){
		m_eCheckStatus = ENOCHECK_STATUS_HOVER;
	}else if ( ( m_eCheckStatus == ENOCHECK_STATUS_HOVER ) || ( m_eCheckStatus == ENOCHECK_STATUS_PRESS )){
		m_eCheckStatus = ENOCHECK_STATUS_NORMAL;
	}else if( ( m_eCheckStatus == ECHECK_STATUS_HOVER ) || ( m_eCheckStatus == ECHECK_STATUS_PRESS )){
		m_eCheckStatus = ECHECK_STATUS_NORMAL;
	}else{
		assert(false);
	}

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC(), m_eCheckStatus );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), (rcClient.bottom - rcClient.top), memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
	//
	if ( ( m_uFlag & YBCHECK_FLAG_TOOLTIP ) == YBCHECK_FLAG_TOOLTIP ) {
		YBToolTip::getInstance()->hide();
	}
}

HWND YBCheckBox::Create( DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance ){
	HWND hWnd;

	dwStyle |= WS_CLIPSIBLINGS;
	hWnd = _createHWND( 0, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, NULL );
	if( !hWnd ){
		assert(false);
	}
	m_wordCtrlId = ( unsigned int )hMenu;
	return hWnd;
}

Gdiplus::Image* YBCheckBox::_getGPImage( ECHECKBOXSTATUS eCheckStatus ){
	YBCtrlImageStatic* pYBImageStatic = NULL;

	pYBImageStatic = _getImageStatic( eCheckStatus );
	if ( !pYBImageStatic )
		return NULL;

	return pYBImageStatic->getImage();
}

YBCtrlImageBase* YBCheckBox::_getImageBase( ECHECKBOXSTATUS eCheckStatus ){
	YBCtrlImageBase* pYBImage = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( _getCheckImageID( eCheckStatus ), &pYBImage ) )
		return NULL;

	return pYBImage;
	
}

YBCtrlImageStatic* YBCheckBox::_getImageStatic( ECHECKBOXSTATUS eCheckStatus ){
	YBCtrlImageBase* pYBImageBase = NULL;
	pYBImageBase = _getImageBase( eCheckStatus );

	if ( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

unsigned int YBCheckBox::_getCheckImageID( ECHECKBOXSTATUS eCheckStatus ){
	switch( eCheckStatus ){
	case ECHECK_STATUS_NORMAL:
		{
			return m_uImageIdCNor;
		}
		break;
	case ECHECK_STATUS_HOVER:
		{
			return m_uImageIdCHov;
		}
		break;
	case ECHECK_STATUS_PRESS:
		{
			return m_uImageIdCPre;
		}
		break;
	case ENOCHECK_STATUS_HOVER:
		{
			return m_uImageIdNCHov;
		}
		break;
	case ENOCHECK_STATUS_NORMAL:
		{
			return m_uImageIdNCNor;
		}
		break;
	case ENOCHECK_STATUS_PRESS:
		{
			return m_uImageIdNCPre;
		}
		break;
	case ECHECK_STATUS_DISABLE:
		{
			return m_uImageIdCDisable;
		}
		break;
	case ENOCHECK_STATUS_DISABLE:
		{
			return m_uImageIdNCDisable;
		}
		break;
	default:
		{
			assert(false);
		}
		break;
	}
	assert(false);
	return YBCTRL_IMAGE_ID_INVALID;
}

void YBCheckBox::_getBoxRect( RECT& rcCheckBox ){
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	
	m_uIconOriginT = ( rcClient.bottom - rcClient.top - m_uIconHeight )/2;
	rcCheckBox.left = rcClient.left + m_uIconOriginL;
	rcCheckBox.right = rcCheckBox.left + m_uIconWidth ;
	rcCheckBox.top = rcClient.top + m_uIconOriginT;
	rcCheckBox.bottom = rcCheckBox.top + m_uIconHeight ;
}

void YBCheckBox::_drawStatus( HDC hDC, ECHECKBOXSTATUS eCheckStatus ){
	if ( !getHWND() )
		return;
	RECT rcClient;
	RECT rcChickTick;
	RECT rcDrawText;
	int iBkModeOld;
	COLORREF clrTxtOld;

	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	YBCtrlImageStatic* pYBImageCheck = NULL;
	YBCtrlImageStatic* pYBImageCheckBK = NULL;

	::GetClientRect( m_hWnd, &rcClient );
	pYBImageCheck = _getImageStatic( eCheckStatus );
	pYBImageCheckBK = _getBKImageStatic( m_uImageIdBK );

	if ( !pYBImageCheck ) {
		return;
	}
	if ( !pYBImageCheckBK ) {
		return;
	}

	m_uIconWidth = pYBImageCheck->getWidth();
	m_uIconHeight = pYBImageCheck->getHeight();
	////draw BackGround
	BackGroundBitBlt( hDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top )
		, pYBImageCheckBK->getMemDC(), 0, 0, pYBImageCheckBK->getWidth(), pYBImageCheckBK->getHeight()
		, m_uLFixed, m_uTFixed, m_uRFixed, m_uBFixed );
	// draw text
	if ( _getDrawTextRect( rcDrawText ) 
		&& ( rcDrawText.right - rcDrawText.left )
		&& ( rcDrawText.bottom - rcDrawText.top )
		&& ( m_tstrText.length() > 0 ) ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontId );
			if ( pYBCtrlFont ) {
				hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
				iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
				if (m_eCheckStatus == ECHECK_STATUS_DISABLE || m_eCheckStatus == ENOCHECK_STATUS_DISABLE)
				{
					clrTxtOld = ::SetTextColor( hDC, ::GetSysColor(COLOR_GRAYTEXT));
				}
				else
				{
					clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
				}
				::DrawText( hDC, m_tstrText.c_str(), m_tstrText.length(), &rcDrawText, DT_VCENTER|DT_END_ELLIPSIS|DT_SINGLELINE );
				::SetTextColor( hDC, clrTxtOld );				
				::SetBkMode( hDC, iBkModeOld );
				::SelectObject( hDC, hFontOld );
			}
	}
	// draw icon 
	//	Gdiplus::Graphics graphics( hDC );
	// 	GdipBitBlt( &graphics, pYBImageCheck->getImage(), rcChickTick.left, rcChickTick.top, (rcChickTick.right - rcChickTick.left), (rcChickTick.bottom - rcChickTick.top)
	// 		, 0, 0, pYBImageCheck->getWidth(), pYBImageCheck->getHeight() );
	_getBoxRect( rcChickTick );
	BackGroundBitBlt( hDC, rcChickTick.left, rcChickTick.top, rcChickTick.right - rcChickTick.left, rcChickTick.bottom - rcChickTick.top, pYBImageCheck->getMemDC(),
		0, 0, pYBImageCheck->getWidth(), pYBImageCheck->getHeight(), 0, 0, 0, 0 );
}

void YBCheckBox::setImage( ECHECKBOXSTATUS eCheckStatus, unsigned int uImageID ) {
	switch ( eCheckStatus ) {
	case ENOCHECK_STATUS_NORMAL:
		{
			m_uImageIdNCNor = uImageID;
		}
		break;
	case ENOCHECK_STATUS_HOVER:
		{
			m_uImageIdNCHov = uImageID;
		}
		break;
	case ENOCHECK_STATUS_PRESS:
		{
			m_uImageIdNCPre = uImageID;
		}
		break;
	case ECHECK_STATUS_NORMAL:
		{
			m_uImageIdCNor = uImageID;
		}
		break;
	case ECHECK_STATUS_HOVER:
		{
			m_uImageIdCHov = uImageID;
		}
		break;
	case ECHECK_STATUS_PRESS:
		{
			m_uImageIdCPre = uImageID;
		}
		break;
	case ECHECK_STATUS_DISABLE:
		{
			m_uImageIdCDisable = uImageID;
		}
		break;
	case ENOCHECK_STATUS_DISABLE:
		{
			m_uImageIdNCDisable = uImageID;
		}
		break;
	default:
		{
			assert( false );
		}
		break;
	}
}

Gdiplus::Image* YBCheckBox::_getBKGPImage( unsigned int uImageId ){
	
	YBCtrlImageStatic* pYBImageStatic = NULL;

	pYBImageStatic = _getBKImageStatic( uImageId );
	if ( !pYBImageStatic )
		return NULL;

	return pYBImageStatic->getImage();
}
YBCtrlImageBase* YBCheckBox::_getBKImageBase( unsigned int uImageId ){
	YBCtrlImageBase* pYBImage = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( m_uImageIdBK, &pYBImage ) )
		return NULL;

	return pYBImage;

}
YBCtrlImageStatic* YBCheckBox::_getBKImageStatic( unsigned int uImageId ){
	YBCtrlImageBase* pYBImageBase = NULL;
	pYBImageBase = _getBKImageBase( uImageId );

	if ( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

void YBCheckBox::setBKImageID( unsigned int uImageId ){
	
	m_uImageIdBK  = uImageId;
}

bool YBCheckBox::_getDrawTextRect( RECT& rcDrawText ){
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcDrawText.left = rcClient.left + m_uTxtSpaceL;
	rcDrawText.top = rcClient.top + m_uTxtSpaceT;
	rcDrawText.right = rcClient.right - m_uTxtSpaceR;
	rcDrawText.bottom = rcClient.bottom - m_uTxtSpaceB;

	return ( rcDrawText.right >= rcDrawText.left ) && ( rcDrawText.bottom >= rcDrawText.top );
}

void YBCheckBox::setText( const tstring_type& tstrText ){
	m_tstrText = tstrText;
}

void YBCheckBox::setTextSpace( unsigned int uTxtSpaceL,unsigned int uTxtSpaceT, unsigned int uTxtSpaceR, unsigned int uTxtSpaceB ){
	m_uTxtSpaceL = uTxtSpaceL;
	m_uTxtSpaceB = uTxtSpaceB;
	m_uTxtSpaceR = uTxtSpaceR;
	m_uTxtSpaceT = uTxtSpaceT;
}

void YBCheckBox::setFontId( unsigned int uFontId ){
	if( !YBCtrlFontDepository::getInstance()->hasYBFont( uFontId ) ){
		return;
	}
	m_uFontId = uFontId;
}
// 
// void YBCheckBox::enableStatus( bool bEnableCheck, bool bEnableNoCheck ){
// 	if( !bEnableCheck ){
// 		if( !bEnableNoCheck ){
// 			assert( false );
// 			return;
// 		}else{
// 			m_eCheckStatus = ECHECK_STATUS_DISABLE;
// 		}
// 	}else{
// 		if( !bEnableNoCheck ){
// 			m_eCheckStatus = ENOCHECK_STATUS_DISABLE;
// 		}else{
// 			return;
// 		}
// 	}
// 	return;
// }
void YBCheckBox::setEnable( bool bEnable ){
	if( bEnable )
	{
		if (isCheck())
		{
			m_eCheckStatus = ECHECK_STATUS_NORMAL;
		}
		else{
			m_eCheckStatus = ENOCHECK_STATUS_NORMAL;
		}
	}
	else
	{
		if( isCheck() ){
			m_eCheckStatus = ECHECK_STATUS_DISABLE;
		}else{
			m_eCheckStatus = ENOCHECK_STATUS_DISABLE;
		}
	}
}

bool YBCheckBox::isCheck(){

	return ( ( m_eCheckStatus == ECHECK_STATUS_DISABLE ) || ( m_eCheckStatus == ECHECK_STATUS_HOVER ) ||
		( m_eCheckStatus == ECHECK_STATUS_NORMAL ) || ( m_eCheckStatus == ECHECK_STATUS_PRESS ) );
}

void YBCheckBox::setCheckLSpace( unsigned int uCheckLSpace ){
	m_uIconOriginL = uCheckLSpace;
}

void YBCheckBox::setbeChecked( bool bChecked )
{
	if( bChecked ){
		m_eCheckStatus = ECHECK_STATUS_NORMAL;
	}else{
		m_eCheckStatus = ENOCHECK_STATUS_NORMAL;
	}
}

void YBCheckBox::addToolTip( bool bEnable ) {
	if ( bEnable ) {
		m_uFlag |= YBCHECK_FLAG_TOOLTIP;
	} else {
		m_uFlag &= ~YBCHECK_FLAG_TOOLTIP;
	}
}

void YBCheckBox::setToolTipTxt( const tstring_type& tstrTxt ) {
	m_bTipText = true;
	m_tstrToolTipTxt = tstrTxt;
}