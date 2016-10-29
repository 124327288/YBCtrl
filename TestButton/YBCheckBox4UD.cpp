#include "stdafx.h"
#include "frame/YBCtrlBase.h"
#include "YBCheckBox4UD.h"
#include "frame/YBCtrlImageBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlMisc.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlFontDepository.h"
#include "gdiplus.h"
#include  <assert.h>

#define YBCHECKBOX_BOX_LSPACE ( 8 )
#define YBBTN_FLAG_NONE (0x00000000)
#define YBBTN_FLAG_LBTDOWN (0x00000001)
#define YBCHECKBOX_TEXT_LEFT (4)
#define YBCHECKBOX_TEXT_RIGHT (18)
#define YBCHECKBOX_TEXT_TOP (9)

USING_NAMESPACE_YBCTRL

YBCheckBox4UD::YBCheckBox4UD()
: m_eBtnStatus( EBTN_STATUS_NORMAL )
, m_eBoxStatus( EBOX_STATUS_NOCHECK )
, m_uFlag( YBBTN_FLAG_NONE )
, m_tstrText( _T("D  ÅÌ") )
, m_uFontId( YBCTRL_FONT_ID_INVALID )
, m_uImageIdBoxNoCheck( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBoxCheck( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnPre( YBCTRL_IMAGE_ID_INVALID )
{
	_loadImage();
	_loadFont();
}

YBCheckBox4UD::~YBCheckBox4UD(){

}

YBCTRL_WNDMSGMAP_BEGIN( YBCheckBox4UD, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_LBUTTONUP, _onWM_LBUTTONUP )
	YBCTRL_ONWNDMSG( WM_MOUSELEAVE, _onWM_MOUSELEAVE )
YBCTRL_WNDMSGMAP_END()

void YBCheckBox4UD::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
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

void YBCheckBox4UD::_onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
	POINT ptMouse;
	RECT rcClient;
	HDC hdc;

	::GetClientRect( m_hWnd, &rcClient );
	ptMouse.x = LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = HIWORD( pYBCtrlWndMsg->m_lParam );

	m_uFlag |= YBBTN_FLAG_LBTDOWN;

	if( ::PtInRect( &m_rcBox, ptMouse ) ){
		if( m_eBoxStatus == EBOX_STATUS_NOCHECK ){
			m_eBoxStatus = EBOX_STATUS_CHECK;
		}else{
			m_eBoxStatus = EBOX_STATUS_NOCHECK;
		}
		WPARAM wParam;
		LPARAM lParam;
		wParam = (WPARAM)m_wordCtrlId;
		lParam = (LPARAM)isCheck();
		::SendMessage( ::GetParent( m_hWnd ), WM_VOLBTNCHECK, wParam, lParam );
	}else{
		m_eBtnStatus = EBTN_STATUS_PRESS;
	}

	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hdc = ::GetDC( m_hWnd );
	::BitBlt( hdc, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hdc );
}

void YBCheckBox4UD::_onWM_LBUTTONUP( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONUP );
	POINT ptMouse;
	RECT rcClient;
	HDC hdc;

	::GetClientRect( m_hWnd, &rcClient );
	ptMouse.x = LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = HIWORD( pYBCtrlWndMsg->m_lParam );

	m_uFlag &= ~YBBTN_FLAG_LBTDOWN;
	m_eBtnStatus = EBTN_STATUS_HOVER;

	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hdc = ::GetDC( m_hWnd );
	::BitBlt( hdc, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hdc );

}

void YBCheckBox4UD::_onWM_MOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	RECT rcClient;
	POINT ptMouse;
	HDC hdc;
	TRACKMOUSEEVENT trackMouseEvt;

	::GetClientRect( m_hWnd, &rcClient );
	ptMouse.x = LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = HIWORD( pYBCtrlWndMsg->m_lParam );

	if( m_uFlag & YBBTN_FLAG_LBTDOWN ){
		m_eBtnStatus = EBTN_STATUS_PRESS;
	}else{
		m_eBtnStatus = EBTN_STATUS_HOVER;
	}

	trackMouseEvt.cbSize = sizeof( TRACKMOUSEEVENT );
	trackMouseEvt.dwFlags = TME_LEAVE;
	trackMouseEvt.hwndTrack = m_hWnd;
	trackMouseEvt.dwHoverTime = HOVER_DEFAULT;
	::TrackMouseEvent( &trackMouseEvt );

	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hdc = ::GetDC( m_hWnd );
	::BitBlt( hdc, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hdc );
}

