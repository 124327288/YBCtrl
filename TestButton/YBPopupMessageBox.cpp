#include "stdafx.h"
#include "YBPopupMessageBox.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlMisc.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlFontDepository.h"
#include <assert.h>

#define YBPOPMBID_BTN_CLOSE (455)
#define YBPOPMBID_BTN_DEFAULT1 (456)
#define YBPOPMBID_BTN_DEFAULT2 (457)
#define YBPOPMBID_BTN_DEFAULT3 (458)
//
#define YBPOPMB_MAX_CHARWIDTH (710)
#define YBPOPMB_SIZE_SMALL_WIDTH (185)
//
USING_NAMESPACE_YBCTRL
//
YBPopupMessageBox::YBPopupMessageBox()
: m_nRetCode( -1 )
, m_eSizeType( EMBSIZE_SMALL )
, m_uFlag( YBMB_FLAG_NONE ) 
, m_uFontIdBtnDefault( YBCTRL_FONT_ID_INVALID )
, m_uImageIdBk( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnCloDisable( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnCloNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnCloHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnCloPre( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnDefNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnDefHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnDefPre( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdIconHand( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdIconQuestion( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdIconExclamation( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdIconAsterisk( YBCTRL_IMAGE_ID_INVALID )
, m_tstrText( _T("") )
, m_tstrCaption( _T("") )
, m_pMemDC( new ybctrl::YBCtrlMemDC( 100, 100 ) )
, m_uHeight( 0 )
, m_uLineHeight( 0 )
, m_uWidth( 0 )
, m_uCharWidth( 0 )
, m_uFontIdText( YBCTRL_FONT_ID_INVALID ) {
	_loadFont();
	_loadImage();
	_init();
}

YBPopupMessageBox::~YBPopupMessageBox() {
	if ( m_pMemDC ) {
		delete m_pMemDC;
		m_pMemDC = NULL;
	}
}

YBPopupMessageBox* YBPopupMessageBox::getInstance() {
	static YBPopupMessageBox s_instance;
	return &s_instance;
}

int YBPopupMessageBox::YBMessageBox( const tstring_type& tstrText, const tstring_type& tstrCaption, UINT uType  ) {
	unsigned int uScreenWidth;
	unsigned int uScreenHeight;
	uScreenWidth = ::GetSystemMetrics( SM_CXSCREEN );
	uScreenHeight = ::GetSystemMetrics( SM_CYSCREEN );

	m_tstrText = tstrText;
	m_tstrCaption = tstrCaption;
	_setType( uType );
	_calcAllCharWidth();
	m_transDlg4MB.create( 0, 0, uScreenWidth, uScreenHeight );
	_create( m_transDlg4MB.getHWND() );
	::ShowWindow( m_hWnd, SW_NORMAL );
	return m_nRetCode;;
}

void YBPopupMessageBox::_getWndSize( ENUMMBSIZETYPE eSizeType, int& nWidth, int& nHeight ) {
	switch ( eSizeType ) {
	case EMBSIZE_SMALL:
		{
			nWidth = 304;
			nHeight = 201;
		}
		break;
	case EMBSIZE_BIG:
		{
			nWidth = 650;
			nHeight = 201;
		}
		break;
	default:
		nWidth = 304;
		nHeight = 201;
	}
}

HWND YBPopupMessageBox::_create( HWND hWndParent ) {
	HWND hWnd;
	int x;
	int y;
	int nWidth;
	int nHeight;
	unsigned int uScreenWidth;
	unsigned int uScreenHeight;
	uScreenWidth = ::GetSystemMetrics( SM_CXSCREEN );
	uScreenHeight = ::GetSystemMetrics( SM_CYSCREEN );
	//
	if ( !_calcWidth() )
		return NULL;
	if ( !_calcHeight() )
		return NULL;
	_getWndSize( m_eSizeType, nWidth, nHeight );
	//
	assert( (int)uScreenWidth > nWidth );
	assert( (int)uScreenHeight > nHeight );
	x = ( uScreenWidth - nWidth )/2;
	y = ( uScreenHeight - nHeight )/2;

	hWnd = _createHWND( WS_EX_TOOLWINDOW|WS_EX_TOPMOST, WS_POPUP|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, x, y, nWidth, nHeight, hWndParent, (HMENU)NULL, NULL, NULL );
	if ( !hWnd ) {
		assert( false );
		return NULL;
	}
	_createUI();
	_clipWnd();
	//enter message loop
	_runMessageLoop();
	return hWnd;
}

void YBPopupMessageBox::_runMessageLoop() {
	MSG msg;
	RECT rcClient;
	POINT ptMouse;

	::GetCursorPos( &ptMouse );
	::GetClientRect( m_hWnd, &rcClient );
	::ScreenToClient( m_hWnd, &ptMouse );
	while ( ::GetMessage( &msg, NULL, 0, 0 ) ) {
		if ( msg.message == WM_LBUTTONDOWN ) {
			if ( !::PtInRect( &rcClient, ptMouse ) ) {
				::SetFocus( m_hWnd );
			}
		}
		if ( !TranslateAccelerator( msg.hwnd, NULL, &msg ) ) {
			::TranslateMessage( &msg );
			::DispatchMessage( &msg );
		}
	}
}

void YBPopupMessageBox::_createUI() {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	m_btnClose.Create( 0, WS_CHILD | WS_VISIBLE, rcClient.right - 35, 0, 35, 34, m_hWnd, ( HMENU )YBPOPMBID_BTN_CLOSE, NULL );
	m_btnDefault1.Create( 0, WS_CHILD, 0, 0, 70, 30, m_hWnd, (HMENU)YBPOPMBID_BTN_DEFAULT1, NULL );
	m_btnDefault2.Create( 0, WS_CHILD, 0, 0, 70, 30, m_hWnd, (HMENU)YBPOPMBID_BTN_DEFAULT2, NULL );
	m_btnDefault3.Create( 0, WS_CHILD, 0, 0, 70, 30, m_hWnd, (HMENU)YBPOPMBID_BTN_DEFAULT3, NULL );
	_setBtnInfo();
}

void YBPopupMessageBox::_clipWnd() {
	HRGN hRgn;
	HRGN hRgnBtnDefault1;
	HRGN hRgnBtnDefault2;
	HRGN hRgnBtnDefault3;
	RECT rcClient;
	RECT rcBtnDefault1;
	RECT rcBtnDefault2;
	RECT rcBtnDefault3;

	::GetClientRect( m_hWnd, &rcClient );
	::GetClientRect( m_btnDefault1.getHWND(), &rcBtnDefault1 );
	::GetClientRect( m_btnDefault2.getHWND(), &rcBtnDefault2 );
	::GetClientRect( m_btnDefault3.getHWND(), &rcBtnDefault3 );
	hRgn = ::CreateRoundRectRgn( 0, 0, rcClient.right + 1, rcClient.bottom + 1, 8, 9 );
	hRgnBtnDefault1 = ::CreateRoundRectRgn( 0, 0, rcBtnDefault1.right + 1, rcBtnDefault1.bottom + 1, 4, 4 );
	hRgnBtnDefault2 = ::CreateRoundRectRgn( 0, 0, rcBtnDefault2.right + 1, rcBtnDefault2.bottom + 1, 4, 4 );
	hRgnBtnDefault3 = ::CreateRoundRectRgn( 0, 0, rcBtnDefault3.right + 1, rcBtnDefault3.bottom + 1, 4, 4 );

	::SetWindowRgn( m_btnDefault3.getHWND(), hRgnBtnDefault3, TRUE );
	::SetWindowRgn( m_btnDefault2.getHWND(), hRgnBtnDefault2, TRUE );
	::SetWindowRgn( m_btnDefault1.getHWND(), hRgnBtnDefault1, TRUE );
	::SetWindowRgn( m_hWnd, hRgn, TRUE );
	::DeleteObject( hRgn );
	::DeleteObject( hRgnBtnDefault1 );
	::DeleteObject( hRgnBtnDefault2 );
	::DeleteObject( hRgnBtnDefault3 );
}

YBCTRL_WNDMSGMAP_BEGIN( YBPopupMessageBox, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_NCHITTEST, _onWM_NCHITTEST )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_ONWNDMSG( WM_KEYDOWN, _onWM_KEYDOWN )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_DESTROY, _onWM_DESTROY )
YBCTRL_WNDMSGMAP_END()

void YBPopupMessageBox::_onWM_DESTROY( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_DESTROY );
	m_uFlag &= ~YBMB_FLAG_NONE;
}

void YBPopupMessageBox::_onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	RECT rcClient;

	pYBCtrlWndMsg->m_dwFlag = (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = 1;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
}

void YBPopupMessageBox::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	RECT rcClient;
	HDC hPaintDC;
	PAINTSTRUCT ps;

	::GetClientRect( m_hWnd, &rcClient );
	m_pMemDC->resize( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( m_pMemDC->getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, m_pMemDC->getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void YBPopupMessageBox::_onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_NCHITTEST );
	pYBCtrlWndMsg->m_dwFlag = (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = HTCAPTION;
}

void YBPopupMessageBox::_onWM_KEYDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_KEYDOWN );
	if ( pYBCtrlWndMsg->m_wParam == VK_ESCAPE ) {
		if ( ( ( m_uFlag & YBMB_ABORTRETRYIGNORE ) == YBMB_ABORTRETRYIGNORE )
			|| ( ( m_uFlag & YBMB_YESNO ) == YBMB_YESNO ) ) {
			return;
		} else {
			_endMB( 0 );
		}
	}
	if ( pYBCtrlWndMsg->m_wParam == VK_RETURN ) {
		_endMB( 1 );
	}
}

void YBPopupMessageBox::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlID = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wCtrlEvt = HIWORD( pYBCtrlWndMsg->m_wParam );
	switch( wCtrlID ) {
	case YBPOPMBID_BTN_CLOSE:
		{
			_endMB( 0 );
		}
		break;
	case YBPOPMBID_BTN_DEFAULT1:
		{
			_endMB( 1 );
		}
		break;
	case YBPOPMBID_BTN_DEFAULT2:
		{
			_endMB( 2 );
		}
		break;
	case YBPOPMBID_BTN_DEFAULT3:
		{
			_endMB( 3 );
		}
		break;
	default:
		break;
	}
}

void YBPopupMessageBox::_drawStatus( HDC hDC ) {
	_drawBk( hDC ) ;
	_drawIcon( hDC );
	_drawText( hDC );
	_drawCaption( hDC );
}

void YBPopupMessageBox::_endMB( int nBtnIndex ) {
	switch ( nBtnIndex ) {
	case 0:
		{
			if ( ( m_uFlag & YBMB_OK ) == YBMB_OK ) {
				m_nRetCode = YBIDOK;
			} else {
				m_nRetCode = YBIDCANCEL;
			}	
		}
		break;
	case 1:
		{
			if ( ( ( m_uFlag & YBMB_OK ) == YBMB_OK )
				|| ( ( m_uFlag & YBMB_OKCANCEL ) == YBMB_OKCANCEL ) ) {
				m_nRetCode = YBIDOK;
			}
			if ( ( m_uFlag & YBMB_ABORTRETRYIGNORE ) == YBMB_ABORTRETRYIGNORE ) {
				m_nRetCode = YBIDABORT;
			}
			if ( ( ( m_uFlag & YBMB_YESNOCANCEL ) == YBMB_YESNOCANCEL )
				|| ( ( m_uFlag & YBMB_YESNO ) == YBMB_YESNO ) ) {
				m_nRetCode = YBIDYES;
			}
			if ( ( m_uFlag & YBMB_RETRYCANCEL ) == YBMB_RETRYCANCEL ) {
				m_nRetCode = YBIDRETRY;
			}
			if ( ( m_uFlag & YBMB_CANCELTRYCONTINUE ) == YBMB_CANCELTRYCONTINUE ) {
				m_nRetCode = YBIDCANCEL;
			}
		}
		break;
	case 2:
		{
			if ( ( ( m_uFlag & YBMB_OKCANCEL ) == YBMB_OKCANCEL )
				|| ( ( m_uFlag & YBMB_RETRYCANCEL ) == YBMB_RETRYCANCEL ) ) {
					m_nRetCode = YBIDCANCEL;
			}
			if ( ( ( m_uFlag & YBMB_ABORTRETRYIGNORE ) == YBMB_ABORTRETRYIGNORE ) ) {
					m_nRetCode = YBIDRETRY;
			}
			if ( ( ( m_uFlag & YBMB_YESNOCANCEL ) == YBMB_YESNOCANCEL )
				|| ( ( m_uFlag & YBMB_YESNO ) == YBMB_YESNO ) ) {
					m_nRetCode = YBIDNO;
			}
			if ( ( ( m_uFlag & YBMB_CANCELTRYCONTINUE ) == YBMB_CANCELTRYCONTINUE ) ) {
					m_nRetCode = YBIDTRYAGAIN;
			}
		}
		break;
	case 3:
		{
			if ( ( m_uFlag & YBMB_ABORTRETRYIGNORE ) == YBMB_ABORTRETRYIGNORE ) {
				m_nRetCode = YBIDIGNORE;
			}
			if ( ( m_uFlag & YBMB_YESNOCANCEL ) == YBMB_YESNOCANCEL ) {
				m_nRetCode = YBIDCANCEL;
			}
			if ( ( m_uFlag & YBMB_CANCELTRYCONTINUE ) == YBMB_CANCELTRYCONTINUE ) {
				m_nRetCode = YBIDCONTINUE;
			}
		}
		break;
	default:
		break;
	}
	::PostMessage( m_transDlg4MB.getHWND(), WM_CLOSE, 0, 0 );
	::PostMessage( m_hWnd, WM_QUIT, 0, 0 );
}

void YBPopupMessageBox::_setBtnInfo() {
	if ( ( m_uFlag & YBMB_OK ) == YBMB_OK ) {
		_setBtnInfo( 1, true, _T("确定"), _T(""), _T("") );
		return;
	}
	if ( ( m_uFlag & YBMB_OKCANCEL ) == YBMB_OKCANCEL ) {
		_setBtnInfo( 2, true, _T("确定"), _T("取消"), _T("") );
		return;
	}
	if ( ( m_uFlag & YBMB_ABORTRETRYIGNORE ) == YBMB_ABORTRETRYIGNORE ) {
		_setBtnInfo( 3, false, _T("中止(A)"), _T("重试(R)"), _T("忽略(I)") );
		return;
	}
	if ( ( m_uFlag & YBMB_YESNOCANCEL ) == YBMB_YESNOCANCEL ) {
		_setBtnInfo( 3, true, _T("是(Y)"), _T("否(N)"), _T("取消") );
		return;
	}
	if ( ( m_uFlag & YBMB_YESNO ) == YBMB_YESNO ) {
		_setBtnInfo( 2, false, _T("是(Y)"), _T("否(N)"), _T("") );
		return;
	}
	if ( ( m_uFlag & YBMB_RETRYCANCEL ) == YBMB_RETRYCANCEL ) {
		_setBtnInfo( 2, true, _T("重试(R)"), _T("取消"), _T("") );
		return;
	}
	if ( ( m_uFlag & YBMB_CANCELTRYCONTINUE ) == YBMB_CANCELTRYCONTINUE ) {
		_setBtnInfo( 3, true, _T("取消"), _T("重试(T)"), _T("继续(C)") );
		return;
	}
}


void YBPopupMessageBox::_setType( UINT uType ) {
	//m_uFlag |= uType;
	m_uFlag = uType;
}

void YBPopupMessageBox::_setBtnInfo( unsigned int uIndexBtn, bool bBtnEnable, tstring_type tstrTextBtnDef1, tstring_type tstrTextBtnDef2, tstring_type tstrTextBtnDef3 ) {
	RECT rcClient;
	unsigned int uWidth;
	unsigned int u2BtnSpace;
	unsigned int u3BtnSpace;
	unsigned int uBtnWidth = 70;
	unsigned int uBtnHeight = 30;
	// set Close Btn Enable
	m_btnClose.enable( bBtnEnable );

	::GetClientRect( m_hWnd, &rcClient );
	uWidth = rcClient.right - rcClient.left;
	if ( m_eSizeType == EMBSIZE_BIG ) {
		u2BtnSpace = 60;
		u3BtnSpace = 50;
	} else {
		u2BtnSpace = 30;
		u3BtnSpace = 20;
	}

	switch ( uIndexBtn ) {
	case 1:
		{
			m_btnDefault1.setText( tstrTextBtnDef1 );
			::MoveWindow( m_btnDefault1.getHWND(), ( uWidth - uBtnWidth )/2, 155, uBtnWidth, uBtnHeight, FALSE );
			::ShowWindow( m_btnDefault1.getHWND(), SW_NORMAL );
		}
		break;
	case 2:
		{
			m_btnDefault1.setText( tstrTextBtnDef1 );
			m_btnDefault2.setText( tstrTextBtnDef2 );
			::MoveWindow( m_btnDefault1.getHWND(), ( uWidth - u2BtnSpace )/2 - uBtnWidth, 155, uBtnWidth, uBtnHeight, FALSE );
			::MoveWindow( m_btnDefault2.getHWND(), ( uWidth + u2BtnSpace )/2, 155, uBtnWidth, uBtnHeight, FALSE );
			::ShowWindow( m_btnDefault1.getHWND(), SW_NORMAL );
			::ShowWindow( m_btnDefault2.getHWND(), SW_NORMAL );
		}
		break;
	case 3:
		{
			m_btnDefault1.setText( tstrTextBtnDef1 );
			m_btnDefault2.setText( tstrTextBtnDef2 );
			m_btnDefault3.setText( tstrTextBtnDef3 );
			::MoveWindow( m_btnDefault1.getHWND(), ( uWidth - uBtnWidth )/2, 155, uBtnWidth, uBtnHeight, FALSE );
			::MoveWindow( m_btnDefault2.getHWND(), ( uWidth - uBtnWidth )/2 - u3BtnSpace - uBtnWidth, 155, uBtnWidth, uBtnHeight, FALSE );
			::MoveWindow( m_btnDefault3.getHWND(), ( uWidth + uBtnWidth )/2 + u3BtnSpace, 155, uBtnWidth, uBtnHeight, FALSE );
			::ShowWindow( m_btnDefault1.getHWND(), SW_NORMAL );
			::ShowWindow( m_btnDefault2.getHWND(), SW_NORMAL );
			::ShowWindow( m_btnDefault3.getHWND(), SW_NORMAL );
		}
		break;
	default:
		break;
	}
}

// void YBPopupMessageBox::_drawText( HDC hDC ) {
// 	HFONT hFontOld = NULL;
// 	const YBCtrlFont* pYBCtrlFont = NULL;
// 	COLORREF clrTxtOld;
// 	int iBkModeOld;
// 	RECT rcText;
// 
// 	if( !_getTextRect( rcText ) )
// 		return;
// 	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdText );
// 	if( pYBCtrlFont ){
// 		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
// 		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
// 		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
// 		unsigned int uTxtHeight = ::DrawText( hDC, m_tstrText.c_str(), m_tstrText.length(), &rcText, DT_CENTER | DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL );
// 		::DrawText( hDC, m_tstrText.c_str(), m_tstrText.length(), &rcText, DT_LEFT|DT_VCENTER|DT_EDITCONTROL|DT_WORDBREAK );
// 		::SetTextColor( hDC, clrTxtOld );
// 		::SetBkMode( hDC, iBkModeOld );
// 		::SelectObject( hDC, hFontOld );
// 	}
// }

void YBPopupMessageBox::_drawCaption( HDC hDC ) {
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcCaption;

	if( !_getCaptionRect( rcCaption ) )
		return;
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdText );
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
		::DrawText( hDC, m_tstrCaption.c_str(), m_tstrCaption.length(), &rcCaption, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

bool YBPopupMessageBox::_getTextRect( RECT& rcText ) const {
	RECT rcIcon;
	RECT rcClient;
	unsigned int uImageIconId;

	::GetClientRect( m_hWnd, &rcClient );
	if ( !_getIconRect( rcIcon ) )
		return false;
	uImageIconId = _getIconId();
	if ( uImageIconId == -1 ) {
		rcText.left = rcIcon.left;
		rcText.top = rcIcon.top + ( rcIcon.bottom - rcIcon.top )/2 - 20;
	} else {
		rcText.left = rcIcon.right + 5;
		rcText.top = rcIcon.top + 1;
	}
	rcText.right = rcClient.right - 15;
	rcText.bottom = rcText.top + 50;

	return ( ( rcText.right > rcText.left ) && ( rcText.bottom > rcText.top ) );
}

bool YBPopupMessageBox::_getCaptionRect( RECT& rcCaption ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcCaption.left = rcClient.left + 6;
	rcCaption.right = rcClient.right;
	rcCaption.top = rcClient.top + 6;
	rcCaption.bottom = rcCaption.top + 18;

	return ( ( rcCaption.right > rcCaption.left ) && ( rcCaption.bottom > rcCaption.top ) );
}

void YBPopupMessageBox::_drawBk( HDC hDC ) {
	YBCtrlImageStatic* pYBImage = NULL;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	if ( m_eSizeType == EMBSIZE_SMALL ) {
		pYBImage = _getImageStatic( m_uImageIdBk );
	} else if ( m_eSizeType == EMBSIZE_BIG ){
		pYBImage = _getImageStatic( m_uImageIdBkBigSize );
	}
	if ( !pYBImage ) {
		assert( false );
		return;
	}

	BackGroundBitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pYBImage->getMemDC(), 0, 0,
		pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0 );
}

void YBPopupMessageBox::_drawIcon( HDC hDC ) {
	YBCtrlImageStatic* pYBImageIcon = NULL;
	unsigned int uImageIconId;
	RECT rcIcon;

	uImageIconId = _getIconId();
	if ( uImageIconId == -1 )
		return;
	pYBImageIcon = _getImageStatic( uImageIconId );
	if ( !pYBImageIcon )
		return;
	//
	Gdiplus::Graphics graphics( hDC );
	if ( _getIconRect( rcIcon ) ) {
		GdipBitBlt( &graphics, pYBImageIcon->getImage(), rcIcon.left, rcIcon.top, (rcIcon.right - rcIcon.left), (rcIcon.bottom - rcIcon.top)
			, 0, 0, pYBImageIcon->getWidth(), pYBImageIcon->getHeight() );
	}
}

bool YBPopupMessageBox::_getIconRect( RECT& rcIcon ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcIcon.left = rcClient.left + 32;
	rcIcon.top = rcClient.top + 60;
	rcIcon.right = rcIcon.left + 67;
	rcIcon.bottom = rcIcon.top + 58;
	return ( ( rcIcon.right > rcIcon.left ) && ( rcIcon.bottom > rcIcon.top ) );
}

unsigned int YBPopupMessageBox::_getIconId() const {
	if ( ( m_uFlag & YBMB_ICONHAND ) == YBMB_ICONHAND ) {
		return m_uImageIdIconHand;
	}
	if ( ( m_uFlag & YBMB_ICONQUESTION ) == YBMB_ICONQUESTION ) {
		return m_uImageIdIconQuestion;
	}
	if ( ( m_uFlag & YBMB_ICONEXCLAMATION ) == YBMB_ICONEXCLAMATION ) {
		return m_uImageIdIconExclamation;
	}
	if ( ( m_uFlag & YBMB_ICONASTERISK ) == YBMB_ICONASTERISK ) {
		return m_uImageIdIconAsterisk;
	}
	return -1;
}

#define BINDING_IMAGE_FNAME2ID( filename, imageid ) \
	do { \
	tstring_type tstrImageDirPath; \
	tstring_type tstrImageFilePath; \
	TCHAR szModulePath[MAX_PATH]; \
	TCHAR* posTmp = NULL; \
	::GetModuleFileName( NULL, szModulePath, MAX_PATH ); \
	posTmp = _tcsrchr( szModulePath, _T('\\')); \
	if ( !posTmp ) { \
		assert( false ); \
	return; \
	} \
	posTmp += 1; \
	*posTmp = 0; \
	tstrImageDirPath = szModulePath; \
	tstrImageDirPath += _T("Image\\"); \
	tstrImageFilePath = tstrImageDirPath; \
	tstrImageFilePath += filename; \
	if ( !ybctrl::YBImageLoader::getInstance()->loadImage( tstrImageFilePath, imageid ) ) { \
	assert( false ); \
	return; \
	} \
	} while( 0 )

void YBPopupMessageBox::_loadImage() {
	BINDING_IMAGE_FNAME2ID( _T("YBPopupMessageBoxBk.png"), m_uImageIdBk );
	BINDING_IMAGE_FNAME2ID( _T( "YBPopupMessageBoxStretchBk.png" ), m_uImageIdBkBigSize );
	BINDING_IMAGE_FNAME2ID( _T("YBPopupMBCloBtnDisable.png"), m_uImageIdBtnCloDisable );
	BINDING_IMAGE_FNAME2ID( _T("YBPopupMBCloBtnNor.png"), m_uImageIdBtnCloNor );
	BINDING_IMAGE_FNAME2ID( _T("YBPopupMBCloBtnHov.png"), m_uImageIdBtnCloHov );
	BINDING_IMAGE_FNAME2ID( _T("YBPopupMBCloBtnPre.png"), m_uImageIdBtnCloPre );
	BINDING_IMAGE_FNAME2ID( _T("YBPopupMBDefBtnNor.png"), m_uImageIdBtnDefNor );
	BINDING_IMAGE_FNAME2ID( _T("YBPopupMBDefBtnHov.png"), m_uImageIdBtnDefHov );
	BINDING_IMAGE_FNAME2ID( _T("YBPopupMBDefBtnPre.png"), m_uImageIdBtnDefPre );
	BINDING_IMAGE_FNAME2ID( _T("YBMBIconExclamation.png"), m_uImageIdIconExclamation );
	BINDING_IMAGE_FNAME2ID( _T("YBMBIconQuestion.png"), m_uImageIdIconQuestion );
	BINDING_IMAGE_FNAME2ID( _T("YBMBIconHand.png"), m_uImageIdIconHand );
	BINDING_IMAGE_FNAME2ID( _T("YBMBIconAsterisk.png"), m_uImageIdIconAsterisk );
}

ybctrl::YBCtrlImageStatic* YBPopupMessageBox::_getImageStatic( unsigned int uImageId ) {
	YBCtrlImageBase* pYBImageBase = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pYBImageBase ) )
		return NULL;

	if( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

void YBPopupMessageBox::_init() {
	//
	m_btnClose.setText( _T("关闭") );
	m_btnClose.setImage( YBButton::EBTN_STATUS_DISABLE, m_uImageIdBtnCloDisable );
	m_btnClose.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnCloNor );
	m_btnClose.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnCloHov );
	m_btnClose.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnCloPre );
	//
	m_btnDefault1.setHovTxtClr();
	m_btnDefault1.setTxtVcenter( true );
	m_btnDefault1.setFixedSpace( 5, 5, 5, 5 );
	m_btnDefault1.setFontId( m_uFontIdBtnDefault );
	m_btnDefault1.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnDefNor );
	m_btnDefault1.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnDefHov );
	m_btnDefault1.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnDefPre );
	//
	m_btnDefault2.setHovTxtClr();
	m_btnDefault2.setTxtVcenter( true );
	m_btnDefault2.setFixedSpace( 5, 5, 5, 5 );
	m_btnDefault2.setFontId( m_uFontIdBtnDefault );
	m_btnDefault2.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnDefNor );
	m_btnDefault2.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnDefHov );
	m_btnDefault2.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnDefPre );
	//
	m_btnDefault3.setHovTxtClr();
	m_btnDefault3.setTxtVcenter( true );
	m_btnDefault3.setFontId( m_uFontIdBtnDefault );
	m_btnDefault3.setFixedSpace( 5, 5, 5, 5 );
	m_btnDefault3.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnDefNor );
	m_btnDefault3.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnDefHov );
	m_btnDefault3.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnDefPre );
}

