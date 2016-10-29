#include "stdafx.h"
#include "YBInstallDlg.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlMisc.h"
#include "frame/YBCtrlFontDepository.h"
#include "frame/YBCtrlImageDepository.h"
#include "loader/YBImageLoader.h"
#include "YBView4InstallFile.h"
#include "scrollbar/YBScrollBar.h"

USING_NAMESPACE_YBCTRL

YBInstallDlg::YBInstallDlg()
: m_listbox4IF( new YBView4InstallFileInner( 300, 26 ), 25, 26 )
{
	_loadFont();
	_loadImage();
	_init();
}

YBInstallDlg::~YBInstallDlg() {

}

YBCTRL_WNDMSGMAP_BEGIN( YBInstallDlg, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	//YBCTRL_ONWNDMSG( WM_NCHITTEST, _onWM_NCHITTEST )
	//YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
YBCTRL_WNDMSGMAP_END()

void YBInstallDlg::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD(pYBCtrlWndMsg->m_wParam);
	WORD wEvtCode = HIWORD(pYBCtrlWndMsg->m_wParam);

	switch( wCtrlId ) {
	case YBINSTALLDLGID_BTN_CANCEL:
		{
			::PostMessage( m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0 );
		}
		break;
	default:
		break;
	}
}

HWND YBInstallDlg::create( int nWidth, int nHeight, HWND hWndParent ) {
	HWND hWnd;
	DWORD dwExStyle = WS_EX_TOPMOST|WS_EX_TOOLWINDOW;
	DWORD dwStyle = WS_CHILD|WS_POPUP;
	int x;
	int y;
	int nScreenWidth;
	int nScreenHeight;

	nScreenWidth = ::GetSystemMetrics( SM_CXSCREEN );
	nScreenHeight = ::GetSystemMetrics( SM_CYSCREEN );
	x = ( nScreenWidth - nWidth )/2;
	y = ( nScreenHeight - nHeight )/2;
	hWnd = _createHWND( dwExStyle, dwStyle, x, y, nWidth, nHeight, hWndParent, (HMENU)NULL, NULL, NULL );
	if ( hWnd ) {
		_clipWnd();
		//
		m_listbox4IF.create( 20, 20, nWidth - 40, nHeight - 100, hWnd, YBINSTALLDLGID_LISTBOX, NULL );
		m_btn4Install.Create( 0, WS_VISIBLE|WS_CHILD, 80, 350, 70, 35, hWnd, (HMENU)YBINSTALLDLGID_BTN_INSTALL, NULL );
		m_btn4Cancel.Create( 0, WS_CHILD|WS_VISIBLE, 270, 350, 70, 35, hWnd, (HMENU)YBINSTALLDLGID_BTN_CANCEL, NULL );
		//
		_insertdata();
	}

	return hWnd;
}

void YBInstallDlg::_insertdata() {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	int nWidth = rcClient.right - rcClient.left;
	int nHeight = rcClient.bottom - rcClient.top;

	if ( m_listbox4IF.getHWND() ) {
		YBView4InstallFileContent* pContentObj = (YBView4InstallFileContent*)m_listbox4IF.getViewInner()->getViewContentObj();
		if ( pContentObj ) {
			pContentObj->setWidth( nWidth - 40 );
		}
	}
	YBView4InstallFileContent* pViewContent4IntallFile = NULL;
	YBListBoxInstallFileItem* pListBoxIFItem = NULL;
	TCHAR szMsgBoxMenu[100];
	unsigned int uItemid;
	pViewContent4IntallFile = (YBView4InstallFileContent*)m_listbox4IF.getViewInner()->getViewContentObj();
	pViewContent4IntallFile->clearAllItem();
	for ( int i = 0; i<100; ++i ) {
		pListBoxIFItem = new YBListBoxInstallFileItem();
		_stprintf_s( szMsgBoxMenu, _T("ROM.WANG %d"), i );
		pListBoxIFItem->m_tstrItemContent = szMsgBoxMenu;
		pViewContent4IntallFile->insertItem( i, pListBoxIFItem, uItemid );
	}
	m_listbox4IF.updateStatus();
	::ShowWindow( m_listbox4IF.getHWND(), SW_NORMAL );
	::ShowWindow( m_hWnd, SW_NORMAL );
}

