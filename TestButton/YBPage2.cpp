#include "stdafx.h"
#include "YBPage2.h"
#include "button\YBButton.h"
#include "frame\YBCtrlImageDepository.h"
#include "frame\YBCtrlDefs.h"
#include "frame\YBCtrlImageStatic.h"
#include "frame\YBCtrlImageBase.h"
#include "frame\YBCtrlMisc.h"
#include "frame\YBCtrlMemDC.h"
#include "loader\YBImageLoader.h"
#include "frame\YBCtrlFontDepository.h"
#include "YBITDlgFileUpload.h"
#include "YBITMsgBox.h"
#include "scrollbar/YBNWScrollBar.h"
#include "YBFileTPDlgMgr4IT.h"
#include "YBPopupMessageBox.h"
#include "YBCtrlMsgBox.h"
#include "gdiplus.h"
#include <assert.h>

#define YBCTRLID_BTN_OPENDOMODALDLG (280)
#define YBCTRLID_BTN_MSGBOX (281)
#define YBCTRLID_BTN_POPUPPB (282)
#define YBCTRLID_BTN_FILETPDLG (283)
#define YBCTRLID_BTN_FSDOWNLOADER (284)
#define YBCTRLID_BTN_WEBEDITDLG (285)
#define YBCTRLID_BTN_INSTALLFILEDLG (286)
#define YBCTRLID_BTN_UPLOADWARMTIPDLG (287)
#define YBCTRLID_BTN_POPUPMESSAGEBOX (288)
#define YBCTRLID_BTN_DETECTPCENVIRONMENT (289)
#define YBCTRLID_SPLITTER (290)
#define YBCTRLID_BTN_IMAGEEDITOR (291)
#define YBCTRLID_MENU_FIRST ( 1 )
//
#define YBPAGE_TIMERID_POPUPPB (0x00001000)

#define YBNWSCROLLBAR_TARROW_ID (0x00000311)
#define YBNWSCROLLBAR_BAROOW_ID (0x00000312)

extern int g_romData;
USING_NAMESPACE_YBCTRL

YBPage2::YBPage2()
: m_testFileTPDlg( YBFileTPDlg::EFILETP_DOWNLOAD )
, m_test4FSMenu( 282 )
, m_test4Splitter( YBSplitter::ESPLITTER_VERTICAL )
//, m_dlgTest( _T( "" ) )
{
 	loadImage();
	loadFont();
	_init();

	int aData = g_romData;
}

YBPage2::~YBPage2(){

}

YBCTRL_WNDMSGMAP_BEGIN( YBPage2, YBTopMostBar )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_RBUTTONDOWN, _onWM_RBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
	YBCTRL_ONWNDMSG( WM_TIMER, _onWM_TIMER )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_MOUSELEAVE, _onWM_MOUSELEAVE )
	YBCTRL_ONWNDMSG( WM_LBUTTONUP, _onWM_LBUTTONUP )
	YBCTRL_ONWNDMSG( WM_MOUSEWHEEL, _onWM_MOUSEWHEEL );
YBCTRL_WNDMSGMAP_END()

void YBPage2::_onWM_TIMER( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_TIMER );

	//m_nwScrollBar.onTIMER( pYBCtrlWndMsg );

	if ( pYBCtrlWndMsg->m_wParam == YBPAGE_TIMERID_POPUPPB ) {
		if ( m_popupPB.getHWND() && ::IsWindowVisible( m_popupPB.getHWND() ) ) {
			__int64 nMinPos;
			__int64 nMaxPos;
			__int64 nCurPos;
			__int64 nStep;

			m_popupPB.getRange( nMinPos, nMaxPos );
			nStep = ( nMaxPos - nMinPos ) / 50;
			nCurPos = m_popupPB.getPos();

			if ( nCurPos == nMaxPos ) {
				nCurPos = nMinPos;
			} else {
				nCurPos = ( nCurPos + nStep > nMaxPos ) ? nMaxPos : nCurPos + nStep;
			}
			m_popupPB.setPos( nCurPos );
			//m_popupPB.GetPos()
		}
	}
}

