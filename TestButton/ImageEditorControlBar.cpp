#include "stdafx.h"
#include "ImageEditorControlBar.h"
#include "frame/YBCtrlMemDC.h"
#include "ImageEditorEnvCfg.h"

USING_NAMESPACE_YBCTRL

ImageEditorControlBar::ImageEditorControlBar()
: m_uProCtrlImageIdBK( 0 )
, m_wordCtrlID( 0 )
, m_uProCtrlImageId( 0 )
, m_uImageIdPCNormal( 0 )
, m_uImageIdPCHover( 0 )
, m_uImageIdPCPress( 0 )
, m_uImageIdBtnBigNor( 0 )
, m_uImageIdBtnBigHov( 0 )
, m_uImageIdBtnBigPre( 0 )
, m_uImageIdBtnSmallNor( 0 )
, m_uImageIdBtnSmallHov( 0 )
, m_uImageIdBtnSmallPre( 0 )
, m_uImageIdBtnRotatNor( 0 )
, m_uImageIdBtnRotatHov( 0 )
, m_uImageIdBtnRotatPre( 0 )
, m_uImageIdBtnCompareNor( 0 )
, m_uImageIdBtnCompareHov( 0 )
, m_uImageIdBtnComparePre( 0 )
, m_uImageIdBtnPreviewNor( 0 )
, m_uImageIdBtnPreviewHov( 0 )
, m_uImageIdBtnPreviewPre( 0 )
, m_uImageIdBtnUndoNor( 0 )
, m_uImageIdBtnUndoHov( 0 )
, m_uImageIdBtnUndoPre( 0 )
, m_uImageIdBtnRedoNor( 0 )
, m_uImageIdBtnRedoHov( 0 )
, m_uImageIdBtnRedoPre( 0 )
, m_uImageIdBtnRestoreNor( 0 )
, m_uImageIdBtnRestoreHov( 0 )
, m_uImageIdBtnRestorePre( 0 )
{
	m_hBrushBk = ::CreateSolidBrush( RGB(239, 239, 239) );
}

ImageEditorControlBar::~ImageEditorControlBar() {
	if(m_hBrushBk) {
		::DeleteObject( m_hBrushBk );
		m_hBrushBk = NULL;
	}
}

void ImageEditorControlBar::setPath( const tstring_type& tstrDirPath ) {
	m_tstrDirPath = tstrDirPath;
	_loadImage();
}

HWND ImageEditorControlBar::create( int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance ) {
	HWND hWnd;

	hWnd = _createHWND( NULL, WS_CHILD|WS_VISIBLE, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, NULL );
	if(hWnd) {
		m_dlgProCtrl.Create( NULL, WS_VISIBLE|WS_CHILD, 30, 12, 90, 6, hWnd, (HMENU)PROCTRL_CONTROLBAR_ID, hInstance );
		m_btnSmall.Create( NULL, WS_VISIBLE|WS_CHILD, 8, 8, 15, 15, hWnd, (HMENU)BTN_CONTROLBAR_SMALL, hInstance );
		m_btnBig.Create( NULL, WS_VISIBLE|WS_CHILD, 133, 8, 15, 15, hWnd, (HMENU)BTN_CONTROLBAR_BIG, hInstance );
		m_btnRotat.Create( NULL, WS_VISIBLE|WS_CHILD, 245, 2, 55, 27, hWnd, (HMENU)BTN_CONTROLBAR_ROTAT, hInstance );
		m_btnCompare.Create( NULL, WS_VISIBLE|WS_CHILD, 310, 2, 55, 27, hWnd, (HMENU)BTN_CONTROLBAR_COMPARE, hInstance );
		m_btnPreview.Create( NULL, WS_VISIBLE|WS_CHILD, 375, 2, 55, 27, hWnd, (HMENU)BTN_CONTROLBAR_PREVIEW, hInstance );
		m_btnUndo.Create( NULL, WS_VISIBLE|WS_CHILD, 440, 2, 55, 24, hWnd, (HMENU)BTN_CONTROLBAR_UNDO, hInstance );
		m_btnRedo.Create( NULL, WS_VISIBLE|WS_CHILD, 505, 2, 55, 27, hWnd, (HMENU)BTN_CONTROLBAR_REDO, hInstance );
		m_btnRetore.Create( NULL, WS_VISIBLE|WS_CHILD, 570, 2, 55, 27, hWnd, (HMENU)BTN_CONTROLBAR_RESTORE, hInstance );
		_init();
	}
	m_wordCtrlID = (WORD)hMenu;
	return hWnd;
}

