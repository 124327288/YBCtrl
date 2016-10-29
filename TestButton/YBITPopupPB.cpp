#include "stdafx.h"
#include "YBITPopupPB.h"
#include "frame/YBCtrlMemDC.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlFontDepository.h"
#include <assert.h>

USING_NAMESPACE_YBCTRL

YBITPopupPB::YBITPopupPB(){
	_LoadFont();
	_LoadImage();
	_init();
}

YBITPopupPB::~YBITPopupPB(){

}

YBCTRL_WNDMSGMAP_BEGIN( YBITPopupPB, YBProgressBar )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
YBCTRL_WNDMSGMAP_END()


void YBITPopupPB::_onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	RECT rcClient;
	HDC hDC;
	
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
}

void YBITPopupPB::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	HDC hPaintDC;
	RECT rcClient;
	PAINTSTRUCT ps;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	YBProgressBar::_drawStatus( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void YBITPopupPB::_init() {
	SetProgressBarSpaced( 4, 1, 4, 1 );
	setTextAttri( m_uFontIdTxt, 2, 2 );
	setRange( 0, 100 );
	setPos( 20 );
	setBitBltFTMode( true );
}

HWND YBITPopupPB::create( int x, int y, int nWidth, int nHeight, HINSTANCE hInstance ){
	HWND hWnd;
	DWORD dwExStyle;
	DWORD dwStyle;

	dwExStyle = WS_EX_TOOLWINDOW|WS_EX_TOPMOST;
	dwStyle = WS_VISIBLE|WS_POPUP;

	hWnd = YBProgressBar::Create( dwExStyle, dwStyle, x, y, nWidth, nHeight, NULL, (HMENU)NULL, hInstance );
	if( !hWnd ){
		assert(false);
	}
	return hWnd;
}

void YBITPopupPB::_LoadImage(){
	TCHAR szModulePath[MAX_PATH];
	TCHAR* posTmp = NULL;
	GetModuleFileName( NULL, szModulePath, MAX_PATH );
	std::basic_string< TCHAR > tstrImageDirPath;
	std::basic_string< TCHAR > tstrImageFilePath;
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
	tstrImageFilePath = tstrImageDirPath + _T("YBUpLoadWTDlgPBFT.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdFT = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBUpLoadWTDlgPBBk.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBK = idImage;
}

void YBITPopupPB::_LoadFont(){
	
	LOGFONT lFont;

	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 22;
	m_uFontIdTxt = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 200, 200, 200 ) );
}