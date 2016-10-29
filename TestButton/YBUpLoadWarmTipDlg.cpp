#include "stdafx.h"
#include "YBUpLoadWarmTipDlg.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlMisc.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlFontDepository.h"

#define YBULWTD_TIMEBK_SPACE ( 31 )
#define YBULWTD_TIMEBK_WIDTH ( 46 )
#define YBULWTD_TIMEBK_HEIGHT ( 41 )
#define YBULWTD_PROGRESSBAR_ID (0x00000502)
#define YBULWTD_BTN_CANCELUL_ID (0x00000503)
#define YBULWTD_BTN_CONTINUEUL_ID (0x00000504)
#define YBULWTD_BTN_RETURNCD_ID (0x00000505)

#define YBULWTD_ULFINISH_FLAG_NONE (0x00000000)
#define YBULWTD_ULFINISH_FLAG_SUCCESS (0x00000001)

USING_NAMESPACE_YBCTRL

YBUpLoadWarmTipDlg::YBUpLoadWarmTipDlg()
: m_uImageIdULWTDlgBk( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdULWTDlgTimeBk( YBCTRL_IMAGE_ID_INVALID )
, m_uIconULSuccess( YBCTRL_IMAGE_ID_INVALID )
, m_uIconULFailed( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdULWTDPBBk( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdULWTDPBFT( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdULWTDBubble( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdULWTDBtnNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdULWTDBtnHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdULWTDBtnPre( YBCTRL_IMAGE_ID_INVALID )
, m_uFlag( YBULWTD_ULFINISH_FLAG_NONE )
, m_tstrFFITxt1( _T("       此次上传文件") )
, m_tstrDFITxt1( _T("18个") )
, m_tstrFFITxt2( _T("，文件大小合计") )
, m_tstrDFITxt2( _T("200MB") )
, m_tstrFFITxt3( _T("，如中途关闭平台，本平台支持下次登录继续上传，本次上传共计需耗时") )
, m_tstrDFITxt3( _T("0时1分10秒") )
, m_tstrFFITxt4( _T("。") )
, m_tstrTitle( _T("温 馨 提 示") )
, m_tstrTimeTxt( _T("离上传完成的时间还有") )
, m_tstrTimeHourTxt( _T("0") )
, m_tstrTimeMinuteTxt( _T("01") )
, m_tstrTimeSecondTxt( _T("10") )
, m_tstrUints4Hour( _T("时") )
, m_tstrUints4Minute( _T("分") )
, m_tstrUints4Second( _T("秒") )
, m_tstrLeftBracket( _T("（") )
, m_tstrRightBracket( _T("点击右键查看已上传文件列表!）") )
, m_tstrSuccessTxt( _T("成功：") )
, m_tstrFailedTxt( _T("失败：") )
, m_tstrSuccessNumber( _T("5个") )
, m_tstrFailedNumber( _T("0个") )
, m_tstrPBProgressTxt( _T("") )
, m_tstrULCompleteTxt( _T("(上传完成)") )
, m_uFontIdULCompleteTxt( YBCTRL_FONT_ID_INVALID )
, m_uFontIdFixedFileInfo( YBCTRL_FONT_ID_INVALID )
, m_uFontIdDynamicFileInfo( YBCTRL_FONT_ID_INVALID )
, m_uFontIdTitle( YBCTRL_FONT_ID_INVALID )
, m_uFontIdTimeTxt( YBCTRL_FONT_ID_INVALID )
, m_uFontIdTime( YBCTRL_FONT_ID_INVALID )
, m_uFontIdULWTDlgRLTxt( YBCTRL_FONT_ID_INVALID )
, m_uFontIdULWTDBtn( YBCTRL_FONT_ID_INVALID )
, m_uFontIdULWTDBubbleTxt( YBCTRL_FONT_ID_INVALID )
, m_uLineHeight( 24 )
, m_uRowIndex( 0 )
{
	_loadImage();
	_loadFont();
	_init();
	_clearAll();

	m_hBrushULWTDlgRLBk = ::CreateSolidBrush( RGB( 184, 218, 250 ) );
}

YBUpLoadWarmTipDlg::~YBUpLoadWarmTipDlg() {
	if ( m_hBrushULWTDlgRLBk ) {
		::DeleteObject( m_hBrushULWTDlgRLBk );
		m_hBrushULWTDlgRLBk = NULL;
	}
}

HWND YBUpLoadWarmTipDlg::trackPopupWTDlg( HWND hWndParent ) {
	HWND hWnd;
	DWORD dwExStyle = WS_EX_TOPMOST|WS_EX_TOOLWINDOW;
	DWORD dwStyle = WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	int x;
	int y;
	int nWidth = 616;
	int nHeight = 361;
	int nScreenWidth;
	int nScreenHeight;

	nScreenWidth = ::GetSystemMetrics( SM_CXSCREEN );
	nScreenHeight = ::GetSystemMetrics( SM_CYSCREEN );
	x = ( nScreenWidth - nWidth )/2;
	y = ( nScreenHeight - nHeight )/2;

	hWnd = _createHWND( dwExStyle, dwStyle, x, y, nWidth, nHeight, hWndParent, (HMENU)NULL, NULL, NULL );
	if ( !hWnd ) {
		assert( false );
		return NULL;
	}
	m_progressBar.Create( 0, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 16, 280, nWidth - 32, 20, hWnd, (HMENU)YBULWTD_PROGRESSBAR_ID, NULL );
	m_btnCancelUL.Create( 0, WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, nWidth/2 - 60 - 85, 311, 85, 36, hWnd, (HMENU)YBULWTD_BTN_CANCELUL_ID, NULL );
	m_btnContinueUL.Create( 0, WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, nWidth/2 + 60, 311, 85, 36, hWnd, (HMENU)YBULWTD_BTN_CONTINUEUL_ID, NULL );
	m_btnReturnCD.Create( 0, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, nWidth/2 + 135, 311, 85, 36, hWnd, (HMENU)YBULWTD_BTN_RETURNCD_ID, NULL );
	//
	_clipWnd();

	return hWnd;

}

void YBUpLoadWarmTipDlg::_clipWnd() {
	HRGN hRgn;
	HRGN hRgnBtnCancelUL;
	HRGN hRgnBtnContinueUL;
	HRGN hRgnBtnReturnCD;
	RECT rcClient;
	RECT rcBtnCancelUL;
	RECT rcBtnContinueUL;
	RECT rcBtnReturnCD;

	::GetClientRect( m_hWnd, &rcClient );
	::GetClientRect( m_btnCancelUL.getHWND(), &rcBtnCancelUL );
	::GetClientRect( m_btnContinueUL.getHWND(), &rcBtnContinueUL );
	::GetClientRect( m_btnReturnCD.getHWND(), &rcBtnReturnCD );
	
	hRgn = ::CreateRoundRectRgn( 0, 0, rcClient.right + 1, rcClient.bottom + 1, 6, 6 );
	hRgnBtnCancelUL = ::CreateRoundRectRgn( 0, 0, rcBtnCancelUL.right + 1, rcBtnCancelUL.bottom + 1, 6, 6 );
	hRgnBtnContinueUL = ::CreateRoundRectRgn( 0, 0, rcBtnContinueUL.right + 1, rcBtnContinueUL.bottom + 1, 6, 6 );
	hRgnBtnReturnCD = ::CreateRoundRectRgn( 0, 0, rcBtnReturnCD.right + 1, rcBtnReturnCD.bottom + 1, 6, 6 );

	::SetWindowRgn( m_btnReturnCD.getHWND(), hRgnBtnReturnCD, TRUE );
	::SetWindowRgn( m_btnContinueUL.getHWND(), hRgnBtnContinueUL, TRUE );
	::SetWindowRgn( m_btnCancelUL.getHWND(), hRgnBtnCancelUL, TRUE );
	::SetWindowRgn( m_hWnd, hRgn, TRUE );
	::DeleteObject( hRgn );
	::DeleteObject( hRgnBtnCancelUL );
	::DeleteObject( hRgnBtnContinueUL );
	::DeleteObject( hRgnBtnReturnCD );
}

YBCTRL_WNDMSGMAP_BEGIN( YBUpLoadWarmTipDlg, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_NCHITTEST, _onWM_NCHITTEST )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
YBCTRL_WNDMSGMAP_END()

void YBUpLoadWarmTipDlg::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );
	//
	switch( wCtrlId ) {
	case YBULWTD_BTN_CANCELUL_ID:
		{
			::PostMessage( m_hWnd, WM_CLOSE, 0, 0 );
		}
		break;
	case YBULWTD_BTN_CONTINUEUL_ID:
		{
			::PostMessage( m_hWnd, WM_CLOSE, 0, 0 );
		}
		break;
	case YBULWTD_BTN_RETURNCD_ID:
		{

		}
		break;
	default:
		break;
	}
}

void YBUpLoadWarmTipDlg::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	HDC hPaintDC;
	PAINTSTRUCT ps;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void YBUpLoadWarmTipDlg::_onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_NCHITTEST );
	pYBCtrlWndMsg->m_dwFlag = (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = HTCAPTION;
}

void YBUpLoadWarmTipDlg::_onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	RECT rcClient;
	//
	pYBCtrlWndMsg->m_dwFlag = (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = 1;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	::BitBlt( hDC, 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top )
		, memDC.getMemDC(), 0, 0, SRCCOPY );
}

void YBUpLoadWarmTipDlg::updateStatus() {
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

void YBUpLoadWarmTipDlg::setPos( __int64 i64PosCur, __int64 i64PosMax ) {
	__int64 i64PosMinPB;
	__int64 i64PosMaxPB;
	m_progressBar.getRange( i64PosMinPB, i64PosMaxPB );
	if ( ( i64PosMinPB != 0 ) || ( i64PosMaxPB != i64PosMax ) ) {
		m_progressBar.setRange( 0, i64PosMax );
	}
	m_progressBar.setPos( i64PosCur );
	//::BringWindowToTop( m_hWnd );
	::SetForegroundWindow( m_hWnd );
}

void YBUpLoadWarmTipDlg::_movewindow() {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	::ShowWindow( m_btnCancelUL.getHWND(), SW_HIDE );
	::ShowWindow( m_btnContinueUL.getHWND(), SW_HIDE );

	::MoveWindow( m_btnCancelUL.getHWND(), 120, 311, 85, 36, TRUE );
	::MoveWindow( m_btnContinueUL.getHWND(), 63+120+85, 311, 85, 36, TRUE );
	::MoveWindow( m_btnReturnCD.getHWND(), 63*2+120+2*85, 311, 85, 36, TRUE );

	::ShowWindow( m_btnCancelUL.getHWND(), SW_NORMAL );
	::ShowWindow( m_btnContinueUL.getHWND(), SW_NORMAL );
	::ShowWindow( m_btnReturnCD.getHWND(), SW_NORMAL );
}

void YBUpLoadWarmTipDlg::_drawStatus( HDC hDC ) {
	_drawBk( hDC );
	_drawFFITxt1( hDC );
	_drawDFITxt1( hDC );
	_drawFFITxt2( hDC );
	_drawDFITxt2( hDC );
	_drawFFITxt3( hDC );
	_drawDFITxt3( hDC );
	_drawFFITxt4( hDC );
	_drawDlgTitle( hDC );
	//
	_drawULProgress( hDC );
	//
	_drawResultListBk( hDC );
	_drawIcon4SuccessAndFailed( hDC );
	_drawResultListTxt( hDC );
	_drawBubble( hDC );
	_drawBubbleTxt( hDC );
}

void YBUpLoadWarmTipDlg::_drawULProgress( HDC hDC ) {
	if ( ( m_uFlag & YBULWTD_ULFINISH_FLAG_SUCCESS )!= YBULWTD_ULFINISH_FLAG_SUCCESS ) {
		_drawTimeTxt( hDC );
		_drawTimeBk( hDC );
		_drawTime( hDC );
		_drawUints4Time( hDC );
	} else {
		_drawULComplete( hDC );
	}
}

void YBUpLoadWarmTipDlg::_drawULComplete( HDC hDC ) {
	if( !getHWND() )
		return;

	RECT rcComplete;
	HFONT hFontOld = NULL;
	int iBkModeOld;
	COLORREF clrTxtOld;
	const YBCtrlFont* pYBFont = NULL;

	if( _getULCompleteTxt( rcComplete ) ){
		pYBFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdULCompleteTxt );
		if( pYBFont ){
			hFontOld = (HFONT)::SelectObject( hDC, pYBFont->m_hFont );
			iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
			clrTxtOld = ::SetTextColor( hDC, pYBFont->m_clrTxt );

			::DrawText( hDC, m_tstrULCompleteTxt.c_str(), m_tstrULCompleteTxt.length(), &rcComplete, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );

			::SetTextColor( hDC, clrTxtOld );
			::SetBkMode( hDC, iBkModeOld );
			::SelectObject( hDC, hFontOld );
		}
	}
}

bool YBUpLoadWarmTipDlg::_getULCompleteTxt( RECT& rcCompleteTxt ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcCompleteTxt.left = rcClient.left;
	rcCompleteTxt.right = rcClient.right;
	rcCompleteTxt.top = rcClient.top + 142;
	rcCompleteTxt.bottom = rcCompleteTxt.top + 35;

	return ( ( rcCompleteTxt.right > rcCompleteTxt.left ) && ( rcCompleteTxt.bottom > rcCompleteTxt.top ) );
}

bool YBUpLoadWarmTipDlg::_getBubbleRect( RECT& rcBubble ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );

	rcBubble.left = rcClient.right/2 - 32;
	rcBubble.right = rcClient.right/2 + 32;
	rcBubble.top = rcClient.top + 249;
	rcBubble.bottom = rcBubble.top + 31;

	return ( ( rcBubble.right > rcBubble.left ) && ( rcBubble.bottom > rcBubble.top ) );
}

