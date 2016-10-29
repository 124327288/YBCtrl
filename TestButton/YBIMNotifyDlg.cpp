#include "stdafx.h"
#include "YBIMNotifyDlg.h"
#include "button\YBButton.h"
#include "frame\YBCtrlDefs.h"
#include "loader\YBImageLoader.h"
#include "frame\YBCtrlImageStatic.h"
#include "frame\YBCtrlFontDepository.h"
#include "frame\YBCtrlImageDepository.h"
#include "frame\YBCtrlMisc.h"
#include "frame\YBCtrlMemDC.h"
#include "tchar.h"
#include <assert.h>
//
#define YBIMND_CTRLID_BTNINSTALL (200)
#define YBIMND_CTRLIN_BTNCANCEL (201)
#define YBIMND_TEXT_DOWNLOAD ( _T("确 定 下 载") )
#define YBIMND_TEXT_INSTALL ( _T("确 定 安 装") )
#define YBIMND_TEXT_TOP ( _T("温馨提示") )
#define YBIMND_TEXT_CANCEL ( _T("取    消") )
#define YBIMND_TEXT_TIME ( _T("    本次下载剩余时间 : ") )
#define YBIMND_TEXT_TIME_MINUTE ( _T("分") )
#define YBIMND_TEXT_TIME_SECOND ( _T("秒") )
#define YBIMND_WIDTH (359)
#define YBIMND_HEIGHT_DOWNLOAD (216)
#define YBIMND_HEIGHT_INSTALL (353)
//
#define YBIMND_REMAINTIME_0 (0)
#define YBIMND_REMAINTIME_1 (1)
#define YBIMND_REMAINTIME_2 (2)
#define YBIMND_REMAINTIME_3 (3)
#define YBIMND_REMAINTIME_4 (4)
//
USING_NAMESPACE_YBCTRL

CYBIMNotifyDlg::CYBIMNotifyDlg()
: m_eState( ESTATE_DOWNLOAD )
, m_uImageDownloadBK( YBCTRL_IMAGE_ID_INVALID )
, m_uImageInstallBK( YBCTRL_IMAGE_ID_INVALID )
, m_tstrText( YBIMND_TEXT_TOP )
, m_tstrTextTime( YBIMND_TEXT_TIME )
, m_tstrTextMinute( YBIMND_TEXT_TIME_MINUTE )
, m_tstrTextSecond( YBIMND_TEXT_TIME_SECOND )
, m_tstrTextMid( _T("") )
, m_tstrTextUnder( _T("") )
, m_uTxtSpaceL( 5 )
, m_uTxtSpaceT( 5 )
, m_uTxtSpaceR( 5 )
, m_uTxtHeight( 30 )
, m_uMTxtSpaceL( 10 )
, m_uMTxtSpaceT( 55 )
, m_uMTxtSpaceR( 10 )
, m_uMTxtSpaceB( 30 )
, m_uUTxtSpaceL( 5 )
, m_uUTxtHeight( 30 )
, m_uUTxtSpaceR( 5 )
, m_uUTxtSpaceB( 80 )
, m_uMinute( _T("") )
, m_uSecond( _T("") )
{
	_loadImage();
	_loadFont();
	_init();
}

CYBIMNotifyDlg::~CYBIMNotifyDlg() {

}

