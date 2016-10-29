#include "stdafx.h"
#include "PictureCtrlDlg.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlMisc.h"
#include "loader/YBImageLoader.h"

#define PICTURE_FORMAT_PNG ( _T(".png") )
#define PICTURE_FORMAT_BMP (_T( ".bmp" ) )
#define PICTURE_FORMAT_JPG ( _T( ".jpg" ) )
USING_NAMESPACE_YBCTRL
unsigned int PictureCtrlDlg::s_index = 0;

PictureCtrlDlg::PictureCtrlDlg( const tstring_type& tstrAppPath )
: m_tstrAppPath( tstrAppPath )
, m_dlg4PicCtrl( tstrAppPath ) {
	_loadImage();
	string str = "iVBORw0KGgoAAAANSUhEUgAAAAkAAAAJCAYAAADgkQYQAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAIGNIUk0AAHolAACAgwAA+f8AAIDpAAB1MAAA6mAAADqYAAAXb5JfxUYAAACySURBVHjadJAxDkVAAESfjYRC6LaQuI1SROEUCq1rKZwBF5CoaDSr2NAoRPgd+fn+tPNmMhkjz/PF932PP5qmaTU9zxNFUfyYx3HQti1lWRrmW/o8T6qqYts2AMQb1DQN8zxzXdcD7ftOXdf0fY9Siq7rkFKSJAkAJoBSimEYGMcR13WRUhLHMUKIpykIAtI0xbIstNaEYXgDdxOAbdtEUYTWGsdxvjYaWZYtwN+fgPUzAOYSQuQDUZ8OAAAAAElFTkSuQmCC";
	decodeImageData(str);
	TCHAR* szData = _T("http://image2.958shop.com/p/2011/04/02/100430818717860.bmp");
	m_tstrAppPath = _T("E:");
	TCHAR* szName = _T( "Picture" );
// 	if ( !m_download.HttpDownload( szData, path ) ) {
// 		assert( false );
// 	}
	downloadImageByHttp( szData, szName );
	szData = _T( "http://www.ppt123.net/beijing/UploadFiles_8374/201202/2012022812392852.jpg" );
	downloadImageByHttp( szData, szName );
}

PictureCtrlDlg::~PictureCtrlDlg( ) {

}

YBCTRL_WNDMSGMAP_BEGIN( PictureCtrlDlg, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_NCHITTEST, _onWM_NCHITTEST )
YBCTRL_WNDMSGMAP_END()

HWND PictureCtrlDlg::create( int uWidth, int uHeight ) {
	int x;
	int y;
	int nScreenWidth;
	int nScreenHeight;
	HWND hWnd;

	nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
	DWORD dwStyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP;
	x = (nScreenWidth - uWidth) / 2;
	y = (nScreenHeight - uHeight) / 2;
	//
	hWnd = _createHWND(dwExStyle, dwStyle, x, y, uWidth, uHeight, NULL, (HMENU)NULL, NULL, NULL);
	if (hWnd) {
		m_dlg4PicCtrl.create( 0, 30, uWidth, uHeight - 30, hWnd );
		m_dlg4PicCtrl.setBigBmpSpace( 30, 50, 400, 350 );
	}
	return hWnd;
}