bool YBUpLoadWarmTipDlg::_getBubbleTextRect( RECT& rcBubbleText ) const {
	RECT rcBubble;

	if ( !_getBubbleRect( rcBubble ) )
		return false;
	rcBubbleText.left = rcBubble.left;
	rcBubbleText.right = rcBubble.right;
	rcBubbleText.top = rcBubble.top;
	rcBubbleText.bottom = rcBubble.bottom - 5;

	return ( ( rcBubble.right > rcBubble.left ) && ( rcBubble.bottom > rcBubble.top ) );
}

void YBUpLoadWarmTipDlg::_drawBubbleTxt( HDC hDC ) {
	if( !getHWND() )
		return;
	assert( m_progressBar.getHWND() );
	assert( ::IsWindowVisible( m_progressBar.getHWND() ) );

	tstring_type tstrTxt;
	RECT rcBubble;
	HFONT hFontOld = NULL;
	int iBkModeOld;
	COLORREF clrTxtOld;
	const YBCtrlFont* pYBFont = NULL;
	
	m_progressBar.getCurText( tstrTxt );
	if( _getBubbleTextRect( rcBubble ) ){
		pYBFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdULWTDBubbleTxt );
		if( pYBFont ){
			hFontOld = (HFONT)::SelectObject( hDC, pYBFont->m_hFont );
			iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
			clrTxtOld = ::SetTextColor( hDC, pYBFont->m_clrTxt );
			
			::DrawText( hDC, tstrTxt.c_str(), tstrTxt.length(), &rcBubble, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );

			::SetTextColor( hDC, clrTxtOld );
			::SetBkMode( hDC, iBkModeOld );
			::SelectObject( hDC, hFontOld );
		}
	}
}

