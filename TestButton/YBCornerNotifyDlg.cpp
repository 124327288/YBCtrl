#include "stdafx.h"
#include "YBCornerNotifyDlg.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlDefs.h"
#include "frame/YBCtrlMisc.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlMemDC.h"
#include <assert.h>
#include "frame/YBCtrlFontDepository.h"
#include "frame/YBCtrlImageGif.h"

#define YBCORNERNOTIFYDLG_TIMERID_AUTOEXIT (0x0020)
#define YBCORNERNOTIFYDLG_BUTTON_NEXTSCANID (322)
#define YBCORNERNOTIFYDLG_BUTTON_IGNOREALLID (323)
#define YBCORNERNOTIFYDLG_BUTTON_CLICKSCAN (324)
#define YBCORNERNOTIFYDLG_ICONTEXT_LSPACE (49)
#define YBCORNERNOTIFYDLG_CONTENTTEXT_LSPACE (10)
#define YBCORNERNOTIFYDLG_CONTENTTEXT_TSPACE (15)
#define YBCORNERNOTIFYDLG_BTNCLOSE_RSPACE (20)
USING_NAMESPACE_YBCTRL

YBCornerNotifyDlg::YBCornerNotifyDlg( int nType, const tstring_type& tstrTime, unsigned int uIndexCNDlg, const tstring_type& tstrTypeName, const tstring_type& tstrContent, pfnWndDestroyProcType pfnWndDestroyProc, void* pParamWndDestroy, pfnWndLBtnDownProcType pfnWndLBProc, const tstring_type& tstrClickEvtParam, unsigned int uExitTimeMS )
: m_pfnWndDestroyProc( pfnWndDestroyProc )
, m_pParamWndDestroy( pParamWndDestroy )
, m_pfnWndLBtnDownProc( pfnWndLBProc )
, m_tstrClickEvtParam( tstrClickEvtParam )
, m_flag4Type( nType )
, m_tstrTypeName( tstrTypeName )
, m_tstrContent( tstrContent )
, m_nIndexCNDlg( uIndexCNDlg )
, m_uExitTimeMS( uExitTimeMS )
, m_tstrTime( tstrTime )
{
	_loadImage();
	_loadFont();
	_init();
	_bind2Type();
	m_hPen = ::CreatePen( PS_SOLID, 1, RGB(0, 29, 56) );
}

YBCornerNotifyDlg::~YBCornerNotifyDlg(){
	if ( m_hPen ) {
		::DeleteObject( m_hPen );
		m_hPen = NULL;
	}
	_unbind2Type();
}

int YBCornerNotifyDlg::getCNDlgIndex() const {
	return m_nIndexCNDlg;
}

HWND YBCornerNotifyDlg::create( int x, int y, unsigned int uWidth, unsigned int uHeight, HWND hWndParent, HINSTANCE hInstance ){
	HWND hWnd;
	DWORD dwExStyle = WS_EX_TOPMOST|WS_EX_TOOLWINDOW|WS_EX_LTRREADING;
	DWORD dwStyle = WS_POPUP|WS_CLIPCHILDREN;

	//
	hWnd = _createHWND( dwExStyle, dwStyle, x, y, uWidth, uHeight, hWndParent, 0, NULL, NULL );
	if( hWnd ){
		::SetWindowPos( hWnd, NULL, 0,0,0,0, SWP_NOACTIVATE|SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE );
		m_btnNextScan.create( 150, 13, 46, 48, hWnd, (HMENU)YBCORNERNOTIFYDLG_BUTTON_NEXTSCANID, NULL );
		m_btnIgnoreAll.create(  150, 66, 46, 48, hWnd, (HMENU)YBCORNERNOTIFYDLG_BUTTON_IGNOREALLID, NULL );
		m_btnClickScan.create(  150, 120, 46, 48, hWnd, (HMENU)YBCORNERNOTIFYDLG_BUTTON_CLICKSCAN, NULL );
	}

	::ShowWindow( m_btnNextScan.getHWND(), SW_NORMAL );
	::ShowWindow( m_btnIgnoreAll.getHWND(), SW_NORMAL );
	::ShowWindow( m_btnClickScan.getHWND(), SW_NORMAL );
	::SetTimer( m_hWnd, YBCORNERNOTIFYDLG_TIMERID_AUTOEXIT, m_uExitTimeMS, NULL );
	return hWnd;
}

