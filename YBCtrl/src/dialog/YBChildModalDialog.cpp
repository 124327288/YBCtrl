#include "stdafx.h"
#include "dialog/YBChildModalDialog.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlDefs.h"
#include "frame/MemFuncPack.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlMisc.h"
#include "frame/YBCtrlMemDC.h"
#include <assert.h>
//
#define YBCHILDMD_WM_BEGIN (WM_USER+200)
#define YBCHILDMD_WM_EXITMODAL (YBCHILDMD_WM_BEGIN + 1)
//
#define YBCHILDMODALDLG_FLAG_NONE (0x00000000)
#define YBCHILDMODALDLG_FLAG_DISABLE_PARENT (0x00000001)
#define YBCHILDMODALDLG_FLAG_AUTOEXIT (0x00000002)
//
#define YBCHILDMODALDLG_TIMERID_AUTOEXIT (0x0020)
//
#define YBCHILDMODALDLG_EXITID_TIMEOUT (0xFFFFFFFF)
//
USING_NAMESPACE_YBCTRL
//
YBChildModalDialog::YBChildModalDialog( bool bAutoExit, unsigned int uExitTimeMS )
: m_uFlag( bAutoExit ? YBCHILDMODALDLG_FLAG_AUTOEXIT : YBCHILDMODALDLG_FLAG_NONE )
, m_nRetCode( 0 )
, m_uImageIdBK( YBCTRL_IMAGE_ID_INVALID )
, m_uLFixed( 0 )
, m_uTFixed( 0 )
, m_uRFixed( 0 )
, m_uBFixed( 0 )
, m_uExitElapse( uExitTimeMS ) {

}

YBChildModalDialog::~YBChildModalDialog() {

}

YBCTRL_WNDMSGMAP_BEGIN_WITHNAMESPACE( YBChildModalDialog, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_SYSCOMMAND, _onWM_SYSCOMMAND )
	YBCTRL_ONWNDMSG( WM_DESTROY, _onWM_DESTROY )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_TIMER, _onWM_TIMER )
YBCTRL_WNDMSGMAP_END()

void YBChildModalDialog::_onWM_TIMER( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_TIMER );

	switch ( pYBCtrlWndMsg->m_wParam ) {
	case YBCHILDMODALDLG_TIMERID_AUTOEXIT:
		{
			endModal( YBCHILDMODALDLG_EXITID_TIMEOUT );
		}
		break;
	default:
		break;
	}
}

void YBChildModalDialog::_onWM_DESTROY( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_DESTROY );

	int a = 0;
}

void YBChildModalDialog::_onWM_SYSCOMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_SYSCOMMAND );

	switch ( pYBCtrlWndMsg->m_wParam ) {
	case SC_CLOSE:
		{
			::PostMessage( m_hWnd, WM_QUIT, 0, 0 );
		}
		break;
	default:
		{
			int a = 0;
		}
		break;
	}
}


void YBChildModalDialog::_onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );

	pYBCtrlWndMsg->m_dwFlag |= ( YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID );
	pYBCtrlWndMsg->m_lRes = 1;

	HDC hDC;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawBK( memDC.getMemDC() );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );

}

void YBChildModalDialog::_onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );

}

void YBChildModalDialog::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	RECT rcClient;
	HDC hPaintDC;
	PAINTSTRUCT ps;
	
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top ) );
	_drawBK( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top ), memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void YBChildModalDialog::_resetStatus() {
	m_nRetCode = 0;
	m_uFlag &= ~YBCHILDMODALDLG_FLAG_DISABLE_PARENT;
}