void YBUpLoadWarmTipDlg::_drawBubble( HDC hDC ) {
	YBCtrlImageStatic* pYBImage = NULL;
	RECT rcBubble;

	pYBImage = _getImageStatic( m_uImageIdULWTDBubble );
	if ( !pYBImage )
		return;
	Gdiplus::Graphics graphics( hDC );
	if ( _getBubbleRect( rcBubble ) ) {
		GdipBitBlt( &graphics, pYBImage->getImage(), rcBubble.left, rcBubble.top, rcBubble.right - rcBubble.left,
			rcBubble.bottom - rcBubble.top, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
	}
}

void YBUpLoadWarmTipDlg::_drawIcon4SuccessAndFailed( HDC hDC ) {
	YBCtrlImageStatic* pYBIconSuccess = NULL;
	YBCtrlImageStatic* pYBIconFailed = NULL;
	RECT rcSuccess;
	RECT rcFailed;

	pYBIconSuccess = _getImageStatic( m_uIconULSuccess );
	pYBIconFailed = _getImageStatic( m_uIconULFailed );
	if ( !pYBIconSuccess || !pYBIconFailed ) {
		return;
	}

	Gdiplus::Graphics graphics( hDC );
	if ( _getIcon4Success( rcSuccess ) ) {
		GdipBitBlt( &graphics, pYBIconSuccess->getImage(), rcSuccess.left, rcSuccess.top, rcSuccess.right - rcSuccess.left,
			rcSuccess.bottom - rcSuccess.top, 0, 0, pYBIconSuccess->getWidth(), pYBIconSuccess->getHeight() );
	}
	if ( _getIcon4Failed( rcFailed ) ) {
		GdipBitBlt( &graphics, pYBIconFailed->getImage(), rcFailed.left, rcFailed.top, rcFailed.right - rcFailed.left,
			rcFailed.bottom - rcFailed.top, 0, 0, pYBIconFailed->getWidth(), pYBIconFailed->getHeight() );
	}
}

bool YBUpLoadWarmTipDlg::_getIcon4Success( RECT& rcIconSuccess ){
	RECT rcResultList;
	YBCtrlImageStatic* pYBIconSuccess = NULL;

	pYBIconSuccess = _getImageStatic( m_uIconULSuccess );
	if ( !pYBIconSuccess )
		return false;
	if ( !_getResultListRect( rcResultList ) )
		return false;
	rcIconSuccess.left = rcResultList.left + 115;
	rcIconSuccess.right = rcIconSuccess.left + pYBIconSuccess->getWidth();
	rcIconSuccess.top = rcResultList.top + ( rcResultList.bottom - rcResultList.top - pYBIconSuccess->getHeight() )/2;
	rcIconSuccess.bottom = rcIconSuccess.top + pYBIconSuccess->getHeight();

	return ( ( rcIconSuccess.right > rcIconSuccess.left ) && ( rcIconSuccess.bottom > rcIconSuccess.top ) );
}

bool YBUpLoadWarmTipDlg::_getIcon4Failed( RECT& rcIconFailed ){
	RECT rcSuccessTxt;
	YBCtrlImageStatic* pYBIconFailed = NULL;
	unsigned int uSuccessNumberCharWidth = _calcAllCharWidth( m_tstrSuccessNumber, m_containerULSuccessNumber );

	pYBIconFailed = _getImageStatic( m_uIconULFailed );
	if ( !pYBIconFailed )
		return false;
	if ( !_getSuccessRect( rcSuccessTxt ) )
		return false;
	rcIconFailed.left = rcSuccessTxt.right + uSuccessNumberCharWidth + 19;
	rcIconFailed.right = rcIconFailed.left + pYBIconFailed->getWidth();
	rcIconFailed.top = rcSuccessTxt.top + ( rcSuccessTxt.bottom - rcSuccessTxt.top - pYBIconFailed->getHeight() )/2 + 1;
	rcIconFailed.bottom = rcIconFailed.top + pYBIconFailed->getHeight();

	return ( ( rcIconFailed.right > rcIconFailed.left ) && ( rcIconFailed.bottom > rcIconFailed.top ) );
}

void YBUpLoadWarmTipDlg::_drawResultListBk( HDC hDC ) {
	RECT rcResultList;
	HBRUSH hBrushOld = NULL;

	if ( !_getResultListRect( rcResultList ) )
		return;
	::FillRect( hDC, &rcResultList, m_hBrushULWTDlgRLBk );
}

bool YBUpLoadWarmTipDlg::_getResultListRect( RECT& rcResultList ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcResultList.left = rcClient.left + 16;
	rcResultList.right = rcClient.right - 16;
	rcResultList.top = rcClient.top + 220;
	rcResultList.bottom = rcResultList.top + 29;

	return ( ( rcResultList.right > rcResultList.left ) && ( rcResultList.bottom > rcResultList.top ) );
}

void YBUpLoadWarmTipDlg::_drawBk( HDC hDC ) {
	YBCtrlImageStatic* pYBImage = NULL;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	pYBImage = _getImageStatic( m_uImageIdULWTDlgBk );

	if( !pYBImage )
		return;

	BackGroundBitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pYBImage->getMemDC(),
		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0 );
}

void YBUpLoadWarmTipDlg::_drawTimeBk( HDC hDC ) {
	RECT rcHourTxt;
	RECT rcMinuteTxt;
	RECT rcSecondTxt;
	YBCtrlImageStatic* pYBImageTimeBk = NULL;
	//
	pYBImageTimeBk = _getImageStatic( m_uImageIdULWTDlgTimeBk );
	if ( !pYBImageTimeBk )
		return;

	Gdiplus::Graphics graphics( hDC );
	if ( _getHourTxtRect( rcHourTxt ) ) {
		GdipBitBlt( &graphics, pYBImageTimeBk->getImage(), rcHourTxt.left, rcHourTxt.top, ( rcHourTxt.right - rcHourTxt.left ),
			( rcHourTxt.bottom - rcHourTxt.top ), 0, 0, pYBImageTimeBk->getWidth(), pYBImageTimeBk->getHeight() );
	}
	//
	if ( _getMinuteTxtRect( rcMinuteTxt ) ) {
		GdipBitBlt( &graphics, pYBImageTimeBk->getImage(), rcMinuteTxt.left, rcMinuteTxt.top, ( rcMinuteTxt.right - rcMinuteTxt.left ),
			( rcMinuteTxt.bottom - rcMinuteTxt.top ), 0, 0, pYBImageTimeBk->getWidth(), pYBImageTimeBk->getHeight() );
	}
	//
	if ( _getSecondTxtRect( rcSecondTxt ) ) {
		GdipBitBlt( &graphics, pYBImageTimeBk->getImage(), rcSecondTxt.left, rcSecondTxt.top, ( rcSecondTxt.right - rcSecondTxt.left ),
			( rcSecondTxt.bottom - rcSecondTxt.top ), 0, 0, pYBImageTimeBk->getWidth(), pYBImageTimeBk->getHeight() );
	}
}

ybctrl::YBCtrlImageStatic* YBUpLoadWarmTipDlg::_getImageStatic( unsigned int uImageId ) {
	YBCtrlImageBase* pYBImageBase = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pYBImageBase ) )
		return NULL;
	if( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

void YBUpLoadWarmTipDlg::_loadImage() {
	tstring_type tstrImageDirPath;
	tstring_type tstrImageFilePath;
	TCHAR szModulePath[MAX_PATH];
	TCHAR* posTmp = NULL;
	unsigned int idImage;

	::GetModuleFileName( NULL, szModulePath, MAX_PATH );
	posTmp = _tcsrchr( szModulePath, _T('\\') );
	if ( !posTmp ) {
		assert( false );
		return;
	}
	posTmp += 1;
	*posTmp = 0;
	tstrImageDirPath = szModulePath;
	tstrImageDirPath += _T("Image\\");
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBUpLoadWTDlgBk.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdULWTDlgBk = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBUpLoadWTDlgTimeBk.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdULWTDlgTimeBk = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBUpLoadWTDlgIconSuccess.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uIconULSuccess = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBUpLoadWTDlgIconFailed.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uIconULFailed = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBUpLoadWTDlgPBBk.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdULWTDPBBk = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBUpLoadWTDlgPBFT.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdULWTDPBFT = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBUpLoadWTDlgPBBubble.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdULWTDBubble = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBUpLoadWTDlgBtnNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdULWTDBtnNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBUpLoadWTDlgBtnHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdULWTDBtnHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBUpLoadWTDlgBtnPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdULWTDBtnPre = idImage;
}

void YBUpLoadWarmTipDlg::_loadFont() {
	LOGFONT lFont;
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 20;
	m_uFontIdFixedFileInfo = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(20, 20, 20) );
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 20;
	m_uFontIdDynamicFileInfo = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(20, 137, 0) );
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 25;
	lFont.lfWeight = 700;
	m_uFontIdTitle = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(255, 255, 255) );
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 23;
	lFont.lfWeight = 550;
	m_uFontIdTimeTxt = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(52, 80, 98) );
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 25;
	lFont.lfWeight = 700;
	m_uFontIdTime = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(159, 2, 6) );
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 19;
	m_uFontIdULWTDlgRLTxt = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(0, 0, 0) );
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 20;
	lFont.lfWeight = 550;
	m_uFontIdULWTDBtn = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(0, 0, 0) );
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 19;
	lFont.lfWeight = 700;
	m_uFontIdULWTDBubbleTxt = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(255, 255, 255) );
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 40;
	lFont.lfWeight = 600;
	m_uFontIdULCompleteTxt = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(91, 124, 3) );
}

void YBUpLoadWarmTipDlg::_init() {
	//m_progressBar.setTextAttri( m_uFontIdTimeTxt, 2, 2 );
	m_progressBar.setRange( 0, 1000 );
	m_progressBar.setPos( 500 );
	m_progressBar.setImageID( m_uImageIdULWTDPBBk, m_uImageIdULWTDPBFT );
	//
	m_btnCancelUL.setFontId( m_uFontIdULWTDBtn );
	m_btnCancelUL.setImage( ybctrl::YBButton::EBTN_STATUS_NORMAL, m_uImageIdULWTDBtnNor );
	m_btnCancelUL.setImage( ybctrl::YBButton::EBTN_STATUS_HOVER, m_uImageIdULWTDBtnHov );
	m_btnCancelUL.setImage( ybctrl::YBButton::EBTN_STATUS_PRESS, m_uImageIdULWTDBtnPre );
	m_btnCancelUL.setTextSpace( 2, 6, 2, 0 );
	m_btnCancelUL.setText( _T("取消上传") );
	//
	m_btnContinueUL.setFontId( m_uFontIdULWTDBtn );
	m_btnContinueUL.setImage( ybctrl::YBButton::EBTN_STATUS_NORMAL, m_uImageIdULWTDBtnNor );
	m_btnContinueUL.setImage( ybctrl::YBButton::EBTN_STATUS_HOVER, m_uImageIdULWTDBtnHov );
	m_btnContinueUL.setImage( ybctrl::YBButton::EBTN_STATUS_PRESS, m_uImageIdULWTDBtnPre );
	m_btnContinueUL.setTextSpace( 2, 6, 2, 0 );
	m_btnContinueUL.setText( _T("继续上传") );
	//
	m_btnReturnCD.setFontId( m_uFontIdULWTDBtn );
	m_btnReturnCD.setImage( ybctrl::YBButton::EBTN_STATUS_NORMAL, m_uImageIdULWTDBtnNor );
	m_btnReturnCD.setImage( ybctrl::YBButton::EBTN_STATUS_HOVER, m_uImageIdULWTDBtnHov );
	m_btnReturnCD.setImage( ybctrl::YBButton::EBTN_STATUS_PRESS, m_uImageIdULWTDBtnPre );
	m_btnReturnCD.setTextSpace( 2, 6, 2, 0 );
	m_btnReturnCD.setText( _T("返回上级") );
}