YBCTRL_WNDMSGMAP_BEGIN( YBCornerNotifyDlg, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
	YBCTRL_ONWNDMSG( WM_MOUSELEAVE, _onWM_MOUSELEAVE )
	YBCTRL_ONWNDMSG( WM_TIMER, _onWM_TIMER )
	YBCTRL_ONWNDMSG( WM_DESTROY, _onWM_DESTROY )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_SETCURSOR, _onWM_SETCURSOR )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( YBWM_MOUSEMOVE_BTN_CNDLG, _onYBWM_MOUSEMOVE_BTN_CNDLG )
YBCTRL_WNDMSGMAP_END()

void YBCornerNotifyDlg::_onWM_DESTROY( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_DESTROY );

	pYBCtrlWndMsg->m_dwFlag |= YBCTRLWNDMSG_FLAG_EATMSG;
	pYBCtrlWndMsg->m_dwFlag |= YBCTRLWNDMSG_FLAG_RESULT_VALID;
	pYBCtrlWndMsg->m_lRes = 0;

	if ( m_pfnWndDestroyProc ) {
		m_pfnWndDestroyProc( this, m_pParamWndDestroy );
	}
}

void YBCornerNotifyDlg::_onWM_SETCURSOR( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_SETCURSOR );

	::SetCursor( ::LoadCursor( NULL, IDC_HAND ) );

	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = TRUE;

}

void YBCornerNotifyDlg::_onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );

	pYBCtrlWndMsg->m_dwFlag |= YBCTRLWNDMSG_FLAG_EATMSG;
	pYBCtrlWndMsg->m_dwFlag |= YBCTRLWNDMSG_FLAG_RESULT_VALID;
	pYBCtrlWndMsg->m_lRes = 0;
	//
	POINT ptMouse;
	RECT rcIcon;
	ptMouse.x = LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = HIWORD( pYBCtrlWndMsg->m_lParam );

	if ( !_getIconRect( rcIcon ) ) {
		return;
	}

	if ( ::PtInRect( &rcIcon, ptMouse ) ) {
		if ( m_pfnWndLBtnDownProc ) {
			m_pfnWndLBtnDownProc( this, (void*)m_tstrClickEvtParam.c_str() );
		}
	}

	::PostMessage( m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0 );
}

void YBCornerNotifyDlg::_onYBWM_MOUSEMOVE_BTN_CNDLG( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( YBWM_MOUSEMOVE_BTN_CNDLG );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );

	if ( wEvtCode == YBBTN_CMDEVT_BTNMOVE ) {
		switch( wCtrlId ){
		case YBCORNERNOTIFYDLG_BUTTON_NEXTSCANID:
			{
				::KillTimer( m_hWnd, YBCORNERNOTIFYDLG_TIMERID_AUTOEXIT );
			}
			break;
		case YBCORNERNOTIFYDLG_BUTTON_IGNOREALLID:
			{
				::KillTimer( m_hWnd, YBCORNERNOTIFYDLG_TIMERID_AUTOEXIT );
			}
			break;
		case YBCORNERNOTIFYDLG_BUTTON_CLICKSCAN:
			{
				::KillTimer( m_hWnd, YBCORNERNOTIFYDLG_TIMERID_AUTOEXIT );
			}
			break;
		default:
			break;
		}
	}
}

void YBCornerNotifyDlg::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );
	
	switch( wCtrlId ){
	case YBCORNERNOTIFYDLG_BUTTON_NEXTSCANID:
		{
			int a = 0;
			//::KillTimer( m_hWnd, YBCORNERNOTIFYDLG_TIMERID_AUTOEXIT );
			::PostMessage( m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0 );
		}
		break;
	case YBCORNERNOTIFYDLG_BUTTON_IGNOREALLID:
		{
			//::KillTimer( m_hWnd, YBCORNERNOTIFYDLG_TIMERID_AUTOEXIT );
			::PostMessage( m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0 );
		}
		break;
	case YBCORNERNOTIFYDLG_BUTTON_CLICKSCAN:
		{
			if ( m_pfnWndLBtnDownProc ) {
				m_pfnWndLBtnDownProc( this, (void*)m_tstrClickEvtParam.c_str() );
			}
			::PostMessage( m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0 );
		}
		break;
	default:
		break;
	}
}