void YBPage2::_onWM_MOUSEWHEEL( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEWHEEL );


}

void YBPage2::_onWM_LBUTTONUP( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONUP );

	//m_nwScrollBar.onLBUTTONUP( pYBCtrlWndMsg );

}

void YBPage2::_onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );

	//m_nwScrollBar.onPAINT( pYBCtrlWndMsg );
}

void YBPage2::_onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
	POINT ptMouse;

	ptMouse.x = LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = HIWORD( pYBCtrlWndMsg->m_lParam );

	//m_nwScrollBar.onLBUTTONDOWN( pYBCtrlWndMsg );
}

void YBPage2::_onWM_MOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	POINT pt;

	pt.x = LOWORD( pYBCtrlWndMsg->m_lParam );
	pt.y = HIWORD( pYBCtrlWndMsg->m_lParam );

	//m_nwScrollBar.onMOUSEMOVE( pYBCtrlWndMsg );

	//
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );

	switch( wCtrlId ){
	case YBCTRLID_BTN_OPENDOMODALDLG:
		{
			
		}
		break;
	default:
		break;
	}

}

void YBPage2::_onWM_MOUSELEAVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_MOUSELEAVE );
	
}

void YBPage2::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );
	/*
	WORD wTArrowCtrlId;
	WORD wBArrowCtrlId;*/
	//m_nwScrollBar.getCtrlId( wTArrowCtrlId, wBArrowCtrlId );

	switch ( wCtrlId ) {
	case YBNWSCROLLBAR_TARROW_ID:
		{
			int a = 0;
		}
		break;
	case YBNWSCROLLBAR_BAROOW_ID:
		{
			int b = 0;
		}
		break;
	case YBCTRLID_BTN_OPENDOMODALDLG:
		{
			YBITDlgFileUpLoad testModalDlg;
			int nRetCode;

			nRetCode = testModalDlg.doModal( m_hWnd );

			if ( nRetCode == YBCTRLID_SAVE ) {
				int a = 0;
			} else if ( nRetCode == YBCTRLID_CANCEL ) {

			}

			bool bCheck;

			bCheck = testModalDlg.getCheckState( YBCTRLID_CHECK_FILE );
			bCheck = testModalDlg.getCheckState( YBCTRLID_CHECK_DATABANK );
			bCheck = testModalDlg.getCheckState( YBCTRLID_CHECK_LOCAL );
		}
		break;
	case YBCTRLID_BTN_MSGBOX:
		{
// 			YBITMsgBox testMsgBox(true);
// 			int nRetCode;
// 
// 			nRetCode = testMsgBox.doModal( m_hWnd );
								//m_dlgTest.create(800, 650);
								//::ShowWindow( m_dlgTest.getHWND(), SW_NORMAL );
		}
		break;
	case YBCTRLID_BTN_POPUPPB:
		{
			if ( !m_popupPB.getHWND() ) { 
				m_popupPB.create( 200, 200, 400, 20, NULL );
			}
			::ShowWindow( m_popupPB.getHWND(), SW_NORMAL );
			
		}
		break;
	case YBCTRLID_MENU_FIRST:
		{
			int a = 0;
		}
		break;
	case YBCTRLID_BTN_FILETPDLG:
		{
			//
			//m_testFileTPDlg.create( 100, 400, 800, 32, m_hWnd, NULL );
			//::ShowWindow( m_testFileTPDlg.getHWND(), SW_NORMAL );
			YBFileTPDlgMgr4IT::getInstance()->setStatus( YBFileTPDlg::EFILETP_DOWNLOAD, _T("大家看打啊啊"), 500, 1000 );
			YBFileTPDlgMgr4IT::getInstance()->setStatus( YBFileTPDlg::EFILETP_UPLOAD, _T("大家看打啊啊"), 50, 100 );
		}
		break;
	case YBCTRLID_BTN_FSDOWNLOADER:
		{
			//
			m_testFSDownLoader.create( 523, 164, m_hWnd );
		}
		break;
	case YBCTRLID_BTN_WEBEDITDLG:
		{
			int i;
			 i = m_test4WebEditDlg.doModal( m_hWnd );
			 if ( YBWEBEDITID_BTN_OK == i ) {
				 int a = m_test4WebEditDlg.getWEType();
				 if ( a == 0 ) {
					 int b = 0;
				 }else if ( a == 1 ) {
					 int c = 0;
				 } else if ( a == 2 ) {
					 int d = 0;
				 }
			 } else if( YBWEBEDITID_BTN_CANCEL == i ) {
				 int a = 0;
			 }
		}
		break;
	case YBCTRLID_BTN_INSTALLFILEDLG:
		{
			m_test4InstallFileDlg.create( 400, 400, m_hWnd );
		}
		break;
	case YBCTRLID_BTN_UPLOADWARMTIPDLG:
		{
			m_test4UpLoadWarmTipDlg.trackPopupWTDlg( m_hWnd );
			::ShowWindow( m_test4UpLoadWarmTipDlg.getHWND(), SW_NORMAL );
		}
		break;
	case YBCTRLID_BTN_POPUPMESSAGEBOX:
		{
			tstring_type tstrTxt = _T("标题");
			if ( YBIDOK == YBCtrlMsgBox( _T( "文件保存至E:\\open-source\\install chromium tool\\depot_tools.zip 失败!,该文件已被打开件已被打开打开件已被打打" ) ) ) {
				TCHAR szPath[MAX_PATH] = {0};
				unsigned int uSize;
				uSize = ::GetModuleFileName( NULL, szPath, MAX_PATH );
				if ( uSize != 0 ){
					SYSTEMTIME time;
					::GetLocalTime( &time );
					WORD wMinute = time.wMinute;
					WORD wHour = time.wHour;
					int a = 0;
				}
			}
		}
		break;
	case YBCTRLID_BTN_DETECTPCENVIRONMENT:
		{
			m_test4DetectPCEDlg.create( 827, 542, m_hWnd, 100 );
		}
		break;
	case YBCTRLID_SPLITTER:
		{
			int a = 0;
		}
		break;
	case YBCTRLID_BTN_IMAGEEDITOR:
		{
			m_dlg4ImageEditor.setPath( _T("E:\\Code\\YBCtrl\\YBCtrl\\Win32\\Debug\\"));
			m_dlg4ImageEditor.create( 220, 40, 900, 600, m_hWnd, NULL );
		}
		break;
	default:
		break;
	}
	
}

