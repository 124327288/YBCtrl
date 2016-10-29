#include "stdafx.h"
#include "YBITDlgFileUpLoad.h"
#include "button\YBButton.h"
#include "button\YBButtonX.h"
#include "checkbox\YBCheckBox.h"
#include "radiobtn\YBRadioBtn.h"
#include "progressbar\YBProgressCtrl.h"
#include "frame\YBCtrlImageDepository.h"
#include "frame\YBCtrlDefs.h"
#include "frame\YBCtrlImageStatic.h"
#include "frame\YBCtrlImageBase.h"
#include "frame\YBCtrlMisc.h"
#include "frame\YBCtrlMemDC.h"
#include "loader\YBImageLoader.h"
#include "frame\YBCtrlFontDepository.h"
#include "gdiplus.h"
#include <assert.h>

#define YBITDFUL_BTNTEXT_SAVE (_T("保  存"))
#define YBITDFUL_BTNTEXT_CANCEL (_T("取  消"))
#define YBITDFUL_TEXT_TITTLE (_T("保存文件标题："))
#define YBITDFUL_TEXT_SAVE (_T("请选择文件保存去处：(系统默认“本地电脑”固定保存)"))

#define YBITDFUL_RGB_EDITFRAME (RGB( 36, 134, 204 ))

USING_NAMESPACE_YBCTRL

YBITDlgFileUpLoad::YBITDlgFileUpLoad()
: m_hEditFileName( NULL )
, m_tstrFileName( _T("") )
, m_uImageYBITFULBk( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdDisable( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnPre( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnXNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnXHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnXPre( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdTittleBk( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdWhite( YBCTRL_IMAGE_ID_INVALID )
, m_uFontIdTxt( YBCTRL_FONT_ID_INVALID )
, m_uFontIdbtn( YBCTRL_FONT_ID_INVALID )
, m_uFontIdCheckBox( YBCTRL_FONT_ID_INVALID )
, m_uImageIdCheckBk( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdNoCheckNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdNoCheckHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdNoCheckPre( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdCheckNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdCheckHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdCheckPre( YBCTRL_IMAGE_ID_INVALID )
, m_hPenEditFrame( NULL )
{
	m_hPenEditFrame = ::CreatePen( PS_SOLID, 1, YBITDFUL_RGB_EDITFRAME );
	_loadImage();
	_loadFont();
};

YBITDlgFileUpLoad::~YBITDlgFileUpLoad(){
	if ( m_hPenEditFrame ) {
		::DeleteObject( m_hPenEditFrame );
		m_hPenEditFrame = NULL;
	}
}

YBCTRL_WNDMSGMAP_BEGIN( YBITDlgFileUpLoad, YBModalDialog )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_ONWNDMSG( WM_NCHITTEST, _onWM_NCHITTEST )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_DESTROY, _onWM_DESTROY )
	YBCTRL_ONWNDMSG( WM_KEYDOWN, _onWM_KEYDOWN )
	YBCTRL_WNDMSGMAP_END()


void YBITDlgFileUpLoad::_onWM_DESTROY( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_DESTROY );
	TCHAR szFileName[ _MAX_FNAME + 1 ];

	if ( ::IsWindow( m_hEditFileName ) ) {
		::GetWindowText( m_hEditFileName, szFileName, _MAX_FNAME );
		m_tstrFileName = szFileName;
	}
}

void YBITDlgFileUpLoad::_onWM_KEYDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_KEYDOWN );
	switch( pYBCtrlWndMsg->m_wParam )
	{
	case VK_ESCAPE:
		{
			::PostMessage( m_hWnd, WM_CLOSE, 0, 0 );
		}
		break;
	default:
		break;
	}
}

void YBITDlgFileUpLoad::_onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_NCHITTEST );

	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = HTCAPTION;

// 	HDC hDC;
// 	RECT rcClient;
// 
// 	::GetClientRect( pYBCtrlWndMsg->m_hWnd, &rcClient );
// 	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
// 	YBModalDialog::_drawBK( memDC.getMemDC() );
// 	_drawBk( memDC.getMemDC() );
// 	//
// 	hDC = ::GetDC( m_hWnd );
// 	::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), ( rcClient.bottom - rcClient.top ),
// 		memDC.getMemDC(), 0, 0, SRCCOPY );
// 	::ReleaseDC( m_hWnd, hDC );
}

void YBITDlgFileUpLoad::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );

	RECT rcClient;
	PAINTSTRUCT ps;
	HDC hPaintDC;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	YBModalDialog::_drawBK( memDC.getMemDC() );
	_drawBk( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );

}