void YBCheckBox4UD::_onWM_MOUSELEAVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSELEAVE );
	RECT rcClient;
	HDC hdc;

	m_uFlag &= ~YBBTN_FLAG_LBTDOWN;
	m_eBtnStatus = EBTN_STATUS_NORMAL;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hdc = ::GetDC( m_hWnd );
	::BitBlt( hdc, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hdc );
}


HWND YBCheckBox4UD::create( DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance ){
	HWND hWnd;

	hWnd = _createHWND( 0, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, NULL );
	if( !hWnd ){
		assert(false);
	}
	m_wordCtrlId = (unsigned int)hMenu;
	return hWnd;
}

bool YBCheckBox4UD::_getBoxImageId( EBOX4UDSTATUS eBoxStatus, unsigned int& uImageId ){
	switch( eBoxStatus ){
	case EBOX_STATUS_CHECK:
		{
			uImageId = m_uImageIdBoxCheck;
		}
		break;
	case EBOX_STATUS_NOCHECK:
		{
			uImageId = m_uImageIdBoxNoCheck;
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

bool YBCheckBox4UD::_getBtnImageId( EBTNSTATUS eBtnStatus, unsigned int& uImageId ){
	switch( eBtnStatus ){
	case EBTN_STATUS_NORMAL:
		{
			uImageId = m_uImageIdBtnNor;
		}
		break;
	case EBTN_STATUS_HOVER:
		{
			uImageId = m_uImageIdBtnHov;
		}
		break;
	case EBTN_STATUS_PRESS:
		{
			uImageId = m_uImageIdBtnPre;
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

ybctrl::YBCtrlImageStatic* YBCheckBox4UD::_getImageStatic( unsigned int uImageId ){
	YBCtrlImageBase* pImageBase = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pImageBase ) )
		return NULL;
	if( !pImageBase || ( pImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;
	return (YBCtrlImageStatic*)pImageBase;
}

void YBCheckBox4UD::_drawStatus( HDC hDC ){
	_drawBtn( hDC );
	_drawBox( hDC );
	_drawText( hDC );
	_drawTriangle( hDC );
}

void YBCheckBox4UD::_drawBox( HDC hDC ){
	
	RECT rcClient;
	unsigned int uImageId;
	YBCtrlImageStatic* pYBImage = NULL;

	::GetClientRect( m_hWnd, &rcClient );
	if( !_getBoxImageId( m_eBoxStatus, uImageId ) ){
		return;
	}
	pYBImage = _getImageStatic( uImageId );
	if ( !pYBImage ) {
		return;
	}
	// the rect of box
	m_rcBox.left = YBCHECKBOX_BOX_LSPACE ;
	m_rcBox.top = ( rcClient.bottom - rcClient.top - pYBImage->getHeight() )/2 ;
	m_rcBox.right = m_rcBox.left + pYBImage->getWidth();
	m_rcBox.bottom = m_rcBox.top + pYBImage->getHeight();

	Gdiplus::Graphics graphics( hDC );
	GdipBitBlt( &graphics, pYBImage->getImage(), m_rcBox.left, m_rcBox.top, pYBImage->getWidth(), pYBImage->getHeight()
		, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
}

void YBCheckBox4UD::_drawBtn( HDC hDC ){
	RECT rcClient;
	unsigned int uImageId;
	YBCtrlImageStatic* pYBImage = NULL;

	::GetClientRect( m_hWnd, &rcClient );
	if( !_getBtnImageId( m_eBtnStatus, uImageId ) ){
		return;
	}
	pYBImage = _getImageStatic( uImageId );
	if ( !pYBImage ) {
		return;
	}

	BackGroundBitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pYBImage->getMemDC(), 
		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 1, 1, 1, 1 );
}

void YBCheckBox4UD::_loadImage(){
	TCHAR szModulePath[MAX_PATH];
	TCHAR* posTmp = NULL;
	::GetModuleFileName( NULL, szModulePath, MAX_PATH );
	std::basic_string< TCHAR >tstrImageDirPath;
	std::basic_string< TCHAR >tstrImageFilePath;
	unsigned int idImage;

	posTmp = _tcsrchr( szModulePath, _T('\\') );
	if( !posTmp ){
		assert(false);
		return;
	}
	posTmp += 1;
	*posTmp = 0;

	tstrImageDirPath = szModulePath;
	tstrImageDirPath += _T("Image\\");
	// 
	tstrImageFilePath = tstrImageDirPath + _T("CheckExBoxNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBoxNoCheck = idImage;
	// 
	tstrImageFilePath = tstrImageDirPath + _T("CheckExBoxPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBoxCheck = idImage;
	// 
	tstrImageFilePath = tstrImageDirPath + _T("CheckExBtnNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnNor = idImage;
	// 
	tstrImageFilePath = tstrImageDirPath + _T("CheckExBtnHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnHov = idImage;
	// 
	tstrImageFilePath = tstrImageDirPath + _T("CheckExBtnPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnPre = idImage;
}

void YBCheckBox4UD::setCheckStatus( bool bChecked )
{
	if( bChecked ){
		m_eBoxStatus = EBOX_STATUS_CHECK;
	}else{
		m_eBoxStatus = EBOX_STATUS_NOCHECK;
	}

}

bool YBCheckBox4UD::isCheck(){

	return ( m_eBoxStatus == EBOX_STATUS_CHECK );
}

bool YBCheckBox4UD::_getDrawTextRect( RECT& rcDrawText ){
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcDrawText.left = rcClient.left + YBCHECKBOX_BOX_LSPACE + m_rcBox.right - m_rcBox.left + YBCHECKBOX_TEXT_LEFT;
	rcDrawText.right = rcClient.right - YBCHECKBOX_TEXT_RIGHT;
	rcDrawText.top = rcClient.top + YBCHECKBOX_TEXT_TOP;
	rcDrawText.bottom = rcClient.bottom - YBCHECKBOX_TEXT_TOP;

	return ( rcDrawText.right >= rcDrawText.left ) && ( rcDrawText.bottom >= rcDrawText.top );
}

void YBCheckBox4UD::setText( tstring_type tstrText ){
	m_tstrText = tstrText;
}

void YBCheckBox4UD::_drawText( HDC hDC ){
	RECT rcDrawText;
	int iBkModeOld;
	COLORREF clrTxtOld;
	const YBCtrlFont* pYBCtrlFont = NULL;
	HFONT hFontOld = NULL;

	if( ( _getDrawTextRect( rcDrawText ) )
		&& ( rcDrawText.right - rcDrawText.left )
		&& ( rcDrawText.bottom - rcDrawText.top )
		&& ( m_tstrText.length() > 0 ) ){
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontId );
			if( pYBCtrlFont ){
				hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
				iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
				clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
				::DrawText( hDC, m_tstrText.c_str(), m_tstrText.length(), &rcDrawText, DT_VCENTER );
				::SetTextColor( hDC, clrTxtOld );
				::SetBkMode( hDC, iBkModeOld );
				::SelectObject( hDC, hFontOld );
			}
	}
}

void YBCheckBox4UD::_loadFont(){
	LOGFONT lFont;

	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 20;
	m_uFontId = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 222, 232, 211 ));
}

void YBCheckBox4UD::_drawTriangle( HDC hDC ){
	HBRUSH hBrushTriangle;
	HBRUSH hBrushOld;
	HPEN hPenTriangle;
	HPEN hPenOld;
	RECT rcClient;
	POINT pt1;
	POINT pt2;
	POINT pt3;

	::GetClientRect( m_hWnd, &rcClient );
	unsigned int uWidthRc = rcClient.right - rcClient.left;
	unsigned int uHeightRc = rcClient.bottom - rcClient.top;
	pt1.x = rcClient.left + 3*uWidthRc/4;
	pt1.y = rcClient.top + 2*uHeightRc/5;

	pt2.x = rcClient.left + 17*uWidthRc/20;
	pt2.y = pt1.y;

	pt3.x = pt1.x + ( pt2.x - pt1.x )/2;
	pt3.y = rcClient.bottom - 2*uHeightRc/5;
	hPenTriangle = ::CreatePen( PS_SOLID, 1, RGB( 255, 255, 255 ) );
	hBrushTriangle = ::CreateSolidBrush( RGB( 255, 255, 255 ) );
	
	POINT aryPoint[ 3 ] = { pt1, pt2, pt3 };
	hBrushOld = (HBRUSH)::SelectObject( hDC, hBrushTriangle );
	hPenOld = (HPEN)::SelectObject( hDC, hPenTriangle );
	::Polygon( hDC, aryPoint, 3 );
	::SelectObject( hDC, hBrushOld );
	::DeleteObject( hBrushTriangle );
	hBrushTriangle = NULL;
	::SelectObject( hDC, hPenOld );
	::DeleteObject( hPenTriangle );
	hPenTriangle = NULL;
}