void YBPopupMessageBox::_loadFont() {
	//
	LOGFONT lFont;
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 19;
	m_uFontIdBtnDefault = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(255, 255, 255) );
	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 20;
	m_uFontIdText = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(0, 0, 0) );
}

void YBPopupMessageBox::_calcAllCharWidth() {
	m_containerBrCharIndex.clear();
	m_containerCharWidth.clear();
	RECT rcTextCalc;

	TCHAR tchData;
	TCHAR tszData[ 2 ];
	unsigned int uLenStrData;
	int nScreenWidth;
	int nScreenHeight;
	const YBCtrlFont* pYBCtrlFont = NULL;
	HFONT hFontOld;

	nScreenWidth = GetSystemMetrics( SM_CXSCREEN );
	nScreenHeight = GetSystemMetrics( SM_CYSCREEN );
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdText );
	if ( !pYBCtrlFont ) {
		assert( false );
		return;
	}
	hFontOld = ( HFONT )::SelectObject( m_pMemDC->getMemDC(), pYBCtrlFont->m_hFont );
	uLenStrData = m_tstrText.length();
	for ( unsigned int uIndex = 0; uIndex < uLenStrData; ++uIndex ) {
		tchData = m_tstrText[ uIndex ];
		tszData[ 0 ] = tchData;
		tszData[ 1 ] = 0;

		if ( tchData == '\n' ) {
			m_containerBrCharIndex.push_back( uIndex );
		}
		::SetRect( &rcTextCalc, 0, 0, nScreenWidth, nScreenHeight );
		::DrawText( m_pMemDC->getMemDC(), tszData, 1, &rcTextCalc, DT_CALCRECT | DT_SINGLELINE | DT_LEFT );
		m_containerCharWidth.push_back( rcTextCalc.right - rcTextCalc.left );
	}
	::SelectObject( m_pMemDC->getMemDC(), hFontOld );
	//
	assert( m_containerCharWidth.size() == m_tstrText.length() );
}

