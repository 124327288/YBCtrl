#include "stdafx.h"
#include "YBPage4.h"
#include "button\YBButton.h"
#include "frame\YBCtrlImageDepository.h"
#include "frame\YBCtrlDefs.h"
#include "frame\YBCtrlImageStatic.h"
#include "frame\YBCtrlImageBase.h"
#include "frame\YBCtrlMisc.h"
#include "frame\YBCtrlMemDC.h"
#include "loader\YBImageLoader.h"
#include "gdiplus.h"
#include <assert.h>

#define YBCTRL_CHECK_BOX (280)
USING_NAMESPACE_YBCTRL



YBPage4::YBPage4()
{
	loadImage();
}

YBPage4::~YBPage4(){

}

YBCTRL_WNDMSGMAP_BEGIN( YBPage4, YBTopMostBar );
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT );
YBCTRL_WNDMSGMAP_END()


	void YBPage4::loadImage() {
		TCHAR szModulePath[MAX_PATH];
		TCHAR* posTmp = NULL;
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

		tstrImageFilePath = tstrImageDirPath + _T("YBPage4Bk.png");
		setBkImage( tstrImageFilePath );

}

HWND YBPage4::create( int x, int y, int nWidth, int nHeight, HWND hWndParent ){
	HWND hWnd;

	hWnd = YBTopMostBar::create( x, y, nWidth, nHeight, hWndParent, NULL );
	if ( hWnd ) {

	}
	return hWnd;
}