int YBChildModalDialog::doModal( HWND hWndParent ) {
	_resetStatus();
	DWORD dwStyle;
	DWORD dwExStyle;
	HWND hWndModalDlg = NULL;
	HWND hWndTop = NULL;
	int nWidth = _getWndWidth();
	int nHeight = _getWndHeight();
	int nScreenX;
	int nScreenY;
	int nWndLeft;
	int nWndTop;

	// enter - disable parent window.
	// exit - enable parent window.
	hWndTop = hWndParent;
	_disableParentWnd( hWndTop );
	CMemFuncPack mfpkEnableParentWnd( this, &YBChildModalDialog::_enableParentWnd, hWndTop );
	nScreenX = ::GetSystemMetrics( SM_CXSCREEN );
	nScreenY = ::GetSystemMetrics( SM_CYSCREEN );
	nWndLeft = ( nScreenX - nWidth ) / 2;
	nWndTop = ( nScreenY - nHeight ) / 2;
	
	try {
		// create modal dialog
		//dwExStyle = WS_EX_DLGMODALFRAME|WS_EX_WINDOWEDGE;
		//dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|DS_MODALFRAME;
		dwExStyle = 0;
		dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN;
		hWndModalDlg = _createHWND( dwExStyle, dwStyle, nWndLeft, nWndTop, nWidth, nHeight, hWndParent, (HMENU)NULL, NULL, NULL );
		if ( !hWndModalDlg ) {
			assert( false );
			return 0;
		}
		::BringWindowToTop( hWndModalDlg );

		// create user define UI.
		_createUI( hWndModalDlg, NULL );

		// start exit timer, if need.
		if ( m_uFlag & YBCHILDMODALDLG_FLAG_AUTOEXIT ) {
			::SetTimer( hWndModalDlg, YBCHILDMODALDLG_TIMERID_AUTOEXIT, m_uExitElapse, NULL );
		}

		// enter message loop
		_runModalLoop();

		// hide the window before enabling the parent, etc.
		if ( ( m_hWnd != NULL ) && ::IsWindow( m_hWnd )	) {
			::SetWindowPos( m_hWnd, NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER );
		}
	} catch ( ... ) {
		m_nRetCode = -1;
	}

	// close hWndModalDlg
	if ( ::IsWindow( hWndModalDlg ) ) {
		::DestroyWindow( hWndModalDlg );
	}
	hWndModalDlg = NULL;
	m_hWnd = NULL;

	return m_nRetCode;
}

void YBChildModalDialog::endModal( int nCode ) {
	m_nRetCode = nCode;
	::PostMessage( m_hWnd, WM_QUIT, 0, 0 );
}

void YBChildModalDialog::_runModalLoop() {
	MSG msg;

	while ( ::GetMessage( &msg, NULL, 0, 0 ) ) {
		if ( !TranslateAccelerator( msg.hwnd, NULL, &msg ) ) {
			::TranslateMessage( &msg );
			::DispatchMessage( &msg );
		}
	}

}

YBCtrlImageStatic* YBChildModalDialog::_getImageStatic( unsigned int uImageId ) {
	YBCtrlImageBase* pYBImageBase = NULL;
	YBCtrlImageStatic* pImageStatic = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pYBImageBase ) )
		return NULL;

	if( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

void YBChildModalDialog::_drawBK( HDC hDC ) {
	if( !getHWND() )
		return;
	 
	RECT rcClient;
	YBCtrlImageStatic* pYBImageBK = NULL;
	::GetClientRect( m_hWnd, &rcClient );

	pYBImageBK = _getImageStatic( m_uImageIdBK );
	if ( !pYBImageBK ) {
		return;
	}
	BackGroundBitBlt( hDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top ), 
		pYBImageBK->getMemDC(), 0, 0, pYBImageBK->getWidth(), pYBImageBK->getHeight(), 
		m_uLFixed, m_uTFixed, m_uRFixed, m_uBFixed );
}

void YBChildModalDialog::_disableParentWnd( HWND hWndParent ) {
	if ( ::IsWindow( hWndParent ) 
	&& ( hWndParent != ::GetDesktopWindow() )
	&& ::IsWindowEnabled( hWndParent ) ) {
		::EnableWindow( hWndParent, FALSE );
		m_uFlag |= YBCHILDMODALDLG_FLAG_DISABLE_PARENT;
	}
}

void YBChildModalDialog::_enableParentWnd( HWND hWndParent ) {
	if ( ::IsWindow( hWndParent )
	&& !::IsWindowEnabled( hWndParent ) ) {
		if ( m_uFlag & YBCHILDMODALDLG_FLAG_DISABLE_PARENT ) {
			::EnableWindow( hWndParent, TRUE );
			::BringWindowToTop( hWndParent );
			m_uFlag &= ~YBCHILDMODALDLG_FLAG_DISABLE_PARENT;
		}
	}
}

HWND YBChildModalDialog::_getTopWnd( HWND hWndParent ) {
	HWND hWndDesktop = NULL;
	HWND hWndPParent;
	HWND hWndCur;

	hWndDesktop = ::GetDesktopWindow();
	hWndPParent = hWndParent;
	hWndCur = hWndParent;
	while ( 1 ) {
		hWndCur = ::GetParent( hWndCur );
		if ( !hWndCur || hWndCur == hWndDesktop ) {
			break;
		}
		hWndPParent = hWndCur;
	}
	return hWndPParent;
}

void YBChildModalDialog::setBKImageId( unsigned int uImageId, unsigned int uLFixed, unsigned int uTFixed, unsigned int uRFixed, unsigned int uBFixed ) {
	m_uImageIdBK = uImageId;
	m_uLFixed = uLFixed;
	m_uTFixed = uTFixed;
	m_uRFixed = uRFixed;
	m_uBFixed = uBFixed;
}