unsigned int YBPopupMessageBox::_calcEveryPartCharCount( unsigned int uIndexPart ) {
	unsigned int uCountBr = m_containerBrCharIndex.size();
	unsigned int uIndexStart = 0;
	unsigned int uIndexEnd;
	unsigned int uCountPartChar;

	if ( uIndexPart != 0 ) {
		uIndexStart = m_containerBrCharIndex[ uIndexPart - 1 ];
	}
	if ( uIndexPart == uCountBr ) {
		uIndexEnd = m_containerCharWidth.size();
	} else {
		uIndexEnd = m_containerBrCharIndex[ uIndexPart ];
	}
	uCountPartChar = uIndexEnd - uIndexStart;

	return uCountPartChar;
}

unsigned int YBPopupMessageBox::_calcEveryPartWidth( unsigned int uIndexPart ) {
	unsigned int uWidthPart = 0;
	unsigned int uIndexStart = 0;
	unsigned int uIndexEnd;
	unsigned int uCountBr = m_containerBrCharIndex.size();

	if ( uIndexPart != 0 ) {
		uIndexStart = m_containerBrCharIndex[ uIndexPart - 1 ];
	}
	if ( uIndexPart == uCountBr ) {
		uIndexEnd = m_containerCharWidth.size();
	} else {
		uIndexEnd = m_containerBrCharIndex[ uIndexPart ];
	}
	for ( uIndexStart; uIndexStart != uIndexEnd; ++uIndexStart ) {
		uWidthPart += m_containerCharWidth[ uIndexStart ];
	}

	return uWidthPart;
}

