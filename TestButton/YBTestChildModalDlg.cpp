#include "stdafx.h"
#include "YBTestChildModalDlg.h"
#include "loader/YBImageLoader.h"
#include <assert.h>

YBTestChildModalDlg::YBTestChildModalDlg( bool bAutoExit , unsigned int uExitTimeMS )
: YBChildModalDialog( bAutoExit, uExitTimeMS ){
	loadImage();
}

YBTestChildModalDlg::~YBTestChildModalDlg() {

}

void YBTestChildModalDlg::_createUI( HWND hWndModalDlg, HINSTANCE hInst ) {

	
}

void YBTestChildModalDlg::loadImage() {
	TCHAR szModulePath[MAX_PATH];
	TCHAR* posTmp = NULL;
	GetModuleFileName( NULL, szModulePath, MAX_PATH );
	std::basic_string< TCHAR > tstrImageDirPath;
	std::basic_string< TCHAR > tstrImageFilePath;
	unsigned int uImageId;

	posTmp = _tcsrchr( szModulePath, _T('\\') );
	if( !posTmp ){
		assert(false);
		return;
	}
	posTmp += 1;
	*posTmp = 0;

	tstrImageDirPath = szModulePath;
	tstrImageDirPath += _T("Image\\");

	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBk.png");
	if( !ybctrl::YBImageLoader::getInstance()->loadImage( tstrImageFilePath, uImageId ) ) {
		assert( false );
		return;
	}
	setBKImageId( uImageId, 0, 30, 0, 0 );
	
}

unsigned int YBTestChildModalDlg::_getWndHeight() const {
	return 400;
}

unsigned int YBTestChildModalDlg::_getWndWidth() const {
	return 600;
}