void YBCornerNotifyDlg::_onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	RECT rcClient;

	pYBCtrlWndMsg->m_dwFlag |= YBCTRLWNDMSG_FLAG_EATMSG;
	pYBCtrlWndMsg->m_dwFlag |= YBCTRLWNDMSG_FLAG_RESULT_VALID;
	pYBCtrlWndMsg->m_lRes = 1;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
}

void YBCornerNotifyDlg::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	PAINTSTRUCT ps;
	HDC hPaintDC;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void YBCornerNotifyDlg::_onWM_MOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	HDC hDC;
	RECT rcClient;
	TRACKMOUSEEVENT trackMouseEvt;

	::GetClientRect( m_hWnd, &rcClient );
	trackMouseEvt.cbSize = sizeof( TRACKMOUSEEVENT );
	trackMouseEvt.dwFlags = TME_LEAVE;
	trackMouseEvt.hwndTrack = pYBCtrlWndMsg->m_hWnd;
	trackMouseEvt.dwHoverTime = HOVER_DEFAULT;
	::TrackMouseEvent ( &trackMouseEvt );

	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );

	::KillTimer( m_hWnd, YBCORNERNOTIFYDLG_TIMERID_AUTOEXIT );
}

void YBCornerNotifyDlg::_onWM_MOUSELEAVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSELEAVE );

	::SetTimer( m_hWnd, YBCORNERNOTIFYDLG_TIMERID_AUTOEXIT, 2000, NULL );
}

void YBCornerNotifyDlg::_onWM_TIMER( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_TIMER );
	switch ( pYBCtrlWndMsg->m_wParam ) {
	case YBCORNERNOTIFYDLG_TIMERID_AUTOEXIT:
		{
			::KillTimer( m_hWnd, YBCORNERNOTIFYDLG_TIMERID_AUTOEXIT );
			::DestroyWindow( m_hWnd );
		}
		break;
	default:
		break;
	}
}

ybctrl::YBCtrlImageGif* YBCornerNotifyDlg::_getImageGif(unsigned int uImageId) {
	YBCtrlImageBase* pYBImageBase = NULL;

	if (!YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage(uImageId, &pYBImageBase))
		return NULL;
	if (!pYBImageBase || (pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_GIF ))
		return NULL;

	return (YBCtrlImageGif*)pYBImageBase;
}