bool YBPopupMessageBox::_calcHeight() {
	const YBCtrlFont* pYBFont = NULL;

	pYBFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdText );
	if ( !pYBFont ) {
		assert( false );
		return false;
	}
	if ( pYBFont->m_logFont.lfHeight > 0 ) {
		m_uLineHeight = pYBFont->m_logFont.lfHeight;
	} else {
		m_uLineHeight = -( pYBFont->m_logFont.lfHeight );
	}
	//
	unsigned int uTxtWidth;
	unsigned int uCharIndex = 0;
	unsigned int uLineIndex = 0;
	unsigned int uCharCountCalc;
	unsigned int uLineTop;
	unsigned int uWidthPart;
	unsigned int uCountPart = m_containerBrCharIndex.size() + 1;

	tstring_type tstrLineData;
	uTxtWidth = YBPOPMB_SIZE_SMALL_WIDTH;

	if ( m_containerBrCharIndex.size() == 0 ) {
		//
		if ( m_uCharWidth > uTxtWidth ) {
			while ( uCharIndex < m_tstrText.length() ) {
				if ( _calcCharCount( uCharIndex, uTxtWidth, uCharCountCalc ) ) {
					//
					uLineTop = uLineIndex*m_uLineHeight;
					tstrLineData = m_tstrText.substr( uCharIndex, uCharCountCalc );
					//
					uCharIndex += uCharCountCalc;
					++uLineIndex;
				} else {
					assert( false );
					break;
				}
				// 字体距离上下距离各位2像素
				m_uHeight = uLineIndex*( m_uLineHeight + 4 );
			}
		} else {
			m_uHeight = m_uLineHeight + 4;
		}
	} else {
		for ( unsigned int uIndexPart = 0; uIndexPart != uCountPart; ++uIndexPart ) {
			uWidthPart = _calcEveryPartWidth( uIndexPart );
			if ( uWidthPart > uTxtWidth ) {
				++uLineIndex;
			}
			++uLineIndex;
		}
		// 字体距离上下距离各位2像素
		m_uHeight = uLineIndex*( m_uLineHeight + 4 );
	}
	if ( uLineIndex > 4 ) {
		m_eSizeType = EMBSIZE_BIG;
	} else {
		m_eSizeType = EMBSIZE_SMALL;
	}

	return true;
}