YBCTRL_WNDMSGMAP_BEGIN( CYBIMNotifyDlg, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_ONWNDMSG( WM_KEYDOWN, _onWM_KEYDOWN )
	YBCTRL_ONWNDMSG( WM_NCHITTEST, _onWM_NCHITTEST )
	YBCTRL_ONWNDMSG( WM_SETCURSOR, _onWM_SETCURSOR )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
YBCTRL_WNDMSGMAP_END()

void CYBIMNotifyDlg::_reset() {
	m_eState = ESTATE_DOWNLOAD;
}

HWND CYBIMNotifyDlg::Create( int x, int y, HINSTANCE hInstance ) {
	if ( m_hWnd ) {
		assert( false );
		return NULL;
	}

	HWND hWnd;
	DWORD dwExStyle;
	DWORD dwStyle;
	RECT rcBtnInstall;
	RECT rcBtnCancel;
	int nWidth = YBIMND_WIDTH;
	int nHeight = YBIMND_HEIGHT_DOWNLOAD;

	//
	_reset();

	//
	dwExStyle = WS_EX_TOPMOST|WS_EX_TOOLWINDOW;
	dwStyle = WS_POPUP|WS_CLIPCHILDREN;
	hWnd = _createHWND( dwExStyle, dwStyle, x, y, nWidth, nHeight, NULL, (HMENU)0, hInstance, NULL );

	if ( hWnd ) {
		//Install Button
		_getBtnInstallRect( rcBtnInstall );
		m_btnInstall.Create( 0, WS_VISIBLE|WS_CHILD, 
			rcBtnInstall.left, rcBtnInstall.top, ( rcBtnInstall.right - rcBtnInstall.left ), ( rcBtnInstall.bottom - rcBtnInstall.top ),
			hWnd, (HMENU)YBIMND_CTRLID_BTNINSTALL, hInstance );
		m_btnInstall.setTextSpace( 5, 5, 5, 5 );
		m_btnInstall.setText( YBIMND_TEXT_DOWNLOAD );
		//Cancel Button
		_getBtnCancelRect( rcBtnCancel );
		m_btnCancel.Create( 0, WS_VISIBLE|WS_CHILD,
			rcBtnCancel.left , rcBtnCancel.top, ( rcBtnCancel.right - rcBtnCancel.left ), ( rcBtnCancel.bottom - rcBtnCancel.top  ),
			hWnd, (HMENU)YBIMND_CTRLIN_BTNCANCEL, hInstance );
 		m_btnCancel.setTextSpace( 5, 5, 5, 5 );
 		m_btnCancel.setText( YBIMND_TEXT_CANCEL );
	}
	::ShowWindow( hWnd, SW_NORMAL );
	return hWnd;
}

bool CYBIMNotifyDlg::_getBtnInstallRect( RECT& rcBtnInstall ) {
	if ( !::IsWindow( m_hWnd ) ) {
		return false;
	}
	RECT rcClient;
	unsigned int uBtnTopSpace = 175;
	unsigned int uBtnWidth = 90;
	unsigned int uBtnHeight = 30;

	if ( !::GetClientRect( m_hWnd, &rcClient ) ) {
		assert( false );	
		return false;
	}
	rcBtnInstall.top = ( rcClient.top + uBtnTopSpace );
	rcBtnInstall.left = 80;
	rcBtnInstall.right = rcBtnInstall.left + uBtnWidth;
	rcBtnInstall.bottom = rcBtnInstall.top + uBtnHeight;

	return true;
}

bool CYBIMNotifyDlg::_getBtnCancelRect( RECT& rcBtnCancel ) {
	if ( !::IsWindow( m_hWnd ) ) {
		return false;
	}
	RECT rcClient;
	unsigned int uBtnTopSpace = 175;
	unsigned int uBtnWidth = 90;
	unsigned int uBtnHeight = 30;

	if ( !::GetClientRect( m_hWnd, &rcClient ) ) {
		assert( false );	
		return false;
	}
	rcBtnCancel.top = ( rcClient.top + uBtnTopSpace );
	rcBtnCancel.left = 200;
	rcBtnCancel.right = rcBtnCancel.left + uBtnWidth;
	rcBtnCancel.bottom = rcBtnCancel.top + uBtnHeight;

	return true;
}

void CYBIMNotifyDlg::_onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = 1;

	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	RECT rcClient;

	::GetClientRect( pYBCtrlWndMsg->m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawState( memDC.getMemDC() );
	//
	::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), ( rcClient.bottom - rcClient.top ),
		memDC.getMemDC(), 0, 0, SRCCOPY );

	if ( ::IsWindow( m_btnCancel.getHWND() ) ) {
		::InvalidateRect( m_btnCancel.getHWND(), NULL, TRUE );
	}
	if ( ::IsWindow( m_btnInstall.getHWND() ) ) {
		::InvalidateRect( m_btnInstall.getHWND(), NULL, TRUE );
	}

}

