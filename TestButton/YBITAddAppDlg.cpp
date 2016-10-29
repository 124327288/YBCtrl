#include "stdafx.h"
#include "YBITAddAppDlg.h"
#include "frame/YBCtrlDefs.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlFontDepository.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlMisc.h"
#include <CommDlg.h>
#include <WindowsX.h>

#define YBITAADLGID_BTN_BSSELECT (0x00000359)
#define YBITAADLGID_BTN_CSSELECT (0x00000360)
#define YBITAADLGID_BTN_CONFIRM (0x00000361)
#define YBITAADLGID_BTN_CANCEL (0x00000362)
#define YBITAADLGID_RADIO_BSSELECT (0x00000363)
#define YBITAADLGID_RADIO_CSSELECT (0x00000364)
#define YBITAADLGID_BTN_CSADD (0x00000365)
#define YBITAADLGID_EDIT_APPTITLE (0x00000366)
#define YBITAADLGID_EDIT_BS (0x00000367)
#define YBITAADLGID_EDIT_CS (0x00000368)
#define COLOR_EDIT_INPUT (RGB(0, 45, 92 ))

USING_NAMESPACE_YBCTRL

YBITAddAppDlg::YBITAddAppDlg()
: m_uImageIdBk( YBCTRL_IMAGE_ID_INVALID )
, m_uFontIdSBtn( YBCTRL_FONT_ID_INVALID )
, m_uFontIdBBtn( YBCTRL_FONT_ID_INVALID )
, m_uFontIdRadio( YBCTRL_FONT_ID_INVALID )
, m_uImageIdBtnNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnPre( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdRBBk( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdRBNormal( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdNRBNormal( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnNor2( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnHov2( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnPre2( YBCTRL_IMAGE_ID_INVALID )
, m_tstrLocalPath( _T("") )
, m_tstrFileName( _T("") )
, m_tstrDefExt( _T("") )
, m_tstrWebSiteUrl( _T("") )
, m_tstrAppTitle( _T("") )
, m_eAppType( EAPP_TYPE_WEBSITE )
, m_hEdit4CS( NULL )
, m_hEdit4BS( NULL )
, m_hEditAppTitle( NULL )
, m_uFontIdEdit( YBCTRL_FONT_ID_INVALID )
, m_iFirstWrite( 0 )
{
	_loadFont();
	_loadImage();
	m_hBrushEdit = ::CreateSolidBrush( RGB( 255, 255, 255 ) );
	m_hPenEditFrame = ::CreatePen( PS_SOLID, 1, RGB( 36, 134, 204 ) );
}

YBITAddAppDlg::~YBITAddAppDlg() {
	if ( m_hBrushEdit ) {
		::DeleteObject( m_hBrushEdit );
		m_hBrushEdit = NULL;
	}
	if ( m_hPenEditFrame ) {
		::DeleteObject( m_hPenEditFrame );
		m_hPenEditFrame = NULL;
	}
}

YBCTRL_WNDMSGMAP_BEGIN( YBITAddAppDlg, YBModalDialog )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_ONWNDMSG( WM_CTLCOLOREDIT, _onWM_CTLCOLOREDIT )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
YBCTRL_WNDMSGMAP_END()
//

void YBITAddAppDlg::_onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
	POINT ptMouse;
	RECT rcAppTitle;

	ptMouse.x = GET_X_LPARAM(pYBCtrlWndMsg->m_lParam);
	ptMouse.y = GET_Y_LPARAM(pYBCtrlWndMsg->m_lParam);

	::GetClientRect( m_hEditAppTitle, &rcAppTitle );
	if ( ::PtInRect( &rcAppTitle, ptMouse ) ) {
		m_iFirstWrite = 1;
		::SetWindowText( m_hEditAppTitle, _T("") );
	}

}

void YBITAddAppDlg::_onWM_CTLCOLOREDIT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_CTLCOLOREDIT );
	HWND hEditCtrl = ( HWND )pYBCtrlWndMsg->m_lParam;
	HDC hEditDC = ( HDC )pYBCtrlWndMsg->m_wParam;
	COLORREF clrTxt = ybctrl::YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdEdit)->m_clrTxt;

	if ( hEditCtrl == m_hEdit4BS ) {
		::SetTextColor( hEditDC, COLOR_EDIT_INPUT );
		::SetBkColor( hEditDC, RGB(202, 223, 242) );
	}
	if ( hEditCtrl == m_hEdit4CS  ) {
		::SetTextColor( hEditDC, COLOR_EDIT_INPUT );
		::SetBkColor( hEditDC, RGB(202, 223, 242) );
	}
	if ( hEditCtrl == m_hEditAppTitle ) {
		::SetTextColor( hEditDC, COLOR_EDIT_INPUT );
		::SetBkColor( hEditDC, RGB( 202, 223, 242 ) );
	}
	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = (LRESULT)m_hBrushEdit;

}

//
void YBITAddAppDlg::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );
	switch( wCtrlId ) {
	case YBITAADLGID_RADIO_CSSELECT:
		{
			m_eAppType = EAPP_TYPE_LOCALAPP;
			m_radioBtn4BS.InitialState();
			::SetWindowPos( m_hEdit4BS, HWND_BOTTOM, 0, 0, 0, 0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE );
			::SetWindowPos( m_hEdit4CS, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE );
			::ShowWindow( m_btn4CSAdd.getHWND(), SW_NORMAL );
		}
		break;
	case YBITAADLGID_RADIO_BSSELECT:
		{
			m_eAppType = EAPP_TYPE_WEBSITE;
			m_radioBtn4CS.InitialState();
			::SetWindowPos( m_hEdit4CS, HWND_BOTTOM, 0, 0, 0, 0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE );
			::SetWindowPos( m_hEdit4BS, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE );
			::ShowWindow( m_btn4CSAdd.getHWND(), SW_HIDE );
		}
		break;
	case YBITAADLGID_BTN_CONFIRM:
		{
			endModal( ::GetDlgCtrlID( m_btn4Confirm.getHWND() ) );
		}
		break;
	case YBITAADLGID_BTN_CANCEL:
		{
			endModal( ::GetDlgCtrlID( m_btn4Cancel.getHWND() ) );
		}
		break;
	case YBITAADLGID_BTN_CSADD:
		{	
			if ( !_getFileInfo() ){
				//assert( false );
				return;
			}
			::OutputDebugString( m_tstrFileName.c_str() );
			::OutputDebugString( m_tstrLocalPath.c_str() );
			//endModal( ::GetDlgCtrlID( m_btn4CSAdd.getHWND() ) );
		}
		break;
	case YBITAADLGID_BTN_BSSELECT:
		{
			m_eAppType = EAPP_TYPE_WEBSITE;
			m_radioBtn4BS.setbeSelect( true );
			m_radioBtn4CS.InitialState();
			::SetWindowPos( m_hEdit4CS, HWND_BOTTOM, 0, 0, 0, 0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE );
			::SetWindowPos( m_hEdit4BS, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE );
			::ShowWindow( m_btn4CSAdd.getHWND(), SW_HIDE );
		}
		break;
	case YBITAADLGID_BTN_CSSELECT:
		{
			m_eAppType = EAPP_TYPE_LOCALAPP;
			m_radioBtn4CS.setbeSelect( true );
			m_radioBtn4BS.InitialState();
			::SetWindowPos( m_hEdit4BS, HWND_BOTTOM, 0, 0, 0, 0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE );
			::SetWindowPos( m_hEdit4CS, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE );
			::ShowWindow( m_btn4CSAdd.getHWND(), SW_NORMAL );
		}
		break;
	default:
		break;
	}
}

