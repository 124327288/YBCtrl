#include "stdafx.h"
#include "frame/YBCtrlDefs.h"
#include "YBButton4CNDlg.h"

USING_NAMESPACE_YBCTRL

YBButton4CNDlg::YBButton4CNDlg()
: m_wordCtrlID( 0 )
{

}

YBButton4CNDlg::~YBButton4CNDlg() {

}

HWND YBButton4CNDlg::create( int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance ) {
	HWND hWnd;
	DWORD dwExStyle = 0;
	DWORD dwStyle = WS_CHILD;

	hWnd = Create( dwExStyle, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance );

	m_wordCtrlID = (unsigned int)hMenu;

	if ( !hWnd ) {
		assert(false);
	}
	return hWnd;
}

YBCTRL_WNDMSGMAP_BEGIN( YBButton4CNDlg, YBButton )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
YBCTRL_WNDMSGMAP_END()

void YBButton4CNDlg::_onWM_MOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	WPARAM wParam;
	LPARAM lParam;

	wParam = MAKELONG( m_wordCtrlID, YBBTN_CMDEVT_BTNMOVE );
	lParam = (LPARAM)m_hWnd;
	::SendMessage( ::GetParent( m_hWnd ), YBWM_MOUSEMOVE_BTN_CNDLG, wParam, lParam );
}