#include "stdafx.h"
#include "YBTransitCenterDlg.h"
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

USING_NAMESPACE_YBCTRL

YBTransitCenterDlg::YBTransitCenterDlg(){
	_loadImage();
	_init();
//	_loadFont();
}

YBTransitCenterDlg::~YBTransitCenterDlg(){

}

HWND YBTransitCenterDlg::create( int x, int y, int nWidth, int nHeight, HWND hWndParent ){
	HWND hWnd;
	HWND hWndBtnXUpLoad;
	HWND hWndBtnXDownLoad;
	HWND hWndUpLoadPage;
	HWND hWndDownLoadPage;

	hWnd = _createHWND( 0, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, x, y, nWidth, nHeight, hWndParent, (HMENU)YBTRACENDLG_BODY_ID, NULL, NULL );
	if( hWnd ){
		//download Button
		hWndBtnXDownLoad = m_btnXDownLoad.Create( 0, WS_VISIBLE|WS_CHILD, 360, 100, 100, 30, hWnd, (HMENU)YBTRACENDLG_BTNDOWNLOAD_ID, NULL );
		if( hWndBtnXDownLoad ){
			hWndDownLoadPage = m_downloadPage.create( 0, 130, nWidth, nHeight, hWnd );
			::ShowWindow( hWndDownLoadPage, SW_NORMAL );
		}
		m_btnXDownLoad.enableBtnX( false );
		//upload Button
		hWndBtnXUpLoad = m_btnXUpLoad.Create( 0, WS_CHILD|WS_VISIBLE, 50, 100, 100, 30, hWnd, (HMENU)YBTRACENDLG_BTNUPLOAD_ID, NULL );
		if( hWndBtnXUpLoad ){
			hWndUpLoadPage = m_uploadPage.create( 0, 130, nWidth, nHeight, hWnd );
			::ShowWindow( hWndUpLoadPage, SW_HIDE );
		}
		m_btnXUpLoad.enableBtnX( false );
	}
	return hWnd;

}

YBCTRL_WNDMSGMAP_BEGIN( YBTransitCenterDlg, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
YBCTRL_WNDMSGMAP_END()

void YBTransitCenterDlg::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	RECT rcClient;
	HDC hPaintDC;
	PAINTSTRUCT ps;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );

}

void YBTransitCenterDlg::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );

	int wmId;
	int wmEvent;

	wmId = LOWORD( pYBCtrlWndMsg->m_wParam );
	wmEvent = HIWORD( pYBCtrlWndMsg->m_wParam );

	switch( wmId ){
	case YBTRACENDLG_BTNDOWNLOAD_ID:
		{
			m_btnXUpLoad.popUp();
			::ShowWindow( m_uploadPage.getHWND(), SW_NORMAL );
			::ShowWindow( m_downloadPage.getHWND(), SW_HIDE );

		}
		break;
	case YBTRACENDLG_BTNUPLOAD_ID:
		{
			m_btnXDownLoad.popUp();
			::ShowWindow( m_downloadPage.getHWND(), SW_NORMAL );
			::ShowWindow( m_uploadPage.getHWND(), SW_HIDE );
		}
		break;
	default:
		break;
	}

}

void YBTransitCenterDlg::_drawBk( HDC hDC ){
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlImageStatic* pYBImage = NULL;
	pYBImage = _getImageStatic( m_uImageIdBk );

	if( !pYBImage ){
		assert(false);
		return;
	}
	BackGroundBitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pYBImage->getMemDC(),
		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 2, 2, 2, 2 );

}

void YBTransitCenterDlg::_drawStatus( HDC hDC ){
	_drawBk( hDC );
}

void YBTransitCenterDlg::_init(){
	//
	m_btnXUpLoad.setImage( YBButtonX::EBTN_STATUS_NORMAL, m_uImageIdBtnNor );
	m_btnXUpLoad.setImage( YBButtonX::EBTN_STATUS_HOVER, m_uImageIdBtnHov );
	m_btnXUpLoad.setImage( YBButtonX::EBTN_STATUS_PRESS, m_uImageIdBtnPre );
	m_btnXUpLoad.setImage( YBButtonX::EBTNX_STATUS_NORMAL, m_uImageIdBtnXNor );
	m_btnXUpLoad.setImage( YBButtonX::EBTNX_STATUS_HOVER, m_uImageIdBtnXHov );
	m_btnXUpLoad.setImage( YBButtonX::EBTNX_STATUS_PRESS, m_uImageIdBtnXPre );
	//
	m_btnXDownLoad.setImage( YBButtonX::EBTN_STATUS_NORMAL, m_uImageIdBtnNor );
	m_btnXDownLoad.setImage( YBButtonX::EBTN_STATUS_HOVER, m_uImageIdBtnHov );
	m_btnXDownLoad.setImage( YBButtonX::EBTN_STATUS_PRESS, m_uImageIdBtnPre );
	m_btnXDownLoad.setImage( YBButtonX::EBTNX_STATUS_NORMAL, m_uImageIdBtnXNor );
	m_btnXDownLoad.setImage( YBButtonX::EBTNX_STATUS_HOVER, m_uImageIdBtnXHov );
	m_btnXDownLoad.setImage( YBButtonX::EBTNX_STATUS_PRESS, m_uImageIdBtnXPre );
}

YBCtrlImageStatic* YBTransitCenterDlg::_getImageStatic( unsigned int uImageId ){
	YBCtrlImageBase* pYBImageBase = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pYBImageBase ) )
		return NULL;
	if( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;

}

void YBTransitCenterDlg::_loadImage(){
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
	tstrImageFilePath = tstrImageDirPath + _T("Gray.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdBk = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("x_normal.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdBtnXNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("x_hover.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdBtnXHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("x_press.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdBtnXPre = idImage;
	//
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
	m_uImageIdBtnHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBBtnPress.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdBtnPre = idImage;
}