void YBITDlgFileUpLoad::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );

	switch ( wCtrlId ) {
	case YBCTRLID_SAVE:
		{
			endModal( ::GetDlgCtrlID( m_btnSave.getHWND() ) );
		}
		break;;
	case YBCTRLID_CANCEL:
		{
			endModal( ::GetDlgCtrlID( m_btnCancel.getHWND() ) );
		}
		break;
	case YBCTRLID_CLOSE:
		{
			endModal( ::GetDlgCtrlID( m_btnClose.getHWND() ) );
		}
		break;
	default:
		break;
	}
}

void YBITDlgFileUpLoad::_loadImage(){

	tstring_type tstrImageDirPath;
	tstring_type tstrImageFilePath;
	TCHAR szModulePath[MAX_PATH];
	TCHAR* posTmp = NULL; 
	unsigned int idImage;

	GetModuleFileName( NULL, szModulePath, MAX_PATH );
	posTmp =  _tcsrchr( szModulePath, _T('\\') );
	if ( !posTmp ) {
		assert( false );
		return;
	}
	posTmp += 1;
	*posTmp = 0;
	tstrImageDirPath = szModulePath;
	tstrImageDirPath += _T("Image\\");
	// BK
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBk.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	setBKImageId( idImage, 2, 5, 5, 2 );
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBtnSaveNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("buttonPress.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBtnSavePre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgTitleBk.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdTittleBk = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBtnXNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnXNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBtnXHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnXHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBtnXPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnXPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCheckBoxBk.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdWhite = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCheckNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdCheckNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCheckHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdCheckHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCheckPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdCheckPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNoCheckNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdNoCheckNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNoCheckHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdNoCheckHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNoCheckPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdNoCheckPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCheckDisable.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdDisable = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNoCheckDisable.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdNDisable = idImage;
}

void YBITDlgFileUpLoad::_loadFont(){
	LOGFONT lFont;

	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 22;
	lFont.lfWeight = 700;
	m_uFontIdbtn = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );
	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 20;
	lFont.lfWeight = 550;
	m_uFontIdTxt = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 0, 93, 147 ) );
	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("宋体") );
	lFont.lfHeight = 13;
	m_uFontIdCheckBox = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 0, 0, 0 ) );
	
}

unsigned int YBITDlgFileUpLoad::_getWndHeight() const { return 205; }

unsigned int YBITDlgFileUpLoad::_getWndWidth() const { return 450; }

