#include "stdafx.h"
#include "YBITDlgFileUpLoadRaw.h"
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
#include "YBITMsgBox.h"
#include "gdiplus.h"
#include <assert.h>

#define YBITDLG_FILE_UPLOAD (258)
#define YBITDLG_BTN_CLOSE (259)
#define YBITDLG_BTN_SAVE (260)
#define YBITDLG_BTN_CANCEL (261)
#define YBITDLG_CHECK_FILEOF (262)
#define YBITDLG_CHECK_DATABANK (263)
#define YBITDLG_STATIC_TITTLE (264)
#define YBITDLG_STATIC_SAVE (265)
//
#define YBITDLG_BTNTEXT_SAVE (_T("日子\n我真的好难过请选择文件存))我真\n的好难过请选择文件保存去处：(系统默认\n“本地电脑”固定保存)"))
#define YBITDLG_BTNTEXT_CANCEL (_T("取  消"))
#define YBITDLG_STATICTEXT_TITTLE (_T("保存文件标题"))
#define YBITDLG_STATICTEXT_SAVE (_T("请选择文件保存去处：(系统默认“本地电脑”固定保存)"))
//
USING_NAMESPACE_YBCTRL

YBITDlgFileUpLoadRaw::YBITDlgFileUpLoadRaw()
: m_uBkImageId(YBCTRL_IMAGE_ID_INVALID)
{
	_loadImage();
	_loadFont();
//	_init();
}

YBITDlgFileUpLoadRaw::~YBITDlgFileUpLoadRaw(){

}

YBCTRL_WNDMSGMAP_BEGIN( YBITDlgFileUpLoadRaw, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_WNDMSGMAP_END()
	
	void YBITDlgFileUpLoadRaw::_onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg ){
		YBCTRLMSG_ENTER_CHECK( WM_PAINT );
		RECT rcClient;
		PAINTSTRUCT ps;
		HDC hPaintDC;

		::GetClientRect( m_hWnd, &rcClient );
		YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
		_drawBk( memDC.getMemDC() );

		hPaintDC = ::BeginPaint( m_hWnd, &ps );
		::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
		::EndPaint( m_hWnd, &ps );

}

void YBITDlgFileUpLoadRaw::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );

	switch( wCtrlId ){
	case YBITDLG_BTN_CLOSE:
		{
			::DestroyWindow( m_hWnd );
		}
		break;
	case YBITDLG_BTN_SAVE:
		{
// 			YBITMsgBox m_testMsgBox( true );
// 			m_testMsgBox.doModal( m_hWnd );
// 			m_testMsgBox.setText( _T(""), _T("对话框"), _T("确定") );
// //			::DestroyWindow( m_hWnd );
				YBCtrlImageStatic* pYBImage = NULL;
				pYBImage = _getBkImageStatic( m_uBkImageId );
				YBCtrlMemDC memDC( pYBImage->getWidth(), pYBImage->getHeight() );
				tstring_type tstrPath = _T("E:\\Code\\YBCtrl\\YBCtrl\\Win32\\Debug\\image\\YBCNType16.png");
				HBITMAP hBmp;
				Gdiplus::Bitmap* pBitMap = Gdiplus::Bitmap::FromFile( tstrPath.c_str() );
				if(!pBitMap) {
					assert( false );
				}
				pBitMap->GetHBITMAP( NULL, &hBmp );
				tstring_type tstrSavePath;
				TCHAR* posTmp = NULL;
				posTmp = (TCHAR*)_tcsrchr( tstrPath.c_str(), _T( '.' ) );
				tstring_type tstrMimeType(posTmp);
				//_tcsstr
				tstrSavePath = _T("C:\\Users\\Administrator\\Desktop\\new123") + tstrMimeType;
				tstrMimeType = tstrMimeType.substr( 1, tstrMimeType.length() -1 );
				tstrMimeType = _T( "image/" ) + tstrMimeType;
				//HBITMAP hBmp = OpenBmpFile( memDC.getMemDC(), _T("E:\\Code\\YBCtrl\\YBCtrl\\Win32\\Debug\\image\\YBITDlgBk.png") );
				//HBITMAP hBmp = OpenBmpFile( memDC.getMemDC(), _T("E:\\Code\\YBCtrl\\YBCtrl\\Win32\\Debug\\image\\DownloadBK.bmp") );
				//
				LOGFONT lFont;
				memset( &lFont, 0, sizeof(LOGFONT) );
				_tcscpy_s( lFont.lfFaceName, _T( "微软雅黑" ) );
				lFont.lfHeight = 22;
				HFONT hFont = ::CreateFontIndirect( &lFont );
				::SelectObject( memDC.getMemDC(), hFont );
				RECT rcText = { 10, 10, 100, 100 };
				RECT rcText2 = { 10, 80, 200, 150};
				::SetBkMode( memDC.getMemDC(), TRANSPARENT );
				//
				RECT rcClient = { 0, 0, 400, 400 };
				tstring_type tstrText = _T( "小马哥" );
				HBRUSH hBrush = ::CreateSolidBrush( RGB(255, 255, 255 ));
				::FillRect( memDC.getMemDC(), &rcClient, hBrush );
				for(int i = 0; i < 2; ++i) {
					//::SelectObject( memDC.getMemDC(), hBmp );
					
					::DrawText( memDC.getMemDC( ), tstrText.c_str(), -1, &rcText, DT_LEFT );
					rcText = rcText2;
					tstrText = _T( "1234" );
				}
// 				::DrawText( memDC.getMemDC( ), _T( "小马哥" ), -1, &rcText, DT_LEFT );
// 				::DrawText( memDC.getMemDC(), _T( "meimei" ), -1, &rcText2, DT_LEFT );
				::DeleteObject( hBrush );
				YBCtrlMemDC memDC2( 315, 315 );
				::BitBlt( memDC2.getMemDC(), 0, 0, 315,  315, memDC.getMemDC(), 0, 0, SRCCOPY );
				if(!_save( memDC2.getBitMap(), tstrMimeType, tstrSavePath )) {
					assert( false );
				}
				//PBITMAPINFO pBmpInfo = CreateBitmapInfoStruct( m_hWnd, hBmp );
				//CreateBMPFile( m_hWnd, _T("C:\\Users\\Administrator\\Desktop\\YBITDlgBk.png"), pBmpInfo, hBmp, memDC.getMemDC() );
				::DeleteObject( hBmp );
				::DeleteObject( hFont );
		}
		break;
	case YBITDLG_BTN_CANCEL:
		{
			::DestroyWindow( m_hWnd );
		}
		break;
	}

}

