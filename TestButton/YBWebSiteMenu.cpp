#include "stdafx.h"
#include "YBWebSiteMenu.h"
#include "frame/YBCtrlMemDC.h"
#include "scrollbar/YBScrollBar.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlFontDepository.h"

// ID
#define YBWBMENUID_FSHEADER (300)
#define YBWBMENUID_LISTBOX (301)
// 
#define YBWEBSITEMENU_ITEM_HEIGHT (26)
#define YBWEBSITEMENU_HEADER_HEIGHT (36)
#define YBWEBSITEMENU_CONTENT_WIDTH (300)
#define YBWEBSITEMENU_VIEW_HEIGHT (300)
#define YBWEBSITEMENU_FRAME_THICK (2)
#define YBWEBSITEMENU_WND_WIDTH (YBWEBSITEMENU_CONTENT_WIDTH + 2*YBWEBSITEMENU_FRAME_THICK)
#define YBWEBSITEMENU_WND_HEIGHT (YBWEBSITEMENU_VIEW_HEIGHT + 2*YBWEBSITEMENU_FRAME_THICK + YBWEBSITEMENU_HEADER_HEIGHT)

USING_NAMESPACE_YBCTRL

YBWebSiteMenu::YBWebSiteMenu()
: m_listbox4WebSite( new YBView4WSAWMenuInner( YBWEBSITEMENU_CONTENT_WIDTH, YBWEBSITEMENU_ITEM_HEIGHT ), 20, 26 )
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

YBWebSiteMenu::~YBWebSiteMenu() {
	if ( m_hBrush ) {
		::DeleteObject( m_hBrush );
		m_hBrush = NULL;
	}
}

YBCTRL_WNDMSGMAP_BEGIN( YBWebSiteMenu, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_NCACTIVATE, _onWM_NCACTIVATE )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_WNDMSGMAP_END()

void YBWebSiteMenu::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
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

void YBWebSiteMenu::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );

	switch( wCtrlId ) {
	case YBWBMENUID_LISTBOX:
		{
			unsigned int uItemId;
			unsigned int uIndex;
			POINT pt;
			pt.x = 0;
			pt.y = 0;
			uIndex = pYBCtrlWndMsg->m_lParam;
			//
			YBView4WSAWMenuContent* pViewContent4WebSiteMenu = NULL;
			pViewContent4WebSiteMenu = (YBView4WSAWMenuContent*)m_listbox4WebSite.getViewInner()->getViewContentObj();
			pViewContent4WebSiteMenu->index2ItemId( uIndex, uItemId );
			pViewContent4WebSiteMenu->deleteMenuItemById( uItemId );
			m_listbox4WebSite.updateStatus();

			if ( pViewContent4WebSiteMenu->getItemCount()*YBWEBSITEMENU_ITEM_HEIGHT<300 ){
				::ClientToScreen( m_hWnd, &pt );
				::MoveWindow( m_hWnd, pt.x, pt.y, YBWEBSITEMENU_WND_WIDTH, YBWEBSITEMENU_HEADER_HEIGHT + YBWEBSITEMENU_FRAME_THICK + pViewContent4WebSiteMenu->getItemCount()*YBWEBSITEMENU_ITEM_HEIGHT, TRUE );
				if ( pViewContent4WebSiteMenu->getItemCount() == 0 ) {
					::ShowWindow( m_hWnd, SW_HIDE );
				}
				::MoveWindow( m_listbox4WebSite.getHWND(), YBWEBSITEMENU_FRAME_THICK, YBWEBSITEMENU_HEADER_HEIGHT + YBWEBSITEMENU_FRAME_THICK, YBWEBSITEMENU_CONTENT_WIDTH, pViewContent4WebSiteMenu->getItemCount()*YBWEBSITEMENU_ITEM_HEIGHT, TRUE );
			}

			::InvalidateRect( m_listbox4WebSite.getViewInner()->getHWND(), NULL, false );
			::InvalidateRect( m_listbox4WebSite.getScrollBarV()->getHWND(), NULL, false );
		}
		break;
	default:
		break;
	}
}

void YBWebSiteMenu::_onWM_NCACTIVATE( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_NCACTIVATE );
	if ( pYBCtrlWndMsg->m_wParam == FALSE ) { // inactive
		::PostMessage( m_hWnd, WM_CLOSE, 0, 0 );
		return;
	}
}

