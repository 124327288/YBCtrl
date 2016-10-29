#include "stdafx.h"
#include "YBITSetDlg4ScreenSaver.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlFontDepository.h"

	USING_NAMESPACE_YBCTRL

YBITSetDlg4ScreenSaver::YBITSetDlg4ScreenSaver() {
	_loadImage();
	_loadFont();
}

YBITSetDlg4ScreenSaver::~YBITSetDlg4ScreenSaver() {

}

YBCTRL_WNDMSGMAP_BEGIN( YBITSetDlg4ScreenSaver, YBModalDialog )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_ONWNDMSG( WM_NCHITTEST, _onWM_NCHITTEST )
	YBCTRL_WNDMSGMAP_END()

void YBITSetDlg4ScreenSaver::_onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_NCHITTEST );

	pYBCtrlWndMsg->m_dwFlag = (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = HTCAPTION;
}

void YBITSetDlg4ScreenSaver::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );
	switch( wCtrlId ) {
	case YBITDLG4RESOLUTION_OKBUTTON_ID:
		{
			endModal( ::GetDlgCtrlID( m_btn4Res.getHWND() ) );
		}
		break;
	default:
		break;
	}
}

unsigned int YBITSetDlg4ScreenSaver::_getWndHeight() const { return 250; }

unsigned int YBITSetDlg4ScreenSaver::_getWndWidth() const { return 350; }

void YBITSetDlg4ScreenSaver::_createUI( HWND hWndModalDlg, HINSTANCE hInst ) {
	setBKImageId( m_uImageIdBk, 0, 0, 0, 0 );
	// button
	m_btn4Res.Create( 0, WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 245, 190, 70, 33, hWndModalDlg, (HMENU)YBITDLG4RESOLUTION_OKBUTTON_ID, hInst );
	m_btn4Res.setText( _T("È·  ¶¨") );
	m_btn4Res.setFontId( m_uFontIdBtn );
	m_btn4Res.setTextSpace( 4, 4, 4, 0 );
	m_btn4Res.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnNor );
	m_btn4Res.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnHor );
	m_btn4Res.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnPre );
}

void YBITSetDlg4ScreenSaver::_loadImage() {
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
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlg4ResolutionBk.jpg");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdBk = idImage;
	// button
	tstrImageFilePath = tstrImageDirPath + _T("YBBtnNormal.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdBtnNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBBtnHover.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdBtnHor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBBtnPress.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdBtnPre = idImage;
}

void YBITSetDlg4ScreenSaver::_loadFont() {
	LOGFONT lFont;

	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 23;
	m_uFontIdBtn = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );
}