bool YBITDlgFileUpLoadRaw::_save( HBITMAP hBitmap, tstring_type tstrMimeType, tstring_type tstrFileName ) {
	if(!hBitmap) {
		assert( false );
		return false;
	}
	CLSID imageClsid;
	Gdiplus::Bitmap bmp( hBitmap, NULL );
	if(!_getEncoderClsid( tstrMimeType, &imageClsid )){
		assert( false );
		return false;
	}
	bmp.Save( tstrFileName.c_str(), &imageClsid );

	return true;
}

bool YBITDlgFileUpLoadRaw::_getEncoderClsid( tstring_type tstrMimeType, CLSID* pClsid ) {
	unsigned int uEncoderNum;
	unsigned int uEncodecSize;
	Gdiplus::ImageCodecInfo* pCodecInfo;
	Gdiplus::GetImageEncodersSize( &uEncoderNum, &uEncodecSize );
	if( uEncodecSize < 0 ) {
		assert( false );
		return false;
	}
	pCodecInfo = new Gdiplus::ImageCodecInfo[ uEncodecSize ];
	if(!pCodecInfo){
		assert( false );
		return false;
	}
	Gdiplus::GetImageEncoders( uEncoderNum, uEncodecSize, pCodecInfo );
	for(unsigned int uIndex = 0; uIndex < uEncoderNum; ++uIndex) {
		if(_tcscmp( pCodecInfo[uIndex].MimeType, tstrMimeType.c_str() )) {
			*pClsid = pCodecInfo[uIndex].Clsid;
			delete[] pCodecInfo;
			return true;
		}
	}
	delete[] pCodecInfo;
	return false;
}

