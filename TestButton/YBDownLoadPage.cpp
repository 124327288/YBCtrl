#include "stdafx.h"
#include "YBDownLoadPage.h"
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

YBDownLoadPage::YBDownLoadPage(){
	_loadImage();
//	_loadFont();
}

YBDownLoadPage::~YBDownLoadPage(){

}

YBCTRL_WNDMSGMAP_BEGIN( YBDownLoadPage, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_WNDMSGMAP_END()

void YBDownLoadPage::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
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

HWND YBDownLoadPage::create( int x, int y, int nWidth, int nHeight, HWND hWndParent ){
	HWND hWnd;

	hWnd = _createHWND( 0, WS_CHILD|WS_VISIBLE, x, y, nWidth, nHeight, hWndParent, (HMENU)YBDOWNLOADPAGE_WND_ID, NULL, NULL );
	if( hWnd ){

	}
	return hWnd;

}

void YBDownLoadPage::_drawBk( HDC hDC ){
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

void YBDownLoadPage::_init(){


}

YBCtrlImageStatic* YBDownLoadPage::_getImageStatic( unsigned int uImageId ){
	YBCtrlImageBase* pYBImageBase = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pYBImageBase ) )
		return NULL;
	if( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;

}

void YBDownLoadPage::_loadImage(){
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
	tstrImageFilePath = tstrImageDirPath + _T("DownloadBK.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdBk = idImage;
}