bool YBUpLoadWarmTipDlg::_getTimeTxtRect( RECT& rcTimeTxt ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcTimeTxt.left = rcClient.left;
	rcTimeTxt.right = rcClient.right;
	rcTimeTxt.top = rcClient.top + 130;
	rcTimeTxt.bottom = rcTimeTxt.top + 22;

	return ( ( rcTimeTxt.right > rcTimeTxt.left ) && ( rcTimeTxt.bottom > rcTimeTxt.top ) );
}

bool YBUpLoadWarmTipDlg::_getLeftBracket( RECT& rcLeftBracket ) const {
	RECT rcResultList;

	if ( !_getResultListRect( rcResultList ) )
		return false;
	rcLeftBracket.left = rcResultList.left + 105;
	rcLeftBracket.right = rcLeftBracket.left + 115;
	rcLeftBracket.top = rcResultList.top;
	rcLeftBracket.bottom = rcResultList.bottom;

	return ( ( rcLeftBracket.right > rcLeftBracket.left ) && ( rcLeftBracket.bottom > rcLeftBracket.top ) );
}

bool YBUpLoadWarmTipDlg::_getSuccessRect( RECT& rcSuccess ) const {
	RECT rcResultList;

	if ( !_getResultListRect( rcResultList ) )
		return false;
	rcSuccess.left = rcResultList.left + 135;
	rcSuccess.right = rcSuccess.left + 40;
	rcSuccess.top = rcResultList.top;
	rcSuccess.bottom = rcResultList.bottom;

	return ( ( rcSuccess.right > rcSuccess.left ) && ( rcSuccess.bottom > rcSuccess.top ) );
}

bool YBUpLoadWarmTipDlg::_getSuccessNumberRect( RECT& rcNumber4Success ){
	RECT rcResultList;
	unsigned int uNumberCharWidth;

	if ( !_getResultListRect( rcResultList ) )
		return false;
	uNumberCharWidth = _calcAllCharWidth( m_tstrSuccessNumber, m_containerULSuccessNumber );

	rcNumber4Success.left = rcResultList.left + 174;
	rcNumber4Success.right = rcNumber4Success.left + uNumberCharWidth;
	rcNumber4Success.top = rcResultList.top;
	rcNumber4Success.bottom = rcResultList.bottom;

	return ( ( rcNumber4Success.right > rcNumber4Success.left ) && ( rcNumber4Success.bottom > rcNumber4Success.top ) );
}

bool YBUpLoadWarmTipDlg::_getFailedRect( RECT& rcFailed ){
	RECT rcIconFailed;

	if ( !_getIcon4Failed( rcIconFailed ) )
		return false;
	rcFailed.left = rcIconFailed.right + 5;
	rcFailed.right = rcFailed.left + 40;
	rcFailed.top = rcIconFailed.top;
	rcFailed.bottom = rcIconFailed.bottom;

	return ( ( rcFailed.right > rcFailed.left ) && ( rcFailed.bottom > rcFailed.top ) );
}

bool YBUpLoadWarmTipDlg::_getFailedNumberRect( RECT& rcNumber4Failed ){
	RECT rcFailTxt;
	unsigned int uFailedNumberCharWidth = _calcAllCharWidth( m_tstrFailedNumber, m_containerULFailedNumber );

	if ( !_getFailedRect( rcFailTxt ) )
		return false;
	rcNumber4Failed.left = rcFailTxt.right + 2;
	rcNumber4Failed.right = rcNumber4Failed.left + uFailedNumberCharWidth;
	rcNumber4Failed.top = rcFailTxt.top;
	rcNumber4Failed.bottom = rcFailTxt.bottom;

	return ( ( rcNumber4Failed.right > rcNumber4Failed.left ) && ( rcNumber4Failed.bottom > rcNumber4Failed.top ) );
}

bool YBUpLoadWarmTipDlg::_getRightBracket( RECT& rcRightBracket ){
	RECT rcFailedNumer;
	RECT rcResultList;

	if ( !_getResultListRect( rcResultList ) ) 
		return false;
	if ( !_getFailedNumberRect( rcFailedNumer ) )
		return false;
	rcRightBracket.left = rcFailedNumer.right + 19;
	rcRightBracket.right = rcResultList.right;
	rcRightBracket.top = rcFailedNumer.top;
	rcRightBracket.bottom = rcFailedNumer.bottom;

	return ( ( rcRightBracket.right > rcRightBracket.left ) && ( rcRightBracket.bottom > rcRightBracket.top ) );
}