HWND YBITDlgFileUpLoadRaw::Create(  int x, int y, int nWidth, int nHeight, HWND hWndParent, HINSTANCE hInstance ){
	HWND hWnd;
	HWND hWndBtnX;
	HWND hWndBtnSave;
	HWND hWndBtnCancel;
	DWORD dwExStyle;
	DWORD dwStyle;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	dwExStyle = 0;
	dwStyle = WS_VISIBLE|WS_CHILD;

	hWnd = _createHWND( dwExStyle, dwStyle, x, y, nWidth, nHeight, hWndParent, (HMENU)YBITDLG_FILE_UPLOAD, hInstance, NULL );
	if ( hWnd ){
		hWndBtnX = m_btnClose.Create( 0, WS_CHILD|WS_VISIBLE, 400, 7, 14, 14, hWnd,
			(HMENU)YBITDLG_BTN_CLOSE, hInstance );
 		m_btnClose.setFixedSpace( 1, 1, 1, 1 );
		m_btnClose.setImage( ybctrl::YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnXNor );
		m_btnClose.setImage( ybctrl::YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnXHov );
		m_btnClose.setImage( ybctrl::YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnXPre );
		// 保存按钮
		hWndBtnSave = m_btnSave.Create( 0, WS_VISIBLE|WS_CHILD, 142, 150, 68, 31, hWnd, (HMENU)YBITDLG_BTN_SAVE, hInstance );
		m_btnSave.setFixedSpace( 5, 2, 2, 2 );
		m_btnSave.setImage( ybctrl::YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnSaveNor );
		m_btnSave.setImage( ybctrl::YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnSaveHov );
		m_btnSave.setImage( ybctrl::YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnSavePre );
		m_btnSave.setFontId( m_uFontIdBtn );
		m_btnSave.setTextSpace( 2, 2, 2, 2 );
		m_btnSave.setText( YBITDLG_BTNTEXT_SAVE );
		
		// 取消按钮
		hWndBtnCancel = m_btnCancel.Create( 0, WS_CHILD|WS_VISIBLE, 216, 150, 68, 31, hWnd, (HMENU)YBITDLG_BTN_CANCEL, hInstance );
		m_btnCancel.setFixedSpace( 5, 2, 2, 2 );
		m_btnCancel.addToolTip( false );
		m_btnCancel.setImage( ybctrl::YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnCancelNor );
		m_btnCancel.setImage( ybctrl::YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnCancelHov );
		m_btnCancel.setImage( ybctrl::YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnCancelPre );
		m_btnCancel.setFontId( m_uFontIdBtn );
		m_btnCancel.setTextSpace( 2, 2, 2, 2 );
		m_btnCancel.setText( YBITDLG_BTNTEXT_CANCEL );

		// checkbox 文件大全
		m_checkFileOf.Create( 0, WS_VISIBLE|WS_CHILD, 32, 100, 120, 40, hWnd, (HMENU)YBITDLG_CHECK_FILEOF, hInstance );
		m_checkFileOf.setTextSpace( 18, 2, 5, 2 );
		m_checkFileOf.addToolTip( true );
		m_checkFileOf.setText( _T("文件大全"));
		m_checkFileOf.setFontId( m_uFontIdCheckBox );
		m_checkFileOf.setBKImageID( m_uImageIdCheckBk );
		m_checkFileOf.setImage( YBCheckBox::ENOCHECK_STATUS_NORMAL, m_uImageIdNoCheckNor );
		m_checkFileOf.setImage( YBCheckBox::ENOCHECK_STATUS_HOVER, m_uImageIdNoCheckHov );
		m_checkFileOf.setImage( YBCheckBox::ENOCHECK_STATUS_PRESS, m_uImageIdNoCheckPre );
		m_checkFileOf.setImage( YBCheckBox::ECHECK_STATUS_NORMAL, m_uImageIdCheckNor );
		m_checkFileOf.setImage( YBCheckBox::ECHECK_STATUS_HOVER, m_uImageIdCheckHov );
		m_checkFileOf.setImage( YBCheckBox::ECHECK_STATUS_PRESS, m_uImageIdCheckPre );

		// checkbox 数据银行
		m_checkDataBank.Create( 0, WS_VISIBLE|WS_CHILD, 150, 100, 120, 40, hWnd, (HMENU)YBITDLG_CHECK_FILEOF, hInstance );
		m_checkDataBank.setTextSpace( 18, 2, 5, 2 );
		m_checkDataBank.addToolTip( true );
		m_checkDataBank.setText( _T("数据银行"));
		m_checkDataBank.setFontId( m_uFontIdCheckBox );
		m_checkDataBank.setBKImageID( m_uImageIdCheckBk );
		m_checkDataBank.setImage( YBCheckBox::ENOCHECK_STATUS_NORMAL, m_uImageIdNoCheckNor );
		m_checkDataBank.setImage( YBCheckBox::ENOCHECK_STATUS_HOVER, m_uImageIdNoCheckHov );
		m_checkDataBank.setImage( YBCheckBox::ENOCHECK_STATUS_PRESS, m_uImageIdNoCheckPre );
		m_checkDataBank.setImage( YBCheckBox::ECHECK_STATUS_NORMAL, m_uImageIdCheckNor );
		m_checkDataBank.setImage( YBCheckBox::ECHECK_STATUS_HOVER, m_uImageIdCheckHov );
		m_checkDataBank.setImage( YBCheckBox::ECHECK_STATUS_PRESS, m_uImageIdCheckPre );

		// static text 文件标题
		m_staticTxtTittle.Create( 0, WS_CHILD|WS_VISIBLE, 40, 50, 100, 35, hWnd, (HMENU)YBITDLG_STATIC_TITTLE, hInstance );
		m_staticTxtTittle.setFontId( m_uFontIdCheckBox );
		m_staticTxtTittle.setImage( m_uImageIdCheckBk );
		m_staticTxtTittle.setText( YBITDLG_STATICTEXT_TITTLE );
		// static text 文件保存去处
		m_staticTxtSave.Create( 0, WS_CHILD|WS_VISIBLE, 40, 75, 350, 35, hWnd, (HMENU)YBITDLG_STATIC_SAVE, hInstance );
		m_staticTxtSave.setFontId( m_uFontIdCheckBox );
		m_staticTxtSave.setImage( m_uImageIdCheckBk );
		m_staticTxtSave.setText( YBITDLG_STATICTEXT_SAVE );
		
	}
	::ShowWindow( hWnd, SW_NORMAL );
	return hWnd;
}

