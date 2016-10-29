#include "stdafx.h"
#include "YBStepDlg.h"
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlFontDepository.h"
#include "frame/YBCtrlDefs.h"
#include <assert.h>

#define YBUPLOAD_STEP1DLG_WIDTH (60)
USING_NAMESPACE_YBCTRL

YBStepDlg::YBStepDlg()
: m_hBrushStep( NULL )
, m_uFontIdStep( YBCTRL_FONT_ID_INVALID )
, m_uFontIdContent( YBCTRL_FONT_ID_INVALID )
{
	m_hBrushStep = ::CreateSolidBrush( RGB( 1, 117, 192 ) );
	m_hBrushContent = ::CreateSolidBrush( RGB( 197, 223, 248 ) );
}

YBStepDlg::~YBStepDlg(){
	if( m_hBrushStep ){
		::DeleteObject( m_hBrushStep );
		m_hBrushStep = NULL;
	}
	if( m_hBrushContent ){
		::DeleteObject( m_hBrushContent );
		m_hBrushContent = NULL;
	}
}

YBCTRL_WNDMSGMAP_BEGIN( YBStepDlg, YBCtrlBase )
	//YBCTRL_ONWNDMSG()
YBCTRL_WNDMSGMAP_END()

HWND YBStepDlg::create( int x, int y, int nWidth, int nHeight, HWND hWndParent, HINSTANCE hInstance ){
	HWND hWnd;

	hWnd = _createHWND( 0, WS_CHILD|WS_VISIBLE, x, y, nWidth, nHeight, hWndParent, NULL, hInstance, NULL );
	if( !hWnd ){
		assert(false);
	}
	_init();
	return hWnd;
}

void YBStepDlg::drawTextStep( HDC hDC ){
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	HFONT hFontOld = NULL;
	int iBkModeOld;

	if( ( m_rcTextStep.right - m_rcTextStep.left )
		&& ( m_rcTextStep.bottom - m_rcTextStep.top )
		&& ( m_tstrTextStep.length() > 0 ) ){
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdStep );
			if( pYBCtrlFont ){
				hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
				iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
				clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

				::DrawText( hDC, m_tstrTextStep.c_str(), m_tstrTextStep.length(), &m_rcTextStep, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
				::SetTextColor( hDC, clrTxtOld );
				::SetBkMode( hDC, iBkModeOld );
				::SelectObject( hDC, hFontOld );
			}
	}
}

void YBStepDlg::drawTextContent( HDC hDC ){
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	HFONT hFontOld = NULL;
	int iBkModeOld;

	if( ( m_rcTextContent.right - m_rcTextContent.left )
		&& ( m_rcTextContent.bottom - m_rcTextContent.top )
		&& ( m_tstrTextContent.length() > 0 ) ){
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdContent );
			if( pYBCtrlFont ){
				hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
				iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
				clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

				::DrawText( hDC, m_tstrTextContent.c_str(), m_tstrTextContent.length(), &m_rcTextContent, DT_LEFT|DT_VCENTER|DT_SINGLELINE );
				::SetTextColor( hDC, clrTxtOld );
				::SetBkMode( hDC, iBkModeOld );
				::SelectObject( hDC, hFontOld );
			}
	}
}

void YBStepDlg::_init(){
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	m_rcTextStep.left = 0;
	m_rcTextStep.right = YBUPLOAD_STEP1DLG_WIDTH;
	m_rcTextStep.top = 0;
	m_rcTextStep.bottom = rcClient.bottom - rcClient.top;

	m_rcTextContent.left = m_rcTextStep.right;
	m_rcTextContent.top = 0;
	m_rcTextContent.right = rcClient.right - rcClient.left;
	m_rcTextContent.bottom = m_rcTextStep.bottom;

	LOGFONT lFont;

	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 22;
	m_uFontIdStep = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );

	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 22;
	m_uFontIdContent = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 0, 51, 111 ));
}