void YBUpLoadWarmTipDlg::_drawTime( HDC hDC ) {
	if( !getHWND() )
		return;

	RECT rcTimeHour;
	RECT rcTimeMinute;
	RECT rcTimeSecond;
	HFONT hFontOld = NULL;
	int iBkModeOld;
	COLORREF clrTxtOld;
	const YBCtrlFont* pYBFont = NULL;

	if( _getHourTxtRect( rcTimeHour ) &&
		_getSecondTxtRect( rcTimeSecond ) &&
		_getMinuteTxtRect( rcTimeMinute ) ){
		pYBFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdTime );
		if( pYBFont ){
			hFontOld = (HFONT)::SelectObject( hDC, pYBFont->m_hFont );
			iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
			clrTxtOld = ::SetTextColor( hDC, pYBFont->m_clrTxt );

			::DrawText( hDC, m_tstrTimeHourTxt.c_str(), m_tstrTimeHourTxt.length(), &rcTimeHour, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
			::DrawText( hDC, m_tstrTimeMinuteTxt.c_str(), m_tstrTimeMinuteTxt.length(), &rcTimeMinute, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
			::DrawText( hDC, m_tstrTimeSecondTxt.c_str(), m_tstrTimeSecondTxt.length(), &rcTimeSecond, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );

			::SetTextColor( hDC, clrTxtOld );
			::SetBkMode( hDC, iBkModeOld );
			::SelectObject( hDC, hFontOld );
		}
	}
}

void YBUpLoadWarmTipDlg::_drawResultListTxt( HDC hDC ) {
	if( !getHWND() )
		return;

	RECT rcLeftBracket;
	RECT rcSuccessTxt;
	RECT rcSuccessNumber;
	RECT rcFailedTxt;
	RECT rcFailedNumber;
	RECT rcRightBracket;
	HFONT hFontOld = NULL;
	int iBkModeOld;
	COLORREF clrTxtOld;
	const YBCtrlFont* pYBFont = NULL;

	if( _getLeftBracket( rcLeftBracket ) &&
		_getSuccessRect( rcSuccessTxt ) &&
		_getSuccessNumberRect( rcSuccessNumber ) &&
		_getFailedRect( rcFailedTxt ) &&
		_getFailedNumberRect( rcFailedNumber ) &&
		_getRightBracket( rcRightBracket ) ) {
		pYBFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdULWTDlgRLTxt );
		if( pYBFont ){
			hFontOld = (HFONT)::SelectObject( hDC, pYBFont->m_hFont );
			iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
			clrTxtOld = ::SetTextColor( hDC, pYBFont->m_clrTxt );

			::DrawText( hDC, m_tstrLeftBracket.c_str(), m_tstrLeftBracket.length(), &rcLeftBracket, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
			::DrawText( hDC, m_tstrSuccessTxt.c_str(), m_tstrSuccessTxt.length(), &rcSuccessTxt, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
			::DrawText( hDC, m_tstrSuccessNumber.c_str(), m_tstrSuccessNumber.length(), &rcSuccessNumber, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
			::DrawText( hDC, m_tstrFailedTxt.c_str(), m_tstrFailedTxt.length(), &rcFailedTxt, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
			::DrawText( hDC, m_tstrFailedNumber.c_str(), m_tstrFailedNumber.length(), &rcFailedNumber, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
			::DrawText( hDC, m_tstrRightBracket.c_str(), m_tstrRightBracket.length(), &rcRightBracket, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );

			::SetTextColor( hDC, clrTxtOld );
			::SetBkMode( hDC, iBkModeOld );
			::SelectObject( hDC, hFontOld );
		}
	}
}

void YBUpLoadWarmTipDlg::_drawTimeTxt( HDC hDC ) {
	if( !getHWND() )
		return;

	RECT rcTimeTxt;
	HFONT hFontOld = NULL;
	int iBkModeOld;
	COLORREF clrTxtOld;
	const YBCtrlFont* pYBFont = NULL;

	if( _getTimeTxtRect( rcTimeTxt ) ){
		pYBFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdTimeTxt );
		if( pYBFont ){
			hFontOld = (HFONT)::SelectObject( hDC, pYBFont->m_hFont );
			iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
			clrTxtOld = ::SetTextColor( hDC, pYBFont->m_clrTxt );

			::DrawText( hDC, m_tstrTimeTxt.c_str(), m_tstrTimeTxt.length(), &rcTimeTxt, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );

			::SetTextColor( hDC, clrTxtOld );
			::SetBkMode( hDC, iBkModeOld );
			::SelectObject( hDC, hFontOld );
		}
	}
}

void YBUpLoadWarmTipDlg::_drawUints4Time( HDC hDC ) {
	if( !getHWND() )
		return;

	RECT rcUints4Hour;
	RECT rcUints4Minute;
	RECT rcUints4Second;
	HFONT hFontOld = NULL;
	int iBkModeOld;
	COLORREF clrTxtOld;
	const YBCtrlFont* pYBFont = NULL;

	if( _getUints4Hour( rcUints4Hour ) &&
		_getUints4Minute( rcUints4Minute ) &&
		_getUints4Second( rcUints4Second ) ) {
		pYBFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdTimeTxt );
		if( pYBFont ){
			hFontOld = (HFONT)::SelectObject( hDC, pYBFont->m_hFont );
			iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
			clrTxtOld = ::SetTextColor( hDC, pYBFont->m_clrTxt );

			::DrawText( hDC, m_tstrUints4Hour.c_str(), m_tstrUints4Hour.length(), &rcUints4Hour, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
			::DrawText( hDC, m_tstrUints4Minute.c_str(), m_tstrUints4Minute.length(), &rcUints4Minute, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
			::DrawText( hDC, m_tstrUints4Second.c_str(), m_tstrUints4Second.length(), &rcUints4Second, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );

			::SetTextColor( hDC, clrTxtOld );
			::SetBkMode( hDC, iBkModeOld );
			::SelectObject( hDC, hFontOld );
		}
	}
}

bool YBUpLoadWarmTipDlg::_getUints4Hour( RECT& rcUints4Hour ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcUints4Hour.left = rcClient.left + 200 + YBULWTD_TIMEBK_WIDTH;
	rcUints4Hour.right = rcUints4Hour.left + YBULWTD_TIMEBK_SPACE;
	rcUints4Hour.top = rcClient.top + 164;
	rcUints4Hour.bottom = rcUints4Hour.top + YBULWTD_TIMEBK_HEIGHT;

	return ( ( rcUints4Hour.right > rcUints4Hour.left ) && ( rcUints4Hour.bottom > rcUints4Hour.top ) );
}

bool YBUpLoadWarmTipDlg::_getUints4Minute( RECT& rcUints4Minute ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcUints4Minute.left = rcClient.left + 200 + YBULWTD_TIMEBK_WIDTH*2 + YBULWTD_TIMEBK_SPACE;
	rcUints4Minute.right = rcUints4Minute.left + YBULWTD_TIMEBK_SPACE;
	rcUints4Minute.top = rcClient.top + 164;
	rcUints4Minute.bottom = rcUints4Minute.top + YBULWTD_TIMEBK_HEIGHT;

	return ( ( rcUints4Minute.right > rcUints4Minute.left ) && ( rcUints4Minute.bottom > rcUints4Minute.top ) );
}

bool YBUpLoadWarmTipDlg::_getUints4Second( RECT& rcUints4Second ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcUints4Second.left = rcClient.left + 200 + YBULWTD_TIMEBK_WIDTH*3 + YBULWTD_TIMEBK_SPACE*2;
	rcUints4Second.right = rcUints4Second.left + YBULWTD_TIMEBK_SPACE;
	rcUints4Second.top = rcClient.top + 164;
	rcUints4Second.bottom = rcUints4Second.top + YBULWTD_TIMEBK_HEIGHT;

	return ( ( rcUints4Second.right > rcUints4Second.left ) && ( rcUints4Second.bottom > rcUints4Second.top ) );
}

bool YBUpLoadWarmTipDlg::_getHourTxtRect( RECT& rcHourTxt ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcHourTxt.left = rcClient.left + 200;
	rcHourTxt.top = rcClient.top + 164;
	rcHourTxt.right = rcHourTxt.left + YBULWTD_TIMEBK_WIDTH;
	rcHourTxt.bottom = rcHourTxt.top + YBULWTD_TIMEBK_HEIGHT;

	return ( ( rcHourTxt.right > rcHourTxt.left ) && ( rcHourTxt.bottom > rcHourTxt.top ) );
}

bool YBUpLoadWarmTipDlg::_getMinuteTxtRect( RECT& rcMinuteTxt ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcMinuteTxt.left = rcClient.left + 200 + YBULWTD_TIMEBK_WIDTH +YBULWTD_TIMEBK_SPACE;
	rcMinuteTxt.top = rcClient.top + 164;
	rcMinuteTxt.right = rcMinuteTxt.left + YBULWTD_TIMEBK_WIDTH;
	rcMinuteTxt.bottom = rcMinuteTxt.top + YBULWTD_TIMEBK_HEIGHT;

	return ( ( rcMinuteTxt.right > rcMinuteTxt.left ) && ( rcMinuteTxt.bottom > rcMinuteTxt.top ) );
}

bool YBUpLoadWarmTipDlg::_getSecondTxtRect( RECT& rcSecondTxt ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcSecondTxt.left = rcClient.left + 200 + ( YBULWTD_TIMEBK_WIDTH + YBULWTD_TIMEBK_SPACE )*2;
	rcSecondTxt.top = rcClient.top + 164;
	rcSecondTxt.right = rcSecondTxt.left + YBULWTD_TIMEBK_WIDTH;
	rcSecondTxt.bottom = rcSecondTxt.top + YBULWTD_TIMEBK_HEIGHT;

	return ( ( rcSecondTxt.right > rcSecondTxt.left ) && ( rcSecondTxt.bottom > rcSecondTxt.top ) );
}

bool YBUpLoadWarmTipDlg::_getTitleRect( RECT& rcTitle ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcTitle.left = rcClient.left;
	rcTitle.right = rcClient.right;
	rcTitle.top = rcClient.top;
	rcTitle.bottom = rcTitle.top + 46;

	return ( ( rcTitle.right > rcTitle.left ) && ( rcTitle.bottom > rcTitle.top ) );
}

void YBUpLoadWarmTipDlg::_drawDlgTitle( HDC hDC ) {
	if( !getHWND() )
		return;

	RECT rcTitle;
	HFONT hFontOld = NULL;
	int iBkModeOld;
	COLORREF clrTxtOld;
	const YBCtrlFont* pYBFont = NULL;
	
	if( _getTitleRect( rcTitle ) ){
		pYBFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdTitle );
		if( pYBFont ){
			hFontOld = (HFONT)::SelectObject( hDC, pYBFont->m_hFont );
			iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
			clrTxtOld = ::SetTextColor( hDC, pYBFont->m_clrTxt );

			::DrawText( hDC, m_tstrTitle.c_str(), m_tstrTitle.length(), &rcTitle, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );

			::SetTextColor( hDC, clrTxtOld );
			::SetBkMode( hDC, iBkModeOld );
			::SelectObject( hDC, hFontOld );
		}
	}
}

bool YBUpLoadWarmTipDlg::_getFADFileInfoTxtRect( RECT& rcFITxt ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcFITxt.left = rcClient.left + 35;
	rcFITxt.right = rcClient.right - 35;
	rcFITxt.top = rcClient.top + 68;
	rcFITxt.bottom = rcFITxt.top + 55;

	return ( ( rcFITxt.right > rcFITxt.left ) && ( rcFITxt.bottom > rcFITxt.top ) );
}

void YBUpLoadWarmTipDlg::_drawFFITxt4( HDC hDC ) {
	RECT rcCalc;
	RECT rcFITxt;
	unsigned int uWidth;
	unsigned int uCharIndex = 0;
	unsigned int uCharCountCalc;
	tstring_type tstrLineData;
	const YBCtrlFont* pYBCtrlFont = NULL;
	HFONT hFontOld;

	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdFixedFileInfo );
	if( !pYBCtrlFont )
		return ;
	if ( !_getFADFileInfoTxtRect( rcFITxt ) )
		return;

	hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
	uWidth = rcFITxt.right - rcFITxt.left;
	uCharIndex = 0;
	int iBkModeOld;
	COLORREF clrTxtOld;

	iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
	clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
	unsigned int uChar7Width = _calcAllCharWidth( m_tstrFFITxt4, m_containerFixedCharWidth4 );
	if ( uWidth - m_uSingleRowCharWidth > uChar7Width ) {
		while ( uCharIndex < m_tstrFFITxt4.length() ) {
			if ( _calcCharCount( m_containerFixedCharWidth4, uCharIndex, uWidth - m_uSingleRowCharWidth, uCharCountCalc ) ) {
				//
				::SetRect( &rcCalc, rcFITxt.left + m_uSingleRowCharWidth, rcFITxt.top + ( m_uRowIndex - 1 )*m_uLineHeight, rcFITxt.right, rcFITxt.top + m_uRowIndex*m_uLineHeight );
				//
				tstrLineData = m_tstrFFITxt4.substr( uCharIndex, uCharCountCalc );
				//
				::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
				//
				uCharIndex += uCharCountCalc;
			} else {
				assert( false );
				break;
			}
		}
		m_uSingleRowCharWidth += uChar7Width;
	} else {
		if ( _calcCharCount( m_containerFixedCharWidth4, uCharIndex, uWidth - m_uSingleRowCharWidth, uCharCountCalc ) ) {
			//
			::SetRect( &rcCalc, rcFITxt.left + m_uSingleRowCharWidth, rcFITxt.top + ( m_uRowIndex - 1 )*m_uLineHeight, rcFITxt.right, rcFITxt.top + m_uRowIndex*m_uLineHeight );
			//
			tstrLineData = m_tstrFFITxt4.substr( uCharIndex, uCharCountCalc );
			//
			::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
			//
			uCharIndex += uCharCountCalc;
			++m_uRowIndex;
		}
		while ( uCharIndex < m_tstrFFITxt4.length() ) {
			if ( _calcCharCount( m_containerFixedCharWidth4, uCharIndex, uWidth, uCharCountCalc ) ) {
				//
				::SetRect( &rcCalc, rcFITxt.left, rcFITxt.top + ( m_uRowIndex - 1 )*m_uLineHeight, rcFITxt.right, rcFITxt.top + m_uRowIndex*m_uLineHeight );
				//
				tstrLineData = m_tstrFFITxt4.substr( uCharIndex, uCharCountCalc );
				//
				::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
				//
				uCharIndex += uCharCountCalc;
				if ( uCharIndex < m_tstrFFITxt4.length() ) {
					++m_uRowIndex;
				}
			}
		}
		m_uSingleRowCharWidth = _calcAllCharWidth( tstrLineData, m_containerSingleRowChar );
	}
	::SetTextColor( hDC, clrTxtOld );
	::SetBkMode( hDC, iBkModeOld );
	::SelectObject( hDC, hFontOld );
}

void YBUpLoadWarmTipDlg::_drawDFITxt3( HDC hDC ) {
	RECT rcCalc;
	RECT rcFITxt;
	unsigned int uWidth;
	unsigned int uCharIndex = 0;
	unsigned int uCharCountCalc;
	tstring_type tstrLineData;
	const YBCtrlFont* pYBCtrlFont = NULL;
	HFONT hFontOld;

	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdDynamicFileInfo );
	if( !pYBCtrlFont )
		return ;
	if ( !_getFADFileInfoTxtRect( rcFITxt ) )
		return;

	hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
	uWidth = rcFITxt.right - rcFITxt.left;
	uCharIndex = 0;
	int iBkModeOld;
	COLORREF clrTxtOld;

	iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
	clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
	unsigned int uChar6Width = _calcAllCharWidth( m_tstrDFITxt3, m_containerDynamicCharWidth3 );
	if ( uWidth - m_uSingleRowCharWidth > uChar6Width ) {
		while ( uCharIndex < m_tstrDFITxt3.length() ) {
			if ( _calcCharCount( m_containerDynamicCharWidth3, uCharIndex, uWidth - m_uSingleRowCharWidth, uCharCountCalc ) ) {
				//
				::SetRect( &rcCalc, rcFITxt.left + m_uSingleRowCharWidth, rcFITxt.top + ( m_uRowIndex - 1 )*m_uLineHeight, rcFITxt.right, rcFITxt.top + m_uRowIndex*m_uLineHeight );
				//
				tstrLineData = m_tstrDFITxt3.substr( uCharIndex, uCharCountCalc );
				//
				::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
				//
				uCharIndex += uCharCountCalc;
			} else {
				assert( false );
				break;
			}
		}
		m_uSingleRowCharWidth += uChar6Width;
	} else {
		if ( _calcCharCount( m_containerDynamicCharWidth3, uCharIndex, uWidth - m_uSingleRowCharWidth, uCharCountCalc ) ) {
			//
			::SetRect( &rcCalc, rcFITxt.left + m_uSingleRowCharWidth, rcFITxt.top + ( m_uRowIndex - 1 )*m_uLineHeight, rcFITxt.right, rcFITxt.top + m_uRowIndex*m_uLineHeight );
			//
			tstrLineData = m_tstrDFITxt3.substr( uCharIndex, uCharCountCalc );
			//
			::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
			//
			uCharIndex += uCharCountCalc;
			++m_uRowIndex;
		}
		while ( uCharIndex < m_tstrDFITxt3.length() ) {
			if ( _calcCharCount( m_containerDynamicCharWidth3, uCharIndex, uWidth, uCharCountCalc ) ) {
				//
				::SetRect( &rcCalc, rcFITxt.left, rcFITxt.top + ( m_uRowIndex - 1 )*m_uLineHeight, rcFITxt.right, rcFITxt.top + m_uRowIndex*m_uLineHeight );
				//
				tstrLineData = m_tstrDFITxt3.substr( uCharIndex, uCharCountCalc );
				//
				::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
				//
				uCharIndex += uCharCountCalc;
				if ( uCharIndex < m_tstrDFITxt3.length() ) {
					++m_uRowIndex;
				}
			}
		}
		m_uSingleRowCharWidth = _calcAllCharWidth( tstrLineData, m_containerSingleRowChar );
	}
	::SetTextColor( hDC, clrTxtOld );
	::SetBkMode( hDC, iBkModeOld );
	::SelectObject( hDC, hFontOld );
}

void YBUpLoadWarmTipDlg::_drawFFITxt3( HDC hDC ) {
	RECT rcCalc;
	RECT rcFITxt;
	unsigned int uWidth;
	unsigned int uCharIndex = 0;
	unsigned int uCharCountCalc;
	tstring_type tstrLineData;
	const YBCtrlFont* pYBCtrlFont = NULL;
	HFONT hFontOld;

	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdFixedFileInfo );
	if( !pYBCtrlFont )
		return ;
	if ( !_getFADFileInfoTxtRect( rcFITxt ) )
		return;

	hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
	uWidth = rcFITxt.right - rcFITxt.left;
	uCharIndex = 0;
	int iBkModeOld;
	COLORREF clrTxtOld;

	iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
	clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
	unsigned int uChar5Width = _calcAllCharWidth( m_tstrFFITxt3, m_containerFixedCharWidth3 );
	if ( uWidth - m_uSingleRowCharWidth > uChar5Width ) {
		while ( uCharIndex < m_tstrFFITxt3.length() ) {
			if ( _calcCharCount( m_containerFixedCharWidth3, uCharIndex, uWidth - m_uSingleRowCharWidth, uCharCountCalc ) ) {
				//
				::SetRect( &rcCalc, rcFITxt.left + m_uSingleRowCharWidth, rcFITxt.top + ( m_uRowIndex - 1 )*m_uLineHeight, rcFITxt.right, rcFITxt.top + m_uRowIndex*m_uLineHeight );
				//
				tstrLineData = m_tstrFFITxt3.substr( uCharIndex, uCharCountCalc );
				//
				::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
				//
				uCharIndex += uCharCountCalc;
			} else {
				assert( false );
				break;
			}
		}
		m_uSingleRowCharWidth += uChar5Width;
	} else {
		if ( _calcCharCount( m_containerFixedCharWidth3, uCharIndex, uWidth - m_uSingleRowCharWidth, uCharCountCalc ) ) {
			//
			::SetRect( &rcCalc, rcFITxt.left + m_uSingleRowCharWidth, rcFITxt.top + ( m_uRowIndex - 1 )*m_uLineHeight, rcFITxt.right, rcFITxt.top + m_uRowIndex*m_uLineHeight );
			//
			tstrLineData = m_tstrFFITxt3.substr( uCharIndex, uCharCountCalc );
			//
			::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
			//
			uCharIndex += uCharCountCalc;
			++m_uRowIndex;
		}
		while ( uCharIndex < m_tstrFFITxt3.length() ) {
			if ( _calcCharCount( m_containerFixedCharWidth3, uCharIndex, uWidth, uCharCountCalc ) ) {
				//
				::SetRect( &rcCalc, rcFITxt.left, rcFITxt.top + ( m_uRowIndex - 1 )*m_uLineHeight, rcFITxt.right, rcFITxt.top + m_uRowIndex*m_uLineHeight );
				//
				tstrLineData = m_tstrFFITxt3.substr( uCharIndex, uCharCountCalc );
				//
				::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
				//
				uCharIndex += uCharCountCalc;
				if ( uCharIndex < m_tstrFFITxt3.length() ) {
					++m_uRowIndex;
				}
			}
		}
		m_uSingleRowCharWidth = _calcAllCharWidth( tstrLineData, m_containerSingleRowChar );
	}
	::SetTextColor( hDC, clrTxtOld );
	::SetBkMode( hDC, iBkModeOld );
	::SelectObject( hDC, hFontOld );
}

void YBUpLoadWarmTipDlg::_drawDFITxt2( HDC hDC ) {
	RECT rcCalc;
	RECT rcFITxt;
	unsigned int uWidth;
	unsigned int uCharIndex = 0;
	unsigned int uCharCountCalc;
	tstring_type tstrLineData;
	const YBCtrlFont* pYBCtrlFont = NULL;
	HFONT hFontOld;

	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdDynamicFileInfo );
	if( !pYBCtrlFont )
		return ;
	if ( !_getFADFileInfoTxtRect( rcFITxt ) )
		return;

	hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
	uWidth = rcFITxt.right - rcFITxt.left;
	uCharIndex = 0;
	int iBkModeOld;
	COLORREF clrTxtOld;

	iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
	clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
	unsigned int uChar4Width = _calcAllCharWidth( m_tstrDFITxt2, m_containerDynamicCharWidth2 );
	if ( uWidth - m_uSingleRowCharWidth > uChar4Width ) {
		while ( uCharIndex < m_tstrDFITxt2.length() ) {
			if ( _calcCharCount( m_containerDynamicCharWidth2, uCharIndex, uWidth - m_uSingleRowCharWidth, uCharCountCalc ) ) {
				//
				::SetRect( &rcCalc, rcFITxt.left + m_uSingleRowCharWidth, rcFITxt.top + ( m_uRowIndex - 1 )*m_uLineHeight, rcFITxt.right, rcFITxt.top + m_uRowIndex*m_uLineHeight );
				//
				tstrLineData = m_tstrDFITxt2.substr( uCharIndex, uCharCountCalc );
				//
				::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
				//
				uCharIndex += uCharCountCalc;
			} else {
				assert( false );
				break;
			}
		}
		m_uSingleRowCharWidth += uChar4Width;
	} else {
		if ( _calcCharCount( m_containerDynamicCharWidth2, uCharIndex, uWidth - m_uSingleRowCharWidth, uCharCountCalc ) ) {
			//
			::SetRect( &rcCalc, rcFITxt.left + m_uSingleRowCharWidth, rcFITxt.top + ( m_uRowIndex - 1 )*m_uLineHeight, rcFITxt.right, rcFITxt.top + m_uRowIndex*m_uLineHeight );
			//
			tstrLineData = m_tstrDFITxt2.substr( uCharIndex, uCharCountCalc );
			//
			::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
			//
			uCharIndex += uCharCountCalc;
			++m_uRowIndex;
		}
		while ( uCharIndex < m_tstrDFITxt2.length() ) {
			if ( _calcCharCount( m_containerDynamicCharWidth2, uCharIndex, uWidth, uCharCountCalc ) ) {
				//
				::SetRect( &rcCalc, rcFITxt.left, rcFITxt.top + ( m_uRowIndex - 1 )*m_uLineHeight, rcFITxt.right, rcFITxt.top + m_uRowIndex*m_uLineHeight );
				//
				tstrLineData = m_tstrDFITxt2.substr( uCharIndex, uCharCountCalc );
				//
				::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
				//
				uCharIndex += uCharCountCalc;
				if ( uCharIndex < m_tstrDFITxt2.length() ) {
					++m_uRowIndex;
				}
			}
		}
		m_uSingleRowCharWidth = _calcAllCharWidth( tstrLineData, m_containerSingleRowChar );
	}
	::SetTextColor( hDC, clrTxtOld );
	::SetBkMode( hDC, iBkModeOld );
	::SelectObject( hDC, hFontOld );
}

