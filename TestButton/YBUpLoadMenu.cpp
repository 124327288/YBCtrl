#include "stdafx.h"
#include "YBUpLoadMenu.h"
#include "frame/YBCtrlMemDC.h"
#include "scrollbar/YBScrollBar.h"
#include "loader/YBImageLoader.h"
#include "YBUploadFileMenu.h"
#include "frame/YBCtrlFontDepository.h"
#include <assert.h>

#define YBULMENUID_FSHEADER (300)
#define YBULMENUID_LISTBOX (301)

USING_NAMESPACE_YBCTRL

YBUpLoadMenu::YBUpLoadMenu()
: m_listbox4YBULFileMenu( new YBView4ULFileMenuInner( 300, 26 ), 20, 26 )
, m_uImageIdScrBarBkH( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdScrBarBkV( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdScrTArrPreNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdScrTArrNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdScrTArrHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdScrTArrPre( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdScrBArrPreNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdScrBArrNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdScrBArrHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdScrBArrPre( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdScrFTNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdScrFTHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdScrFTPre( YBCTRL_IMAGE_ID_INVALID )
, m_uFontIdHeader( YBCTRL_FONT_ID_INVALID )
, m_uImageIdHeader( YBCTRL_IMAGE_ID_INVALID )
{
	_loadFont();
	_loadImage();
	_init();
	m_hBrush = ::CreateSolidBrush( RGB(240,251,255) );
}

YBUpLoadMenu::~YBUpLoadMenu() {
	if ( m_hBrush ) {
		::DeleteObject( m_hBrush );
		m_hBrush = NULL;
	}
}

YBCTRL_WNDMSGMAP_BEGIN( YBUpLoadMenu, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
YBCTRL_WNDMSGMAP_END()

void YBUpLoadMenu::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	PAINTSTRUCT ps;
	HDC hPaintDC;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawBk( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );

}

void YBUpLoadMenu::_drawBk( HDC hDC ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	::FillRect( hDC, &rcClient, m_hBrush );
}

bool YBUpLoadMenu::trackPopupMenu( int x, int y, HWND hWndParent ) {
	HWND hWnd;
	DWORD dwExStyle = WS_EX_TOPMOST|WS_EX_TOOLWINDOW;
	DWORD dwStyle = WS_VISIBLE|WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	int nWidth = 304;
	int nHeight = 338;

	if ( ::IsWindow( m_hWnd ) ) {
		_destorySelf();
		assert( m_hWnd == 0 );
	}

	hWnd = _createHWND( dwExStyle, dwStyle, x, y, nWidth, nHeight, hWndParent, (HMENU)NULL, NULL, NULL );

	if ( hWnd ) {
		m_fsMenuHeader.create( 2, 2, 300, 36, hWnd, (HMENU)YBULMENUID_FSHEADER, NULL );
		m_listbox4YBULFileMenu.create( 2, 38, 300, 300, hWnd, YBULMENUID_LISTBOX, NULL );
		if ( m_listbox4YBULFileMenu.getHWND() ) {
			YBView4ULFileMenuContent* pContentObj = (YBView4ULFileMenuContent*)m_listbox4YBULFileMenu.getViewInner()->getViewContentObj();
			if ( pContentObj ) {
				pContentObj->setWidth( 300 );
			}
		}
		YBView4ULFileMenuContent* pViewContent4WSAWMenu = NULL;
		YBListBoxULFileMenuItem* pListBoxMenuItem = NULL;
		TCHAR szMsgBoxMenu[100];
		unsigned int uItemId;
		pViewContent4WSAWMenu = (YBView4ULFileMenuContent*)m_listbox4YBULFileMenu.getViewInner()->getViewContentObj();
		pViewContent4WSAWMenu->clearAllItem();
		for ( int i = 0; i<30; ++i ) {
			pListBoxMenuItem = new YBListBoxULFileMenuItem();
			_stprintf_s( szMsgBoxMenu, _T("%d"), i + 1 );
			pListBoxMenuItem->m_tstrItemContent = _T("Adobe Photoshop CS4");
			pListBoxMenuItem->m_tstrItemState = _T("³É¹¦");
			pViewContent4WSAWMenu->push_front( pListBoxMenuItem, uItemId );
		}
		m_listbox4YBULFileMenu.updateStatus();
		::ShowWindow( m_listbox4YBULFileMenu.getHWND(), SW_NORMAL );
		//////////////////////////////////////////////////////////////////////////
		return true;
	}

	return false;
}

void YBUpLoadMenu::_destorySelf() {
	if ( m_hWnd == 0 )
		return;
	if ( ::IsWindow( m_hWnd ) ) {
		::DestroyWindow( m_hWnd );
	} else {
		assert( false );
	}
	m_hWnd = NULL;
}

void YBUpLoadMenu::_init() {
	ybctrl::YBScrollBar* pYBScrollBarH = m_listbox4YBULFileMenu.getScrollBarH();
	ybctrl::YBScrollBar* pYBScrollBarV = m_listbox4YBULFileMenu.getScrollBarV();

	pYBScrollBarV->setImageIdBk( m_uImageIdScrBarBkV );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrTArrPreNor );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrTArrNor );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrTArrHov );
	pYBScrollBarV->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrTArrPre );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrBArrPreNor );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrBArrNor );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrBArrHov );
	pYBScrollBarV->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrBArrPre );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_NORMAL, m_uImageIdScrFTNor );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_HOVER, m_uImageIdScrFTHov );
	pYBScrollBarV->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_PRESS, m_uImageIdScrFTPre );
	//
	pYBScrollBarH->setImageIdBk( m_uImageIdScrBarBkH );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrTArrPreNor );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrTArrNor );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrTArrHov );
	pYBScrollBarH->setImageIdStartArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrTArrPre );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_NORMAL, m_uImageIdScrBArrPreNor );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_OUTHOVER, m_uImageIdScrBArrNor );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_INHOVER, m_uImageIdScrBArrHov );
	pYBScrollBarH->setImageIdEndArrow( YBScrollBar::EARROW_STATUS_PRESS, m_uImageIdScrBArrPre );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_NORMAL, m_uImageIdScrFTNor );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_HOVER, m_uImageIdScrFTHov );
	pYBScrollBarH->setImageIdThumb( YBScrollBar::ETHUMB_STATUS_PRESS, m_uImageIdScrFTPre );
	//
	m_fsMenuHeader.setHeaderImageId( m_uImageIdHeader );
	m_fsMenuHeader.setFontId( m_uFontIdHeader );
}

void YBUpLoadMenu::_loadFont() {
	LOGFONT lFont;
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 20;
	m_uFontIdHeader = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );
}

void YBUpLoadMenu::_loadImage() {
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
	tstrImageFilePath = tstrImageDirPath + _T("YBScrollBarBkH.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBarBkH = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBScrollBarBkV.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBarBkV = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBMenuScrVSPreNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBArrPreNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBMenuScrVSNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrTArrNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBMenuScrVSHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrTArrHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBMenuScrVSPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrTArrPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBMenuScrVEPreNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBArrPreNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBMenuScrVENor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBArrNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBMenuScrVEHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBArrHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBMenuScrVEPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBArrPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBMenuScrBkNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrFTNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBMenuScrBkHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrFTHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBMenuScrBkPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrFTPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBFSMenuHeaderBk.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdHeader = idImage;
}