ybctrl::YBCtrlImageStatic* YBCornerNotifyDlg::_getImageStatic( unsigned int uImageId ){
	YBCtrlImageBase* pYBImageBase = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pYBImageBase ) )
		return NULL;
	if( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

void YBCornerNotifyDlg::_drawStatus( HDC hDC ){
	_drawBk( hDC );
	_drawIcon( hDC );
	_drawIconText( hDC );
	_drawContentText( hDC );
	_drawFrameLine( hDC );
	_drawTimeText( hDC );
}

void YBCornerNotifyDlg::_drawTimeText( HDC hDC ) {
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcTime;

	if( !_getTimeRect( rcTime ) )
		return;

	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdCNTime );
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

		::DrawText( hDC, m_tstrTime.c_str(), m_tstrTime.length(), &rcTime, DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

bool YBCornerNotifyDlg::_getTimeRect( RECT& rcTime ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcTime.left = rcClient.left + 2;
	rcTime.right = rcTime.left + 148;
	rcTime.top = rcClient.top + 122;
	rcTime.bottom = rcTime.top + 20;

	return ( ( rcTime.right > rcTime.left ) && ( rcTime.bottom > rcTime.top ) );
}

void YBCornerNotifyDlg::_drawFrameLine( HDC hDC ) {
	RECT rcClient;
	HPEN hPenOld = NULL;

	::GetClientRect( m_hWnd, &rcClient );
	hPenOld = (HPEN)::SelectObject( hDC, m_hPen );
	//
	::MoveToEx( hDC, rcClient.left, rcClient.top, NULL );
	::LineTo( hDC, rcClient.left, rcClient.bottom );
	//
	::MoveToEx( hDC, rcClient.left, rcClient.bottom - 1, NULL );
	::LineTo( hDC, rcClient.right, rcClient.bottom - 1 );
	//
	::MoveToEx( hDC, rcClient.right - 1, rcClient.bottom, NULL );
	::LineTo( hDC, rcClient.right - 1, rcClient.top );
	//
	::MoveToEx( hDC, rcClient.left, rcClient.top, NULL );
	::LineTo( hDC, rcClient.right, rcClient.top );

	::SelectObject( hDC, hPenOld );
}

void YBCornerNotifyDlg::_drawBk( HDC hDC ){
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlImageStatic* pYBImage = NULL;
	pYBImage = _getImageStatic( m_uImageIdCNBk );

	if( !pYBImage ){
		return;
	}

 	BackGroundBitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pYBImage->getMemDC(),
 		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0 );
// 	Gdiplus::Graphics graphics(hDC);
// 	GdipBitBlt(&graphics, pYBImage->getImage(), 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top
// 		, 0, 0, pYBImage->getWidth(), pYBImage->getHeight());
}

void YBCornerNotifyDlg::_drawIcon( HDC hDC ){
	RECT rcIcon;
	unsigned int uImageId;

	flag2int_map_type::iterator itType2int;
	itType2int = s_maptype2int.find( m_flag4Type );
	uImageId = itType2int->second;

	if ( !_getIconRect( rcIcon ) ) {
		return;
	}
	YBCtrlImageGif* pYBImage = NULL;
	pYBImage = _getImageGif( uImageId );

	if( !pYBImage ){
		return;
	}

// 	BackGroundBitBlt( hDC, rcIcon.left, rcIcon.top, pYBImage->getWidth(), pYBImage->getHeight(), pYBImage->getMemDC(),
// 		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0 );
	Gdiplus::Graphics graphics(hDC);
	
	GdipBitBlt(&graphics, pYBImage->getImage(), rcIcon.left, rcIcon.top, rcIcon.right - rcIcon.left, rcIcon.bottom - rcIcon.top
		, 0, 0, pYBImage->getWidth(), pYBImage->getHeight());
}

bool YBCornerNotifyDlg::_getIconRect( RECT& rcIcon ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcIcon.left = 27;
	rcIcon.right = 122;
	rcIcon.top = 22;
	rcIcon.bottom = 123;

	return( ( rcIcon.right > rcIcon.left ) && ( rcIcon.bottom > rcIcon.top ) );
}

void YBCornerNotifyDlg::_init(){
	m_btnNextScan.setTextSpace( 5, 5, 5, 5 );
	m_btnNextScan.setText( _T("ÏÂ ´ÎÔÙ ¿´") );
	m_btnNextScan.setFontId( m_uFontIdCNBtn );
	m_btnNextScan.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdCNBtnNor );
	m_btnNextScan.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdCNBtnPre );
	m_btnNextScan.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdCNBtnPre );
	//
	m_btnIgnoreAll.setTextSpace( 5, 5, 5, 5 );
	m_btnIgnoreAll.setFontId( m_uFontIdCNBtn );
	m_btnIgnoreAll.setText( _T("È« ²¿ºö ÂÔ") );
	m_btnIgnoreAll.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdCNBtnNor );
	m_btnIgnoreAll.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdCNBtnPre );
	m_btnIgnoreAll.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdCNBtnPre );
	//
	m_btnClickScan.setTextSpace( 5, 5, 5, 5 );
	m_btnClickScan.setFontId( m_uFontIdCNBtn );
	m_btnClickScan.setText( _T("µã »÷²é ¿´") );
	m_btnClickScan.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdCNBtnNor );
	m_btnClickScan.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdCNBtnPre );
	m_btnClickScan.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdCNBtnPre );
}

void YBCornerNotifyDlg::_loadFont(){
	//

	LOGFONT lFont;
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 19;
	m_uFontIdCNIcon = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 20;
	m_uFontIdCNContent = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 1, 75, 122 ) );
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 20;
	lFont.lfWeight = 700;
	m_uFontIdCNBtn = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 0, 0, 0 ) );
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 20;
	m_uFontIdCNTime = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 152, 146, 188 ) );
}