void CYBIMNotifyDlg::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	HDC hPaintDC;
	PAINTSTRUCT ps;
	RECT rcClient;

	::GetClientRect( pYBCtrlWndMsg->m_hWnd, &rcClient );
	hPaintDC = ::BeginPaint( pYBCtrlWndMsg->m_hWnd, &ps );

	_drawState( hPaintDC );

	::EndPaint( pYBCtrlWndMsg->m_hWnd, &ps );
}

void CYBIMNotifyDlg::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlID = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );
	switch ( wCtrlID ) {
	case YBIMND_CTRLID_BTNINSTALL:
		{
			_onBtnInstallClick();
		}
		break;
	default:
		break;
	}
}

void CYBIMNotifyDlg::_onWM_KEYDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_KEYDOWN );
	switch ( pYBCtrlWndMsg->m_wParam )
	{
	case VK_ESCAPE:
		{
 			::PostMessage( m_hWnd, WM_CLOSE, 0, 0 );
		}
		break;
	default:
		break;
	}
}

void CYBIMNotifyDlg::_onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_NCHITTEST );

	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = HTCAPTION;
}

void CYBIMNotifyDlg::_onWM_SETCURSOR( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_SETCURSOR );

	::SetCursor( ::LoadCursor( NULL, IDC_HAND ) );

	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = TRUE;
}

void CYBIMNotifyDlg::_onBtnInstallClick() {
	RECT rcWnd;

	if ( !::GetWindowRect( m_hWnd, &rcWnd ) ) {
		assert( false );
		return;
	}
	switch ( m_eState ) {
	case ESTATE_DOWNLOAD:
		{
			// do download job.

			// change UI state.
			m_btnInstall.setText( YBIMND_TEXT_INSTALL );
			m_eState = ESTATE_INSTALL;
			::MoveWindow( m_hWnd, rcWnd.left, rcWnd.top, YBIMND_WIDTH, YBIMND_HEIGHT_INSTALL, TRUE );
		}
		break;
	case ESTATE_INSTALL:
		{
			// start install job.

			// quit.
			::PostMessage( m_hWnd, WM_CLOSE, 0, 0 );
		}
		break;
	default:
		break;
	}
}

ybctrl::YBButton* CYBIMNotifyDlg::getBtnInstall() {
	return &m_btnInstall;
}

ybctrl::YBButton* CYBIMNotifyDlg::getBtnCancel() {
	return &m_btnCancel;
}

void CYBIMNotifyDlg::drawState() {
	if ( !m_hWnd )
		return;
	RECT rcClient;
	HDC hDC;

	::GetClientRect( m_hWnd, &rcClient );

	YBCtrlMemDC memDC( ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top ) );

	_drawState( memDC.getMemDC() );

	//
	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), ( rcClient.bottom - rcClient.top ), memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
}

void CYBIMNotifyDlg::_drawState( HDC hDC ) {

	_drawBK( hDC );
	_drawTextTop( hDC );
	_drawTextMiddle( hDC );
	_drawTextUnder( hDC );
	_drawTime( hDC );
}

void CYBIMNotifyDlg::_drawBK( HDC hDC ) {
	ybctrl::YBCtrlImageStatic* pImageBK = NULL;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	pImageBK = _getBKImageObj();
	if ( !pImageBK ) {
		return;
	}
	::BitBlt( hDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top ),
		pImageBK->getMemDC(), 0, 0, SRCCOPY );
	
}