void YBITDlgFileUpLoad::_createUI( HWND hWndModalDlg, HINSTANCE hInst) {
	// 保存按钮
	m_btnSave.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnNor );
	m_btnSave.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnHov );
	m_btnSave.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnPre );
	m_btnSave.setFontId( m_uFontIdbtn );
	m_btnSave.setTextSpace( 2, 5, 2, 2 );
	m_btnSave.setText( YBITDFUL_BTNTEXT_SAVE );
	m_btnSave.Create( 0, WS_VISIBLE|WS_CHILD,157, 155, 68, 31, hWndModalDlg, (HMENU)YBCTRLID_SAVE, hInst );
	// 取消按钮
	m_btnCancel.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnNor );
	m_btnCancel.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnHov );
	m_btnCancel.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnPre );
	m_btnCancel.setFontId( m_uFontIdbtn );
	m_btnCancel.setTextSpace( 2, 5, 2, 2 );
	m_btnCancel.setText( YBITDFUL_BTNTEXT_CANCEL );
	m_btnCancel.Create( 0, WS_VISIBLE|WS_CHILD, 231, 155, 68, 31, hWndModalDlg, (HMENU)YBCTRLID_CANCEL, hInst );
	// 关闭按钮
	m_btnClose.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnXNor );
	m_btnClose.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnXHov );
	m_btnClose.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnXPre );
	m_btnClose.Create( 0, WS_VISIBLE|WS_CHILD,  430, 7, 14, 14, hWndModalDlg, (HMENU)YBCTRLID_CLOSE, hInst );
	// 保存文件标题：
	m_staticTxtTittle.setImage( m_uImageIdWhite );
	m_staticTxtTittle.setFontId( m_uFontIdTxt );
	m_staticTxtTittle.Create( 0, WS_VISIBLE|WS_CHILD, 30, 50, 100, 25, hWndModalDlg, (HMENU)YBCTRLID_TEXT_SAVE, hInst );
	m_staticTxtTittle.setText( YBITDFUL_TEXT_TITTLE );
	// 请选择文件保存去处：
	m_staticTxtSave.setImage( m_uImageIdWhite );
	m_staticTxtSave.setFontId( m_uFontIdTxt );
	m_staticTxtSave.Create( 0, WS_VISIBLE|WS_CHILD, 30, 85, 360, 25, hWndModalDlg, (HMENU)YBCTRLID_TEXT_SAVE, hInst );
	m_staticTxtSave.setText( YBITDFUL_TEXT_SAVE );
	// checkbox 文件大全
	m_checkFileOf.setCheckLSpace( 2 );
	m_checkFileOf.Create( 0, WS_VISIBLE|WS_CHILD, 30, 110, 80, 40, hWndModalDlg, (HMENU)YBCTRLID_CHECK_FILE, hInst );
	m_checkFileOf.setTextSpace( 15, 10, 5, 10 );
	m_checkFileOf.setText( _T("文件大全"));
	m_checkFileOf.setEnable( true );
	m_checkFileOf.setFontId( m_uFontIdCheckBox );
	m_checkFileOf.setBKImageID( m_uImageIdWhite );
	m_checkFileOf.setImage( YBCheckBox::ENOCHECK_STATUS_NORMAL, m_uImageIdNoCheckNor );
	m_checkFileOf.setImage( YBCheckBox::ENOCHECK_STATUS_HOVER, m_uImageIdNoCheckHov );
	m_checkFileOf.setImage( YBCheckBox::ENOCHECK_STATUS_PRESS, m_uImageIdNoCheckPre );
	m_checkFileOf.setImage( YBCheckBox::ECHECK_STATUS_NORMAL, m_uImageIdCheckNor );
	m_checkFileOf.setImage( YBCheckBox::ECHECK_STATUS_HOVER, m_uImageIdCheckHov );
	m_checkFileOf.setImage( YBCheckBox::ECHECK_STATUS_PRESS, m_uImageIdCheckPre );
	m_checkFileOf.setImage( YBCheckBox::ECHECK_STATUS_DISABLE, m_uImageIdDisable );
	m_checkFileOf.setImage( YBCheckBox::ENOCHECK_STATUS_DISABLE, m_uImageIdNDisable );
	// checkbox 数据银行
	m_checkDataBank.setCheckLSpace( 2 );
	m_checkDataBank.Create( 0, WS_VISIBLE|WS_CHILD, 110, 110, 80, 40, hWndModalDlg, (HMENU)YBCTRLID_CHECK_DATABANK, hInst );
	m_checkDataBank.setTextSpace( 15, 10, 5, 10 );
	m_checkDataBank.setText( _T("数据银行"));
	m_checkDataBank.setFontId( m_uFontIdCheckBox );
	m_checkDataBank.setBKImageID( m_uImageIdWhite );
	m_checkDataBank.setEnable( true );
	m_checkDataBank.setImage( YBCheckBox::ENOCHECK_STATUS_NORMAL, m_uImageIdNoCheckNor );
	m_checkDataBank.setImage( YBCheckBox::ENOCHECK_STATUS_HOVER, m_uImageIdNoCheckHov );
	m_checkDataBank.setImage( YBCheckBox::ENOCHECK_STATUS_PRESS, m_uImageIdNoCheckPre );
	m_checkDataBank.setImage( YBCheckBox::ECHECK_STATUS_NORMAL, m_uImageIdCheckNor );
	m_checkDataBank.setImage( YBCheckBox::ECHECK_STATUS_HOVER, m_uImageIdCheckHov );
	m_checkDataBank.setImage( YBCheckBox::ECHECK_STATUS_PRESS, m_uImageIdCheckPre );
	m_checkDataBank.setImage( YBCheckBox::ECHECK_STATUS_DISABLE, m_uImageIdDisable );
	m_checkDataBank.setImage( YBCheckBox::ENOCHECK_STATUS_DISABLE, m_uImageIdNDisable );
	// check photo 图片相册
	m_checkPhoto.setCheckLSpace( 2 );
	m_checkPhoto.Create( 0, WS_VISIBLE|WS_CHILD, 190, 110, 80, 40, hWndModalDlg, (HMENU)YBCTRLID_CHECK_PHOTO, hInst );
	m_checkPhoto.setTextSpace( 15, 10, 5, 10 );
	m_checkPhoto.setText( _T("图片相册") );
	m_checkPhoto.setFontId( m_uFontIdCheckBox );
	m_checkPhoto.setBKImageID( m_uImageIdWhite );
	m_checkPhoto.setEnable( true );
	m_checkPhoto.setImage( YBCheckBox::ENOCHECK_STATUS_NORMAL, m_uImageIdNoCheckNor );
	m_checkPhoto.setImage( YBCheckBox::ENOCHECK_STATUS_HOVER, m_uImageIdNoCheckHov );
	m_checkPhoto.setImage( YBCheckBox::ENOCHECK_STATUS_PRESS, m_uImageIdNoCheckPre );
	m_checkPhoto.setImage( YBCheckBox::ECHECK_STATUS_NORMAL, m_uImageIdCheckNor );
	m_checkPhoto.setImage( YBCheckBox::ECHECK_STATUS_HOVER, m_uImageIdCheckHov );
	m_checkPhoto.setImage( YBCheckBox::ECHECK_STATUS_PRESS, m_uImageIdCheckPre );
	m_checkPhoto.setImage( YBCheckBox::ECHECK_STATUS_DISABLE, m_uImageIdDisable );
	m_checkPhoto.setImage( YBCheckBox::ENOCHECK_STATUS_DISABLE, m_uImageIdNDisable );
	// check chat 裕邦聊天
	m_checkChat.setCheckLSpace( 2 );
	m_checkChat.Create( 0, WS_VISIBLE|WS_CHILD, 270, 110, 80, 40, hWndModalDlg, (HMENU)YBCTRLID_CHECK_PHOTO, hInst );
	m_checkChat.setTextSpace( 15, 10, 5, 10 );
	m_checkChat.setText( _T("聊天相集") );
	m_checkChat.setFontId( m_uFontIdCheckBox );
	m_checkChat.setBKImageID( m_uImageIdWhite );
	m_checkChat.setEnable( false );
	m_checkChat.setImage( YBCheckBox::ENOCHECK_STATUS_NORMAL, m_uImageIdNoCheckNor );
	m_checkChat.setImage( YBCheckBox::ENOCHECK_STATUS_HOVER, m_uImageIdNoCheckHov );
	m_checkChat.setImage( YBCheckBox::ENOCHECK_STATUS_PRESS, m_uImageIdNoCheckPre );
	m_checkChat.setImage( YBCheckBox::ECHECK_STATUS_NORMAL, m_uImageIdCheckNor );
	m_checkChat.setImage( YBCheckBox::ECHECK_STATUS_HOVER, m_uImageIdCheckHov );
	m_checkChat.setImage( YBCheckBox::ECHECK_STATUS_PRESS, m_uImageIdCheckPre );
	m_checkChat.setImage( YBCheckBox::ECHECK_STATUS_DISABLE, m_uImageIdDisable );
	m_checkChat.setImage( YBCheckBox::ENOCHECK_STATUS_DISABLE, m_uImageIdNDisable );
	// checkbox disable 状态
	m_checkLocal.setCheckLSpace( 2 );
	m_checkLocal.Create( 0, WS_VISIBLE|WS_CHILD, 350, 110, 80, 40, hWndModalDlg, (HMENU)YBCTRLID_CHECK_LOCAL, hInst );
	m_checkLocal.setTextSpace( 15, 10, 5, 10 );
	m_checkLocal.setText( _T("本地电脑"));
	m_checkLocal.setFontId( m_uFontIdCheckBox );
	m_checkLocal.setBKImageID( m_uImageIdWhite );
	m_checkLocal.setbeChecked( true );
	m_checkLocal.setEnable( false );
	m_checkLocal.setImage( YBCheckBox::ENOCHECK_STATUS_NORMAL, m_uImageIdNoCheckNor );
	m_checkLocal.setImage( YBCheckBox::ENOCHECK_STATUS_HOVER, m_uImageIdNoCheckHov );
	m_checkLocal.setImage( YBCheckBox::ENOCHECK_STATUS_PRESS, m_uImageIdNoCheckPre );
	m_checkLocal.setImage( YBCheckBox::ECHECK_STATUS_NORMAL, m_uImageIdCheckNor );
	m_checkLocal.setImage( YBCheckBox::ECHECK_STATUS_HOVER, m_uImageIdCheckHov );
	m_checkLocal.setImage( YBCheckBox::ECHECK_STATUS_PRESS, m_uImageIdCheckPre );
	m_checkLocal.setImage( YBCheckBox::ECHECK_STATUS_DISABLE, m_uImageIdDisable );
	m_checkLocal.setImage( YBCheckBox::ENOCHECK_STATUS_DISABLE, m_uImageIdNDisable );
	
	m_hEditFileName = ::CreateWindow( _T("Edit"), m_tstrFileName.c_str(), WS_VISIBLE|WS_CHILD|ES_AUTOHSCROLL|ES_MULTILINE|ES_WANTRETURN, 140, 53, 215, 20, hWndModalDlg, (HMENU)YBCTRLID_EDIT, hInst, NULL );
	//LOGFONT lf;
	//memset( &lf, 0, sizeof(LOGFONT) );
	//lf.lfHeight = 20;
	//_tcscpy_s( lf.lfFaceName, _T("微软雅黑") );
	//HFONT hFont = ::CreateFontIndirect( &lf );
	HFONT hFont = ybctrl::YBCtrlFontDepository::getInstance()->getYBFont(m_uFontIdCheckBox)->m_hFont;
	::SendMessage( m_hEditFileName, WM_SETFONT, (WPARAM)hFont, 0 );
	
}