bool YBITAddAppDlg::_getFileInfo() {
	OPENFILENAME openfilename;

	ZeroMemory( &openfilename, sizeof(OPENFILENAME) );
	LPTSTR szFile = new TCHAR[MAX_PATH* 101 + 1 ];
	szFile[0] = 0x0;
	openfilename.lStructSize = sizeof(OPENFILENAME);
	openfilename.hwndOwner = m_hWnd;
	openfilename.hInstance = NULL;
	openfilename.lpstrFilter = _T("应用程序(*.exe)\0*.exe");
	//openfilename.lpstrCustomFilter = NULL;
	openfilename.nFilterIndex = 1;
	openfilename.lpstrFile = szFile;
	openfilename.nMaxFile = MAX_PATH*101 + 1;
	openfilename.lpstrFileTitle = NULL;
	//openfilename.nMaxFileTitle = 0;
	openfilename.lpstrInitialDir = NULL;
	openfilename.lpstrTitle = _T("添加");
	//openfilename.nFileOffset = 0;
	//openfilename.nFileExtension = 0;
	openfilename.lpstrDefExt = _T("*.exe");
	//openfilename.lCustData = 0;
	openfilename.Flags = OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
	if ( ::GetOpenFileName(&openfilename) ) {
		m_tstrLocalPath = openfilename.lpstrFile;
		TCHAR szExtName[ _MAX_EXT + 1 ];
		TCHAR szFileName[ _MAX_FNAME + 1 ];
		_tsplitpath_s( m_tstrLocalPath.c_str(), NULL, 0, NULL, 0, szFileName, _MAX_EXT, szExtName, _MAX_EXT );
		m_tstrFileName = szFileName;
		m_tstrDefExt = szExtName;
		::SetWindowText( m_hEditAppTitle, m_tstrFileName.c_str() );
		return true;
	}

	return false;
}