void YBPage2::_onWM_RBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_RBUTTONDOWN );
	m_test4FSMenu.trackPopupMenu( m_hWnd );
}

void YBPage2::loadImage() {
	TCHAR szModulePath[MAX_PATH];
	TCHAR* posTmp = NULL;
	unsigned int idImage;

	GetModuleFileName( NULL, szModulePath, MAX_PATH );
	std::basic_string< TCHAR > tstrImageDirPath;
	std::basic_string< TCHAR > tstrImageFilePath;

	posTmp = _tcsrchr( szModulePath, _T('\\') );
	if( !posTmp ){
		assert(false);
		return;
	}
	posTmp += 1;
	*posTmp = 0;

	tstrImageDirPath = szModulePath;
	tstrImageDirPath += _T("Image\\");

	tstrImageFilePath = tstrImageDirPath + _T("YBPage2Bk.png");
	setBkImage( tstrImageFilePath );
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBStaticBK.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uMenuBkImageId = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWScrollBarBk.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBarBk = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWTArrowNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrTArrNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWTArrowHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrTArrHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWTArrowPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrTArrPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWBArrowNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBArrNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWBArrowHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBArrHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWBArrowPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBArrPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWTArrowPreNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrTArrPreNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWBArrowPreNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBArrPreNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWScrollBarNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrFTNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWScrollBarHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrFTHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWScrollBarPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrFTPre = idImage;
}

