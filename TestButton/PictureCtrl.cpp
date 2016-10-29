#include "stdafx.h"
#include "PictureCtrl.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlFontDepository.h"
#include "frame/YBCtrlMisc.h"
#include "loader/YBImageLoader.h"

#define ROTATING_BTN_ID (300)
//
#define SMALL_BMP_TOPSPACE ( 30 )
#define SMALL_BMP_RIGHTSPACE ( 60 )
#define SMALL_BMP_SPACE ( 32 )
#define SMALL_BMP_WIDHT ( 150 )
#define SMALL_BMP_HEIGHT ( 150 )

USING_NAMESPACE_YBCTRL

PictureCtrl::PictureCtrl( const tstring_type& tstrAppPath )
: m_tstrAppPath( tstrAppPath )
, m_eRectStatus( ERECT_STATUS_NONE )
, m_bRotating( false )
, m_bShowZoomBmp( false )
, m_uImageBtnNor( 0 )
, m_uImageBtnHov( 0 )
, m_uImageBtnPre( 0 )
, m_uFontIdBtn( 0 )
, m_uImageIdBk( 0 ) {
	_loadImage();
	_loadFont();
	::memset( &m_rcBigBmp, 0, sizeof( RECT ) );
}

PictureCtrl::~PictureCtrl() {

}

YBCTRL_WNDMSGMAP_BEGIN(PictureCtrl, YBCtrlBase)
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_LBUTTONUP )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
YBCTRL_WNDMSGMAP_END()

HWND PictureCtrl::create( int x, int y, int uWidth, int uHeight, HWND hWndParent ) {
	HWND hWnd;
	DWORD dwStyle =  WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	//
	hWnd = _createHWND(NULL, dwStyle, x, y, uWidth, uHeight, hWndParent, (HMENU)321, NULL, NULL);
	if ( hWnd ) {
		m_btnRotating.Create( 0, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 300, 540, 100, 50, hWnd, (HMENU)ROTATING_BTN_ID, NULL );
		m_btnRotating.setText( _T( "Ðý  ×ª" ) );
		m_btnRotating.setTxtVcenter( true );
		m_btnRotating.setFontId( m_uFontIdBtn );
		m_btnRotating.setTextSpace( 4, 4, 4, 0 );
		m_btnRotating.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageBtnNor );
		m_btnRotating.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageBtnHov );
		m_btnRotating.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageBtnPre );
	}
	_clipWnd();

	return hWnd;
}

void PictureCtrl::_clipWnd() {
	HRGN hRgn;
	RECT rcClient;

	::GetClientRect( m_btnRotating.getHWND(), &rcClient );
	hRgn = ::CreateRoundRectRgn( 0, 0, rcClient.right, rcClient.bottom, 7, 7 );
	::SetWindowRgn( m_btnRotating.getHWND(), hRgn, TRUE );
	::DeleteObject( hRgn );
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
	unsigned int uImageBkRight = min( uClientWidth, ( uLSpace + uWidth ) );
	unsigned int uImageBkBottom = min( uClientHeight, (uTSpace + uHeight) );
	::SetRect( &m_rcBigBmpBk, uLSpace, uTSpace, uImageBkRight, uImageBkBottom );
}

