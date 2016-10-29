#include "stdafx.h"
#include "YBStepDlg.h"
#include "YBUpLoadStep2Dlg.h"
#include "frame/YBCtrlMemDC.h"
#include <assert.h>

#define YBUPLOAD_STEP1 ( _T("第二步") )
#define YBUPLOAD_STEP1_CONTENT ( _T(" 请选择文件上传去处：（系统默认：平台文件 ) ") );
#define YBCHECKBOX_DOCUMENT_ID (0x00000201)
#define YBCHECKBOX_DATABANK_ID (0x00000202)
#define YBCHECKBOX_MAILATTACHMENT_ID (0x00000203)

USING_NAMESPACE_YBCTRL

YBUpLoadStep2Dlg::YBUpLoadStep2Dlg(){

}

YBUpLoadStep2Dlg::~YBUpLoadStep2Dlg(){

}

HWND YBUpLoadStep2Dlg::create( int x, int y, int nWidth, int nHeight, HWND hWndParent ){
	HWND hWnd;

	hWnd = YBStepDlg::create( x, y, nWidth, nHeight, hWndParent, NULL );
	if( hWnd ){
		//m_checkBoxDocument.Create( 0, WS_CHILD|WS_VISIBLE, 370, 0, 470, nHeight, hWnd, (HMENU)NULL, NULL );
	}
	return hWnd;
}

YBCTRL_WNDMSGMAP_BEGIN( YBUpLoadStep2Dlg, YBStepDlg )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_WNDMSGMAP_END()

void YBUpLoadStep2Dlg::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
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