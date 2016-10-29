#include "stdafx.h"
#include "YBWebEditDlg.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlFontDepository.h"

USING_NAMESPACE_YBCTRL

YBWebEditDlg::YBWebEditDlg()
: m_tstrTitle( _T("欢迎使用网站编辑功能") )
, m_bEnable( true )
, m_bSelect( false )
{
	_loadImage();
	_loadFont();
}

YBWebEditDlg::~YBWebEditDlg() {

}

YBCTRL_WNDMSGMAP_BEGIN( YBWebEditDlg, YBModalDialog )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_ONWNDMSG( WM_NCHITTEST, _onWM_NCHITTEST )
	YBCTRL_ONWNDMSG( WM_KEYDOWN, _onWM_KEYDOWN )
YBCTRL_WNDMSGMAP_END()

void YBWebEditDlg::_onWM_KEYDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_KEYDOWN );
	if ( VK_ESCAPE == pYBCtrlWndMsg->m_wParam ) {
		endModal( ::GetDlgCtrlID( m_btnClose.getHWND() ) );
	} else if ( VK_RETURN == pYBCtrlWndMsg->m_wParam )
	{
		endModal( ::GetDlgCtrlID( m_btnOk.getHWND() ) );
	}
}

void YBWebEditDlg::_onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_NCHITTEST );

	pYBCtrlWndMsg->m_dwFlag = (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = HTCAPTION;
}

void YBWebEditDlg::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );
	switch( wCtrlId ) {
	case YBWEBEDITID_BTN_OK:
		{
			endModal( ::GetDlgCtrlID( m_btnOk.getHWND() ) );
		}
		break;
	case YBWEBEDITID_BTN_CANCEL:
		{
			endModal( ::GetDlgCtrlID( m_btnCancel.getHWND() ) );
		}
		break;
	case YBWEBEDITID_RADIOBTN_BODYTEXT:
		{
			m_rb4FullText.InitialState();
		}
		break;
	case YBWEBEDITID_RADIOBTN_FULLTEXT:
		{
			m_rb4BodyText.InitialState();
		}
		break;
	case YBWEBEDITID_BTN_CLOSE:
		{
			endModal( ::GetDlgCtrlID( m_btnClose.getHWND() ) );
		}
		break;
	default:
		break;
	}
}

void YBWebEditDlg::setbSelect( bool bSelect ) {
	if ( bSelect ) {
		m_bSelect = true;
	} else {
		m_bSelect = false;
	}
}

void YBWebEditDlg::setEnable( bool bEnable ) {
	if ( bEnable ) {
		m_bEnable = true;
	} else {
		m_bEnable = false;
	}
}

int YBWebEditDlg::getWEType() {
	if ( m_rb4BodyText.isSelect() ) {
		return 1; // 正文获取
	}
	if ( m_rb4FullText.isSelect() ) {
		return 2; // 全文获取
	}
	return 0; // 
}

unsigned int YBWebEditDlg::_getWndHeight() const { return 191; }

unsigned int YBWebEditDlg::_getWndWidth() const { return 287; }

void YBWebEditDlg::_clipWnd() {
	HRGN hRgn;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	hRgn = ::CreateRoundRectRgn( 0, 0, rcClient.right + 1, rcClient.bottom + 1, 5, 5 );

	::SetWindowRgn( m_hWnd, hRgn, TRUE );
	::DeleteObject( hRgn );
}