HWND YBPage2::create( int x, int y, int nWidth, int nHeight, HWND hWndParent ){
	HWND hWnd;
	hWnd = YBTopMostBar::create( x, y, nWidth, nHeight, hWndParent, NULL );
	//
	if ( hWnd ) {
		m_btnTestModalDlg.setHovTxtClr( RGB(0, 0,0) );
		m_btnTestModalDlg.setFontId( m_uFontIdBtn );
		m_btnTestModalDlg.setText( _T("打开模态对话框") );
		m_btnTestModalDlg.Create( 0, WS_CHILD|WS_VISIBLE, 0, 0, 160, 30, hWnd, (HMENU)YBCTRLID_BTN_OPENDOMODALDLG, NULL );

		m_btnTestMsgBox.setFontId( m_uFontIdBtn );
		m_btnTestMsgBox.setText( _T("打开MsgBox") );
		m_btnTestMsgBox.Create( 0, WS_CHILD|WS_VISIBLE, 0, 50, 160, 30, hWnd, (HMENU)YBCTRLID_BTN_MSGBOX, NULL );

		m_btnTestPopupPB.setFontId( m_uFontIdBtn );
		m_btnTestPopupPB.setText( _T("打开弹出式进度条") );
		m_btnTestPopupPB.Create( 0, WS_CHILD|WS_VISIBLE, 0, 100, 160, 30, hWnd, (HMENU)YBCTRLID_BTN_POPUPPB, NULL );
		
		m_btnTestFileTPDlg.setFontId( m_uFontIdBtn );
		m_btnTestFileTPDlg.setText( _T("打开上传进度条对话框") );
		m_btnTestFileTPDlg.Create( 0, WS_CHILD|WS_VISIBLE, 0, 150, 160, 30, hWnd, (HMENU)YBCTRLID_BTN_FILETPDLG, NULL );

		m_btnTestFSDownLoader.setFontId( m_uFontIdBtn );
		m_btnTestFSDownLoader.setText( _T("打开全屏下载器对话框") );
		m_btnTestFSDownLoader.Create( 0, WS_CHILD|WS_VISIBLE, 0, 200, 160, 30, hWnd, (HMENU)YBCTRLID_BTN_FSDOWNLOADER, NULL );

		m_btnTestWebEditDlg.setFontId( m_uFontIdBtn );
		m_btnTestWebEditDlg.setText( _T("打开网页编辑对话框") );
		m_btnTestWebEditDlg.Create( 0, WS_CHILD|WS_VISIBLE, 0, 250, 160, 30, hWnd, (HMENU)YBCTRLID_BTN_WEBEDITDLG, NULL );

		m_btnTestInstallFileDlg.setFontId( m_uFontIdBtn );
		m_btnTestInstallFileDlg.setText( _T("打开安装文件列表") );
		m_btnTestInstallFileDlg.Create( 0, WS_CHILD|WS_VISIBLE, 0, 300, 160, 30, hWnd, (HMENU)YBCTRLID_BTN_INSTALLFILEDLG, NULL );

		m_btnTestUpLoadWarmTipDlg.setFontId( m_uFontIdBtn );
		m_btnTestUpLoadWarmTipDlg.setText( _T("打开上传的温馨提示界面") );
		m_btnTestUpLoadWarmTipDlg.Create( 0, WS_CHILD|WS_VISIBLE, 0, 350, 160, 30, hWnd, (HMENU)YBCTRLID_BTN_UPLOADWARMTIPDLG, NULL );

		m_btnTestYBPopupMB.setFontId( m_uFontIdBtn );
		m_btnTestYBPopupMB.setText( _T("打开带门板的MessageBox.") );
		m_btnTestYBPopupMB.Create( 0, WS_CHILD|WS_VISIBLE, 0, 400, 160, 30, hWnd, (HMENU)YBCTRLID_BTN_POPUPMESSAGEBOX, NULL );

		m_btnTestDPCDlg.setFontId( m_uFontIdBtn );
		m_btnTestDPCDlg.setText( _T( "打开\r检测电脑环境\r对话框." ) );
		m_btnTestDPCDlg.Create( 0, WS_CHILD | WS_VISIBLE, 0, 450, 160, 60, hWnd, (HMENU)YBCTRLID_BTN_DETECTPCENVIRONMENT, NULL );

		m_btnTestImageEditor.setFontId( m_uFontIdBtn );
		m_btnTestImageEditor.setText( _T( "打开\r图片编辑器\r对话框." ) );
		m_btnTestImageEditor.Create( 0, WS_CHILD | WS_VISIBLE, 0, 530, 160, 60, hWnd, (HMENU)YBCTRLID_BTN_IMAGEEDITOR, NULL );
		//
		::SetTimer( hWnd, YBPAGE_TIMERID_POPUPPB, 100, NULL );
		//
		m_test4Splitter.Create( hWnd, 400, 30, 1000, 5, 100, 250, YBCTRLID_SPLITTER );

	//	m_nwScrollBar.setHWND( hWnd );
	}
	::ShowWindow( hWnd, SW_SHOWNORMAL );
	return hWnd;
}