bool YBPopupMessageBox::_calcWidth() {
	if ( m_containerCharWidth.size() != m_tstrText.length() ) {
		assert( false );
		return false;
	}
	unsigned int uTxtWidth;

	uTxtWidth = YBPOPMB_SIZE_SMALL_WIDTH;
	// 是否有换行符.
	if ( m_containerBrCharIndex.size() == 0 ) {
		for ( unsigned int uIndex = 0; uIndex < m_containerCharWidth.size(); ++uIndex ) {
			m_uCharWidth += m_containerCharWidth[ uIndex ];
		}
		// 字体距离前后边框距离各位5像素
		m_uWidth = min( m_uCharWidth, uTxtWidth );
	} else {
		unsigned int uCountDivPart = m_containerBrCharIndex.size() + 1;
		unsigned int uPartWidth = 0;
		unsigned int uPrePartWidth = 0;
		unsigned int uCharWidth = 0;

		for ( unsigned int uIndex = 0; uIndex != uCountDivPart; ++uIndex ) {
			uPartWidth = _calcEveryPartWidth( uIndex );
			uCharWidth = max( uPartWidth, uPrePartWidth );
			uPrePartWidth = uPartWidth;
		}
		// 字体距离前后边框距离各位5像素
		m_uWidth = min( uCharWidth, uTxtWidth );
	}
	return true;
}