void YBWebEditDlg::_createUI( HWND hWndModalDlg, HINSTANCE hInst ) {
	_clipWnd();

	setBKImageId( m_uImageIdWEDBk, 0, 0, 0, 0 );
	// button
	m_btnClose.Create( 0, WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 255, 10, 25, 25, hWndModalDlg, (HMENU)YBWEBEDITID_BTN_CLOSE, hInst );
	m_btnClose.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdWEDCBtnNor );
	m_btnClose.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdWEDCBtnHor );
	m_btnClose.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdWEDCBtnPre );
	//
	m_btnOk.Create( 0, WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 50, 140, 78, 35, hWndModalDlg, (HMENU)YBWEBEDITID_BTN_OK, hInst );
	m_btnOk.setText( _T("立即获取") );
	m_btnOk.setFontId( m_uFontIdBtn );
	m_btnOk.setTextSpace( 4, 4, 4, 0 );
	m_btnOk.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdWEDBtnNor );
	m_btnOk.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdWEDBtnHor );
	m_btnOk.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdWEDBtnPre );
	// button
	m_btnCancel.Create( 0, WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 170, 140, 78, 35, hWndModalDlg, (HMENU)YBWEBEDITID_BTN_CANCEL, hInst );
	m_btnCancel.setText( _T("放弃获取") );
	m_btnCancel.setFontId( m_uFontIdBtn );
	m_btnCancel.setTextSpace( 4, 4, 4, 0 );
	m_btnCancel.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdWEDBtnNor );
	m_btnCancel.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdWEDBtnHor );
	m_btnCancel.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdWEDBtnPre );
	// radio btn
	m_rb4BodyText.Create( 0, WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, 50, 85, 100, 25, hWndModalDlg, (HMENU)YBWEBEDITID_RADIOBTN_BODYTEXT, hInst );
	m_rb4BodyText.setbeSelect( m_bSelect );
	m_rb4BodyText.setText( _T("正文获取") );
	m_rb4BodyText.setFontId( m_uFontIdRadio );
	m_rb4BodyText.setEnable( m_bEnable );
	m_rb4BodyText.setRadioBtnLSpace( 0 );
	m_rb4BodyText.setTextSpace( 0, 1, 0, 0 );
	m_rb4BodyText.setImageIdBk( m_uImageIdWEDRBBk );
	m_rb4BodyText.setImage( YBRadioBtn::ENORADIOBTN_STATUS_DISABLE, m_uImageIdWEDNRBDisable );
	m_rb4BodyText.setImage( YBRadioBtn::ERADIOBTN_STATUS_DISABLE, m_uImageIdWEDRBDisable );
	m_rb4BodyText.setImage( YBRadioBtn::ENORADIOBTN_STATUS_NORMAL, m_uImageIdWEDNRBNor );
	m_rb4BodyText.setImage( YBRadioBtn::ENORADIOBTN_STATUS_HOVER, m_uImageIdWEDNRBHor );
	m_rb4BodyText.setImage( YBRadioBtn::ENORADIOBTN_STATUS_PRESS, m_uImageIdWEDNRBPre );
	m_rb4BodyText.setImage( YBRadioBtn::ERADIOBTN_STATUS_NORMAL, m_uImageIdWEDRBNor );
	m_rb4BodyText.setImage( YBRadioBtn::ERADIOBTN_STATUS_HOVER, m_uImageIdWEDRBHor );
	m_rb4BodyText.setImage( YBRadioBtn::ERADIOBTN_STATUS_PRESS, m_uImageIdWEDRBPre );
	// radio btn
	m_rb4FullText.Create( 0, WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, 167, 85, 100, 25, hWndModalDlg, (HMENU)YBWEBEDITID_RADIOBTN_FULLTEXT, hInst );
	m_rb4FullText.setbeSelect( m_bSelect );
	m_rb4FullText.setText( _T("全文获取") );
	m_rb4FullText.setFontId( m_uFontIdRadio );
	m_rb4FullText.setEnable( m_bEnable );
	m_rb4FullText.setRadioBtnLSpace( 0 );
	m_rb4FullText.setTextSpace( 0, 1, 0, 0 );
	m_rb4FullText.setImageIdBk( m_uImageIdWEDRBBk );
	m_rb4FullText.setImage( YBRadioBtn::ENORADIOBTN_STATUS_DISABLE, m_uImageIdWEDNRBDisable );
	m_rb4FullText.setImage( YBRadioBtn::ERADIOBTN_STATUS_DISABLE, m_uImageIdWEDRBDisable );
	m_rb4FullText.setImage( YBRadioBtn::ENORADIOBTN_STATUS_NORMAL, m_uImageIdWEDNRBNor );
	m_rb4FullText.setImage( YBRadioBtn::ENORADIOBTN_STATUS_HOVER, m_uImageIdWEDNRBHor );
	m_rb4FullText.setImage( YBRadioBtn::ENORADIOBTN_STATUS_PRESS, m_uImageIdWEDNRBPre );
	m_rb4FullText.setImage( YBRadioBtn::ERADIOBTN_STATUS_NORMAL, m_uImageIdWEDRBNor );
	m_rb4FullText.setImage( YBRadioBtn::ERADIOBTN_STATUS_HOVER, m_uImageIdWEDRBHor );
	m_rb4FullText.setImage( YBRadioBtn::ERADIOBTN_STATUS_PRESS, m_uImageIdWEDRBPre );
}

void YBWebEditDlg::_drawSelf( HDC hDC ) {
	//_drawTitle( hDC );
}

void YBWebEditDlg::_drawTitle( HDC hDC ) {
	RECT rcDrawText;
	int iBkModeOld;
	COLORREF clrTxtOld;
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	// draw text
	if ( _getTitleRect( rcDrawText ) 
		&& ( rcDrawText.right - rcDrawText.left )
		&& ( rcDrawText.bottom - rcDrawText.top )
		&& ( m_tstrTitle.length() > 0 ) ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdTitle );
			if ( pYBCtrlFont ) {
				hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
				iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
				clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
				::DrawText( hDC, m_tstrTitle.c_str(), m_tstrTitle.length(), &rcDrawText, DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS|DT_SINGLELINE );
				::SetTextColor( hDC, clrTxtOld );
				::SetBkMode( hDC, iBkModeOld );
				::SelectObject( hDC, hFontOld );
			}
	}
}

bool YBWebEditDlg::_getTitleRect( RECT& rcTitle ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcTitle.left = rcClient.left;
	rcTitle.top = rcClient.top + 10;
	rcTitle.bottom = rcTitle.top + 25;
	rcTitle.right = rcClient.right;
	return ( ( rcTitle.right > rcTitle.left ) && ( rcTitle.bottom > rcTitle.top ) );
}

void YBWebEditDlg::_loadImage() {
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
	tstrImageFilePath = tstrImageDirPath + _T("YBWEDlgBk.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdWEDBk = idImage;
	// button
	tstrImageFilePath = tstrImageDirPath + _T("YBWEDBtnNor.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdWEDBtnNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWEDBtnHor.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdWEDBtnHor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWEDBtnPre.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdWEDBtnPre = idImage;
	// Radio btn
	tstrImageFilePath = tstrImageDirPath + _T("YBWEDRBNor.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdWEDRBNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWEDRBHov.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdWEDRBHor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWEDRBPre.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdWEDRBPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWEDRBDis.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdWEDRBDisable = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWEDNRBDis.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdWEDNRBDisable = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWEDNRBNor.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdWEDNRBNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWEDNRBHov.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdWEDNRBHor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWEDNRBPre.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdWEDNRBPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWEDRBBk.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdWEDRBBk = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWEDCBtnNor.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdWEDCBtnNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWEDCBtnHor.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdWEDCBtnHor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWEDCBtnPre.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdWEDCBtnPre = idImage;
}

void YBWebEditDlg::_loadFont() {
	LOGFONT lFont;

	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 22;
	m_uFontIdBtn = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );
	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 21;
	m_uFontIdRadio = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 0, 0, 10 ) );
	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 21;
	m_uFontIdTitle = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 7, 74, 149 ) );
}
