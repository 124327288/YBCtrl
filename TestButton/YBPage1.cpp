#include "stdafx.h"
#include "YBPage1.h"
#include "button\YBButton.h"
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
#include "dialog\YBModalDialog.h"
#include "YBITDlgFileUpload.h"
#include "frame\YBCtrlFontDepository.h"
#include "YBCornerNotifyDlg.h"
#include "gdiplus.h"
#include <assert.h>
#include "YBITAddAppDlg.h"
#include "YBITSetDlg4Resolution.h"

#define YBCTRL_CHECK_BOX (280)
#define YBCTRL_PROGRESS_BAR (281)
#define YBCTRL_PROGRESS_CTRL (284)
#define YBCTRL_RADIO_BUTTON (282)
#define YBCTRL_RADIO_BUTTON2 (283)
#define YBCTRL_STATIC (285)
#define YBCTRL_CHECK_4UDBOX (286)
USING_NAMESPACE_YBCTRL

static void NotifyDlgDestroyCallback( YBCornerNotifyDlg* pCNDlg, void* pParam );

YBPage1::YBPage1()
: m_uNoCIIdNormal( YBCTRL_IMAGE_ID_INVALID )
, m_uNoCIIdHover( YBCTRL_IMAGE_ID_INVALID )
, m_uNoCIIdPress( YBCTRL_IMAGE_ID_INVALID )
, m_uCIIdHover( YBCTRL_IMAGE_ID_INVALID )
, m_uCIIdPress( YBCTRL_IMAGE_ID_INVALID )
, m_uCIIdNormal( YBCTRL_IMAGE_ID_INVALID )
, m_uCIIdBK( YBCTRL_IMAGE_ID_INVALID )
, m_fontIdCheckTxt( YBCTRL_FONT_ID_INVALID )
, m_uImageIdFT( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdRBNormal( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdRBHover( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdRBPress( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdNRBNormal( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdNRBHover( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdNRBPress( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdRBBK( YBCTRL_IMAGE_ID_INVALID )
, m_uFontIdRdBtn( YBCTRL_FONT_ID_INVALID )
, m_uFontIdProTxt( YBCTRL_FONT_ID_INVALID )
, m_uImageIdRdDisable( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdNoRdDisable( YBCTRL_IMAGE_ID_INVALID )
{
	loadImage();
	loadFont();
	_init();
}


YBPage1::~YBPage1(){
	
}

YBCTRL_WNDMSGMAP_BEGIN( YBPage1, YBTopMostBar );
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT );
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND );
	YBCTRL_ONWNDMSG( WM_VOLBTNCHECK, _onWM_VOLBTNCHECK )
YBCTRL_WNDMSGMAP_END()

	void YBPage1::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
		YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
		WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
		WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );
		switch( wCtrlId )
		{
		case YBCTRL_RADIO_BUTTON:
			{
				m_testRadioBtn2.setbeSelect( m_testRadioBtn2.isSelect() );
				m_testRadioBtn2.InitialState();
			}
			break;
		case YBCTRL_RADIO_BUTTON2:
			{
				m_testRadioBtn.setbeSelect( m_testRadioBtn.isSelect() );
				m_testRadioBtn.InitialState();
			}
			break;
		case YBCTRL_CHECK_BOX:
			{
				bool a = m_testCheck.isCheck();
			}
			break;
		case YBCTRL_CHECK_4UDBOX:
			{
				int a = 0;
			}
			break;
		default:
			break;
		}

}

void YBPage1::_onWM_VOLBTNCHECK( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_VOLBTNCHECK );
	int a = 0;
}

void YBPage1::loadImage() {
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

	tstrImageFilePath = tstrImageDirPath + _T("YBPage1Bk.png");
	setBkImage( tstrImageFilePath );
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNoCheckNormal.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uNoCIIdNormal = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNoCheckHover.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uNoCIIdHover = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNoCheckPress.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uNoCIIdPress = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCheckNormal.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uCIIdNormal = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCheckHover.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uCIIdHover = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCheckPress.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uCIIdPress = idImage;
	//Check BK
	tstrImageFilePath = tstrImageDirPath + _T("Press.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uCIIdBK = idImage;
	//progress bar
	tstrImageFilePath = tstrImageDirPath + _T("ProgressBarBK.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBk = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("ProgressBarFT.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdFT = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBRdBtnNormal.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdRBNormal = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBRdBtnHover.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdRBHover = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBRdBtnPress.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdRBPress = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNoRdBtnNormal.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdNRBNormal = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNoRdBtnHover.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdNRBHover = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNoRdBtnPress.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdNRBPress = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBRdBtnBK.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdRBBK = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBProCtrlBK.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uProCtrlImageIdBK = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBProCtrl.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uProCtrlImageId = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBProBtnHover.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdPCHover = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBProBtnPress.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdPCPress = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBProBtnNormal.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdPCNormal = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBStaticBK.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdStatic = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNoRdBtnDisable.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdNoRdDisable = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBRdBtnDisable.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdRdDisable = idImage;
}

HWND YBPage1::create( int x, int y, int nWidth, int nHeight, HWND hWndParent ){
	HWND hWnd;
	HWND hWndCheckBox;
	HWND hWndCheckBox4UD;
	HWND hWndProBar;
	HWND hWndRdBtn;
	HWND hWndRdBtn2;
	HWND hWndProCtrl;
	HWND hWndStatic;
	HWND hWndYBITDlg;

	hWnd = YBTopMostBar::create( x, y, nWidth, nHeight, hWndParent, NULL );
	if( hWnd ){

		hWndCheckBox4UD = m_testCheck4UD.create( 0, WS_VISIBLE|WS_CHILD, 100, 100, 300, 100, hWnd, (HMENU)YBCTRL_CHECK_4UDBOX, NULL );
		m_testCheck4UD.setCheckStatus( true );
		hWndRdBtn = m_testRadioBtn.Create( 0, WS_VISIBLE|WS_CHILD, 100, 400, 100, 40, hWnd, (HMENU)YBCTRL_RADIO_BUTTON, NULL );
		hWndRdBtn2 = m_testRadioBtn2.Create( 0, WS_VISIBLE|WS_CHILD, 100, 450, 100, 40, hWnd, (HMENU)YBCTRL_RADIO_BUTTON2, NULL );
		hWndProBar = m_testProBar.Create( 0, WS_VISIBLE|WS_CHILD, 100, 250, 280, 50, hWnd, (HMENU)YBCTRL_PROGRESS_BAR, NULL );
		m_testProBar.setRange( 0, 10000 );
		m_testProBar.setPos( 8888 );
		// ProgressCtrl
		hWndProCtrl = m_testProCtrl.Create( 0, WS_VISIBLE|WS_CHILD, 100, 600, 769, 31, hWnd, (HMENU)YBCTRL_PROGRESS_CTRL, NULL );
		m_testProCtrl.SetProgressCtrlSpaced( 0, 0, 0, 0 );
		m_testProCtrl.setRange( 0, 100 );
		m_testProCtrl.setPos( 0 );
		hWndStatic = m_testStatic.Create( 0, WS_CHILD|WS_VISIBLE, 500, 100, 220, 130, hWnd, (HMENU)YBCTRL_STATIC, NULL );
		//
		hWndYBITDlg = m_testYBITDlg.Create( 500, 300, 420, 197, hWnd, NULL );
		
		m_testYBULStep1Dlg.create( 730, 10, 650, 40, hWnd );
		m_testYBULStep2Dlg.create( 730, 70, 650, 40, hWnd );
		m_testYBULStep3Dlg.create( 730, 130, 650, 40, hWnd );
		m_testYBULStep4Dlg.create( 730, 190, 650, 40, hWnd );
		m_testYBULStep5Dlg.create( 730, 250, 650, 40, hWnd );
		//
		m_testBtn.Create( 0, WS_VISIBLE|WS_CHILD, 900, 500, 400, 300, hWnd, (HMENU)258, NULL );
		hWndCheckBox = m_testCheck.Create( 0, WS_CHILD|WS_VISIBLE, 100, 100, 100, 40, m_testBtn.getHWND(), (HMENU)YBCTRL_CHECK_BOX, NULL );
		//
		m_testBtnDisable.Create( 0, WS_VISIBLE|WS_CHILD, 0, 0, 80, 26, hWnd, (HMENU)259, NULL );
		m_testBtnDisable.enable( false );

		//YBITAddAppDlg m_testMoadal;
		//m_testMoadal.doModal( hWnd );
		//tstring_type tstrTxt;
		//tstrTxt = m_testMoadal.getAppTitle();
		//m_testMoadal.setAppTitle( tstrTxt );
		//::OutputDebugString( tstrTxt.c_str() );

		//YBITSetDlg4Resolution m_testModal;
		//m_testModal.doModal( hWnd );

	}
	::ShowWindow( hWnd, SW_SHOWNORMAL );
	return hWnd;
}

void YBPage1::_init(){
	//Check Box
	m_testCheck.setTextSpace( 0, 2, 5, 2 );
	m_testCheck.setText( _T("Check"));
	m_testCheck.setFontId( m_fontIdCheckTxt );
	m_testCheck.setBKImageID( m_uCIIdBK );
	m_testCheck.setbeChecked( true );
	m_testCheck.setImage( YBCheckBox::ENOCHECK_STATUS_NORMAL, m_uNoCIIdNormal );
	m_testCheck.setImage( YBCheckBox::ENOCHECK_STATUS_HOVER, m_uNoCIIdHover );
	m_testCheck.setImage( YBCheckBox::ENOCHECK_STATUS_PRESS, m_uNoCIIdPress );
	m_testCheck.setImage( YBCheckBox::ECHECK_STATUS_NORMAL, m_uCIIdNormal );
	m_testCheck.setImage( YBCheckBox::ECHECK_STATUS_HOVER, m_uCIIdHover );
	m_testCheck.setImage( YBCheckBox::ECHECK_STATUS_PRESS, m_uCIIdPress );
	//Progress Bar
	m_testProBar.setImageID( m_uImageIdBk, m_uImageIdFT );
	m_testProBar.SetProgressBarSpaced( 10, 5, 10, 5 );
	m_testProBar.setTextAttri( m_uFontIdProTxt, 5, 5 );
	//Progress Ctrl
	m_testProCtrl.SetImageId( m_uProCtrlImageIdBK, m_uProCtrlImageId );
	m_testProCtrl.SetImageId( YBProgressCtrl::ETHUMB_STATUS_NORMAL, m_uImageIdPCNormal );
	m_testProCtrl.SetImageId( YBProgressCtrl::ETHUMB_STATUS_HOVER, m_uImageIdPCHover );
	m_testProCtrl.SetImageId( YBProgressCtrl::ETHUMB_STATUS_PRESS, m_uImageIdPCPress );
	// Radio Button1
	//m_testRadioBtn.setbeSelect( true );
	m_testRadioBtn.setFontId( m_uFontIdRdBtn );
	//m_testRadioBtn.setEnable( false );
	m_testRadioBtn.setText( _T("Radio") );
	m_testRadioBtn.setRadioBtnLSpace( 0 );
	m_testRadioBtn.setTextSpace( 22, 11, 5, 9 );
	m_testRadioBtn.setImageIdBk( m_uImageIdRBBK );
	m_testRadioBtn.setImage( YBRadioBtn::ENORADIOBTN_STATUS_DISABLE, m_uImageIdNoRdDisable );
	m_testRadioBtn.setImage( YBRadioBtn::ERADIOBTN_STATUS_DISABLE, m_uImageIdRdDisable );
	m_testRadioBtn.setImage( YBRadioBtn::ENORADIOBTN_STATUS_NORMAL, m_uImageIdNRBNormal );
	m_testRadioBtn.setImage( YBRadioBtn::ENORADIOBTN_STATUS_HOVER, m_uImageIdNRBHover );
	m_testRadioBtn.setImage( YBRadioBtn::ENORADIOBTN_STATUS_PRESS, m_uImageIdNRBPress );
	m_testRadioBtn.setImage( YBRadioBtn::ERADIOBTN_STATUS_NORMAL, m_uImageIdRBNormal );
	m_testRadioBtn.setImage( YBRadioBtn::ERADIOBTN_STATUS_HOVER, m_uImageIdRBHover );
	m_testRadioBtn.setImage( YBRadioBtn::ERADIOBTN_STATUS_PRESS, m_uImageIdRBPress );
	// Radio Button2
	m_testRadioBtn2.setFontId( m_uFontIdRdBtn );
	m_testRadioBtn2.setText( _T("Radio") );
	//m_testRadioBtn2.setEnable( false );
	m_testRadioBtn2.setbeSelect( true );
	m_testRadioBtn2.setTextSpace( 22, 11, 5, 9 );
	m_testRadioBtn2.setImageIdBk( m_uImageIdRBBK );
	m_testRadioBtn2.setImage( YBRadioBtn::ENORADIOBTN_STATUS_DISABLE, m_uImageIdNoRdDisable );
	m_testRadioBtn2.setImage( YBRadioBtn::ERADIOBTN_STATUS_DISABLE, m_uImageIdRdDisable );
	m_testRadioBtn2.setImage( YBRadioBtn::ENORADIOBTN_STATUS_NORMAL, m_uImageIdNRBNormal );
	m_testRadioBtn2.setImage( YBRadioBtn::ENORADIOBTN_STATUS_HOVER, m_uImageIdNRBHover );
	m_testRadioBtn2.setImage( YBRadioBtn::ENORADIOBTN_STATUS_PRESS, m_uImageIdNRBPress );
	m_testRadioBtn2.setImage( YBRadioBtn::ERADIOBTN_STATUS_NORMAL, m_uImageIdRBNormal );
	m_testRadioBtn2.setImage( YBRadioBtn::ERADIOBTN_STATUS_HOVER, m_uImageIdRBHover );
	m_testRadioBtn2.setImage( YBRadioBtn::ERADIOBTN_STATUS_PRESS, m_uImageIdRBPress );
	// Static 
	m_testStatic.setImage( m_uImageIdStatic );
	m_testStatic.setFontId( m_uFontIdStatic );
	m_testStatic.setText( _T("       尊敬的%s,“%s”输入法在登录设备中没有安装，如果您需要使用本输入法， \
		我们将从本输入法的官方网站“%s”中为您自动下载安装，请点击“确认”按钮进行下载。") );
	// Button
	m_testBtn.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdPCNormal );
	m_testBtn.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdPCHover );
	m_testBtn.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdNRBPress );
	//
	m_testBtnDisable.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdPCNormal );
	m_testBtnDisable.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdPCHover );
	m_testBtnDisable.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdNRBPress );
	m_testBtnDisable.setImage( YBButton::EBTN_STATUS_DISABLE, m_uImageIdRdDisable );

}

void YBPage1::loadFont() {
	LOGFONT lFont;

	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 22;
	m_fontIdCheckTxt = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 0, 0 ) );
	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 19;
	m_uFontIdRdBtn = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 156, 26, 56 ) );
	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 20;
	m_uFontIdStatic = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 23, 84, 152 ) );
	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 20;
	m_uFontIdProTxt = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );
}

void NotifyDlgDestroyCallback( YBCornerNotifyDlg* pCNDlg, void* pParam ) {
	if ( !pCNDlg ) {
		return;
	}

	::MessageBox( 0, 0, 0, 0 );
}

int g_romData = 258;