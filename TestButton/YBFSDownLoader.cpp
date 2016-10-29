#include "stdafx.h"
#include "YBFSDownLoader.h"
#include "frame/YBCtrlImageBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlMisc.h"
#include "frame/YBCtrlFontDepository.h"
#include "frame/YBCtrlImageDepository.h"
#include "loader/YBImageLoader.h"
#include <WindowsX.h>

USING_NAMESPACE_YBCTRL

typedef BOOL (*UPDATELAYEREDWINDOWFUNCTION)(HWND, HDC, POINT*, SIZE*, HDC, POINT*, COLORREF, BLENDFUNCTION*, DWORD);

YBFSDownLoader::YBFSDownLoader()
: m_uImageIdBk( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdMinBtnNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdMinBtnHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdMinBtnPre( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdCloseBtnNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdCloseBtnHor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdCloseBtnPre( YBCTRL_IMAGE_ID_INVALID )
, m_tstrTitle( _T("已下载50%"))
, m_uFontIdTitle( YBCTRL_FONT_ID_INVALID )
, m_eBtnStatus( EBTN_STATUS_CANCEL )
{
	_loadFont();
	_loadImage();
	_init();
}

YBFSDownLoader::~YBFSDownLoader() {
	::Shell_NotifyIcon( NIM_DELETE, &m_nid );
}

HWND YBFSDownLoader::create( int nWidth, int nHeight, HWND hWndParent ) {
	HWND hWnd;
	DWORD dwExStyle = WS_EX_TOPMOST|WS_EX_TOOLWINDOW|WS_EX_LAYERED;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW|WS_VISIBLE;
	//DWORD dwExStyle = WS_EX_TOPMOST|WS_EX_TOOLWINDOW;
	//DWORD dwStyle = WS_CHILD|WS_POPUP|WS_VISIBLE;
	int x;
	int y;
	int nScreenWidth;
	int nScreenHeight;
	int nBtnWidth = 44;
	int nBtnHeight = 25;

	nScreenWidth = ::GetSystemMetrics( SM_CXSCREEN );
	nScreenHeight = ::GetSystemMetrics( SM_CYSCREEN );
	x = ( nScreenWidth - nWidth )/2;
	y = ( nScreenHeight - nHeight )/2;
	hWnd = _createHWND( dwExStyle, dwStyle, x, y, nWidth, nHeight, hWndParent, (HMENU)NULL, NULL, NULL );
	if ( hWnd ) {
		_clipWnd();
		//
		m_btnMin.Create( 0, WS_CHILD|WS_VISIBLE, nWidth - 2*nBtnWidth - 7, 7, nBtnWidth, nBtnHeight, hWnd, (HMENU)YBFSDLID_BTN_MINIMIZE, NULL );
		m_btnClose.Create( 0, WS_VISIBLE|WS_CHILD, nWidth - 2*nBtnWidth + nBtnWidth - 7, 7, nBtnWidth, nBtnHeight, hWnd, (HMENU)YBFSDLID_BTN_CLOSE, NULL );
		m_progressBar.Create( 0, WS_CHILD|WS_VISIBLE, 30, 78, 364, 27, hWnd, (HMENU)YBFSDLID_PROGRESSBAR, NULL );
		m_btnCancel.Create( 0, WS_VISIBLE|WS_CHILD, 410, 74, 91, 33, hWnd, (HMENU)YBFSDLID_BTN_CANCEL, NULL );
		//
		_setWndTransparent();
	}

	return hWnd;
}

void YBFSDownLoader::_setWndTransparent() {
	HDC hDC;
	RECT rcClient;
	POINT ptSrc = {0, 0};
	POINT ptDst = {200, 200};

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hDC = ::GetDC( m_hWnd );
	//
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	SIZE wndSize = { rcClient.right - rcClient.left, rcClient.bottom - rcClient.top };
	BLENDFUNCTION blendFunction;
	blendFunction.AlphaFormat = AC_SRC_ALPHA;
	blendFunction.BlendFlags = 0;
	blendFunction.BlendOp = AC_SRC_OVER;
	blendFunction.SourceConstantAlpha = 123;
	UpdateLayeredWindow( m_hWnd, hDC, NULL, &wndSize, memDC.getMemDC(), &ptSrc, 0, &blendFunction, 2 );
	//
	::ReleaseDC( m_hWnd, hDC );
}

void YBFSDownLoader::_clipWnd() {
	HRGN hRgn;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	hRgn = ::CreateRoundRectRgn( 0, 0, rcClient.right + 1, rcClient.bottom + 1, 6, 6 );

	::SetWindowRgn( m_hWnd, hRgn, TRUE );
	::DeleteObject( hRgn );
}

YBCTRL_WNDMSGMAP_BEGIN( YBFSDownLoader, YBCtrlBase )
	//YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_ONWNDMSG( WM_SHOWTASK, _onWM_SHOWTASK )
	YBCTRL_ONWNDMSG( WM_CTLCOLOR, _onWM_CTLCOLOR )
	//YBCTRL_ONWNDMSG( WM_NCHITTEST, _onWM_NCHITTEST )
	YBCTRL_ONWNDMSG( WM_LBUTTONDBLCLK, _onWM_LBUTTONDBLCLK )
	//YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
YBCTRL_WNDMSGMAP_END()

void YBFSDownLoader::_onWM_CTLCOLOR( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_CTLCOLOR );
	int a = 0;
}

void YBFSDownLoader::_onWM_LBUTTONDBLCLK( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDBLCLK );
	int a = 0;
}