void PictureCtrl::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );
	switch(wCtrlId) {
		case ROTATING_BTN_ID: {
				m_bRotating = true;
				RECT rcClient;

				::GetClientRect( m_hWnd, &rcClient );
				YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
				_drawStatus( memDC.getMemDC() );

				HDC hDC = ::GetDC( m_hWnd );
				::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
				::ReleaseDC( m_hWnd, hDC );
		}
		default:
			break;
	}
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
	RECT rcTopBmp;
	RECT rcMiddlBmp;
	RECT rcBottomBmp;

	ptMouse.x = LOWORD(pYBCtrlWndMsg->m_lParam);
	ptMouse.y = HIWORD(pYBCtrlWndMsg->m_lParam);

	m_ptMouse.x = ptMouse.x;
	m_ptMouse.y = ptMouse.y;
	if(::PtInRect( &m_rcBigBmp, ptMouse )) {
		if(m_bShowZoomBmp) {
			m_bShowZoomBmp = false;
		} else {
			m_bShowZoomBmp = true;
		}
	}
	//
	if(!_getTopBmpRect( rcTopBmp ) || !_getMiddleBmpRect( rcMiddlBmp ) || !_getBottomBmpRect( rcBottomBmp )) {
		assert( false );
		return;
	}
	if(::PtInRect( &rcTopBmp, ptMouse )){
		m_eRectStatus = ERECT_STATUS_TOP;
	} else if(::PtInRect( &rcMiddlBmp, ptMouse )) {
		m_eRectStatus = ERECT_STATUS_MIDDLE;
	} else if(::PtInRect( &rcBottomBmp, ptMouse )) {
		m_eRectStatus = ERECT_STATUS_BOTTOM;
	} else {
		//m_eRectStatus = ERECT_STATUS_NONE;
	}

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
	_drawTopBmp( hDC );
	_drawMiddleBmp( hDC );
	_drawBottomBmp( hDC );
	_drawBigBmp( hDC );
	_drawZoomBmp( hDC );
}

