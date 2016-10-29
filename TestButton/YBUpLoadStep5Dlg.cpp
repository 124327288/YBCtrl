#include "stdafx.h"
#include "YBStepDlg.h"
#include "YBUpLoadStep5Dlg.h"
#include "frame/YBCtrlMemDC.h"
#include <assert.h>

#define YBUPLOAD_STEP1 ( _T("第五步") )
#define YBUPLOAD_STEP1_CONTENT ( _T(" 请选择原文件是否保留：") );

USING_NAMESPACE_YBCTRL

YBUpLoadStep5Dlg::YBUpLoadStep5Dlg(){

}

YBUpLoadStep5Dlg::~YBUpLoadStep5Dlg(){

}

HWND YBUpLoadStep5Dlg::create( int x, int y, int nWidth, int nHeight, HWND hWndParent ){
	HWND hWnd;

	hWnd = YBStepDlg::create( x, y, nWidth, nHeight, hWndParent, NULL );
	if( !hWnd ){
		assert(false);
	}
	return hWnd;
}

YBCTRL_WNDMSGMAP_BEGIN( YBUpLoadStep5Dlg, YBStepDlg )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
YBCTRL_WNDMSGMAP_END()

void YBUpLoadStep5Dlg::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	RECT rcClient;
	HDC hPaintDC;
	PAINTSTRUCT ps;

	::GetClientRect( m_hWnd, &rcClient );

	YBStepDlg::m_tstrTextStep = YBUPLOAD_STEP1;
	YBStepDlg::m_tstrTextContent = YBUPLOAD_STEP1_CONTENT;
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	::FillRect( memDC.getMemDC(), &m_rcTextStep, m_hBrushStep );
	::FillRect( memDC.getMemDC(), &m_rcTextContent, m_hBrushContent );
	drawTextStep( memDC.getMemDC() );
	drawTextContent( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}