void YBFSDownLoader::_onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	HDC hDC;
	RECT rcClient;

	pYBCtrlWndMsg->m_dwFlag = (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = 1;

	hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
}

void YBFSDownLoader::_onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_NCHITTEST );
	pYBCtrlWndMsg->m_dwFlag = (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = HTCAPTION;
}

void YBFSDownLoader::_onWM_SHOWTASK( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_SHOWTASK );
	
	if ( ( WM_LBUTTONDOWN == pYBCtrlWndMsg->m_lParam ) ||
		( WM_LBUTTONDBLCLK == pYBCtrlWndMsg->m_lParam ) ) {
		::ShowWindow( m_hWnd, SW_SHOWNORMAL );
		::SetWindowPos( m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
		//::Shell_NotifyIcon( NIM_DELETE, &m_nid );
	}
}

void YBFSDownLoader::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wCtrlEvt = HIWORD( pYBCtrlWndMsg->m_wParam );

	switch( wCtrlId ) {
	case YBFSDLID_BTN_MINIMIZE:
		{
			m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
			m_nid.hWnd = m_hWnd;
			m_nid.uID = m_uImageIdMinBtnNor;
			m_nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
			m_nid.uCallbackMessage = WM_SHOWTASK;
			m_nid.hIcon = LoadIcon( NULL, IDI_ERROR );
			_tcscpy_s( m_nid.szTip, _T("裕邦全屏") );
			Shell_NotifyIcon( NIM_ADD, &m_nid );
			::ShowWindow( m_hWnd, SW_HIDE );
		}
		break;
	case YBFSDLID_BTN_CLOSE:
		{
			::Shell_NotifyIcon( NIM_DELETE, &m_nid );
			::DestroyWindow( m_hWnd );
		}
		break;
	case YBFSDLID_BTN_CANCEL:
		{
			switch( m_eBtnStatus ) {
			case EBTN_STATUS_CANCEL:
				{
					//::MessageBox( m_hWnd, _T("正在下载中"), _T("裕邦全屏"), MB_OK );
					onFinished( true );
				}
				break;
			case EBTN_STATUS_INSTALL:
				{
					onFinished( false );
					//::DestroyWindow( m_hWnd );
				}
				break;
			default:
				break;
			}
		}
	default:
		break;
	}

}

void YBFSDownLoader::setPos( __int64 i64PosCur, __int64 i64PosMax ) {
	__int64 i64PosMinPB;
	__int64 i64PosMaxPB;
	m_progressBar.getRange( i64PosMinPB, i64PosMaxPB );
	if( ( i64PosMinPB != 0 ) || ( i64PosMax != i64PosMaxPB ) ) {
		m_progressBar.setRange( 0, i64PosMax );
	}
	m_progressBar.setPos( i64PosCur );
	::SetForegroundWindow( m_hWnd );
}

void YBFSDownLoader::onFinished( bool bSuccess ) {
	if ( bSuccess ) {
		m_eBtnStatus = EBTN_STATUS_INSTALL;
		m_btnCancel.setText( _T("安   装") );
	} else {
		m_eBtnStatus = EBTN_STATUS_CANCEL;
		m_btnCancel.setText( _T("下   载") );
	}
	m_btnCancel.updateStatus();
}

void YBFSDownLoader::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	PAINTSTRUCT ps;
	HDC hPaintDC;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	POINT ptSrc = {0, 0};
	POINT ptDst = {200, 200};
	SIZE wndSize = { rcClient.right - rcClient.left, rcClient.bottom - rcClient.top };
	BLENDFUNCTION blendFunction;
	blendFunction.AlphaFormat = AC_SRC_ALPHA;
	blendFunction.BlendFlags = 0;
	blendFunction.BlendOp = AC_SRC_OVER;
	blendFunction.SourceConstantAlpha = 255;
	UpdateLayeredWindow( m_hWnd, hPaintDC, &ptDst, &wndSize, memDC.getMemDC(), &ptSrc, 0, &blendFunction, 2 );
	::EndPaint( m_hWnd, &ps );
}