void ImageEditorControlBar::_init() {
	//Progress Ctrl
	m_dlgProCtrl.SetImageId( m_uProCtrlImageIdBK, m_uProCtrlImageId );
	m_dlgProCtrl.SetImageId( YBProgressCtrl::ETHUMB_STATUS_NORMAL, m_uImageIdPCNormal );
	m_dlgProCtrl.SetImageId( YBProgressCtrl::ETHUMB_STATUS_HOVER, m_uImageIdPCHover );
	m_dlgProCtrl.SetImageId( YBProgressCtrl::ETHUMB_STATUS_PRESS, m_uImageIdPCPress );
	m_dlgProCtrl.SetProgressCtrlSpaced( 0, 4, 0, 4 );
	m_dlgProCtrl.setRange( 0, 100 );
	m_dlgProCtrl.setPos( 0 );
	//big button
	m_btnBig.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnBigNor );
	m_btnBig.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnBigHov );
	m_btnBig.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnBigPre );
	m_btnSmall.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnSmallNor );
	m_btnSmall.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnSmallHov );
	m_btnSmall.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnSmallPre );
	m_btnRotat.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnRotatNor );
	m_btnRotat.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnRotatHov );
	m_btnRotat.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnRotatPre );
	m_btnCompare.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnCompareNor );
	m_btnCompare.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnCompareHov );
	m_btnCompare.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnComparePre );
	m_btnPreview.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnPreviewNor );
	m_btnPreview.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnPreviewHov );
	m_btnPreview.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnPreviewPre );
	m_btnUndo.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnUndoNor );
	m_btnUndo.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnUndoHov );
	m_btnUndo.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnUndoPre );
	m_btnRedo.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnRedoNor );
	m_btnRedo.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnRedoHov );
	m_btnRedo.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnRedoPre );
	m_btnRetore.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnRestoreNor );
	m_btnRetore.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnRestoreHov );
	m_btnRetore.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnRestorePre );
}

YBCTRL_WNDMSGMAP_BEGIN( ImageEditorControlBar, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
YBCTRL_WNDMSGMAP_END()

void ImageEditorControlBar::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	PAINTSTRUCT ps;
	HDC hPaintDC;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void ImageEditorControlBar::_onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG | YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = 1;

	RECT rcClient;
	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;

	::GetClientRect( pYBCtrlWndMsg->m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
}

void ImageEditorControlBar::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );
	WPARAM wParam;
	LPARAM lParam;
	switch(wCtrlId) {
		case PROCTRL_CONTROLBAR_ID:{
									   WPARAM wParam;
									   LPARAM lParam;

									   unsigned int uCurrPos = m_dlgProCtrl.GetPos();
									   unsigned int uMinPos = m_dlgProCtrl.GetMinPos();
									   unsigned int uMaxPos = m_dlgProCtrl.GetMaxPos();
									   unsigned int uPos = ( (uCurrPos - uMinPos)*100 / (uMaxPos - uMinPos));
									   wParam = MAKELONG( m_wordCtrlID, EDITOR_CONTROLBAR_CMD_PROGRESS );
									   lParam = (LPARAM)uPos;
									   ::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
		}
		break;
		case BTN_CONTROLBAR_SMALL:{//ËõÐ¡
									  unsigned int uCurrPos = m_dlgProCtrl.GetPos();
									  unsigned int uMaxPos = m_dlgProCtrl.GetMaxPos();
									  unsigned int uMinPos = m_dlgProCtrl.GetMinPos();
									  if(uCurrPos > ( uMinPos + 5)) {
										  m_dlgProCtrl.setPos( uCurrPos - 5 );
									  } else {
										  m_dlgProCtrl.setPos( uMinPos );
									  }
									  ::InvalidateRect( m_dlgProCtrl.getHWND(), NULL, FALSE );
									  //
									  uCurrPos = m_dlgProCtrl.GetPos();
									  unsigned int uPos = ((uCurrPos - uMinPos) * 100 / (uMaxPos - uMinPos));
									  wParam = MAKELONG( m_wordCtrlID, EDITOR_CONTROLBAR_CMD_PROGRESS );
									  lParam = (LPARAM)uPos;
									  ::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
			}
			break;
		case BTN_CONTROLBAR_BIG:{//·Å´ó
									unsigned int uCurrPos = m_dlgProCtrl.GetPos();
									unsigned int uMinPos = m_dlgProCtrl.GetMinPos();
									unsigned int uMaxPos = m_dlgProCtrl.GetMaxPos();
									if(uCurrPos < (uMaxPos - 5)) {
										m_dlgProCtrl.setPos( uCurrPos + 5 );
									} else {
										m_dlgProCtrl.setPos( uMaxPos );
									}
									::InvalidateRect( m_dlgProCtrl.getHWND(), NULL, FALSE );
									//
									uCurrPos = m_dlgProCtrl.GetPos();
									unsigned int uPos = ((uCurrPos - uMinPos) * 100 / (uMaxPos - uMinPos));
									wParam = MAKELONG( m_wordCtrlID, EDITOR_CONTROLBAR_CMD_PROGRESS );
									lParam = (LPARAM)uPos;
									::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
			}
			break;
		case BTN_CONTROLBAR_ROTAT:{
									  bool bRotat = true;
									  wParam = MAKELONG( m_wordCtrlID, EDITOR_CONTROLBAR_CMD_ROTAT );
									  lParam = (LPARAM)&bRotat;
									  ::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, wParam, lParam );
			}
			break;
		case BTN_CONTROLBAR_COMPARE:{

			}
			break;
		case BTN_CONTROLBAR_PREVIEW:{

			}
			break;
		case BTN_CONTROLBAR_UNDO:{

			}
			break;
		case BTN_CONTROLBAR_REDO:{

			}
			break;
		case BTN_CONTROLBAR_RESTORE:{

			}
			break;
		default:
			break;
	}

}