void YBUpLoadWarmTipDlg::_drawFFITxt2( HDC hDC ) {
	RECT rcCalc;
	RECT rcFITxt;
	unsigned int uWidth;
	unsigned int uCharIndex = 0;
	unsigned int uCharCountCalc;
	tstring_type tstrLineData;
	const YBCtrlFont* pYBCtrlFont = NULL;
	HFONT hFontOld;

	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdFixedFileInfo );
	if( !pYBCtrlFont )
		return ;
	if ( !_getFADFileInfoTxtRect( rcFITxt ) )
		return;

	hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
	uWidth = rcFITxt.right - rcFITxt.left;
	uCharIndex = 0;
	int iBkModeOld;
	COLORREF clrTxtOld;

	iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
	clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
	unsigned int uChar3Width = _calcAllCharWidth( m_tstrFFITxt2, m_containerFixedCharWidth2 );
	if ( uWidth - m_uSingleRowCharWidth > uChar3Width ) {
		while ( uCharIndex < m_tstrFFITxt2.length() ) {
			if ( _calcCharCount( m_containerFixedCharWidth2, uCharIndex, uWidth - m_uSingleRowCharWidth, uCharCountCalc ) ) {
				//
				::SetRect( &rcCalc, rcFITxt.left + m_uSingleRowCharWidth, rcFITxt.top + ( m_uRowIndex - 1 )*m_uLineHeight, rcFITxt.right, rcFITxt.top + m_uRowIndex*m_uLineHeight );
				//
				tstrLineData = m_tstrFFITxt2.substr( uCharIndex, uCharCountCalc );
				//
				::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
				//
				uCharIndex += uCharCountCalc;
			} else {
				assert( false );
				break;
			}
		}
		m_uSingleRowCharWidth += uChar3Width;
	} else {
		if ( _calcCharCount( m_containerFixedCharWidth2, uCharIndex, uWidth - m_uSingleRowCharWidth, uCharCountCalc ) ) {
			//
			::SetRect( &rcCalc, rcFITxt.left + m_uSingleRowCharWidth, rcFITxt.top + ( m_uRowIndex - 1 )*m_uLineHeight, rcFITxt.right, rcFITxt.top + m_uRowIndex*m_uLineHeight );
			//
			tstrLineData = m_tstrFFITxt2.substr( uCharIndex, uCharCountCalc );
			//
			::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
			//
			uCharIndex += uCharCountCalc;
			++m_uRowIndex;
		}
		while ( uCharIndex < m_tstrFFITxt2.length() ) {
			if ( _calcCharCount( m_containerFixedCharWidth2, uCharIndex, uWidth, uCharCountCalc ) ) {
				//
				::SetRect( &rcCalc, rcFITxt.left, rcFITxt.top + ( m_uRowIndex - 1 )*m_uLineHeight, rcFITxt.right, rcFITxt.top + m_uRowIndex*m_uLineHeight );
				//
				tstrLineData = m_tstrFFITxt2.substr( uCharIndex, uCharCountCalc );
				//
				::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
				//
				uCharIndex += uCharCountCalc;
				if ( uCharIndex < m_tstrFFITxt2.length() ) {
					++m_uRowIndex;
				}
			}
		}
		m_uSingleRowCharWidth = _calcAllCharWidth( tstrLineData, m_containerSingleRowChar );
	}
	::SetTextColor( hDC, clrTxtOld );
	::SetBkMode( hDC, iBkModeOld );
	::SelectObject( hDC, hFontOld );
}