void YBFSDownLoader::_drawBk( HDC hDC ) {
	YBCtrlImageStatic* pYBImage = NULL;
	RECT rcClient;

 	::GetClientRect( m_hWnd, &rcClient );
 	pYBImage = _getImageStatic( m_uImageIdBk );
	if ( !pYBImage ) {
		return;
	}
	//LONG style = ::GetWindowLong( m_hWnd, GWL_STYLE );
	//if ( style&WS_CAPTION ) {
	//	style ^= WS_CAPTION;
	//}
	//if ( style&WS_THICKFRAME ) {
	//	style ^= WS_THICKFRAME;
	//}
	//if ( style&WS_SYSMENU ) {
	//	style ^= WS_SYSMENU;
	//}
	//::SetWindowLong( m_hWnd, GWL_STYLE, style );
	//
	Gdiplus::Graphics graphics( hDC );
	GdipBitBlt( &graphics, pYBImage->getImage(), 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top
		, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
}

void YBFSDownLoader::_drawStatus( HDC hDC ) {
	_drawBk( hDC );
	_drawTitle( hDC );
}

ybctrl::YBCtrlImageStatic* YBFSDownLoader::_getImageStatic( unsigned int uImageId ) {
	YBCtrlImageBase* pYBImage = NULL;

	if ( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pYBImage ) )
		return NULL;
	if ( !pYBImage || ( pYBImage->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImage;
}

void YBFSDownLoader::_drawTitle( HDC hDC ) {
	RECT rcTitle;
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;

	if ( ( _getTitleRect( rcTitle) ) && ( m_tstrTitle.c_str() ) ) {
		pYBFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdTitle );
		if ( pYBFont ) {
			hFontOld = (HFONT)::SelectObject( hDC, pYBFont->m_hFont );
			iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
			clrTxtOld = ::SetTextColor( hDC, pYBFont->m_clrTxt );
			::DrawText( hDC, m_tstrTitle.c_str(), m_tstrTitle.length(), &rcTitle, DT_SINGLELINE|DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER );
			::SetTextColor( hDC, clrTxtOld );
			::SetBkMode( hDC, iBkModeOld );
			::SelectObject( hDC, hFontOld );
		}
	}
}

bool YBFSDownLoader::_getTitleRect( RECT& rcTitle ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcTitle.left = rcClient.left + 12;
	rcTitle.right = rcClient.right;
	rcTitle.top = rcClient.top + 1;
	rcTitle.bottom = rcTitle.top + 40;

	return ( ( rcTitle.right > rcTitle.left ) && ( rcTitle.bottom > rcTitle.top ) );
}

void YBFSDownLoader::_init() {
	m_btnMin.setText( _T("最小化") );
	m_btnMin.setImage( ybctrl::YBButton::EBTN_STATUS_NORMAL, m_uImageIdMinBtnNor );
	m_btnMin.setImage( ybctrl::YBButton::EBTN_STATUS_HOVER, m_uImageIdMinBtnHov );
	m_btnMin.setImage( ybctrl::YBButton::EBTN_STATUS_PRESS, m_uImageIdMinBtnPre );
	//
	m_btnClose.setText( _T("关 闭") );
	m_btnClose.setImage( ybctrl::YBButton::EBTN_STATUS_NORMAL, m_uImageIdCloseBtnNor );
	m_btnClose.setImage( ybctrl::YBButton::EBTN_STATUS_HOVER, m_uImageIdCloseBtnHor );
	m_btnClose.setImage( ybctrl::YBButton::EBTN_STATUS_PRESS, m_uImageIdCloseBtnPre );
	//
	if ( m_eBtnStatus == EBTN_STATUS_CANCEL ) {
		m_btnCancel.setText( _T("下   载") );
	} else {
		m_btnCancel.setText( _T("安   装") );
	}
	m_btnCancel.setTextSpace( 2, 5, 2, 0 );
	m_btnCancel.setFontId( m_uFontIdBtn );
	m_btnCancel.setImage( ybctrl::YBButton::EBTN_STATUS_NORMAL, m_uImageIdCancelBtnNor );
	m_btnCancel.setImage( ybctrl::YBButton::EBTN_STATUS_HOVER, m_uImageIdCancelBtnHor );
	m_btnCancel.setImage( ybctrl::YBButton::EBTN_STATUS_PRESS, m_uImageIdCancelBtnPre );
	//
	m_progressBar.SetProgressBarSpaced( 1, 1, 1, 1 );
	m_progressBar.setTextAttri( m_uFontIdTitle, 2, 2 );
	m_progressBar.setRange( 0, 100 );
	m_progressBar.setPos( 50 );
	m_progressBar.setImageID( m_uImageIdPBBk, m_uImageIdPBFT );
}

void YBFSDownLoader::_loadFont() {
	LOGFONT lFont;

	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 23;
	m_uFontIdTitle = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(255, 255, 255) );
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 24;
	//lFont.lfWeight = 700;
	m_uFontIdBtn = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(94, 23, 9) );
}

void YBFSDownLoader::_loadImage() {
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
	tstrImageFilePath = tstrImageDirPath + _T("Logo.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBk = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBFSBtnMinNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdMinBtnNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBFSBtnMinHor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdMinBtnHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBFSBtnMinPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdMinBtnPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBFSBtnCloNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdCloseBtnNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBFSBtnCloHor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdCloseBtnHor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBFSBtnCloPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdCloseBtnPre = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBFSDLerPBBk.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdPBBk = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBFSDLerPBFT.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdPBFT = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBFSBtnCelNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdCancelBtnNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBFSBtnCelHor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdCancelBtnHor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBFSBtnCelPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdCancelBtnPre = idImage;
}