void ImageEditorControlBar::_drawStatus( HDC hDC ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	::FillRect( hDC, &rcClient, m_hBrushBk );
}

void ImageEditorControlBar::_loadImage() {
	tstring_type tstrDirPath;
	tstring_type tstrFilePath;

	tstrDirPath = m_tstrDirPath;
	tstrDirPath += _T( "BMP\\" );
	//
	LOAD_IMAGE( _T( "editorControlBarProBk.png" ), m_uProCtrlImageIdBK )
	LOAD_IMAGE( _T( "editorControlBarFTBk.png" ), m_uProCtrlImageId )
	LOAD_IMAGE( _T( "editorCBBtnHov.png" ), m_uImageIdPCHover )
	LOAD_IMAGE( _T( "editorCBBtnHov.png" ), m_uImageIdPCPress )
	LOAD_IMAGE( _T( "editorCBBtnNor.png" ), m_uImageIdPCNormal )
	//
	LOAD_IMAGE( _T( "editorCBBtnBigNor.png" ), m_uImageIdBtnBigNor )
	LOAD_IMAGE( _T( "editorCBBtnBigHov.png" ), m_uImageIdBtnBigHov )
	LOAD_IMAGE( _T( "editorCBBtnBigHov.png" ), m_uImageIdBtnBigPre )
	//
	LOAD_IMAGE( _T( "editorCBBtnSmallNor.png" ), m_uImageIdBtnSmallNor )
	LOAD_IMAGE( _T( "editorCBBtnSmallHov.png" ), m_uImageIdBtnSmallHov )
	LOAD_IMAGE( _T( "editorCBBtnSmallHov.png" ), m_uImageIdBtnSmallPre )
	//
	LOAD_IMAGE( _T( "editorCBBtnRotatNor.png" ), m_uImageIdBtnRotatNor )
	LOAD_IMAGE( _T( "editorCBBtnRotatHov.png" ), m_uImageIdBtnRotatHov	 )
	LOAD_IMAGE( _T( "editorCBBtnRotatHov.png" ), m_uImageIdBtnRotatPre )
	//
	LOAD_IMAGE( _T( "editorCBBtnCompareNor.png" ), m_uImageIdBtnCompareNor )
	LOAD_IMAGE( _T( "editorCBBtnCompareHov.png" ), m_uImageIdBtnCompareHov )
	LOAD_IMAGE( _T( "editorCBBtnCompareHov.png" ), m_uImageIdBtnComparePre )
	//
	LOAD_IMAGE( _T( "editorCBBtnPreviewNor.png" ), m_uImageIdBtnPreviewNor )
	LOAD_IMAGE( _T( "editorCBBtnPreviewHov.png" ), m_uImageIdBtnPreviewHov )
	LOAD_IMAGE( _T( "editorCBBtnPreviewHov.png" ), m_uImageIdBtnPreviewPre )
	//
	LOAD_IMAGE( _T( "editorCBBtnUndoNor.png" ), m_uImageIdBtnUndoNor )
	LOAD_IMAGE( _T( "editorCBBtnUndoHov.png" ), m_uImageIdBtnUndoHov )
	LOAD_IMAGE( _T( "editorCBBtnUndoHov.png" ), m_uImageIdBtnUndoPre )
	//
	LOAD_IMAGE( _T( "editorCBBtnRedoNor.png" ), m_uImageIdBtnRedoNor )
	LOAD_IMAGE( _T( "editorCBBtnRedoHov.png" ), m_uImageIdBtnRedoHov )
	LOAD_IMAGE( _T( "editorCBBtnRedoHov.png" ), m_uImageIdBtnRedoPre )
	//
	LOAD_IMAGE( _T( "editorCBBtnRestoreNor.png" ), m_uImageIdBtnRestoreNor )
	LOAD_IMAGE( _T( "editorCBBtnRestoreHov.png" ), m_uImageIdBtnRestoreHov )
	LOAD_IMAGE( _T( "editorCBBtnRestoreHov.png" ), m_uImageIdBtnRestorePre )
}