void PictureCtrl::_drawTopBmp( HDC hDC ) {
	RECT rcTopBmp;
	unsigned int idImage;
	YBCtrlImageStatic* pYBImage = NULL;

	if(!_getTopBmpRect( rcTopBmp ))
		return;
	if(!_getTopBmpIndex( idImage ))
		return;
	pYBImage = _getImageStatic( idImage );
	if(!pYBImage)
		return;

	Gdiplus::Graphics graphics( hDC );
	GdipBitBlt( &graphics, pYBImage->getImage(), rcTopBmp.left, rcTopBmp.top, rcTopBmp.right - rcTopBmp.left, rcTopBmp.bottom - rcTopBmp.top
		, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
}

void PictureCtrl::_drawMiddleBmp( HDC hDC ) {
	RECT rcMiddleBmp;
	unsigned int idImage;
	YBCtrlImageStatic* pYBImage = NULL;

	if(!_getMiddleBmpRect( rcMiddleBmp ))
		return;
	if(!_getMiddleBmpIndex( idImage ))
		return;
	pYBImage = _getImageStatic( idImage );
	if(!pYBImage)
		return;

	Gdiplus::Graphics graphics( hDC );
	GdipBitBlt( &graphics, pYBImage->getImage(), rcMiddleBmp.left, rcMiddleBmp.top, rcMiddleBmp.right - rcMiddleBmp.left, rcMiddleBmp.bottom - rcMiddleBmp.top
		, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
}

void PictureCtrl::_drawBottomBmp( HDC hDC ) {
	RECT rcBottomBmp;
	unsigned int idImage;
	YBCtrlImageStatic* pYBImage = NULL;

	if(!_getBottomBmpRect( rcBottomBmp ))
		return;
	if(!_getBottomBmpIndex( idImage ))
		return;
	pYBImage = _getImageStatic( idImage );
	if(!pYBImage)
		return;

	Gdiplus::Graphics graphics( hDC );
	GdipBitBlt( &graphics, pYBImage->getImage(), rcBottomBmp.left, rcBottomBmp.top, rcBottomBmp.right - rcBottomBmp.left, rcBottomBmp.bottom - rcBottomBmp.top
		, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
}

void PictureCtrl::_drawBigBmp( HDC hDC ) {
	RECT rcBigBmp;
	unsigned int idImage;
	YBCtrlImageStatic* pYBImage = NULL;

	if ( !_getBigBmpRect( rcBigBmp ) )
		return;
	if( !_getBigBmpIndex( idImage ) )
		return;
	pYBImage = _getImageStatic( idImage );
	if (!pYBImage)
		return;

	Gdiplus::Graphics graphics(hDC);
	if( m_bRotating ) {
		pYBImage->getImage()->RotateFlip( Gdiplus::Rotate90FlipNone );
		m_bRotating = false;
	}
	GdipBitBlt(&graphics, pYBImage->getImage(), m_rcBigBmp.left, m_rcBigBmp.top, m_rcBigBmp.right - m_rcBigBmp.left, m_rcBigBmp.bottom - m_rcBigBmp.top
		, 0, 0, pYBImage->getWidth(), pYBImage->getHeight());
}

void PictureCtrl::_drawZoomBmp( HDC hDC ) {
	if( !m_bShowZoomBmp )
		return;
	YBCtrlImageStatic* pYBImage = NULL;
	unsigned int idImage;

	if(!_getBigBmpIndex( idImage ))
		return;
	pYBImage = _getImageStatic( idImage );
	if ( !pYBImage )
		return;
	
	Gdiplus::Graphics graphics(hDC);
	GdipBitBlt(&graphics, pYBImage->getImage(), m_ptMouse.x, m_ptMouse.y, 100, 100
		, m_ptMouse.x - m_rcBigBmp.left, m_ptMouse.y - m_rcBigBmp.top, 30, 30);
}

bool PictureCtrl::_getBigBmpRect( RECT& rcBigBmp ) {
	YBCtrlImageStatic* pYBImage = NULL;
	unsigned int idImage;

	if( !_getBigBmpIndex( idImage ) ){
		return false;
	}
	pYBImage = _getImageStatic( idImage );
	if ( !pYBImage )
		return false;
	if ( ( pYBImage->getWidth() > (unsigned int)( m_rcBigBmpBk.right - m_rcBigBmpBk.left ) ) || ( pYBImage->getHeight() > (unsigned int)( m_rcBigBmpBk.bottom - m_rcBigBmpBk.top ) ) ) {
		m_rcBigBmp = m_rcBigBmpBk;
	} else {
		unsigned int uLeft = m_rcBigBmpBk.left + ( m_rcBigBmpBk.right - m_rcBigBmpBk.left - pYBImage->getWidth() )/2;
		unsigned int uTop = m_rcBigBmpBk.top + ( m_rcBigBmpBk.bottom - m_rcBigBmpBk.top - pYBImage->getHeight() )/2;
		::SetRect( &m_rcBigBmp, uLeft, uTop, uLeft + pYBImage->getWidth(), uTop + pYBImage->getHeight() );
	}
	rcBigBmp = m_rcBigBmp;
	return true;
}

bool PictureCtrl::_getBigBmpIndex( unsigned int& uImageId ) {
	if( m_containerImageIndex.size() == 0 )
		return false;
	containerImageIndex::const_reverse_iterator critBegin, critEnd;
	
	critBegin = m_containerImageIndex.crbegin();

	critEnd = m_containerImageIndex.crend();
	if ( m_eRectStatus == ERECT_STATUS_NONE ) {
		uImageId = (*critBegin);
	} else if( m_eRectStatus == ERECT_STATUS_TOP ) {
		if(m_containerImageIndex.size() != 3) {
			return false;
		}
		uImageId = (*critBegin);
	} else if( m_eRectStatus == ERECT_STATUS_MIDDLE ) {
		if(m_containerImageIndex.size() == 1) {
			return false;
		}
		uImageId = (*( ++ critBegin ) );
	} else if( m_eRectStatus == ERECT_STATUS_BOTTOM ) {
		containerImageIndex::iterator itBegin;
		itBegin = m_containerImageIndex.begin();
		uImageId = ( * itBegin );
	} else {
		uImageId = (*critBegin);
	}

	return true;
}

bool PictureCtrl::_getTopBmpIndex( unsigned int& uImageId ) {
	if(m_containerImageIndex.size() != 3) {
		return false;
	}
	containerImageIndex::const_reverse_iterator critBegin;

	critBegin = m_containerImageIndex.crbegin();
	uImageId = (*critBegin);
	return true;
}
bool PictureCtrl::_getMiddleBmpIndex( unsigned int& uImageId ) {
	if(m_containerImageIndex.size() <= 1) {
		return false;
	}
	containerImageIndex::const_reverse_iterator critBegin;

	critBegin = m_containerImageIndex.crbegin();
	if(m_containerImageIndex.size() == 2) {
		uImageId = (*critBegin );
	} else if(m_containerImageIndex.size() == 3) {
		uImageId = (*(++critBegin ) );
	}
	return true;
}

bool PictureCtrl::_getBottomBmpIndex( unsigned int& uImageId ) {
	if(m_containerImageIndex.size() == 0){
		return false;
	}
	containerImageIndex::iterator itId;
	itId = m_containerImageIndex.begin();
	uImageId = (*itId);
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
	tstrImageFilePath = tstrImageDirPath + _T( "YBWEDBtnNor.png" );
	if(!YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage )){
		assert( false );
	}
	m_uImageBtnNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T( "YBWEDBtnHor.png" );
	if(!YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage )){
		assert( false );
	}
	m_uImageBtnHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T( "YBWEDBtnPre.png" );
	if(!YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage )){
		assert( false );
	}
	m_uImageBtnPre = idImage;
}