void YBUpLoadWarmTipDlg::_drawFFITxt1( HDC hDC ) {
	//
	_clearAll();
	m_uRowIndex = 0;
	//
	RECT rcCalc;
	RECT rcFITxt;
	unsigned int uWidth;
	unsigned int uCharIndex = 0;
	unsigned int uCharCountCalc;
	//unsigned int uLineIndex;
	tstring_type tstrLineData;
	const YBCtrlFont* pYBCtrlFont = NULL;
	HFONT hFontOld;

	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdFixedFileInfo );
	if( !pYBCtrlFont )
		return ;
	if ( !_getFADFileInfoTxtRect( rcFITxt ) )
		return;

	hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
	uWidth = rcFITxt.right - rcFITxt.left;
	//uLineTop = 0;
	uCharIndex = 0;
	int iBkModeOld;
	COLORREF clrTxtOld;

	iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
	clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
	unsigned int uChar1Width = _calcAllCharWidth( m_tstrFFITxt1, m_containerFixedCharWidth1 );
	while ( uCharIndex < m_tstrFFITxt1.length() ) {
		if ( _calcCharCount( m_containerFixedCharWidth1, uCharIndex, uWidth, uCharCountCalc ) ) {
			//
			//uLineTop = uLineIndex*m_uLineHeight;
			::SetRect( &rcCalc, rcFITxt.left, rcFITxt.top, uWidth, rcFITxt.top + m_uLineHeight );
			//
			tstrLineData = m_tstrFFITxt1.substr( uCharIndex, uCharCountCalc );
			//
			::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
			//
			uCharIndex += uCharCountCalc;
			++m_uRowIndex;
		} else {
			assert( false );
			break;
		}
	}
	m_uSingleRowCharWidth = _calcAllCharWidth( tstrLineData, m_containerSingleRowChar );
	::SetTextColor( hDC, clrTxtOld );
	::SetBkMode( hDC, iBkModeOld );
	::SelectObject( hDC, hFontOld );
}