void PictureCtrlDlg::_onWM_PAINT(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg) {
	YBCTRLMSG_ENTER_CHECK(WM_PAINT);
	PAINTSTRUCT ps;
	HDC hPaintDC;
	RECT rcClient;

	::GetClientRect(m_hWnd, &rcClient);
	YBCtrlMemDC memDC(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
	_drawBk(memDC.getMemDC());

	hPaintDC = ::BeginPaint(m_hWnd, &ps);
	::BitBlt(hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY);
	::EndPaint(m_hWnd, &ps);
}
void PictureCtrlDlg::_onWM_ERASEBKGND(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg) {
	YBCTRLMSG_ENTER_CHECK(WM_ERASEBKGND);
	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	RECT rcClient;

	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG | YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = 1;

	::GetClientRect(m_hWnd, &rcClient);
	YBCtrlMemDC memDC(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
	_drawBk(memDC.getMemDC());

	::BitBlt(hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY);
}

void PictureCtrlDlg::_onWM_NCHITTEST(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg) {
	YBCTRLMSG_ENTER_CHECK(WM_NCHITTEST);
	pYBCtrlWndMsg->m_dwFlag = (YBCTRLWNDMSG_FLAG_EATMSG | YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = HTCAPTION;
}

ybctrl::YBCtrlImageStatic* PictureCtrlDlg::_getImageStatic(unsigned int uImageId) {
	YBCtrlImageBase* pYBImageBase = NULL;

	if (!YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage(uImageId, &pYBImageBase))
		return NULL;
	if (!pYBImageBase || (pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC))
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

void PictureCtrlDlg::_drawBk(HDC hDC) {
	RECT rcClient;

	::GetClientRect(m_hWnd, &rcClient);
	YBCtrlImageStatic* pYBImage = NULL;
	pYBImage = _getImageStatic(m_uImageIdBk);

	if (!pYBImage){
		return;
	}

	BackGroundBitBlt(hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pYBImage->getMemDC(),
		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0);

}

void PictureCtrlDlg::_loadImage() {
	TCHAR* posTmp = NULL;
	tstring_type tstrImageDirPath;
	tstring_type tstrImageFilePath;
	TCHAR szModulePath[MAX_PATH];
	unsigned int idImage;

	//szModulePath = m_tstrAppPath;
	GetModuleFileName(NULL, szModulePath, MAX_PATH);
	posTmp = _tcsrchr(szModulePath, _T('\\'));
	if (!posTmp){
		assert(false);
		return;
	}
	posTmp += 1;
	*posTmp = 0;

	tstrImageDirPath = szModulePath;
	tstrImageDirPath += _T("Image\\");

	tstrImageFilePath = tstrImageDirPath + _T("YBFileTPDlgBk.png");
	if (!YBImageLoader::getInstance()->loadImage(tstrImageFilePath, idImage)) {
		assert(false);
	}
	m_uImageIdBk = idImage;

}

void PictureCtrlDlg::decodeImageData( string tstrData ) {
	string tstrTmp;
	int OutByte = 0;
	int nlength = 0;

	tstrTmp = m_base64Coder.Decode( tstrData.c_str(), tstrData.length(), OutByte );

	nlength = tstrTmp.length();
	BYTE* buffer = new BYTE[ nlength ];

	for ( int index = 0; index < nlength; ++index ) {
		buffer[ index ] = tstrTmp[ index ];
	}
	m_dlg4PicCtrl.loadImage( buffer, OutByte );
}

void PictureCtrlDlg::downloadImageByFtp( const TCHAR* szUrl, const TCHAR* szName, const TCHAR* pUser, const TCHAR* pPassword ) {
	tstring_type tstrImageDirPath;
	tstring_type tstrImageFilePath;
	tstring_type tstrType;
	tstring_type tstrUrl;
	TCHAR szMsg[100] = { 0 };

	tstrUrl = szUrl;
	tstrImageFilePath = m_tstrAppPath;
	tstrImageFilePath += _T( "\\Image" );
	++s_index;
	if(tstrUrl.find( PICTURE_FORMAT_PNG ) != -1) {
		tstrType = PICTURE_FORMAT_PNG;
	}
	else if(tstrUrl.find( PICTURE_FORMAT_BMP ) != -1) {
		tstrType = PICTURE_FORMAT_BMP;
	}
	else if(tstrUrl.find( PICTURE_FORMAT_JPG ) != -1) {
		tstrType = PICTURE_FORMAT_JPG;
	}
	else {
		tstrType = PICTURE_FORMAT_JPG;
	}
	_stprintf_s( szMsg, _T( "\\%s%d%s" ), szName, s_index, tstrType.c_str() );
	tstrImageFilePath += szMsg;

	while(1) {
		if(_taccess_s( tstrImageFilePath.c_str(), 0 ) != 0)
			break;
		++s_index;
		tstrImageFilePath = m_tstrAppPath;
		tstrImageFilePath += _T( "\\Image" );
		::memset( szMsg, 0, 100 );
		_stprintf_s( szMsg, _T( "\\%s%d%s" ), szName, s_index, tstrType.c_str() );
		tstrImageFilePath += szMsg;
	}

	if(!m_download.FTPdownload( szUrl, pUser, pPassword, tstrImageFilePath.c_str() )) {
		return;
	}
	m_dlg4PicCtrl.loadImage( tstrImageFilePath );
}

void PictureCtrlDlg::downloadImageByHttp( const TCHAR* szUrl, const TCHAR* szName ) {
	tstring_type tstrImageDirPath;
	tstring_type tstrImageFilePath;
	tstring_type tstrType;
	tstring_type tstrUrl;
	TCHAR szMsg[ 100 ] = { 0 };

	tstrUrl = szUrl;
	tstrImageFilePath = m_tstrAppPath;
	tstrImageFilePath += _T( "\\Image" );
	++s_index;
	if ( tstrUrl.find( PICTURE_FORMAT_PNG ) != -1 ) {
		tstrType = PICTURE_FORMAT_PNG;
	} else if(tstrUrl.find( PICTURE_FORMAT_BMP ) != -1) {
		tstrType = PICTURE_FORMAT_BMP;
	} else if(tstrUrl.find( PICTURE_FORMAT_JPG) != -1) {
		tstrType = PICTURE_FORMAT_JPG;
	} else {
		tstrType = PICTURE_FORMAT_JPG;
	}
	_stprintf_s( szMsg, _T("\\%s%d%s"), szName, s_index,tstrType.c_str() );
	tstrImageFilePath += szMsg;

	while(1) {
		if( _taccess_s( tstrImageFilePath.c_str(), 0 ) != 0)
			break;
		++s_index;
		tstrImageFilePath = m_tstrAppPath;
		tstrImageFilePath += _T( "\\Image" );
		::memset( szMsg, 0, 100 );
		_stprintf_s( szMsg, _T( "\\%s%d%s" ), szName, s_index, tstrType.c_str() );
		tstrImageFilePath += szMsg;
	}
	if( !m_download.HttpDownload( szUrl, tstrImageFilePath.c_str() ) ) {
		assert( false );
		return;
	}
	m_dlg4PicCtrl.loadImage( tstrImageFilePath );
}