void PictureCtrl::_loadFont() {
	LOGFONT lFont;

	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T( "Î¢ÈíÑÅºÚ" ) );
	lFont.lfHeight = 22;
	m_uFontIdBtn = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );
}
void PictureCtrl::loadImage( const void *buf, size_t size ) {
	unsigned int uImageId;
	if(YBImageLoader::getInstance()->loadImage( buf, size, uImageId )) {
		m_containerImageIndex.push_back( uImageId );
	}
}

void PictureCtrl::loadImage( tstring_type tstrImageFilePath ) {
	unsigned int uImageId;
	if(YBImageLoader::getInstance()->loadImage( tstrImageFilePath, uImageId )) {
		m_containerImageIndex.push_back( uImageId );
	}
}

bool PictureCtrl::_getBottomBmpRect( RECT& rcBottomBmp ) {
	RECT rcMiddle;

	if(!_getMiddleBmpRect( rcMiddle )) {
		assert( false );
		return false;
	}
	rcBottomBmp.top = rcMiddle.bottom + SMALL_BMP_SPACE;
	rcBottomBmp.bottom  = rcBottomBmp.top + SMALL_BMP_HEIGHT;
	rcBottomBmp.right = rcMiddle.right;
	rcBottomBmp.left = rcMiddle.left;

	return ((rcBottomBmp.right > rcBottomBmp.left) && ( rcBottomBmp.bottom > rcBottomBmp.top ) );
}

bool PictureCtrl::_getTopBmpRect( RECT& rcTopBmp ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcTopBmp.right = rcClient.right - SMALL_BMP_RIGHTSPACE;
	rcTopBmp.left = rcTopBmp.right - SMALL_BMP_WIDHT;
	rcTopBmp.top = rcClient.top + SMALL_BMP_TOPSPACE;
	rcTopBmp.bottom = rcTopBmp.top + SMALL_BMP_HEIGHT;
	return ( (rcTopBmp.right - rcTopBmp.left >0) && ( rcTopBmp.bottom - rcTopBmp.top ) );
}

bool PictureCtrl::_getMiddleBmpRect( RECT& rcMiddlBmp ) {
	RECT rcTopBmp;

	if(!_getTopBmpRect( rcTopBmp )) {
		assert( false );
		return false;
	}
	rcMiddlBmp.top = rcTopBmp.bottom + SMALL_BMP_SPACE;
	rcMiddlBmp.bottom = rcMiddlBmp.top + SMALL_BMP_HEIGHT;
	rcMiddlBmp.left = rcTopBmp.left;
	rcMiddlBmp.right = rcTopBmp.right;
	return ((rcMiddlBmp.right > rcMiddlBmp.left) && ( rcMiddlBmp.bottom > rcMiddlBmp.top ) );
}