void CYBIMNotifyDlg::_drawTextTop( HDC hDC ){
	ybctrl::YBCtrlImageStatic* pImageText = NULL;
	
	const YBCtrlFont* pYBCtrlFont = NULL;
	HFONT hFontOld = NULL;
	int iBkModeOld;
	COLORREF clrTxtOld;
	RECT rcClient;
	RECT rcDrawText;

	::GetClientRect( m_hWnd, &rcClient );
	pImageText = _getBKImageObj();
	if( !pImageText ){
		return;
	}
	//draw text
	if ( _getDrawTextRect( rcDrawText ) 
		&& ( rcDrawText.right - rcDrawText.left )
		&& ( rcDrawText.bottom - rcDrawText.top )
		&& ( m_tstrText.length() > 0 ) ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_fontIdBtnTxtTop );
			if ( pYBCtrlFont ) {
				hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
				iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
				clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
				::DrawText( hDC, m_tstrText.c_str(), m_tstrText.length(), &rcDrawText, DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS|DT_SINGLELINE );
				::SetTextColor( hDC, clrTxtOld );
				::SetBkMode( hDC, iBkModeOld );
				::SelectObject( hDC, hFontOld );
			}
	}
}

void CYBIMNotifyDlg::_drawTextMiddle( HDC hDC ){
	ybctrl::YBCtrlImageStatic* pImageText = NULL;

	const YBCtrlFont* pYBCtrlFont = NULL;
	HFONT hFontOld = NULL;
	int iBkModeOld;
	COLORREF clrTxtOld;
	RECT rcClient;
	RECT rcDrawText;

	::GetClientRect( m_hWnd, &rcClient );
	pImageText = _getBKImageObj();
	if( !pImageText ){
		return;
	}
	//draw textmiddle
	if ( _getDrawTextRectMid( rcDrawText ) 
		&& ( rcDrawText.right - rcDrawText.left )
		&& ( rcDrawText.bottom - rcDrawText.top )
		&& ( m_tstrTextMid.length() > 0 ) ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_fontIdBtnTxtMid );
			if ( pYBCtrlFont ) {
				hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
				iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
				clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
				::DrawText( hDC, m_tstrTextMid.c_str(), m_tstrTextMid.length(), &rcDrawText, DT_WORDBREAK|DT_LEFT );
				::SetTextColor( hDC, clrTxtOld );
				::SetBkMode( hDC, iBkModeOld );
				::SelectObject( hDC, hFontOld );
			}
	}
}

void CYBIMNotifyDlg::_drawTextUnder( HDC hDC ){
	if ( m_eState == ESTATE_DOWNLOAD )
		return;
	ybctrl::YBCtrlImageStatic* pImageText = NULL;

	const YBCtrlFont* pYBCtrlFont = NULL;
	HFONT hFontOld = NULL;
	int iBkModeOld;
	COLORREF clrTxtOld;
	RECT rcClient;
	RECT rcDrawText;

	::GetClientRect( m_hWnd, &rcClient );
	pImageText = _getBKImageObj();
	if( !pImageText ){
		return;
	}
	//draw textunder
	if ( _getDrawTextRectUnder( rcDrawText ) 
		&& ( rcDrawText.right - rcDrawText.left )
		&& ( rcDrawText.bottom - rcDrawText.top )
		&& ( m_tstrTextUnder.length() > 0 ) ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_fontIdBtnTxtTop );
			if ( pYBCtrlFont ) {
				hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
				iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
				clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
				::DrawText( hDC, m_tstrTextUnder.c_str(), m_tstrTextUnder.length(), &rcDrawText, DT_WORDBREAK|DT_CENTER );
				::SetTextColor( hDC, clrTxtOld );
				::SetBkMode( hDC, iBkModeOld );
				::SelectObject( hDC, hFontOld );
			}
	}

}

