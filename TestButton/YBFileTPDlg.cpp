#include "stdafx.h"
#include "YBFileTPDlg.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlDefs.h"
#include "frame/YBCtrlMisc.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlFontDepository.h"

#define YBFILETPDLG_PROGRESSBAR_ID (0x00000500)
//
#define YBFILETPDLG_TIMERID_AUTOHIDE (0x0020)
#define TIME_4FILETPDLG_AUTOHIDE (1000)

#define YBFILETPDLG_PROGRESSBAR_RSPACE (30)
#define YBFILETPDLG_PBTOICON_SPACE (7)
#define YBFILETPDLG_CONTROL_TSPACE (7)

USING_NAMESPACE_YBCTRL

YBFileTPDlg::YBFileTPDlg( EFILETPTYPE eFileTPType )
: m_eFileTPType( eFileTPType )
, m_tstrTitleText( _T("") )
, m_uImageIdBk( YBCTRL_IMAGE_ID_INVALID )
, m_uFontIdTxt( YBCTRL_IMAGE_ID_INVALID )
, m_uFontIdPBTxt( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdPBBk( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdPBFT( YBCTRL_IMAGE_ID_INVALID )
, m_uIconLSpace( 0 )
{
	_loadImage();
	_loadFont();
	_init();
	m_hPen = (HPEN)::CreatePen( PS_SOLID, 1, RGB(255, 255, 255) );
}

YBFileTPDlg::~YBFileTPDlg() {
	if ( m_hPen ) {
		::DeleteObject( m_hPen );
		m_hPen = NULL;
	}
}

YBCTRL_WNDMSGMAP_BEGIN( YBFileTPDlg, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_SIZE, _onWM_SIZE )
	YBCTRL_ONWNDMSG( WM_TIMER, _onWM_TIMER )
	YBCTRL_WNDMSGMAP_END()

HWND YBFileTPDlg::create( int x, int y, int uWidth, int uHeight, HWND hWndParent, HINSTANCE hInstance ) {
	HWND hWnd;
	DWORD dwExStyle = WS_EX_TOOLWINDOW|WS_EX_TOPMOST;
	DWORD dwStyle = WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	hWnd = _createHWND( dwExStyle, dwStyle, x, y, uWidth, uHeight, hWndParent, (HMENU)NULL, hInstance, NULL );
	if ( !hWnd ) {
		assert(false);
	}
	//
	_createAllControl();

	return hWnd;
}

void YBFileTPDlg::_createAllControl() {
	RECT rcPB;

	if ( _getPBRect( rcPB ) ) {
		m_progressBar.Create( WS_EX_TOOLWINDOW|WS_EX_TOPMOST, WS_VISIBLE|WS_CHILD, rcPB.left, rcPB.top, rcPB.right - rcPB.left, rcPB.bottom - rcPB.top, m_hWnd, (HMENU)YBFILETPDLG_PROGRESSBAR_ID, NULL );
	}
}

void YBFileTPDlg::_calcLocation() {
	RECT rcPB;

	if ( _getPBRect( rcPB ) ) {
		::MoveWindow( m_progressBar.getHWND(), rcPB.left, rcPB.top, rcPB.right - rcPB.left, rcPB.bottom - rcPB.top, TRUE );
	}

}

bool YBFileTPDlg::_getPBRect( RECT& rcPB ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcPB.left = rcClient.left + ( rcClient.right - rcClient.left )*3/5;
	rcPB.right = rcClient.right - YBFILETPDLG_PROGRESSBAR_RSPACE;
	rcPB.top = rcClient.top + YBFILETPDLG_CONTROL_TSPACE;
	rcPB.bottom = rcClient.bottom - YBFILETPDLG_CONTROL_TSPACE;

	return ( ( rcPB.right > rcPB.left ) && ( rcPB.bottom > rcPB.top ) );
}

void YBFileTPDlg::_onWM_TIMER( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_TIMER );
	switch ( pYBCtrlWndMsg->m_wParam ) {
	case YBFILETPDLG_TIMERID_AUTOHIDE:
		{
			::KillTimer( m_hWnd, YBFILETPDLG_TIMERID_AUTOHIDE );
			::ShowWindow( m_hWnd, SW_HIDE );
		}
		break;
	default:
		break;
	}
}

void YBFileTPDlg::onFinished( bool bSuccess ) {
	if ( bSuccess ) {
		if ( ::IsWindowVisible( m_hWnd ) ) {
			::SetTimer( m_hWnd, YBFILETPDLG_TIMERID_AUTOHIDE, TIME_4FILETPDLG_AUTOHIDE, NULL );
		}
	} else {
		::ShowWindow( m_hWnd, SW_HIDE );
	}	
}

void YBFileTPDlg::_onWM_SIZE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_SIZE );
	//
	_calcLocation();

}

void YBFileTPDlg::_onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	RECT rcClient;

	//
	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = 1;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	::BitBlt( hDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top )
		, memDC.getMemDC(), 0, 0, SRCCOPY );
}

void YBFileTPDlg::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
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

ybctrl::YBCtrlImageStatic* YBFileTPDlg::_getImageStatic( unsigned int uImageId ) {
	YBCtrlImageBase* pYBImageBase = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pYBImageBase ) )
		return NULL;
	if( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

void YBFileTPDlg::updateStatus() {
	if ( !::IsWindow( m_hWnd ) ) {
		return;
	}
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );
	HDC hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
}

void YBFileTPDlg::_drawStatus( HDC hDC ) {
	_drawBk( hDC );
	_drawTitleText( hDC );
	_drawFrameLine( hDC );
	_drawIcon( hDC );
}