unsigned int YBITAddAppDlg::_getWndHeight() const { return 406; }

unsigned int YBITAddAppDlg::_getWndWidth() const { return 736; }

void YBITAddAppDlg::_createUI( HWND hWndModalDlg, HINSTANCE hInst ) {
	setBKImageId( m_uImageIdBk, 2, 2, 2, 2 );
	//
	m_btn4BS.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnNor );
	m_btn4BS.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnHov );
	m_btn4BS.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnPre );
	m_btn4BS.setFontId( m_uFontIdSBtn );
	m_btn4BS.setTextSpace( 2, 1, 2, 2 );
	m_btn4BS.setText(_T("点击选择"));
	m_btn4BS.Create( 0, WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, 265, 253, 63, 25, hWndModalDlg, (HMENU)YBITAADLGID_BTN_BSSELECT, hInst );
	//
	m_btn4CS.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnNor );
	m_btn4CS.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnHov );
	m_btn4CS.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnPre );
	m_btn4CS.setFontId( m_uFontIdSBtn );
	m_btn4CS.setTextSpace( 2, 1, 2, 2 );
	m_btn4CS.setText(_T("点击选择"));
	m_btn4CS.Create( 0, WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, 614, 253, 63, 25, hWndModalDlg, (HMENU)YBITAADLGID_BTN_CSSELECT, hInst );
	//
	m_btn4CSAdd.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnNor );
	m_btn4CSAdd.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnHov );
	m_btn4CSAdd.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnPre );
	m_btn4CSAdd.setFontId( m_uFontIdSBtn );
	m_btn4CSAdd.setTextSpace( 2, 1, 2, 2 );
	m_btn4CSAdd.setText(_T("立即添加"));
	m_btn4CSAdd.Create( 0, WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, 614, 290, 63, 25, hWndModalDlg, (HMENU)YBITAADLGID_BTN_CSADD, hInst );
	//
	m_btn4Confirm.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnNor2 );
	m_btn4Confirm.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnHov2 );
	m_btn4Confirm.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnPre2 );
	m_btn4Confirm.setFontId( m_uFontIdBBtn );
	m_btn4Confirm.setTextSpace( 2, 7, 2, 0 );
	m_btn4Confirm.setText(_T("确认添加"));
	m_btn4Confirm.Create( 0, WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, 247, 350, 98, 42, hWndModalDlg, (HMENU)YBITAADLGID_BTN_CONFIRM, hInst );
	//
	m_btn4Cancel.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnNor2 );
	m_btn4Cancel.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnHov2 );
	m_btn4Cancel.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnPre2 );
	m_btn4Cancel.setFontId( m_uFontIdBBtn );
	m_btn4Cancel.setTextSpace( 2, 7, 2, 0 );
	m_btn4Cancel.setText(_T("放弃添加"));
	m_btn4Cancel.Create( 0, WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, 388, 350, 98, 42, hWndModalDlg, (HMENU)YBITAADLGID_BTN_CANCEL, hInst );
	// radio btn
	m_radioBtn4BS.Create( 0, WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, 100, 253, 200, 20, hWndModalDlg, (HMENU)YBITAADLGID_RADIO_BSSELECT, hInst );
	m_radioBtn4BS.setbeSelect( true );
	m_radioBtn4BS.setText( _T("B/S网站式办公系统") );
	m_radioBtn4BS.setFontId( m_uFontIdRadio );
	m_radioBtn4BS.setEnable( true );
	m_radioBtn4BS.setRadioBtnLSpace( 0 );
	m_radioBtn4BS.setTextSpace( 5, 1, 0, 0 );
	m_radioBtn4BS.setImageIdBk( m_uImageIdRBBk );
	m_radioBtn4BS.setImage( YBRadioBtn::ENORADIOBTN_STATUS_DISABLE, m_uImageIdNRBNormal );
	m_radioBtn4BS.setImage( YBRadioBtn::ERADIOBTN_STATUS_DISABLE, m_uImageIdRBNormal );
	m_radioBtn4BS.setImage( YBRadioBtn::ENORADIOBTN_STATUS_NORMAL, m_uImageIdNRBNormal );
	m_radioBtn4BS.setImage( YBRadioBtn::ENORADIOBTN_STATUS_HOVER, m_uImageIdNRBNormal );
	m_radioBtn4BS.setImage( YBRadioBtn::ENORADIOBTN_STATUS_PRESS, m_uImageIdNRBNormal );
	m_radioBtn4BS.setImage( YBRadioBtn::ERADIOBTN_STATUS_NORMAL, m_uImageIdRBNormal );
	m_radioBtn4BS.setImage( YBRadioBtn::ERADIOBTN_STATUS_HOVER, m_uImageIdRBNormal );
	m_radioBtn4BS.setImage( YBRadioBtn::ERADIOBTN_STATUS_PRESS, m_uImageIdRBNormal );
	//
	m_radioBtn4CS.Create( 0, WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, 368, 253, 250, 20, hWndModalDlg, (HMENU)YBITAADLGID_RADIO_CSSELECT, hInst );
	m_radioBtn4CS.setbeSelect( false );
	m_radioBtn4CS.setText( _T("C/S（客户端）网站式办公系统") );
	m_radioBtn4CS.setFontId( m_uFontIdRadio );
	m_radioBtn4CS.setEnable( true );
	m_radioBtn4CS.setRadioBtnLSpace( 0 );
	m_radioBtn4CS.setTextSpace( 5, 1, 0, 0 );
	m_radioBtn4CS.setImageIdBk( m_uImageIdRBBk );
	m_radioBtn4CS.setImage( YBRadioBtn::ENORADIOBTN_STATUS_DISABLE, m_uImageIdNRBNormal );
	m_radioBtn4CS.setImage( YBRadioBtn::ERADIOBTN_STATUS_DISABLE, m_uImageIdRBNormal );
	m_radioBtn4CS.setImage( YBRadioBtn::ENORADIOBTN_STATUS_NORMAL, m_uImageIdNRBNormal );
	m_radioBtn4CS.setImage( YBRadioBtn::ENORADIOBTN_STATUS_HOVER, m_uImageIdNRBNormal );
	m_radioBtn4CS.setImage( YBRadioBtn::ENORADIOBTN_STATUS_PRESS, m_uImageIdNRBNormal );
	m_radioBtn4CS.setImage( YBRadioBtn::ERADIOBTN_STATUS_NORMAL, m_uImageIdRBNormal );
	m_radioBtn4CS.setImage( YBRadioBtn::ERADIOBTN_STATUS_HOVER, m_uImageIdRBNormal );
	m_radioBtn4CS.setImage( YBRadioBtn::ERADIOBTN_STATUS_PRESS, m_uImageIdRBNormal );
	//
	m_hEditAppTitle = ::CreateWindow( _T("Edit"), m_tstrAppTitle.c_str(), WS_VISIBLE|WS_CHILD|ES_AUTOHSCROLL|WS_BORDER, 293, 190, 235, 25, hWndModalDlg, (HMENU)YBITAADLGID_EDIT_APPTITLE, hInst, NULL );
	HFONT hFont = ybctrl::YBCtrlFontDepository::getInstance()->getYBFont(m_uFontIdEdit)->m_hFont;
	::SendMessage( m_hEditAppTitle, WM_SETFONT, (WPARAM)hFont, 0 );
	::SetWindowText( m_hEditAppTitle, _T("  请输入办公系统名称") );
	//
	m_hEdit4BS = ::CreateWindow( _T("Edit"), m_tstrWebSiteUrl.c_str(), WS_VISIBLE|WS_CHILD|ES_AUTOHSCROLL|WS_BORDER, 100, 290, 500, 25, hWndModalDlg, (HMENU)YBITAADLGID_EDIT_BS, hInst, NULL );
	::SendMessage( m_hEdit4BS, WM_SETFONT, (WPARAM)hFont, 0 );
	::SetWindowText( m_hEdit4BS, _T("  请输入B/S网站式办公系统网址") );
	//
	m_hEdit4CS = ::CreateWindow( _T("Edit"), m_tstrLocalPath.c_str(), WS_CHILD|ES_AUTOHSCROLL|WS_BORDER, 100, 290, 500, 25, hWndModalDlg, (HMENU)YBITAADLGID_EDIT_BS, hInst, NULL );
	::SendMessage( m_hEdit4CS, WM_SETFONT, (WPARAM)hFont, 0 );
	::SetWindowText( m_hEdit4CS, _T("  请立即点击添加按钮添加C/S（客户端）本地办公系统") );
}

