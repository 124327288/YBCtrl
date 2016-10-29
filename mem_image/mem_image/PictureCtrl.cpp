#include "stdafx.h"
#include "PictureCtrl.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlMisc.h"
#include "loader/YBImageLoader.h"

#define PICTURE_STATIC_ID (300)
USING_NAMESPACE_YBCTRL

PictureCtrl::PictureCtrl( const tstring_type& tstrAppPath )
: m_tstrAppPath( tstrAppPath )
, m_uImageBigBmp( 0 )
, m_uImageIdBk( 0 ) {
	_loadImage();
	::memset( &m_rcBigBmp, 0, sizeof( RECT ) );
}

PictureCtrl::~PictureCtrl() {

}

YBCTRL_WNDMSGMAP_BEGIN(PictureCtrl, YBCtrlBase)
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_LBUTTONUP )
YBCTRL_WNDMSGMAP_END()

HWND PictureCtrl::create( int x, int y, int uWidth, int uHeight, HWND hWndParent ) {
	HWND hWnd;
	DWORD dwStyle =  WS_CHILD|WS_VISIBLE;
	//
	hWnd = _createHWND(NULL, dwStyle, x, y, uWidth, uHeight, hWndParent, (HMENU)321, NULL, NULL);
	if ( !hWnd ) {
		assert( false );
		return NULL;
	}
	return hWnd;
}

void PictureCtrl::setBigBmpSpace( unsigned int uLSpace, unsigned int uTSpace, unsigned int uWidth, unsigned int uHeight ) {
	RECT rcClient;
	unsigned int uClientWidth;
	unsigned int uClientHeight;
	YBCtrlImageStatic* pYBImage = NULL;

	::GetClientRect( m_hWnd, &rcClient );
	uClientWidth = rcClient.right - rcClient.left;
	uClientHeight = rcClient.bottom - rcClient.top;
	//
	unsigned int uImageBkRight = min( uClientWidth, (uLSpace + uWidth)*uClientWidth/100 );
	unsigned int uImageBkBottom = min( uClientHeight, (uTSpace + uHeight)*uClientHeight/100 );
	::SetRect( &m_rcBigBmpBk, uLSpace*uClientWidth/100, uTSpace*uClientHeight/100, uImageBkRight, uImageBkBottom );
}

void PictureCtrl::_onWM_PAINT(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	PAINTSTRUCT ps;
	HDC hPaintDC;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}
void PictureCtrl::_onWM_ERASEBKGND(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	RECT rcClient;

	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = 1;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
}

void PictureCtrl::_onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
	POINT ptMouse;
	HDC hDC;
	RECT rcClient;

	ptMouse.x = LOWORD(pYBCtrlWndMsg->m_lParam);
	ptMouse.y = HIWORD(pYBCtrlWndMsg->m_lParam);

	if (!::PtInRect(&m_rcBigBmp, ptMouse))
		return;
	m_ptMouse.x = ptMouse.x;
	m_ptMouse.y = ptMouse.y;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
}

void PictureCtrl::_onWM_LBUTTONUP( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	//YBCTRLMSG_ENTER_CHECK( WM_LBUTTONUP );


}

void PictureCtrl::_drawStatus( HDC hDC ) {
	_drawBk( hDC );
	_drawBigBmp( hDC );
	_drawZoomBmp( hDC );
}

void PictureCtrl::_drawBigBmp( HDC hDC ) {
	RECT rcBigBmp;
	YBCtrlImageStatic* pYBImage = NULL;

	if ( !_getBigBmpRect( rcBigBmp ) )
		return;
	pYBImage = _getImageStatic(m_uImageId1);
	if (!pYBImage)
		return;

	Gdiplus::Graphics graphics(hDC);
	GdipBitBlt(&graphics, pYBImage->getImage(), m_rcBigBmp.left, m_rcBigBmp.top, m_rcBigBmp.right - m_rcBigBmp.left, m_rcBigBmp.bottom - m_rcBigBmp.top
		, 0, 0, pYBImage->getWidth(), pYBImage->getHeight());
}

void PictureCtrl::_drawZoomBmp( HDC hDC ) {
	YBCtrlImageStatic* pYBImage = NULL;

	pYBImage = _getImageStatic( m_uImageBigBmp );
	if ( !pYBImage )
		return;
	
	Gdiplus::Graphics graphics(hDC);
	GdipBitBlt(&graphics, pYBImage->getImage(), m_ptMouse.x, m_ptMouse.y, 100, 100
		, m_ptMouse.x - m_rcBigBmp.left - 5, m_ptMouse.y - m_rcBigBmp.top - 5, 10, 10);
}

bool PictureCtrl::_getBigBmpRect( RECT& rcBigBmp ) {
	YBCtrlImageStatic* pYBImage = NULL;

	pYBImage = _getImageStatic( m_uImageBigBmp );
	if ( !pYBImage )
		return false;
	if ( ( pYBImage->getWidth() > ( m_rcBigBmpBk.right - m_rcBigBmpBk.left ) ) || ( pYBImage->getHeight() > ( m_rcBigBmpBk.bottom - m_rcBigBmpBk.top ) ) ) {
		m_rcBigBmp = m_rcBigBmpBk;
	} else {
		unsigned int uLeft = m_rcBigBmpBk.left + ( m_rcBigBmpBk.right - m_rcBigBmpBk.left - pYBImage->getWidth() )/2;
		unsigned int uTop = m_rcBigBmpBk.top + ( m_rcBigBmpBk.bottom - m_rcBigBmpBk.top - pYBImage->getHeight() )/2;
		::SetRect( &m_rcBigBmp, uLeft, uTop, uLeft + pYBImage->getWidth(), uTop + pYBImage->getHeight() );
	}
	rcBigBmp = m_rcBigBmp;
	return true;
}

void PictureCtrl::_drawBk(HDC hDC) {
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

ybctrl::YBCtrlImageStatic* PictureCtrl::_getImageStatic(unsigned int uImageId) {
	YBCtrlImageBase* pYBImageBase = NULL;

	if (!YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage(uImageId, &pYBImageBase))
		return NULL;
	if (!pYBImageBase || (pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC))
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

void PictureCtrl::_loadImage() {
	TCHAR* posTmp = NULL;
	tstring_type tstrImageDirPath;
	tstring_type tstrImageFilePath;
	TCHAR szModulePath[MAX_PATH];
	unsigned int idImage;

	//szModulePath = m_tstrAppPath;
	GetModuleFileName( NULL, szModulePath, MAX_PATH );
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
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCNType7.png");
	if (!YBImageLoader::getInstance()->loadImage(tstrImageFilePath, idImage)) {
		assert(false);
	}
	m_uImageBigBmp = idImage;
	
}

void PictureCtrl::loadImage( const void *buf, size_t size ) {
	YBImageLoader::getInstance()->loadImage( buf, size, m_uImageId1 );
}