void YBITDlgFileUpLoad::setFileName( const tstring_type& tstrFileName ) {
	m_tstrFileName = tstrFileName;
	if ( ::IsWindow( m_hEditFileName ) ) {
		::SetWindowText( m_hEditFileName, m_tstrFileName.c_str() );
		
	}
}

YBITDlgFileUpLoad::tstring_type YBITDlgFileUpLoad::getFileName() const {
	TCHAR szFileName[ _MAX_FNAME + 1 ] = {0};

	if ( ::IsWindow( m_hEditFileName ) ) {
		::GetWindowText( m_hEditFileName, szFileName, _MAX_FNAME );
		return szFileName;
	}

	return m_tstrFileName;
}

bool YBITDlgFileUpLoad::getCheckState( unsigned int uCheckBoxID ) {
	switch ( uCheckBoxID ) {
	case YBCTRLID_CHECK_FILE:
		{
			return m_checkFileOf.isCheck();
		}
		break;
	case YBCTRLID_CHECK_DATABANK:
		{
			return m_checkDataBank.isCheck();
		}
		break;
	case YBCTRLID_CHECK_LOCAL:
		{
			return m_checkLocal.isCheck();
		}
		break;
	default:
		assert( false );
		break;
	}

	assert( false );
	return false;
}

void YBITDlgFileUpLoad::_drawBk( HDC hDC ){
	if( !getHWND() )
		return;

	RECT rcClient;
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlImageStatic* pYBTitleImage = NULL;
	
	pYBTitleImage = _getTitleBkImageStatic( m_uImageIdTittleBk );
	//
	BackGroundBitBlt( hDC, 0, 0, rcClient.right - rcClient.left, 30,  pYBTitleImage->getMemDC(),
		0, 0, pYBTitleImage->getWidth(), pYBTitleImage->getHeight(),2, 2, 2, 2 );
	//
	// the input frame
	HPEN hPenOld = (HPEN)SelectObject( hDC, m_hPenEditFrame );
	// top Line
	::MoveToEx( hDC, 130, 47, NULL );
	::LineTo( hDC, 360, 47 );
	// bottom Line
	::MoveToEx( hDC, 130, 73, NULL );
	::LineTo( hDC, 360, 73 );
	// left Line
	::MoveToEx( hDC, 130, 47, NULL );
	::LineTo( hDC, 130, 73 );
	// right Line
	::MoveToEx( hDC, 360, 47, NULL );
	::LineTo( hDC, 360, 73 );
	//
	::SelectObject( hDC, hPenOld );
}


YBCtrlImageBase* YBITDlgFileUpLoad::_getTitleBkImageBase( unsigned int uBkImageId ){
	YBCtrlImageBase* pYBImage = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( m_uImageIdTittleBk, &pYBImage ) )
		return NULL;

	return pYBImage;

}

YBCtrlImageStatic* YBITDlgFileUpLoad::_getTitleBkImageStatic( unsigned int uBkImageId ){
	YBCtrlImageBase* pYBImageBase = NULL;
	pYBImageBase = _getTitleBkImageBase( uBkImageId );

	if ( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;

}