void YBPage2::_init(){

	m_rcNWScrBarBk.bottom = 716;
	m_rcNWScrBarBk.top = 0;

	m_rcNWScrBarBk.left = 950;
	m_rcNWScrBarBk.right = 973;

	m_nwScrollBar.setFTCurHeight( 30 );

	m_nwScrollBar.setCtrlID( YBNWSCROLLBAR_TARROW_ID, YBNWSCROLLBAR_BAROOW_ID );
	m_nwScrollBar.setRectBk( m_rcNWScrBarBk );
	m_nwScrollBar.setImageIdBk( m_uImageIdScrBarBk );
	m_nwScrollBar.setImageIdTArrow(YBNWScrollBar::ESARROW_STATUS_PRENORMAL, m_uImageIdScrTArrPreNor );
	m_nwScrollBar.setImageIdTArrow( YBNWScrollBar::ESARROW_STATUS_HOVER, m_uImageIdScrTArrHov );
	m_nwScrollBar.setImageIdTArrow( YBNWScrollBar::ESARROW_STATUS_NORMAL, m_uImageIdScrTArrNor );
	m_nwScrollBar.setImageIdTArrow( YBNWScrollBar::ESARROW_STATUS_PRESS, m_uImageIdScrTArrPre );
	m_nwScrollBar.setImageIdBArrow( YBNWScrollBar::EEARROW_STATUS_PRENORMAL, m_uImageIdScrBArrPreNor );
	m_nwScrollBar.setImageIdBArrow( YBNWScrollBar::EEARROW_STATUS_HOVER, m_uImageIdScrBArrHov );
	m_nwScrollBar.setImageIdBArrow( YBNWScrollBar::EEARROW_STATUS_NORMAL, m_uImageIdScrBArrNor );
	m_nwScrollBar.setImageIdBArrow( YBNWScrollBar::EEARROW_STATUS_PRESS, m_uImageIdScrBArrPre );
	m_nwScrollBar.setImageIdFT( YBNWScrollBar::EFT_STATUS_NORMAL, m_uImageIdScrFTNor );
	m_nwScrollBar.setImageIdFT( YBNWScrollBar::EFT_STATUS_HOVER, m_uImageIdScrFTHov );
	m_nwScrollBar.setImageIdFT( YBNWScrollBar::EFT_STATUS_PRESS, m_uImageIdScrFTPre );
}

void YBPage2::loadFont() {
	LOGFONT lFont;

	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 22;
	m_fontIdMenuTxt = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 0, 0 ) );
	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 19;
	m_uFontIdBtn = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );
}