void CYBIMNotifyDlg::_drawTime( HDC hDC ) {
	if( m_eState == ESTATE_DOWNLOAD )
		return;
	ybctrl::YBCtrlImageStatic* pImageTime = NULL;

	const YBCtrlFont* pYBCtrlFont = NULL;
	RECT rcClient;
	RECT rcDrawText;
	HFONT hFontOld = NULL;
	COLORREF clrTextOld;
	int iBkModeOld;

	::GetClientRect( m_hWnd, &rcClient );
	pImageTime = _getBKImageObj();
	if( !pImageTime ){
		return;
	}
	//draw "本次下载剩余时间"
	if( _getDrawTextRectTime( rcDrawText, YBIMND_REMAINTIME_0 )
		&& ( rcDrawText.right - rcDrawText.left )
		&& ( rcDrawText.bottom - rcDrawText.top )
		&& ( m_tstrTextTime.length() > 0 ) ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_fontIdBtnLoad );
			if ( pYBCtrlFont ) {
				hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
				iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
				clrTextOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
				::DrawText( hDC, m_tstrTextTime.c_str(), m_tstrTextTime.length(), &rcDrawText, DT_WORDBREAK|DT_LEFT );
				::SetTextColor( hDC, clrTextOld );
				::SetBkMode( hDC, iBkModeOld );
				::SelectObject( hDC, hFontOld );
			}
	}
	// draw "XX" 分

	if( _getDrawTextRectTime( rcDrawText, YBIMND_REMAINTIME_1 )
		&& ( rcDrawText.right - rcDrawText.left )
		&& ( rcDrawText.bottom - rcDrawText.top )
		&& ( m_uMinute.length() > 0 ) ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_fontIdTime );
			if ( pYBCtrlFont ) {
				hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
				iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
				clrTextOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
				::DrawText( hDC, m_uMinute.c_str(), m_uMinute.length(), &rcDrawText, DT_WORDBREAK|DT_LEFT );
				::SetTextColor( hDC, clrTextOld );
				::SetBkMode( hDC, iBkModeOld );
				::SelectObject( hDC, hFontOld );
			}
	}
	// draw "分"
	if( _getDrawTextRectTime( rcDrawText, YBIMND_REMAINTIME_2 )
		&& ( rcDrawText.right - rcDrawText.left )
		&& ( rcDrawText.bottom - rcDrawText.top )
		&& ( m_tstrTextMinute.length() > 0 ) ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_fontIdBtnLoad );
			if ( pYBCtrlFont ) {
				hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
				iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
				clrTextOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
				::DrawText( hDC, m_tstrTextMinute.c_str(), m_tstrTextMinute.length(), &rcDrawText, DT_WORDBREAK|DT_LEFT );
				::SetTextColor( hDC, clrTextOld );
				::SetBkMode( hDC, iBkModeOld );
				::SelectObject( hDC, hFontOld );
			}
	}
	// draw "XX" 秒
	if( _getDrawTextRectTime( rcDrawText, YBIMND_REMAINTIME_3 )
		&& ( rcDrawText.right - rcDrawText.left )
		&& ( rcDrawText.bottom - rcDrawText.top )
		&& ( m_uSecond.length() > 0 ) ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_fontIdTime );
			if ( pYBCtrlFont ) {
				hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
				iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
				clrTextOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
				::DrawText( hDC, m_uSecond.c_str(), m_uSecond.length(), &rcDrawText, DT_WORDBREAK|DT_LEFT );
				::SetTextColor( hDC, clrTextOld );
				::SetBkMode( hDC, iBkModeOld );
				::SelectObject( hDC, hFontOld );
			}
	}
	// draw "秒"
	if( _getDrawTextRectTime( rcDrawText, YBIMND_REMAINTIME_4 )
		&& ( rcDrawText.right - rcDrawText.left )
		&& ( rcDrawText.bottom - rcDrawText.top )
		&& ( m_tstrTextSecond.length() > 0 ) ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_fontIdBtnLoad );
			if ( pYBCtrlFont ) {
				hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
				iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
				clrTextOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
				::DrawText( hDC, m_tstrTextSecond.c_str(), m_tstrTextSecond.length(), &rcDrawText, DT_WORDBREAK|DT_LEFT );
				::SetTextColor( hDC, clrTextOld );
				::SetBkMode( hDC, iBkModeOld );
				::SelectObject( hDC, hFontOld );
			}
	}
}

