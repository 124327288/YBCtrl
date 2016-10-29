#include "stdafx.h"
#include "dialog/YBModalDialog.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlDefs.h"
#include "frame/MemFuncPack.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlMisc.h"
#include "frame/YBCtrlMemDC.h"
#include <assert.h>
//
#define YBMD_WM_BEGIN (WM_USER+200)
#define YBMD_WM_EXITMODAL (YBMD_WM_BEGIN + 1)
//
#ifndef WM_KICKIDLE
#define WM_KICKIDLE (0x036A)  // (params unused) causes idles to kick in
#endif //WM_KICKIDLE
//
#define YBMODALDLG_FLAG_NONE (0x00000000)
#define YBMODALDLG_FLAG_DISABLE_PARENT (0x00000001)
#define YBMODALDLG_FLAG_AUTOEXIT (0x00000002)
//
#define YBMODALDLG_TIMERID_AUTOEXIT (0x0020)
//
#define YBMODALDLG_EXITID_TIMEOUT (0xFFFFFFFF)
//
USING_NAMESPACE_YBCTRL
//
YBModalDialog::YBModalDialog( bool bAutoExit, unsigned int uExitTimeMS )
: m_uFlag( bAutoExit ? YBMODALDLG_FLAG_AUTOEXIT : YBMODALDLG_FLAG_NONE )
, m_nRetCode( 0 )
, m_uImageIdBK( YBCTRL_IMAGE_ID_INVALID )
, m_uLFixed( 0 )
, m_uTFixed( 0 )
, m_uRFixed( 0 )
, m_uBFixed( 0 )
, m_uExitElapse( uExitTimeMS )
, m_uMessageLast( 0 )
{
    memset( &m_ptCursorLast, 0, sizeof( POINT ) );
}

YBModalDialog::~YBModalDialog() {

}

YBCTRL_WNDMSGMAP_BEGIN_WITHNAMESPACE( YBModalDialog, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_SYSCOMMAND, _onWM_SYSCOMMAND )
	YBCTRL_ONWNDMSG( WM_DESTROY, _onWM_DESTROY )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_TIMER, _onWM_TIMER )
YBCTRL_WNDMSGMAP_END()

void YBModalDialog::_onWM_TIMER( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_TIMER );

	switch ( pYBCtrlWndMsg->m_wParam ) {
	case YBMODALDLG_TIMERID_AUTOEXIT:
		{
			endModal( YBMODALDLG_EXITID_TIMEOUT );
		}
		break;
	default:
		break;
	}
}

void YBModalDialog::_onWM_DESTROY( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_DESTROY );

	int a = 0;
}

void YBModalDialog::_onWM_SYSCOMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
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

void YBModalDialog::_onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );

	pYBCtrlWndMsg->m_dwFlag |= ( YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID );
	pYBCtrlWndMsg->m_lRes = 1;

	HDC hDC;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );

}

void YBModalDialog::_onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );

}

void YBModalDialog::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	RECT rcClient;
	HDC hPaintDC;
	PAINTSTRUCT ps;
	
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top ) );
	_drawStatus( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top ), memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void YBModalDialog::_drawStatus( HDC hDC ) {
	_drawBK( hDC );
	_drawSelf( hDC );
}

void YBModalDialog::_resetStatus() {
	m_nRetCode = 0;
	m_uFlag &= ~YBMODALDLG_FLAG_DISABLE_PARENT;
}