void YBPopupMessageBox::_drawText( HDC hDC ) {
	RECT rcCalc;
	RECT rcTxt;
	unsigned int uWidth;
	unsigned int uLineTop;
	unsigned int uCharIndex = 0;
	unsigned int uLineIndex = 0;
	unsigned int uCharCountCalc;
	tstring_type tstrLineData;
	const YBCtrlFont* pYBFont = NULL;
	HFONT hFontOld = NULL;
	COLORREF clrTxtOld;
	int iBkMode;

	pYBFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdText );
	if ( !pYBFont ) {
		assert( false );
		return;
	}
	if ( !_getTextRect( rcTxt ) )
		return;
	uWidth = rcTxt.right - rcTxt.left;
	hFontOld = ( HFONT )::SelectObject( hDC, pYBFont->m_hFont );
	iBkMode = ::SetBkMode( hDC, TRANSPARENT );
	clrTxtOld = ::SetTextColor( hDC, pYBFont->m_clrTxt );

	if ( m_containerBrCharIndex.size() == 0 ) {  //没有换行符.
		// 
		while ( uCharIndex < m_tstrText.length() ) {
			if ( _calcCharCount( uCharIndex, uWidth, uCharCountCalc ) ) {
				//
				uLineTop = rcTxt.top + m_uLineHeight*uLineIndex;
				::SetRect( &rcCalc, rcTxt.left, uLineTop, rcTxt.left + uWidth, uLineTop + m_uLineHeight );
				//
				tstrLineData = m_tstrText.substr( uCharIndex, uCharCountCalc );
				::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_LEFT|DT_SINGLELINE );
				//
				uCharIndex += uCharCountCalc;
				++uLineIndex;
			} else {
				assert( false );
				break;
			}
		}
	} else {    //有换行符.
		unsigned int uWidthPart;
		unsigned int uCountPart = m_containerBrCharIndex.size() + 1;
		for ( unsigned int uIndexPart = 0; uIndexPart != uCountPart; ++uIndexPart ) {
			if ( uIndexPart == 0 ) {
				uCharIndex = 0;
			} else {
				uCharIndex = m_containerBrCharIndex[ uIndexPart - 1 ];
			}
			uWidthPart = _calcEveryPartWidth( uIndexPart );
			if ( uWidthPart > uWidth ) {   //当每部分的长度超过最大屏幕宽度时.
				// 
				while ( uCharIndex < m_tstrText.length() ) {
					if ( _calcCharCount( uCharIndex, uWidth, uCharCountCalc ) ) {
						//
						uLineTop = rcTxt.top + m_uLineHeight*uLineIndex;
						::SetRect( &rcCalc, rcTxt.left, uLineTop, rcTxt.left + uWidth, uLineTop + m_uLineHeight );
						//
						tstrLineData = m_tstrText.substr( uCharIndex, uCharCountCalc );
						::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_LEFT | DT_SINGLELINE );
						//
						uCharIndex += uCharCountCalc;
						++uLineIndex;
					} else {
						assert( false );
						break;
					}
				}
			} else {
				uLineTop = rcTxt.top + m_uLineHeight*uLineIndex;
				uCharCountCalc = _calcEveryPartCharCount( uIndexPart );
				::SetRect( &rcCalc, rcTxt.left, uLineTop, rcTxt.left + uWidth, uLineTop + m_uLineHeight );
				//
				tstrLineData = m_tstrText.substr( uCharIndex, uCharCountCalc );
				::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_LEFT | DT_SINGLELINE );
				++uLineIndex;
			}
		}
	}
	//
	::SetTextColor( hDC, clrTxtOld );
	::SetBkMode( hDC, iBkMode );
	::SelectObject( hDC, hFontOld );
}

bool YBPopupMessageBox::_calcCharCount( unsigned int uCharIndexBegin, unsigned int uWidth, unsigned int& uCharCountCalc ) {
	assert( m_tstrText.length() == m_containerCharWidth.size() );
	if ( uCharIndexBegin >= m_containerCharWidth.size() ) {
		assert( false );
		return false;
	}
	unsigned int uWidthSum = 0;
	uWidthSum = 0;
	uCharCountCalc = 0;
	for ( unsigned int uIndex = uCharIndexBegin; uIndex < m_containerCharWidth.size(); ++uIndex ) {
		uWidthSum += m_containerCharWidth[ uIndex ];
		if ( uWidthSum > uWidth ) {
			break;
		}
		uCharCountCalc++;
	}
	return true;
}