ybctrl::YBCtrlImageStatic* CYBIMNotifyDlg::_getBKImageObj() {
	ybctrl::YBCtrlImageBase* pYBImageBase = NULL;


	if ( !YBCTRLIMAGEDEPOSITORY_INSTANCE_WITHNAMESPACE->getImage( _getBKImageId(), &pYBImageBase ) )
		return NULL;
	if ( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (ybctrl::YBCtrlImageStatic*)pYBImageBase;
}


unsigned int CYBIMNotifyDlg::_getBKImageId() {
	switch ( m_eState ) {
	case ESTATE_DOWNLOAD:
		{
			return m_uImageDownloadBK;
		}
		break;
	case ESTATE_INSTALL:
		{
			return m_uImageInstallBK;
		}
		break;
	default:
		break;
	}

	assert( false );
	return YBCTRL_IMAGE_ID_INVALID;
}

bool CYBIMNotifyDlg::_getDrawTextRect( RECT& rcDrawText ){
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcDrawText.left = rcClient.left + m_uTxtSpaceL;
	rcDrawText.top = rcClient.top + m_uTxtSpaceT;
	rcDrawText.right = rcClient.right - m_uTxtSpaceR;
	rcDrawText.bottom = rcClient.top + m_uTxtHeight;

	return ( rcDrawText.right >= rcDrawText.left ) && ( rcDrawText.bottom >= rcDrawText.top );
}

void CYBIMNotifyDlg::setTextTop( const tstring_type& tstrText ){
	m_tstrText = tstrText;
}

void CYBIMNotifyDlg::setTextMiddle( const tstring_type& tstrText ){
	m_tstrTextMid = tstrText;
}

bool CYBIMNotifyDlg::_getDrawTextRectMid( RECT& rcDrawText ){
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcDrawText.left = rcClient.left + m_uMTxtSpaceL;
	rcDrawText.top = rcClient.top + m_uMTxtSpaceT;
	rcDrawText.right = rcClient.right - m_uMTxtSpaceR;
	rcDrawText.bottom = rcClient.bottom - m_uMTxtSpaceB;

	return ( rcDrawText.right >= rcDrawText.left ) && ( rcDrawText.bottom >= rcDrawText.top );
}

void CYBIMNotifyDlg::setTextUnder( const tstring_type& tstrText ){
	m_tstrTextUnder = tstrText;
}

bool CYBIMNotifyDlg::_getDrawTextRectUnder( RECT& rcDrawText ){
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcDrawText.left = rcClient.left + m_uUTxtSpaceL;
	rcDrawText.right = rcClient.right - m_uUTxtSpaceR;
	rcDrawText.bottom = rcClient.bottom - m_uUTxtSpaceB;
	rcDrawText.top = rcDrawText.bottom - m_uUTxtHeight;

	return ( rcDrawText.right >= rcDrawText.left ) && ( rcDrawText.bottom >= rcDrawText.top );
}

void CYBIMNotifyDlg::setTextTime( tstring_type uMinute, tstring_type uSecond ) {
	m_uMinute = uMinute;
	m_uSecond = uSecond;
}

bool CYBIMNotifyDlg::_getDrawTextRectTime( RECT& rcDrawText, unsigned int uPartIndex ){
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	switch ( uPartIndex ) {
	case YBIMND_REMAINTIME_0:
		{
			rcDrawText.left = rcClient.left + 40 ;
			rcDrawText.top = rcClient.top + 287;
			rcDrawText.right = rcClient.right ;
			rcDrawText.bottom = rcClient.bottom - 41;

			return ( rcDrawText.right >= rcDrawText.left ) && ( rcDrawText.bottom >= rcDrawText.top );
		}
		break;
	case YBIMND_REMAINTIME_1:
		{
			rcDrawText.left = rcClient.left + 200 ;
			rcDrawText.top = rcClient.top + 287;
			rcDrawText.right = rcClient.right;
			rcDrawText.bottom = rcClient.bottom - 41;

			return ( rcDrawText.right >= rcDrawText.left ) && ( rcDrawText.bottom >= rcDrawText.top );
		}
		break;
	case YBIMND_REMAINTIME_2:
		{
			rcDrawText.left = rcClient.left +225 ;
			rcDrawText.top = rcClient.top + 287;
			rcDrawText.right = rcClient.right;
			rcDrawText.bottom = rcClient.bottom - 41;

			return ( rcDrawText.right >= rcDrawText.left ) && ( rcDrawText.bottom >= rcDrawText.top );
		}
		break;
	case YBIMND_REMAINTIME_3:
		{
			rcDrawText.left = rcClient.left + 250;
			rcDrawText.top = rcClient.top + 287;
			rcDrawText.right = rcClient.right;
			rcDrawText.bottom = rcClient.bottom - 41;

			return ( rcDrawText.right >= rcDrawText.left ) && ( rcDrawText.bottom >= rcDrawText.top );
		}
		break;
	case YBIMND_REMAINTIME_4:
		{
			rcDrawText.left = rcClient.left + 276 ;
			rcDrawText.top = rcClient.top + 287;
			rcDrawText.right = rcClient.right;
			rcDrawText.bottom = rcClient.bottom - 41;

			return ( rcDrawText.right >= rcDrawText.left ) && ( rcDrawText.bottom >= rcDrawText.top );
		}
		break;
	default:
		break;
	}
	return false;
}

void CYBIMNotifyDlg::_loadImage(){
	TCHAR szModulePath[MAX_PATH];
	TCHAR* posTmp = NULL; 
	GetModuleFileName( NULL, szModulePath, MAX_PATH );
	std::basic_string< TCHAR > tstrImageDirPath;
	std::basic_string< TCHAR > tstrImageFilePath;
	unsigned int idImage;

	posTmp =  _tcsrchr( szModulePath, _T('\\') );
	if ( !posTmp ) {
		assert( false );
		return;
	}
	posTmp += 1;
	*posTmp = 0;

	tstrImageDirPath = szModulePath;
	tstrImageDirPath += _T("Image\\");
	//
	tstrImageFilePath = tstrImageDirPath + _T("DownloadBK.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageDownloadBK = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("InstallBK.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageInstallBK = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBBtnNormal.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdYBIMNDBtnNormal = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBBtnHover.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdYBIMNDBtnHover = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBBtnPress.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdYBIMNDBtnPress = idImage;

}

void CYBIMNotifyDlg::_loadFont() {
	LOGFONT lFont;

	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 18;
	m_fontIdBtnTxt = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );

	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 25;
	lFont.lfWeight = 700;
	m_fontIdBtnTxtTop = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 0, 85, 191 ) );

	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 20;
	lFont.lfWeight = 600;
	m_fontIdBtnLoad = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 23, 84, 152 ) );

	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 20;
	m_fontIdBtnTxtMid = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 23, 84, 152 ) );

	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 33;
	m_fontIdBtnTxtUnder = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 136, 176, 225 ) );
	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 20;
	lFont.lfWeight = 600;
	m_fontIdTime = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 88, 132, 0 ) );
}

void CYBIMNotifyDlg::_init(){
	YBButton* pBtnInstall = NULL;
	YBButton* pBtnCancel = NULL;

	m_btnInstall.setFixedSpace( 5, 2, 2, 2 );
	m_btnInstall.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdYBIMNDBtnNormal );
	m_btnInstall.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdYBIMNDBtnHover );
	m_btnInstall.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdYBIMNDBtnPress );
	m_btnInstall.setFontId( m_fontIdBtnTxt );
	m_btnInstall.setTextSpace( 20, 5, 5, 5 );
	//
	m_btnCancel.setFixedSpace( 5, 2, 2, 2 );
	m_btnCancel.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdYBIMNDBtnNormal );
	m_btnCancel.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdYBIMNDBtnHover );
	m_btnCancel.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdYBIMNDBtnPress );
	m_btnCancel.setFontId( m_fontIdBtnTxt );
	m_btnCancel.setTextSpace( 20, 5, 5, 5 );
}