void YBWebSiteMenu::_drawBk( HDC hDC ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	::FillRect( hDC, &rcClient, m_hBrush );
}

bool YBWebSiteMenu::trackPopupMenu( int x, int y, HWND hWndParent ) {
	HWND hWnd;
	DWORD dwExStyle = WS_EX_TOPMOST|WS_EX_TOOLWINDOW;
	DWORD dwStyle = WS_VISIBLE|WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	
	if ( ::IsWindow( m_hWnd ) ) {
		_destorySelf();
		assert( m_hWnd == 0 );
	}
	//
	hWnd = _createHWND( dwExStyle, dwStyle, x, y, YBWEBSITEMENU_WND_WIDTH, YBWEBSITEMENU_WND_HEIGHT, hWndParent, (HMENU)NULL, NULL, NULL );

	if ( hWnd ) {
		m_fsMenuHeader.create( YBWEBSITEMENU_FRAME_THICK, YBWEBSITEMENU_FRAME_THICK, YBWEBSITEMENU_CONTENT_WIDTH, YBWEBSITEMENU_HEADER_HEIGHT, hWnd, (HMENU)YBWBMENUID_FSHEADER, NULL );
		m_listbox4WebSite.create( YBWEBSITEMENU_FRAME_THICK, YBWEBSITEMENU_HEADER_HEIGHT + YBWEBSITEMENU_FRAME_THICK, YBWEBSITEMENU_CONTENT_WIDTH, YBWEBSITEMENU_VIEW_HEIGHT, hWnd, YBWBMENUID_LISTBOX, NULL );
		if ( m_listbox4WebSite.getHWND() ) {
			YBView4WSAWMenuContent* pContentObj = (YBView4WSAWMenuContent*)m_listbox4WebSite.getViewInner()->getViewContentObj();
			if ( pContentObj ) {
				pContentObj->setWidth( YBWEBSITEMENU_CONTENT_WIDTH );
			}
		}
		//
		_insert();
		//
		return true;
	}

	return false;
}

void YBWebSiteMenu::_insert() {

	YBView4WSAWMenuContent* pViewContent4WebSite = NULL;
	YBListBoxWSAWMenuItem* pListBoxMenuItem = NULL;
	TCHAR szMsgBoxMenu[100];
	unsigned int uItemId;
	pViewContent4WebSite = (YBView4WSAWMenuContent*)m_listbox4WebSite.getViewInner()->getViewContentObj();
	pViewContent4WebSite->clearAllItem();
	for ( int i = 0; i<30; ++i ) {
		pListBoxMenuItem = new YBListBoxWSAWMenuItem();
		_stprintf_s( szMsgBoxMenu, _T("%d"), i + 1 );
		pListBoxMenuItem->m_tstrItemContent = _T("Adobe Photoshop CS4");
		pListBoxMenuItem->m_uImageIdNor = m_uImageIdWebSiteMenuNor;
		pListBoxMenuItem->m_uImageIdHov = m_uImageIdWebSiteMenuHor;
		pViewContent4WebSite->push_front( pListBoxMenuItem, uItemId );
	}
	m_listbox4WebSite.updateStatus();
	::ShowWindow( m_listbox4WebSite.getHWND(), SW_NORMAL );
}

void YBWebSiteMenu::_destorySelf() {
	if ( m_hWnd == 0 )
		return;
	if ( ::IsWindow( m_hWnd ) ) {
		::DestroyWindow( m_hWnd );
	} else {
		assert( false );
	}
	m_hWnd = NULL;
}

void YBWebSiteMenu::_init() {
	ybctrl::YBScrollBar* pYBScrollBarH = m_listbox4WebSite.getScrollBarH();
	ybctrl::YBScrollBar* pYBScrollBarV = m_listbox4WebSite.getScrollBarV();

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
	//
	
}

void YBWebSiteMenu::_loadFont() {
	LOGFONT lFont;
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 20;
	m_uFontIdHeader = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );
}

void YBWebSiteMenu::_loadImage() {
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
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWSAWMenuNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdWebSiteMenuNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWSAWMenuHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdWebSiteMenuHor = idImage;
}