bool YBCornerNotifyDlg::_getIconTxtRect( RECT& rcTitle ){
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	if( rcClient.right <= YBCORNERNOTIFYDLG_BTNCLOSE_RSPACE ){
		assert(false);
		return false;
	}
	rcTitle.left = rcClient.left + YBCORNERNOTIFYDLG_ICONTEXT_LSPACE;
	rcTitle.right = rcTitle.left + 68;
	rcTitle.top = rcClient.top + 81;
	rcTitle.bottom = rcTitle.top + 20;

	return ( ( rcTitle.right > rcTitle.left ) && ( rcTitle.bottom > rcTitle.top ) );
}

bool YBCornerNotifyDlg::_getContentRect( RECT& rcContent ){
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	if( rcClient.bottom <= ( rcClient.bottom - rcClient.top )*28/147 ){
		assert(false);
		return false;

	}
	rcContent.left = rcClient.left + 2;
	rcContent.right = rcContent.left + 148;
	rcContent.top = 145;
	rcContent.bottom = rcContent.top + 20;

	return ( ( rcContent.right > rcContent.left ) && ( rcContent.bottom > rcContent.top ) );
}

void YBCornerNotifyDlg::_drawIconText( HDC hDC ){
	
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcTitle;

	if( !_getIconTxtRect( rcTitle ) )
		return;

	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdCNIcon );
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

		::DrawText( hDC, m_tstrTypeName.c_str(), m_tstrTypeName.length(), &rcTitle, DT_LEFT|DT_VCENTER );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

void YBCornerNotifyDlg::_drawContentText( HDC hDC ){

	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcContent;

	if( !_getContentRect( rcContent ) )
		return;

	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdCNContent );
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

		::DrawText( hDC, m_tstrContent.c_str(), m_tstrContent.length(), &rcContent, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

void YBCornerNotifyDlg::_bind2Type() {
	s_maptype2int.clear();
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 1, m_uImageIdCNIcon1 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 2, m_uImageIdCNIcon2 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 3, m_uImageIdCNIcon3 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 4, m_uImageIdCNIcon4 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 5, m_uImageIdCNIcon5 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 6, m_uImageIdCNIcon6 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 7, m_uImageIdCNIcon7 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 8, m_uImageIdCNIcon8 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 9, m_uImageIdCNIcon9 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 10, m_uImageIdCNIcon10 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 11, m_uImageIdCNIcon11 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 12, m_uImageIdCNIcon12 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 13, m_uImageIdCNIcon13 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 14, m_uImageIdCNIcon14 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 15, m_uImageIdCNIcon15 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 16, m_uImageIdCNIcon16 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 17, m_uImageIdCNIcon17 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 18, m_uImageIdCNIcon18 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 19, m_uImageIdCNIcon19 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 20, m_uImageIdCNIcon20 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 21, m_uImageIdCNIcon21 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 22, m_uImageIdCNIcon22 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 23, m_uImageIdCNIcon23 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 24, m_uImageIdCNIcon24 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 25, m_uImageIdCNIcon25 ) );
	s_maptype2int.insert( std::map< flag_type, int >::value_type( 26, m_uImageIdCNIcon26 ) );
}

void YBCornerNotifyDlg::_unbind2Type() {
	s_maptype2int.clear();
}

void YBCornerNotifyDlg::_loadImage(){
	tstring_type tstrImageDirPath;
	tstring_type tstrImageFilePath;
	TCHAR szModulePath[MAX_PATH];
	TCHAR* posTmp = NULL;
	unsigned int idImage;

	::GetModuleFileName( NULL, szModulePath, MAX_PATH );
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
	tstrImageFilePath = tstrImageDirPath + _T("YBCorNotifyBk.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNBk = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNBtnNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdCNBtnNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNBtnPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdCNBtnPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType1.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon1 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType2.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon2 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType3.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon3 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType4.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon4 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType5.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){

		assert(false);
	}
	m_uImageIdCNIcon5 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType6.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon6 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType7.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon7 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType8.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon8 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType9.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon9 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType10.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon10 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType11.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon11 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("vip8user.gif");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon12 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType13.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon13 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType14.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon14 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType15.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon15 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType16.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon16 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType17.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon17 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType18.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon18 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType19.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon19 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType20.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon20 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType21.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon21 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType22.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon22 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType23.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon23 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType24.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon24 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType25.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon25 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType26.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdCNIcon26 = idImage;
}