void YBFileTPDlg::_drawBk( HDC hDC ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlImageStatic* pYBImage = NULL;
	pYBImage = _getImageStatic( m_uImageIdBk );

	if( !pYBImage ){
		return;
	}

	BackGroundBitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pYBImage->getMemDC(),
		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0 );

}

void YBFileTPDlg::_drawIcon( HDC hDC ) {
	RECT rcClient;
	unsigned int uIconTSpace;
	unsigned int uIconWidth;
	unsigned int uIconHeight;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlImageStatic* pYBImage = NULL;
	//
	if ( m_eFileTPType == EFILETP_UPLOAD ) {
		pYBImage = _getImageStatic( m_uImageIdIconUL );
	} else {
		pYBImage = _getImageStatic( m_uImageIdIconDL );
	}
	//
	if( !pYBImage ){
		return;
	}
	m_uIconLSpace = rcClient.right - YBFILETPDLG_PROGRESSBAR_RSPACE + YBFILETPDLG_PBTOICON_SPACE;
	uIconWidth = pYBImage->getWidth();
	uIconHeight = pYBImage->getHeight();
	uIconTSpace = ( rcClient.bottom - rcClient.top - uIconHeight )/2 + rcClient.top;

	Gdiplus::Graphics graphics( hDC );
	//graphics.SetInterpolationMode( Gdiplus::InterpolationModeHighQualityBicubic );
	//graphics.RotateTransform( 10, Gdiplus::MatrixOrderPrepend );
	pYBImage->getImage()->RotateFlip( Gdiplus::Rotate180FlipNone );
	GdipBitBlt( &graphics, pYBImage->getImage(), m_uIconLSpace, uIconTSpace, uIconWidth, uIconHeight
		, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
	
}

void YBFileTPDlg::_drawFrameLine( HDC hDC ) {
	RECT rcClient;
	HPEN hPenOld = NULL;

	::GetClientRect( m_hWnd, &rcClient );
	hPenOld = (HPEN)::SelectObject( hDC, m_hPen );
	//
	::MoveToEx( hDC, rcClient.left, rcClient.top, NULL );
	::LineTo( hDC, rcClient.right, rcClient.top );

	::MoveToEx( hDC, rcClient.left, rcClient.top, NULL );
	::LineTo( hDC, rcClient.left, rcClient.bottom );

	::MoveToEx( hDC, rcClient.left, rcClient.bottom - 1, NULL );
	::LineTo( hDC, rcClient.right, rcClient.bottom - 1 );

	::MoveToEx( hDC, rcClient.right - 1, rcClient.top, NULL );
	::LineTo( hDC, rcClient.right - 1, rcClient.bottom );

	::SelectObject( hDC, hPenOld );
}

void YBFileTPDlg::_drawTitleText( HDC hDC ) {
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcTitleText;

	if( !_getTitleTextRect( rcTitleText ) )
		return;

	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdTxt );
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

		::DrawText( hDC, m_tstrTitleText.c_str(), m_tstrTitleText.length(), &rcTitleText, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

bool YBFileTPDlg::_getTitleTextRect( RECT& rcTitleText ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcTitleText.left = rcClient.left;
	rcTitleText.right = rcTitleText.left + ( rcClient.right - rcClient.left )*3/5 - 6;
	rcTitleText.top = rcClient.top;
	rcTitleText.bottom = rcClient.bottom;
	
	return ( ( rcTitleText.right > rcTitleText.left ) && ( rcTitleText.bottom > rcTitleText.top ) );
}

void YBFileTPDlg::setTitleText( const tstring_type& tstrTitleTxt ) {
	m_tstrTitleText = tstrTitleTxt;
}

void YBFileTPDlg::setPos( __int64 i64PosCur, __int64 i64PosMax ) {
	__int64 i64PosMinPB;
	__int64 i64PosMaxPB;
	m_progressBar.getRange( i64PosMinPB, i64PosMaxPB );
	if ( ( i64PosMinPB != 0 ) || ( i64PosMaxPB != i64PosMax ) ) {
		 m_progressBar.setRange( 0, i64PosMax );
	}
	m_progressBar.setPos( i64PosCur );
	//::BringWindowToTop( m_hWnd );
	::SetForegroundWindow( m_hWnd );
	if ( ::IsWindow( m_hWnd ) ) {
		::KillTimer( m_hWnd, YBFILETPDLG_TIMERID_AUTOHIDE );
		::ShowWindow( m_hWnd, SW_NORMAL );
	}
}

void YBFileTPDlg::_init() {
	m_progressBar.SetProgressBarSpaced( 1, 1, 1, 1 );
	m_progressBar.setTextAttri( m_uFontIdPBTxt, 2, 2 );
	m_progressBar.setRange( 0, 100 );
	m_progressBar.setPos( 100 );
	m_progressBar.setImageID( m_uImageIdPBBk, m_uImageIdPBFT );
}

void YBFileTPDlg::_loadImage(){
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
	tstrImageFilePath = tstrImageDirPath + _T("YBFileTPDlgBk.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBk = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBFileTPDlgPBFT.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdPBFT = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBCheckBoxBk.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdPBBk = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBFileTPDlgIconUL.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdIconUL = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBFileTPDlgIconDL.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdIconDL = idImage;
}

void YBFileTPDlg::_loadFont(){

	LOGFONT lFont;

	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 20;
	m_uFontIdTxt = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );
	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 19;
	m_uFontIdPBTxt = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 14, 52, 78 ) );
}