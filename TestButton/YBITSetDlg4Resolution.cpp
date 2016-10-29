#include "stdafx.h"
#include "YBITSetDlg4Resolution.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlFontDepository.h"

USING_NAMESPACE_YBCTRL

YBITSetDlg4Resolution::YBITSetDlg4Resolution() {
	_loadImage();
	_loadFont();
}

YBITSetDlg4Resolution::~YBITSetDlg4Resolution() {

}

YBCTRL_WNDMSGMAP_BEGIN( YBITSetDlg4Resolution, YBModalDialog )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_ONWNDMSG( WM_NCHITTEST, _onWM_NCHITTEST )
YBCTRL_WNDMSGMAP_END()

void YBITSetDlg4Resolution::_onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_NCHITTEST );

	pYBCtrlWndMsg->m_dwFlag = (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = HTCAPTION;
}

void YBITSetDlg4Resolution::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
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

unsigned int YBITSetDlg4Resolution::_getWndHeight() const { return 250; }

unsigned int YBITSetDlg4Resolution::_getWndWidth() const { return 350; }

void YBITSetDlg4Resolution::_createUI( HWND hWndModalDlg, HINSTANCE hInst ) {
	setBKImageId( m_uImageIdBk, 0, 0, 0, 0 );
	//
	m_radioBtn4Res.Create( 0, WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, 50, 100, 100, 20, hWndModalDlg, (HMENU)YBITDLG4RESOLUTION_RADIONBTN_ID, hInst );
	m_radioBtn4Res.setbeSelect( false );
	m_radioBtn4Res.setText( _T("1024¡Á768") );
	m_radioBtn4Res.setFontId( m_uFontIdRadio );
	m_radioBtn4Res.setEnable( true );
	m_radioBtn4Res.setRadioBtnLSpace( 0 );
	m_radioBtn4Res.setTextSpace( 5, 0, 0, 0 );
	m_radioBtn4Res.setImageIdBk( m_uImageIdRdBtnBk );
	m_radioBtn4Res.setImage( YBRadioBtn::ENORADIOBTN_STATUS_DISABLE, m_uImageIdNRdBtnDisable );
	m_radioBtn4Res.setImage( YBRadioBtn::ERADIOBTN_STATUS_DISABLE, m_uImageIdRdBtnDisable );
	m_radioBtn4Res.setImage( YBRadioBtn::ENORADIOBTN_STATUS_NORMAL, m_uImageIdNRdBtnNor );
	m_radioBtn4Res.setImage( YBRadioBtn::ENORADIOBTN_STATUS_HOVER, m_uImageIdNRdBtnHor );
	m_radioBtn4Res.setImage( YBRadioBtn::ENORADIOBTN_STATUS_PRESS, m_uImageIdNRdBtnPre );
	m_radioBtn4Res.setImage( YBRadioBtn::ERADIOBTN_STATUS_NORMAL, m_uImageIdRdBtnNor );
	m_radioBtn4Res.setImage( YBRadioBtn::ERADIOBTN_STATUS_HOVER, m_uImageIdRdBtnHor );
	m_radioBtn4Res.setImage( YBRadioBtn::ERADIOBTN_STATUS_PRESS, m_uImageIdRdBtnPre );
	// button
	m_btn4Res.Create( 0, WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 245, 190, 70, 33, hWndModalDlg, (HMENU)YBITDLG4RESOLUTION_OKBUTTON_ID, hInst );
	m_btn4Res.setText( _T("È·  ¶¨") );
	m_btn4Res.setFontId( m_uFontIdBtn );
	m_btn4Res.setTextSpace( 4, 4, 4, 0 );
	m_btn4Res.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnNor );
	m_btn4Res.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnHor );
	m_btn4Res.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnPre );
}

void YBITSetDlg4Resolution::_loadImage() {
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
	// Radio button
	tstrImageFilePath = tstrImageDirPath + _T("YBMenuItemDisable.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdRdBtnBk = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlg4ResRdNor.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdRdBtnNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlg4ResRdHor.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdRdBtnHor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlg4ResRdPre.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdRdBtnPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlg4ResRdPre.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdRdBtnDisable = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlg4ResNRdNor.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdNRdBtnDisable = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlg4ResNRdNor.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdNRdBtnNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlg4ResNRdHor.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdNRdBtnHor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlg4ResNRdPre.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdNRdBtnPre = idImage;
}

void YBITSetDlg4Resolution::_loadFont() {
	LOGFONT lFont;

	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 23;
	m_uFontIdBtn = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );
	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 20;
	m_uFontIdRadio = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 6, 5, 6 ) );
}

bool YBITSetDlg4Resolution::isSelected() {
	return m_radioBtn4Res.isSelect();
}