void YBInstallDlg::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
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

void YBInstallDlg::_onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );

}

void YBInstallDlg::_onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_NCHITTEST );
}

void YBInstallDlg::_clipWnd() {
	HRGN hRgn;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	hRgn = ::CreateRoundRectRgn( 0, 0, rcClient.right + 1, rcClient.bottom + 1, 6, 6 );

	::SetWindowRgn( m_hWnd, hRgn, TRUE );
	::DeleteObject( hRgn );
}

void YBInstallDlg::_drawStatus( HDC hDC ) {
	_drawBk( hDC );
}

void YBInstallDlg::_drawBk( HDC hDC ) {
	YBCtrlImageStatic* pYBImage = NULL;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	pYBImage = _getImageStatic( m_uImageIdIDBk );
	if ( !pYBImage ) {
		return;
	}

	Gdiplus::Graphics graphics( hDC );
	GdipBitBlt( &graphics, pYBImage->getImage(), 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top
		, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
}

ybctrl::YBCtrlImageStatic* YBInstallDlg::_getImageStatic( unsigned int uImageId ) {
	YBCtrlImageBase* pYBImageBase = NULL;

	if ( !YBCtrlImageDepository::getInstance()->getImage( uImageId, &pYBImageBase ) )
		return NULL;
	if ( !pYBImageBase || pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

void YBInstallDlg::_loadFont() {
	LOGFONT lFont;

	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 22;
	m_uFontIdBtn = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );
}

void YBInstallDlg::_loadImage() {
	tstring_type tstrImageDirPath;
	tstring_type tstrImageFilePath;
	TCHAR szModulePath[MAX_PATH];
	TCHAR* posTmp = NULL;
	unsigned int idImage;

	::GetModuleFileName( NULL, szModulePath, MAX_PATH );
	posTmp = _tcsrchr( szModulePath, _T('\\'));
	if ( !posTmp ) {
		assert( false );
		return;
	}
	posTmp += 1;
	*posTmp = 0;
	tstrImageDirPath = szModulePath;
	tstrImageDirPath += _T("Image\\");
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBStaticBK.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdIDBk = idImage;
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
	tstrImageFilePath = tstrImageDirPath + _T("YBNWTArrowNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrTArrNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWTArrowHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrTArrHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWTArrowPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrTArrPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWBArrowNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBArrNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWBArrowHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBArrHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWBArrowPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBArrPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWTArrowPreNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrTArrPreNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWBArrowPreNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrBArrPreNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWScrollBarNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrFTNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWScrollBarHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrFTHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBNWScrollBarPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdScrFTPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWSAWMenuNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdWSAWMenuNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBWSAWMenuHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdWSAWMenuHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBBtnNormal.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBBtnHover.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBBtnPress.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnPre = idImage;
}

void YBInstallDlg::_init() {
	ybctrl::YBViewInner* pViewInner = m_listbox4IF.getViewInner();
	ybctrl::YBViewContentBase* pViewContent = pViewInner->getViewContentObj();
	YBView4InstallFileContent* pView4DDFolderContent = ( YBView4InstallFileContent* )pViewContent;

	ybctrl::YBScrollBar* pYBScrollBarH = m_listbox4IF.getScrollBarH();
	ybctrl::YBScrollBar* pYBScrollBarV = m_listbox4IF.getScrollBarV();

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
	m_btn4Install.setImage( ybctrl::YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnNor );
	m_btn4Install.setImage( ybctrl::YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnHov );
	m_btn4Install.setImage( ybctrl::YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnPre );
	m_btn4Install.setFontId( m_uFontIdBtn );
	m_btn4Install.setText( _T("安  装") );
	m_btn4Install.setTxtVcenter( true );

	m_btn4Cancel.setImage( ybctrl::YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnNor );
	m_btn4Cancel.setImage( ybctrl::YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnHov );
	m_btn4Cancel.setImage( ybctrl::YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnPre );
	m_btn4Cancel.setFontId( m_uFontIdBtn );
	m_btn4Cancel.setText( _T("取  消") );
	m_btn4Cancel.setTxtVcenter( true );
}