YBCtrlImageBase* YBITDlgFileUpLoadRaw::_getBkImageBase( unsigned int uBkImageId ){
	YBCtrlImageBase* pYBImage = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( m_uBkImageId, &pYBImage ) )
		return NULL;

	return pYBImage;

}

YBCtrlImageStatic* YBITDlgFileUpLoadRaw::_getBkImageStatic( unsigned int uBkImageId ){
	YBCtrlImageBase* pYBImageBase = NULL;
	pYBImageBase = _getBkImageBase( uBkImageId );

	if ( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;

}

void YBITDlgFileUpLoadRaw::_drawBk( HDC hDC ){
	if( !getHWND() )
		return;

	RECT rcClient;
	::GetClientRect( m_hWnd, &rcClient );

	YBCtrlImageStatic* pYBImage = NULL;
	YBCtrlImageStatic* pYBTitleImage = NULL;
	pYBImage = _getBkImageStatic( m_uBkImageId );
	pYBTitleImage = _getTitleBkImageStatic( m_uTitleBkImageId );
	//
	BackGroundBitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pYBImage->getMemDC(),
		0, 0, pYBImage->getWidth(), pYBImage->getHeight(),2, 2, 2, 2 );
	//
	BackGroundBitBlt( hDC, 0, 0, rcClient.right - rcClient.left, 30, pYBTitleImage->getMemDC(),
		0, 0, pYBTitleImage->getWidth(), pYBTitleImage->getHeight(), 3, 3, 3, 3 );
}

// ybctrl::YBButtonX* YBITDlgFileUpLoadRaw::getBtnXTitle(){
// 	return &m_btnClose;
// }
// 
// ybctrl::YBButton* YBITDlgFileUpLoadRaw::getBtnSave(){
// 	return &m_btnSave;
// }
// 
// ybctrl::YBButton* YBITDlgFileUpLoadRaw::getBtnCancel(){
// 	return &m_btnCancel;
// }

void YBITDlgFileUpLoadRaw::_loadImage(){

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
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBk.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uBkImageId = idImage;
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
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBtnSaveNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnSaveNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBtnSaveHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnSaveHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBtnSavePre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnSavePre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBtnCancelPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnCancelPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBtnCancelNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnCancelNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBtnCancelHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnCancelHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgTitleBk.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uTitleBkImageId = idImage;
	// Check box
	tstrImageFilePath = tstrImageDirPath + _T("YBCheckBoxBk.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdCheckBk = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCheckNormal.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdCheckNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCheckHover.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdCheckHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCheckPress.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdCheckPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNoCheckNormal.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdNoCheckNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNoCheckHover.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdNoCheckHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNoCheckPress.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdNoCheckPre = idImage;
	
}

void YBITDlgFileUpLoadRaw::_loadFont(){
	LOGFONT lFont;

	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 22;
	lFont.lfWeight = 700;
	m_uFontIdBtn = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );
	//check box
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 20;
	m_uFontIdCheckBox = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 0, 0 ) );

}

YBCtrlImageBase* YBITDlgFileUpLoadRaw::_getTitleBkImageBase( unsigned int uBkImageId ){
	YBCtrlImageBase* pYBImage = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( m_uTitleBkImageId, &pYBImage ) )
		return NULL;

	return pYBImage;

}

YBCtrlImageStatic* YBITDlgFileUpLoadRaw::_getTitleBkImageStatic( unsigned int uBkImageId ){
	YBCtrlImageBase* pYBImageBase = NULL;
	pYBImageBase = _getTitleBkImageBase( uBkImageId );

	if ( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;

}