void YBUpLoadWarmTipDlg::_drawDFITxt1( HDC hDC ) {
	RECT rcCalc;
	RECT rcFITxt;
	unsigned int uWidth;
	unsigned int uCharIndex = 0;
	unsigned int uCharCountCalc;
	//unsigned int uLineIndex;
	tstring_type tstrLineData;
	const YBCtrlFont* pYBCtrlFont = NULL;
	HFONT hFontOld;

	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdDynamicFileInfo );
	if( !pYBCtrlFont )
		return ;
	if ( !_getFADFileInfoTxtRect( rcFITxt ) )
		return;

	hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
	uWidth = rcFITxt.right - rcFITxt.left;
	uCharIndex = 0;
	//uLineIndex = 0;
	int iBkModeOld;
	COLORREF clrTxtOld;

	iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
	clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
	unsigned int uChar2Width = _calcAllCharWidth( m_tstrDFITxt1, m_containerDynamicCharWidth1 );
	if ( uWidth - m_uSingleRowCharWidth > uChar2Width ) {
		while ( uCharIndex < m_tstrDFITxt1.length() ) {
			if ( _calcCharCount( m_containerDynamicCharWidth1, uCharIndex, uWidth - m_uSingleRowCharWidth, uCharCountCalc ) ) {
				//
				::SetRect( &rcCalc, rcFITxt.left + m_uSingleRowCharWidth, rcFITxt.top + ( m_uRowIndex - 1 )*m_uLineHeight, rcFITxt.right, rcFITxt.top + m_uRowIndex*m_uLineHeight );
				//
				tstrLineData = m_tstrDFITxt1.substr( uCharIndex, uCharCountCalc );
				//
				::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
				//
				uCharIndex += uCharCountCalc;
			} else {
				assert( false );
				break;
			}
		}
		m_uSingleRowCharWidth += uChar2Width;
	} else {
		if ( _calcCharCount( m_containerDynamicCharWidth1, uCharIndex, uWidth - m_uSingleRowCharWidth, uCharCountCalc ) ) {
				//
				::SetRect( &rcCalc, rcFITxt.left + m_uSingleRowCharWidth, rcFITxt.top + ( m_uRowIndex - 1 )*m_uLineHeight, rcFITxt.right, rcFITxt.top + m_uRowIndex*m_uLineHeight );
				//
				tstrLineData = m_tstrDFITxt1.substr( uCharIndex, uCharCountCalc );
				//
				::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
				//
				uCharIndex += uCharCountCalc;
				++m_uRowIndex;
			}
		while ( uCharIndex < m_tstrDFITxt1.length() ) {
			if ( _calcCharCount( m_containerDynamicCharWidth1, uCharIndex, uWidth, uCharCountCalc ) ) {
				//
				::SetRect( &rcCalc, rcFITxt.left, rcFITxt.top + ( m_uRowIndex - 1 )*m_uLineHeight, rcFITxt.right, rcFITxt.top + m_uRowIndex*m_uLineHeight );
				//
				tstrLineData = m_tstrDFITxt1.substr( uCharIndex, uCharCountCalc );
				//
				::DrawText( hDC, tstrLineData.c_str(), tstrLineData.length(), &rcCalc, DT_SINGLELINE|DT_LEFT );
				//
				uCharIndex += uCharCountCalc;
				if ( uCharIndex < m_tstrDFITxt1.length()  ) {
					++m_uRowIndex;
				}
			}
		}
		m_uSingleRowCharWidth = _calcAllCharWidth( tstrLineData, m_containerSingleRowChar );
	}
	::SetTextColor( hDC, clrTxtOld );
	::SetBkMode( hDC, iBkModeOld );
	::SelectObject( hDC, hFontOld );
}

bool YBUpLoadWarmTipDlg::_calcCharCount( charwidth_container_type containerCharWidth, unsigned int uCharIndexBegin, unsigned int uWidth, unsigned int& uCharCountCalc ) {
	if ( uCharIndexBegin >= containerCharWidth.size() ) {
		assert ( false );
		return false;
	}
	unsigned int uWidthSum = 0;
	uWidthSum = 0;
	uCharCountCalc = 0;
	for ( unsigned int uIndex = uCharIndexBegin; uIndex < containerCharWidth.size(); ++uIndex ) {
		uWidthSum += containerCharWidth[ uIndex ];
		if ( uWidthSum > uWidth ) {
			break;
		}
		uCharCountCalc++;
	}
	return true;
}

void YBUpLoadWarmTipDlg::_clearAll() {
	m_containerSingleRowChar.clear();
	m_containerDynamicCharWidth1.clear();
	m_containerDynamicCharWidth2.clear();
	m_containerDynamicCharWidth3.clear();
	m_containerFixedCharWidth1.clear();
	m_containerFixedCharWidth2.clear();
	m_containerFixedCharWidth3.clear();
	m_containerFixedCharWidth4.clear();
	m_containerULSuccessNumber.clear();
	m_containerULFailedNumber.clear();
}

unsigned int YBUpLoadWarmTipDlg::_calcAllCharWidth( const tstring_type& tstrTxt, charwidth_container_type& containerCharWidth ) {
	containerCharWidth.clear();
	YBCtrlMemDC memDC( 100, 100 );
	unsigned int uLenStrData;
	unsigned int uCharWidth = 0;
	TCHAR tchData;
	TCHAR tszData[ 2 ];
	RECT rcTextCalc;
	RECT rcFITxt;
	const YBCtrlFont* pYBCtrlFont = NULL;
	HFONT hFontOld;

	if ( !m_hWnd )
		return uCharWidth;
	if ( !_getFADFileInfoTxtRect( rcFITxt ) )
		return uCharWidth;
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdFixedFileInfo );
	if( !pYBCtrlFont )
		return uCharWidth;
	hFontOld = (HFONT)::SelectObject( memDC.getMemDC(), pYBCtrlFont->m_hFont );
	uLenStrData = tstrTxt.length();
	for ( unsigned int uIndex = 0; uIndex<uLenStrData; ++uIndex ) {
		tchData = tstrTxt[ uIndex ];
		tszData[ 0 ] = tchData;
		tszData[ 1 ] = 0;

		::SetRect( &rcTextCalc, rcFITxt.left, rcFITxt.top, rcFITxt.right, rcFITxt.bottom );
		::DrawText( memDC.getMemDC(), tszData, 1, &rcTextCalc, DT_SINGLELINE|DT_LEFT|DT_CALCRECT );
		containerCharWidth.push_back( rcTextCalc.right - rcTextCalc.left );
		uCharWidth += ( rcTextCalc.right - rcTextCalc.left );
	}
	::SelectObject( memDC.getMemDC(), hFontOld );
	
	return uCharWidth;
}

void YBUpLoadWarmTipDlg::setULFileInfo( const tstring_type& tstrULFileNumber, const tstring_type& tstrULFileSize, const tstring_type& tstrULFileTime ){
	m_tstrDFITxt1 = tstrULFileNumber;
	m_tstrDFITxt2 = tstrULFileSize;
	m_tstrDFITxt3 = tstrULFileTime;
}

void YBUpLoadWarmTipDlg::setULFileTime( const tstring_type& tstrHour, const tstring_type& tstrMinute, const tstring_type& tstrSecond ){
	m_tstrTimeHourTxt = tstrHour;
	m_tstrTimeMinuteTxt = tstrMinute;
	m_tstrTimeSecondTxt = tstrSecond;
}

void YBUpLoadWarmTipDlg::setULFileSFNumber( const tstring_type& tstrSuccessNumber, const tstring_type& tstrFailNumber ){
	m_tstrSuccessNumber = tstrSuccessNumber;
	m_tstrFailedNumber = tstrFailNumber;
}

void YBUpLoadWarmTipDlg::setULFileResult( bool bSuccess ) {
	if ( bSuccess ) {
		m_uFlag |= YBULWTD_ULFINISH_FLAG_SUCCESS;
	} else {
		m_uFlag &= ~YBULWTD_ULFINISH_FLAG_SUCCESS;
	}
	//
	_movewindow();
}