void YBITAddAppDlg::_loadFont() {
	LOGFONT lFont;

	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 23;
	m_uFontIdBBtn = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );
	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 20;
	m_uFontIdSBtn = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );
	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 22;
	m_uFontIdRadio = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 14, 57, 102 ) );
	// Edit
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 22;
	m_uFontIdEdit = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 0, 0, 0 ) );
}

void YBITAddAppDlg::_loadImage() {
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
	tstrImageFilePath = tstrImageDirPath + _T("YBITAddAppDlgBk.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdBk = idImage;
	// Button 
	tstrImageFilePath = tstrImageDirPath + _T("YBITAAD4BtnNor.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdBtnNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITAAD4BtnHov.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdBtnHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITAAD4BtnPre.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdBtnPre = idImage;
	// Button2
	tstrImageFilePath = tstrImageDirPath + _T("YBITAAD4BtnNor2.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdBtnNor2 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITAAD4BtnHov2.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdBtnHov2 = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITAAD4BtnPre2.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdBtnPre2 = idImage;
	// radio button  
	tstrImageFilePath = tstrImageDirPath + _T("YBITAAD4RdBtnBk.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdRBBk = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITAAD4RdBtn.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdRBNormal = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITAAD4NRdBtn.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdNRBNormal = idImage;
}

YBITAddAppDlg::ENUMAPPTYPE YBITAddAppDlg::getAppType() const {
	return m_eAppType;
}

YBITAddAppDlg::tstring_type YBITAddAppDlg::getAppTitle() const {
	TCHAR szAppTitle[ _MAX_FNAME + 1 ] = {0};

	if ( ::IsWindow( m_hEditAppTitle ) ) {
		::GetWindowText( m_hEditAppTitle, szAppTitle, _MAX_FNAME );
		return szAppTitle;
	}
	
	return m_tstrFileName;
}

YBITAddAppDlg::tstring_type YBITAddAppDlg::getAppLocalPath() const {
	TCHAR szAppLocal[ _MAX_FNAME + 1 ] = {0};

	if ( ::IsWindow( m_hEdit4CS ) ) {
		::GetWindowText( m_hEditAppTitle, szAppLocal, _MAX_FNAME );
		return szAppLocal;
	}
	return m_tstrLocalPath;
}

YBITAddAppDlg::tstring_type YBITAddAppDlg::getWebSiteUrl() const {
	TCHAR szWebSiteUrl[ _MAX_FNAME + 1 ] = {0};

	if ( ::IsWindow( m_hEdit4BS ) ) {
		::GetWindowText( m_hEditAppTitle, szWebSiteUrl, _MAX_FNAME );
		return szWebSiteUrl;
	}

	return m_tstrWebSiteUrl;
}

//YBITAddAppDlg::tstring_type YBITAddAppDlg::getAppDefExt() const {
//	return m_tstrDefExt;
//}

void YBITAddAppDlg::setAppTitle( const tstring_type tstrAppTitle ) {
	m_tstrAppTitle = tstrAppTitle;
	if ( ::IsWindow( m_hEditAppTitle ) ) {
		::SetWindowText( m_hEditAppTitle, m_tstrAppTitle.c_str() );
	}
}

void YBITAddAppDlg::setAppLocalPath( const tstring_type tstrLocalPath ) {
	m_tstrLocalPath = tstrLocalPath;
	if ( ::IsWindow( m_hEdit4CS ) ) {
		::SetWindowText( m_hEdit4CS, m_tstrLocalPath.c_str() );
	}
}

void YBITAddAppDlg::setWebSiteUrl( const tstring_type tstrWebSiteUrl ) {
	m_tstrWebSiteUrl = tstrWebSiteUrl;
	if ( ::IsWindow( m_hEdit4BS ) ) {
		::SetWindowText( m_hEdit4BS, m_tstrWebSiteUrl.c_str() );
	}
}

void YBITAddAppDlg::setDefaultType( int iType ) {
	switch( iType ) {
	case 1:
		{
			m_radioBtn4BS.setbeSelect( true );
			m_radioBtn4CS.InitialState();
			::SetWindowPos( m_hEdit4CS, HWND_BOTTOM, 0, 0, 0, 0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE );
			::SetWindowPos( m_hEdit4BS, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE );
		}
		break;
	case 2:
		{
			m_radioBtn4CS.setbeSelect( true );
			m_radioBtn4BS.InitialState();
			::SetWindowPos( m_hEdit4BS, HWND_BOTTOM, 0, 0, 0, 0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE );
			::SetWindowPos( m_hEdit4CS, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE );
		}
		break;
	default:
		break;
	}
}