int YBModalDialog::doModal( HWND hWndParent ) {
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
	hWndTop = _getTopWnd( hWndParent );
	_disableParentWnd( hWndTop );
	CMemFuncPack mfpkEnableParentWnd( this, &YBModalDialog::_enableParentWnd, hWndTop );
	nScreenX = ::GetSystemMetrics( SM_CXSCREEN );
	nScreenY = ::GetSystemMetrics( SM_CYSCREEN );
	nWndLeft = ( nScreenX - nWidth ) / 2;
	nWndTop = ( nScreenY - nHeight ) / 2;
	
	try {
		// create modal dialog
		dwExStyle = WS_EX_TOPMOST;
		dwStyle = WS_POPUP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
		hWndModalDlg = _createHWND( dwExStyle, dwStyle, nWndLeft, nWndTop, nWidth, nHeight, hWndParent, (HMENU)NULL, NULL, NULL );
		if ( !hWndModalDlg ) {
			assert( false );
			return 0;
		}

		// create user define UI.
		_createUI( hWndModalDlg, NULL );

		//
		::ShowWindow( hWndModalDlg, SW_NORMAL );

		// start exit timer, if need.
		if ( m_uFlag & YBMODALDLG_FLAG_AUTOEXIT ) {
			::SetTimer( hWndModalDlg, YBMODALDLG_TIMERID_AUTOEXIT, m_uExitElapse, NULL );
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

void YBModalDialog::endModal( int nCode ) {
	m_nRetCode = nCode;
	::PostMessage( m_hWnd, WM_QUIT, 0, 0 );
}

void YBModalDialog::_runModalLoop() {
    MSG msg;

    while ( ::GetMessage( &msg, NULL, 0, 0 ) ) {
        if ( !TranslateAccelerator( msg.hwnd, NULL, &msg ) ) {
            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
        }
    }

}

//void YBModalDialog::_runModalLoop() {
//    MSG msg;
//    bool bIdle = true;
//    HWND hWndParent;
//    LONG lIdleCount = 0;
//
//    hWndParent = ::GetParent( m_hWnd );
//    for (;;) {
//        //
//        while ( bIdle && !::PeekMessage( &msg, NULL, NULL, NULL, PM_NOREMOVE ) ) {
//            // send WM_ENTERIDLE to the parent
//            ::SendMessage(hWndParent, WM_ENTERIDLE, MSGF_DIALOGBOX, (LPARAM)m_hWnd);
//
//            //
//            ::SendMessage( m_hWnd, WM_KICKIDLE, MSGF_DIALOGBOX, lIdleCount++);
//
//            //
//            bIdle = false;
//        }
//
//        do {
//            // pump message, but quit on WM_QUIT
//            if ( !_pumpMessage() ) {
//                //m_nRetCode = 0;
//                //::PostQuitMessage( m_nRetCode );
//                return;
//            }
//
//            if ( _isIdleMessage( msg.message ) ) {
//                bIdle = true;
//                lIdleCount = 0;
//            }
//        } while ( ::PeekMessage( &msg, NULL, NULL, NULL, PM_NOREMOVE ) );
//    }
//}

bool YBModalDialog::_pumpMessage() {
    MSG msg;

    if ( !::GetMessage( &msg, NULL, NULL, NULL ) ) {
        return false;
    }

    // process this message
    if ( msg.message != WM_KICKIDLE )
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }
    return true;
}

bool YBModalDialog::_isIdleMessage( unsigned int uMessage ) {
    POINT ptCursorCur;

    if ( !::GetCursorPos( &ptCursorCur ) )
        return false;
    if ( uMessage == WM_MOUSEMOVE || uMessage == WM_NCMOUSEMOVE ) {
        if ( ( memcmp( &ptCursorCur, &m_ptCursorLast, sizeof( POINT ) ) == 0 ) && ( uMessage == m_uMessageLast ) ) {
            return false;
        }
        m_ptCursorLast = ptCursorCur;
        m_uMessageLast = uMessage;
        return true;
    }

    return ( uMessage != WM_PAINT ) && ( uMessage != 0x0118 );
}

YBCtrlImageStatic* YBModalDialog::_getImageStatic( unsigned int uImageId ) {
	YBCtrlImageBase* pYBImageBase = NULL;
	YBCtrlImageStatic* pImageStatic = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pYBImageBase ) )
		return NULL;

	if( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

void YBModalDialog::_drawBK( HDC hDC ) {
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

void YBModalDialog::_disableParentWnd( HWND hWndParent ) {
	if ( ::IsWindow( hWndParent ) 
	&& ( hWndParent != ::GetDesktopWindow() )
	&& ::IsWindowEnabled( hWndParent ) ) {
		::EnableWindow( hWndParent, FALSE );
		m_uFlag |= YBMODALDLG_FLAG_DISABLE_PARENT;
	}
}

void YBModalDialog::_enableParentWnd( HWND hWndParent ) {
	if ( ::IsWindow( hWndParent )
	&& !::IsWindowEnabled( hWndParent ) ) {
		if ( m_uFlag & YBMODALDLG_FLAG_DISABLE_PARENT ) {
			::EnableWindow( hWndParent, TRUE );
			::BringWindowToTop( hWndParent );
			m_uFlag &= ~YBMODALDLG_FLAG_DISABLE_PARENT;
		}
	}
}

HWND YBModalDialog::_getTopWnd( HWND hWndParent ) {
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

void YBModalDialog::setBKImageId( unsigned int uImageId, unsigned int uLFixed, unsigned int uTFixed, unsigned int uRFixed, unsigned int uBFixed ) {
	m_uImageIdBK = uImageId;
	m_uLFixed = uLFixed;
	m_uTFixed = uTFixed;
	m_uRFixed = uRFixed;
	m_uBFixed = uBFixed;
}