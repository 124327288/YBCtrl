#include "stdafx.h"
#include "ImageEditorDlg.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlMisc.h"
#include "ImageEditorEnvCfg.h"
#include "frame/YBCtrlFontDepository.h"
//
USING_NAMESPACE_YBCTRL

WNDPROC m_WndProcOld;
ImageEditor* g_pEditorDlg = NULL;
//
ImageEditor::ImageEditor()
: m_uImageIdEdit( 0 )
, m_uFlag( BTN_STATUS_NONE )
, m_hBrushBk( NULL )
, m_bDrawLBtn( false )
, m_bMouseWheel( false )
, m_bRRotating( false )
, m_bBBRectangle( true )
, m_nSrcX( 0 )
, m_nSrcY( 0 )
, m_nDstX( 0 )
, m_nDstY( 0 )
, m_nSrcWidth( 0 )
, m_nSrcHeight( 0 )
, m_nDstWidth( 0 )
, m_nDstHeight( 0 )
, m_nSrcWidthMax( 0 )
, m_nSrcHeightMax( 0 )
, m_ubtnDis( 0 )
, m_ubtnHov( 0 )
, m_ubtnNor( 0 )
, m_ubtnPre( 0 )
, m_uFontId( 0 )
, m_bShowEditDlg( false )
, m_bMoveCaret( false )
, m_bShowEditDlgOld( false )
, m_bSave( false )
, m_bLoadImage( false )
{
	m_hPenDotted = ::CreatePen( PS_DOT, 0, RGB(95,110,107) );
	::memset( &m_ptMouse, 0, sizeof(POINT) );
	::memset( &m_ptLBtnDown, 0, sizeof( POINT ) );
	m_hBrushBk = ::CreateSolidBrush( RGB(255, 255, 255) );
}

ImageEditor::~ImageEditor() {
	if(m_hBrushBk) {
		::DeleteObject( m_hBrushBk );
		m_hBrushBk = NULL;
	}
	if(m_hBitmap) {
		 ::DeleteObject( m_hBitmap );
		 m_hBitmap = NULL;
	}
	if(m_hPenDotted) {
		::DeleteObject( m_hPenDotted );
		m_hPenDotted = NULL;
	}
}

void ImageEditor::_init() {
	tstring_type tstrDirPath;
	tstring_type tstrFilePath;

	tstrDirPath = m_tstrDirPath;
	tstrFilePath += CONFIG_FILENAME;

	m_rcBigBmpMax.left = ::GetPrivateProfileInt( _T( "EditorBmpRect" ), _T( "left" ), 250, tstrFilePath.c_str() );
	m_rcBigBmpMax.top = ::GetPrivateProfileInt( _T( "EditorBmpRect" ), _T( "top" ), 150, tstrFilePath.c_str() );
	m_rcBigBmpMax.right = ::GetPrivateProfileInt( _T( "EditorBmpRect" ), _T( "right" ), 100, tstrFilePath.c_str() );
	m_rcBigBmpMax.bottom = ::GetPrivateProfileInt( _T( "EditorBmpRect" ), _T( "bottom" ), 100, tstrFilePath.c_str() );
	//
	m_nMaxMultiple = ::GetPrivateProfileInt( _T( "MaxMultiple" ), _T( "Multiple" ), 10, tstrFilePath.c_str() );
	//
	m_btnDelete.setImage( YBButton::EBTN_STATUS_NORMAL, m_ubtnNor );
	m_btnDelete.setImage( YBButton::EBTN_STATUS_HOVER, m_ubtnHov );
	m_btnDelete.setImage( YBButton::EBTN_STATUS_PRESS, m_ubtnPre );
	m_btnDelete.setImage( YBButton::EBTN_STATUS_DISABLE, m_ubtnDis );
	m_btnDelete.setFontId( m_uFontId );
	m_btnDelete.setText( _T( "删除" ) );
	m_btnDelete.setFixedSpace( 2, 2, 2, 2 );
	m_btnDelete.setTxtVcenter( true );
	//
	m_btnNext.setImage( YBButton::EBTN_STATUS_NORMAL, m_ubtnNor );
	m_btnNext.setImage( YBButton::EBTN_STATUS_HOVER, m_ubtnHov );
	m_btnNext.setImage( YBButton::EBTN_STATUS_PRESS, m_ubtnPre );
	m_btnNext.setImage( YBButton::EBTN_STATUS_DISABLE, m_ubtnDis );
	m_btnNext.setFontId( m_uFontId );
	m_btnNext.setText( _T( "下一页" ) );
	m_btnNext.setTxtVcenter( true );
	m_btnNext.setFixedSpace( 2, 2, 2, 2 );
	m_btnNext.enable( false );
	//
	m_btnPre.setImage( YBButton::EBTN_STATUS_NORMAL, m_ubtnNor );
	m_btnPre.setImage( YBButton::EBTN_STATUS_HOVER, m_ubtnHov );
	m_btnPre.setImage( YBButton::EBTN_STATUS_PRESS, m_ubtnPre );
	m_btnPre.setImage( YBButton::EBTN_STATUS_DISABLE, m_ubtnDis );
	m_btnPre.setFontId( m_uFontId );
	m_btnPre.setText( _T( "上一页" ) );
	m_btnPre.setTxtVcenter( true );
	m_btnPre.setFixedSpace( 2, 2, 2, 2 );
	m_btnPre.enable( false );
	//
	m_btnAdd.setImage( YBButton::EBTN_STATUS_NORMAL, m_ubtnNor );
	m_btnAdd.setImage( YBButton::EBTN_STATUS_HOVER, m_ubtnHov );
	m_btnAdd.setImage( YBButton::EBTN_STATUS_PRESS, m_ubtnPre );
	m_btnAdd.setImage( YBButton::EBTN_STATUS_DISABLE, m_ubtnDis );
	m_btnAdd.setFontId( m_uFontId );
	m_btnAdd.setText( _T( "新增" ) );
	m_btnAdd.setFixedSpace( 2, 2, 2, 2 );
	m_btnAdd.setTxtVcenter( true );
	//
	m_btnSave.setImage( YBButton::EBTN_STATUS_NORMAL, m_ubtnNor );
	m_btnSave.setImage( YBButton::EBTN_STATUS_HOVER, m_ubtnHov );
	m_btnSave.setImage( YBButton::EBTN_STATUS_PRESS, m_ubtnPre );
	m_btnSave.setImage( YBButton::EBTN_STATUS_DISABLE, m_ubtnDis );
	m_btnSave.setFontId( m_uFontId );
	m_btnSave.setText( _T( "保存" ) );
	m_btnSave.setFixedSpace( 2, 2, 2, 2 );
	m_btnSave.setTxtVcenter( true );
}

YBCTRL_WNDMSGMAP_BEGIN( ImageEditor, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
	//YBCTRL_ONWNDMSG( WM_MOUSEWHEEL, _onWM_MOUSEWHEEL )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_LBUTTONUP, _onWM_LBUTTONUP )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	//YBCTRL_ONWNDMSG( WM_SETCURSOR, _onWM_SETCURSOR )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_CTLCOLOREDIT, _onWM_COLOREDIT )
YBCTRL_WNDMSGMAP_END()

void ImageEditor::setPath( const tstring_type& tstrDirPath ) {
	m_tstrDirPath = tstrDirPath;
	_loadImage();
	_loadFont();
	_init();
	//
 	m_dlgControlBar.setPath( tstrDirPath );
 	m_dlgHeader.setPath( tstrDirPath );
 	m_dlgProperty.setPath( tstrDirPath );
}

HWND ImageEditor::create( int x, int y, int nWidth, int nHeight, HWND hWndParent, HINSTANCE hInstance ) {
	HWND hWnd;
	DWORD dwStyle =  WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	hWnd = _createHWND( NULL, dwStyle, x, y, nWidth, nHeight, hWndParent, (HMENU)321, hInstance, NULL );
	if(hWnd) {
		::SetRect( &m_rcBigBmpMax, m_rcBigBmpMax.left, m_rcBigBmpMax.top, nWidth - m_rcBigBmpMax.right, nHeight - m_rcBigBmpMax.bottom );
		m_dlgHeader.create( hWnd, (HMENU)HEADER_DIALOG_ID, hInstance );
		int nHeight2 = m_rcBigBmpMax.top - SPACE_EDITOR_CONTROLBAR_HEIGHT - 2;
		m_dlgControlBar.create( m_rcBigBmpMax.left, nHeight2, m_rcBigBmpMax.right - m_rcBigBmpMax.left, SPACE_EDITOR_CONTROLBAR_HEIGHT, hWnd, (HMENU)CONTROLBAR_DIALOG_ID, hInstance );
		m_dlgProperty.create( 15, nHeight2, m_rcBigBmpMax.left - 15 -10, m_rcBigBmpMax.bottom - nHeight2, hWnd, (HMENU)PROPERTY_DIALOG_ID, hInstance );
		//m_btnDelete.Create( 0, WS_VISIBLE|WS_CHILD, 15 + 8, m_rcBigBmpMax.bottom + 8, 30, 20, hWnd, (HMENU)BTN_DELETE_ID, hInstance  );
		//m_btnPre.Create( 0, WS_VISIBLE | WS_CHILD, 15 + 8 + 35, m_rcBigBmpMax.bottom + 8, 30, 20, hWnd, (HMENU)BTN_PRE_ID, hInstance );
		//m_btnNext.Create( 0, WS_VISIBLE | WS_CHILD, 15 + 8 + 35 + 35, m_rcBigBmpMax.bottom + 8, 30, 20, hWnd, (HMENU)BTN_NEXT_ID, hInstance );
		//m_btnSave.Create( 0, WS_VISIBLE | WS_CHILD, 15 + 8 + 35 + 35 + 35, m_rcBigBmpMax.bottom + 8, 30, 20, hWnd, (HMENU)BTN_SAVE_ID, hInstance );
		//m_btnAdd.Create( 0, WS_VISIBLE | WS_CHILD, 15 + 8 + 35 + 35 + 35 + 35, m_rcBigBmpMax.bottom + 8, 30, 20, hWnd, (HMENU)BTN_ADD_ID, hInstance );
		//m_btnNext.enable( false );
		//m_btnPre.enable( false );
		m_hWndEdit = ::CreateWindowEx( NULL, _T( "Edit" ), _T( "" ), WS_CHILD | ES_MULTILINE | ES_WANTRETURN, 0, 0, EDIT_DEFAULT_WIDTH, EDIT_DEFAULT_HEIGHT, hWnd, (HMENU)EDIT_BOX_ID, hInstance, NULL );
	}
	m_WndProcOld = (WNDPROC)::SetWindowLong( m_hWndEdit, GWL_WNDPROC, (LONG)WndProcNew );
	g_pEditorDlg = this;
	//
	return hWnd;
}

LRESULT ImageEditor::WndProcNew( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	switch(uMsg) {
	case WM_ERASEBKGND:{
						   RECT rcEdit;
						   ::GetClientRect( g_pEditorDlg->m_hWndEdit, &rcEdit );
						   int nWidth = rcEdit.right - rcEdit.left;
						   int nHeight = rcEdit.bottom - rcEdit.top;

						   HDC hDCDst = (HDC)wParam;
						   HDC hDCSrc = ::GetDC( g_pEditorDlg->m_hWnd );
						   YBCtrlMemDC memDC( nWidth, nHeight );
						   int xPos = min( g_pEditorDlg->m_rcDottedInWnd.left, g_pEditorDlg->m_rcDottedInWnd.right );
						   int yPos = min( g_pEditorDlg->m_rcDottedInWnd.top, g_pEditorDlg->m_rcDottedInWnd.bottom );
						   ::BitBlt( memDC.getMemDC(), 0, 0, nWidth, nHeight, hDCSrc, xPos + 1, yPos + 1, SRCCOPY );
						   ::SetBkMode( hDCDst, TRANSPARENT );
						   //
						   ::BitBlt( hDCDst, 0, 0, nWidth, nHeight, memDC.getMemDC(), 0, 0, SRCCOPY );
						   ::ReleaseDC( g_pEditorDlg->m_hWnd, hDCSrc );
						   return (LRESULT)HBRUSH( ::GetStockObject( HOLLOW_BRUSH ) );
	}
		break;
	default:
		break;
	}
	return ::CallWindowProc( m_WndProcOld, hwnd, uMsg, wParam, lParam );
}

void ImageEditor::_onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG | YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = 1;
	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;

	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
}

void ImageEditor::_onWM_COLOREDIT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_CTLCOLOREDIT );
	HWND hEditCtrl = (HWND)pYBCtrlWndMsg->m_lParam;
	HDC hEditDC = (HDC)pYBCtrlWndMsg->m_wParam;
	RECT rcEdit;

	::GetClientRect( m_hWndEdit, &rcEdit );
	if(hEditCtrl == m_hWndEdit) {
		::SetTextColor( hEditDC, m_dlgProperty.getTextClr() );
		::SetBkMode( hEditDC, TRANSPARENT );
		//HDC hDC = ::GetDC( m_hWnd );
		int xPos = min( m_rcDottedInWnd.left, m_rcDottedInWnd.right );
		int yPos = min( m_rcDottedInWnd.top, m_rcDottedInWnd.bottom );
		RECT rcClient;
		::GetClientRect( m_hWnd, &rcClient );
		YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
		_drawStatus2( memDC.getMemDC() );

		::BitBlt( hEditDC, 0, 0, rcEdit.right - rcEdit.left, rcEdit.bottom - rcEdit.top, memDC.getMemDC(), xPos + 1, yPos + 1, SRCCOPY );
		//::ReleaseDC( m_hWnd, hDC );
	}

	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG | YBCTRLWNDMSG_FLAG_RESULT_VALID);
}

void ImageEditor::_onWM_SETCURSOR( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_SETCURSOR );
	if( m_bDrawLBtn && ::PtInRect( &m_rcBigZoomBmp, m_ptLBtnDown ) ) {
		::SetCursor( ::LoadCursor( NULL, IDC_HAND ) );

		pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG | YBCTRLWNDMSG_FLAG_RESULT_VALID);
		pYBCtrlWndMsg->m_lRes = TRUE;
	} else {
		::SetCursor( ::LoadCursor( NULL, IDC_ARROW ) );
	}
}

void ImageEditor::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT )
	PAINTSTRUCT ps;
	HDC hPaintDC;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );
	//
	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void ImageEditor::_onWM_MOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	TRACKMOUSEEVENT trackMouseEvt;
	RECT rcClient;
	RECT rcReal;
	POINT ptMouse;

	ptMouse.x = (__int16)LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = (__int16)HIWORD( pYBCtrlWndMsg->m_lParam );
	//
	trackMouseEvt.cbSize = sizeof(TRACKMOUSEEVENT);
	trackMouseEvt.dwFlags = TME_LEAVE;
	trackMouseEvt.hwndTrack = pYBCtrlWndMsg->m_hWnd;
	trackMouseEvt.dwHoverTime = HOVER_DEFAULT;
	::TrackMouseEvent( &trackMouseEvt );
	::SetRect( &rcReal, m_nDstX, m_nDstY, m_nDstX + m_nDstWidth, m_nDstY + m_nDstHeight );
	//
	if((m_uFlag & BTN_STATUS_DOWN) == BTN_STATUS_DOWN) {
		m_bDrawLBtn = true;

		m_nSrcX += m_ptLBtnDown.x - ptMouse.x;
		m_nSrcY += m_ptLBtnDown.y - ptMouse.y;
		//
		m_bShowEditDlg = true;
		m_bMoveCaret = true;
		//
		ptMouse.x = max( min( rcReal.right, ptMouse.x ), rcReal.left );
		ptMouse.y = max( min( rcReal.bottom, ptMouse.y ), rcReal.top );
		m_rcDottedInWnd.right = m_rcDottedInWnd.left + ptMouse.x - m_ptMouse.x;
		m_rcDottedInWnd.bottom = m_rcDottedInWnd.top + ptMouse.y - m_ptMouse.y;
		m_ptLBtnDown = ptMouse;
		_moveEdit( false );
	}
	if(!::PtInRect( &rcReal, ptMouse )) {
		//::memset(&m_ptLBtnDown, 0, sizeof(POINT));
		//m_uFlag &= ~BTN_STATUS_DOWN;
	}
	//
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	HDC hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), (rcClient.bottom - rcClient.top), memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
	//
	//::SetFocus( m_hWnd );
}

void ImageEditor::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );

	switch(wCtrlId) {
		case CONTROLBAR_DIALOG_ID:{
			switch(wEvtCode) {
				case EDITOR_CONTROLBAR_CMD_PROGRESS: {
						unsigned int uPos = (unsigned int)pYBCtrlWndMsg->m_lParam;

						YBCtrlImageStatic* pYBImage = NULL;
						unsigned int uWidthImage = 0;
						unsigned int uHeightImage = 0;
						//_reloadImage();
						pYBImage = _getImageStatic( m_uImageIdEdit );
						if(!pYBImage)
							return;

						uWidthImage = m_bBBRectangle ? pYBImage->getWidth() : pYBImage->getHeight();
						uHeightImage = m_bBBRectangle ? pYBImage->getHeight() : pYBImage->getWidth();
						unsigned int uWidthMax = m_nMaxMultiple * uWidthImage;
						unsigned int uHeightMax = m_nMaxMultiple * uHeightImage;
						m_bMouseWheel = true;
						m_bDrawLBtn = false;
						//
						double dFraction = (float)uPos / (float)100;
						unsigned int uWidthNew = unsigned int( uWidthMax*dFraction );
						unsigned int uHeightNew = unsigned int( uHeightMax*dFraction );
						if(uWidthNew >= uWidthMax) {
							uWidthNew = uWidthMax;
						} else if(uWidthNew <= uWidthImage) {
							uWidthNew = uWidthImage;
						}
						if(uHeightNew >= uHeightMax) {
							uHeightNew = uHeightMax;
						} else if(uHeightNew <= uHeightImage) {
							uHeightNew = uHeightImage;
						}
						uWidthMax = m_rcBigBmpMax.right - m_rcBigBmpMax.left;
						uHeightMax = m_rcBigBmpMax.bottom - m_rcBigBmpMax.top;
						::SetRect( &m_rcBigZoomBmp, m_rcBigBmpMax.left + (uWidthMax - uWidthNew) / 2, m_rcBigBmpMax.top + (uHeightMax - uHeightNew) / 2,
							m_rcBigBmpMax.left + (uWidthMax - uWidthNew) / 2 + uWidthNew, m_rcBigBmpMax.top + (uHeightMax - uHeightNew) / 2 + uHeightNew );
						//
						_moveEdit( false );

						m_uFlag |= EDIT_INPUT_OVER;
				}
					break;
				case EDITOR_CONTROLBAR_CMD_ROTAT:{
						m_bRRotating = *(bool *)(pYBCtrlWndMsg->m_lParam);
						m_bDrawLBtn = false;
						m_bMouseWheel = false;
						//
						_moveEdit( false );
				}
				default:
					break;
			}
		}
		break;
		case HEADER_DIALOG_ID:{
								  switch(wEvtCode) {
								  case BTN_HEADER_CMD_OPEN:{//打开文件对话框
															   //CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,_T("BMP Files(*.bmp)|*.bmp|All Files(*.*)|*.*||") );
															   //CFileDialog dlg( TRUE, NULL, NULL, OFN_HIDEREADONLY, _T( "JPG(*.jpg)|*.jpg|PNG(*.png)|*.png|BMP(*.bmp)|*.bmp|All Files(*.*)|*.*||" ) );
															   tstring_type tstrFilePath;
															   //if(dlg.DoModal() == IDOK) {
																  // tstrFilePath = dlg.GetPathName();
															   tstrFilePath = _T( "C:\\Users\\Administrator\\Desktop\\FVS(EPD{9TQMZI0CNPP[O31.jpg" );
																   m_tstrFilePath = tstrFilePath;
																   if(!YBImageLoader::getInstance()->loadImage( tstrFilePath, m_uImageIdEdit )) {
																	   assert( false );
																   }
																   //
																   //
																   TCHAR* posTmp1 = NULL;
																   posTmp1 = (TCHAR*)_tcsrchr( tstrFilePath.c_str(), _T( '.' ) );
																   tstring_type tstrMimeType( posTmp1 );
																   tstring_type tstrDirPath = m_tstrDirPath;
																   tstrDirPath += _T( "BMP\\" );
																   tstrDirPath += _T( "newPic" );
																   tstrDirPath += tstrMimeType;
																   m_tstrNewFilePath = tstrDirPath;
																   tstrDirPath = m_tstrDirPath;
																   tstrDirPath += _T( "BMP\\" );
																   tstrDirPath += _T( "newPic2" );
																   tstrDirPath += tstrMimeType;
																   m_tstrNewFilePath2 = tstrDirPath;
																   //
																   m_tstrTmpFilePath = m_tstrNewFilePath;
																   //
																   tstrMimeType = tstrMimeType.substr( 1, tstrMimeType.length() - 1 );
																   tstrMimeType = _T( "image/" ) + tstrMimeType;
																   m_tstrMimeType = tstrMimeType;
																   //
																   RECT rcClient;
																   ::GetClientRect( m_hWnd, &rcClient );
																   YBCtrlMemDC memDC1( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
																   _drawStatus( memDC1.getMemDC() );

																   HDC hDC = ::GetDC( m_hWnd );
																   ::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), (rcClient.bottom - rcClient.top), memDC1.getMemDC(), 0, 0, SRCCOPY );
																   ::ReleaseDC( m_hWnd, hDC );
															 //  }
								  }
									  break;
								  default:
									  break;
								  }
		}
		break;
		case BTN_DELETE_ID:{
							   WPARAM wParam = MAKELONG( BTN_DELETE_ID, BTN_CMD_DELETE );
							   LPARAM lParam = (LPARAM)m_hWnd;
							   ::SendMessage( m_dlgProperty.getHWND(), WM_COMMAND, wParam, lParam );
		}
		break;
		case BTN_NEXT_ID:{
							 WPARAM wParam = MAKELONG( BTN_NEXT_ID, BTN_CMD_NEXT );
							 LPARAM lParam = (LPARAM)m_hWnd;
							 ::SendMessage( m_dlgProperty.getHWND(), WM_COMMAND, wParam, lParam );
		}
		break;
		case BTN_ADD_ID:{
							WPARAM wParam = MAKELONG( BTN_ADD_ID, BTN_CMD_ADD );
							LPARAM lParam = (LPARAM)m_hWnd;
							::SendMessage( m_dlgProperty.getHWND(), WM_COMMAND, wParam, lParam );
		}
		break;
		case BTN_PRE_ID:{
							WPARAM wParam = MAKELONG( BTN_PRE_ID, BTN_CMD_PRE );
							LPARAM lParam = (LPARAM)m_hWnd;
							::SendMessage( m_dlgProperty.getHWND(), WM_COMMAND, wParam, lParam );
		}
		break;
		case BTN_SAVE_ID:{
							 WPARAM wParam = MAKELONG( BTN_SAVE_ID, BTN_CMD_SAVE );
							 LPARAM lParam = (LPARAM)m_hWnd;
							 ::SendMessage( m_dlgProperty.getHWND(), WM_COMMAND, wParam, lParam );
		}
		break;
		case PROPERTY_DIALOG_ID:{
									switch(wEvtCode) {
									case BTN_CMD_SAVE:{
														  RECT rcText;
														  YBCtrlImageStatic* pYBImage = NULL;
														  _reloadImage();
														  pYBImage = _getImageStatic( m_uImageIdEdit );
														  YBCtrlMemDC memDC( pYBImage->getWidth(), pYBImage->getHeight() );
														  //循环
														  if(m_dlgProperty.m_containerTxtProperty.size() == 0) {
															  assert( false );
															  break;
														  }
														  ImageEditorProperty::containerTxtProperty::iterator itBegin, itEnd;
														  itEnd = m_dlgProperty.m_containerTxtProperty.end();
														  for(itBegin = m_dlgProperty.m_containerTxtProperty.begin(); itBegin != itEnd; ++itBegin) {
															   EditorTxtProperty* pEdiorTxtProperty = NULL;
															   pEdiorTxtProperty = *itBegin;
															   if(pEdiorTxtProperty) {
																   LOGFONT lFont;
																   memset( &lFont, 0, sizeof(LOGFONT) );
																   _tcscpy_s( lFont.lfFaceName, pEdiorTxtProperty->m_tstrFaceName.c_str() );
																   lFont.lfHeight = pEdiorTxtProperty->m_uSizeFont;
																   HFONT hFont = ::CreateFontIndirect( &lFont );
																   ::SelectObject( memDC.getMemDC(), m_hBitmap );
																   ::SelectObject( memDC.getMemDC(), hFont );
																   if(pEdiorTxtProperty->m_bTransparent) {
																	   ::SetBkMode( memDC.getMemDC(), TRANSPARENT );
																   }
																   //
																   ::SetRect( &rcText, pEdiorTxtProperty->m_uXPos, pEdiorTxtProperty->m_uYPos, pEdiorTxtProperty->m_uXPos + 100, pEdiorTxtProperty->m_uYPos + 30 );
																   ::DrawText( memDC.getMemDC(), pEdiorTxtProperty->m_tstrText.c_str(), -1, &rcText, DT_VCENTER | DT_LEFT );
																   if(!_save( m_hBitmap, m_tstrMimeType, m_tstrFilePath )) {
																	   assert( false );
																   }
																   ::DeleteObject( hFont );
															   }
														  }
									}
										break;
									default:
											break;
									}
		}
		break;
	default:
		break;
	}
	//
	RECT rcClient;
	::GetClientRect( m_hWnd, &rcClient );
	HDC hDC = ::GetDC( m_hWnd );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
}

bool ImageEditor::_save( HBITMAP hBitmap, tstring_type tstrMimeType, tstring_type tstrFileName ) {
	if(!hBitmap) {
		assert( false );
		return false;
	}
	CLSID imageClsid;
	Gdiplus::Bitmap bmp( hBitmap, NULL );
	if(!_getEncoderClsid( tstrMimeType, &imageClsid )){
		assert( false );
		return false;
	}
	bmp.Save( tstrFileName.c_str(), &imageClsid );

	return true;
}

bool ImageEditor::_getEncoderClsid( tstring_type tstrMimeType, CLSID* pClsid ) {
	unsigned int uEncoderNum;
	unsigned int uEncodecSize;
	Gdiplus::ImageCodecInfo* pCodecInfo;
	Gdiplus::GetImageEncodersSize( &uEncoderNum, &uEncodecSize );
	if(uEncodecSize < 0) {
		assert( false );
		return false;
	}
	pCodecInfo = new Gdiplus::ImageCodecInfo[uEncodecSize];
	if(!pCodecInfo){
		assert( false );
		return false;
	}
	Gdiplus::GetImageEncoders( uEncoderNum, uEncodecSize, pCodecInfo );
	for(unsigned int uIndex = 0; uIndex < uEncoderNum; ++uIndex) {
		if(_tcscmp( pCodecInfo[uIndex].MimeType, tstrMimeType.c_str() )) {
			*pClsid = pCodecInfo[uIndex].Clsid;
			delete[] pCodecInfo;
			return true;
		}
	}
	delete[] pCodecInfo;
	return false;
}

void ImageEditor::_onWM_MOUSEWHEEL( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEWHEEL );
	__int16 i16WheelDelta;
	YBCtrlImageStatic* pYBImage = NULL;
	unsigned int uWidthImage = 0;
	unsigned int uHeightImage = 0;
	//_reloadImage();
	pYBImage = _getImageStatic( m_uImageIdEdit );
	if(!pYBImage)
		return;

	int uWidthOld = m_rcBigZoomBmp.right - m_rcBigZoomBmp.left;
	int uHeightOld = m_rcBigZoomBmp.bottom - m_rcBigZoomBmp.top;
	uWidthImage = m_bBBRectangle ? pYBImage->getWidth() : pYBImage->getHeight();
	uHeightImage = m_bBBRectangle ? pYBImage->getHeight() : pYBImage->getWidth();
	int uWidthMax = m_nMaxMultiple * uWidthImage;
	int uHeightMax = m_nMaxMultiple * uHeightImage;

	i16WheelDelta = HIWORD( pYBCtrlWndMsg->m_wParam );
	//
	if(uWidthOld == 0 || uHeightOld == 0) {
		uWidthOld = uWidthImage;
		uHeightOld = uHeightImage;
	}
	//
	m_bMouseWheel = true;
	m_bDrawLBtn = false;
	//
	switch(i16WheelDelta){
	case 120:
	{
				int uWidthNew = int( uWidthOld*1.1 + 0.5 );
				int uHeightNew = int( uHeightOld*1.1 + 0.5 );
				uWidthNew = min( uWidthMax, uWidthNew );
				uHeightNew = min( uHeightNew, uHeightMax );
				uWidthMax = m_rcBigBmpMax.right - m_rcBigBmpMax.left;
				uHeightMax = m_rcBigBmpMax.bottom - m_rcBigBmpMax.top;
				::SetRect( &m_rcBigZoomBmp, m_rcBigBmpMax.left + (uWidthMax - uWidthNew) / 2, m_rcBigBmpMax.top + (uHeightMax - uHeightNew) / 2,
					m_rcBigBmpMax.left + (uWidthMax - uWidthNew) / 2 + uWidthNew, m_rcBigBmpMax.top + (uHeightMax - uHeightNew) / 2 + uHeightNew );
	}
		break;
	case -120:
	{
				 int uWidthNew = int( uWidthOld*0.9 + 0.5 );
				 int uHeightNew = int( uHeightOld*0.9 + 0.5 );
				 uWidthNew = max( uWidthNew, (int)(m_bBBRectangle ? pYBImage->getWidth() : pYBImage->getHeight()) );
				 uHeightNew = max( uHeightNew, (int)(m_bBBRectangle ? pYBImage->getHeight() : pYBImage->getWidth()) );
				 uWidthMax = m_rcBigBmpMax.right - m_rcBigBmpMax.left;
				 uHeightMax = m_rcBigBmpMax.bottom - m_rcBigBmpMax.top;
				 ::SetRect( &m_rcBigZoomBmp, m_rcBigBmpMax.left + (uWidthMax - uWidthNew) / 2, m_rcBigBmpMax.top + (uHeightMax - uHeightNew) / 2,
					 m_rcBigBmpMax.left + (uWidthMax - uWidthNew) / 2 + uWidthNew, m_rcBigBmpMax.top + (uHeightMax - uHeightNew) / 2 + uHeightNew );
				 if((m_rcBigZoomBmp.right - m_rcBigZoomBmp.left == uWidthImage) && (m_rcBigZoomBmp.bottom - m_rcBigZoomBmp.top == uHeightImage)){
					 return;
				 }
	}
		break;
	default:
		break;
	}
	//
	RECT rcClient;
	::GetClientRect( m_hWnd, &rcClient );
	HDC hDC = ::GetDC( m_hWnd );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
}

void ImageEditor::_onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
	HDC hDC;
	RECT rcClient;
	RECT rcReal;

	m_ptMouse.x = LOWORD( pYBCtrlWndMsg->m_lParam );
	m_ptMouse.y = HIWORD( pYBCtrlWndMsg->m_lParam );
	m_bShowEditDlgOld = m_bShowEditDlg;//记录之前的状态，在WM_LBUTTONUP时判断down之前是否有editdlg.
	//
	::SetRect( &rcReal, m_nDstX, m_nDstY, m_nDstX + m_nDstWidth, m_nDstY + m_nDstHeight );
	if(::PtInRect( &rcReal, m_ptMouse )) {
		m_uFlag |= BTN_STATUS_DOWN;
		m_ptLBtnDown = m_ptMouse;
		//
		m_bShowEditDlg = false;
		m_rcDottedInWnd.left = m_ptMouse.x;
		m_rcDottedInWnd.top = m_ptMouse.y;

	} else {
		m_bShowEditDlg = false;
		m_uFlag &= ~BTN_STATUS_DOWN;
	}

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
}

void ImageEditor::_onWM_LBUTTONUP( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONUP );
	POINT ptMouse;
	RECT rcClient;
	RECT rcReal;
	YBCtrlImageStatic* pYBImage = NULL;
	unsigned int uWidthImage = 0;
	unsigned int uHeightImage = 0;
	ptMouse.x = LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = HIWORD( pYBCtrlWndMsg->m_lParam );
	
	//_reloadImage();
	if( m_uImageIdEdit == 0)
		return;
	pYBImage = _getImageStatic( m_uImageIdEdit );
	if(!pYBImage) {
		assert( false );
	}
	uWidthImage = m_bBBRectangle ? pYBImage->getWidth() : pYBImage->getHeight();
	uHeightImage = m_bBBRectangle ? pYBImage->getHeight() : pYBImage->getWidth();
	if(m_rcBigZoomBmp.right - m_rcBigZoomBmp.left == 0) {
		m_nSrcWidthMax = uWidthImage;
	} else {
		m_nSrcWidthMax = m_rcBigZoomBmp.right - m_rcBigZoomBmp.left;
	}
	if(m_rcBigZoomBmp.bottom - m_rcBigZoomBmp.top == 0) {
		m_nSrcWidthMax = uHeightImage;
	} else {
		m_nSrcHeightMax = m_rcBigZoomBmp.bottom - m_rcBigZoomBmp.top;
	}
	//
	m_uFlag &= ~BTN_STATUS_DOWN;
	_moveEdit( false );
	//
	::SetRect( &rcReal, m_nDstX, m_nDstY, m_nDstX + m_nDstWidth, m_nDstY + m_nDstHeight );
	if((ptMouse.x == m_ptMouse.x) && (ptMouse.y == m_ptMouse.y)) {
		if(m_nSrcWidthMax ==uWidthImage &&
			m_nSrcHeightMax == uHeightImage ){
			if(::PtInRect( &rcReal, m_ptMouse )) {
					m_bDrawLBtn = false;
					m_bMouseWheel = false;
			}
		}
		//
		if(::PtInRect( &rcReal, m_ptMouse )) {
			if(m_bShowEditDlgOld) {
				if(!::PtInRect( &m_rcDottedInWnd, m_ptMouse )) {
					m_bShowEditDlg = false;
				}
			} else {
				if(m_bShowEditDlgOld){
					m_bShowEditDlg = false;
				} else {
					m_bShowEditDlg = true;
				}
				m_rcDottedInWnd.left = m_ptMouse.x;
				m_rcDottedInWnd.top = m_ptMouse.y;
				if(rcReal.right - m_ptMouse.x < EDIT_DEFAULT_WIDTH) {
					m_rcDottedInWnd.right = rcReal.right;
				} else {
					m_rcDottedInWnd.right = m_rcDottedInWnd.left + EDIT_DEFAULT_WIDTH;
				}
				if(rcReal.bottom - m_ptMouse.y < EDIT_DEFAULT_HEIGHT) {
					m_rcDottedInWnd.bottom = rcReal.right;
				} else {
					m_rcDottedInWnd.bottom = m_rcDottedInWnd.top + EDIT_DEFAULT_HEIGHT;
				}
			}
			if(m_bMoveCaret) {
				m_bShowEditDlg = false;
				m_bMoveCaret = false;
			}
		}
	} else {
		if(m_bMoveCaret) {
			if(abs(m_rcDottedInWnd.right - m_rcDottedInWnd.left) < EDIT_DEFAULT_WIDTH) {
				if(m_rcDottedInWnd.right > m_rcDottedInWnd.left) {
					m_rcDottedInWnd.right = EDIT_DEFAULT_WIDTH + m_rcDottedInWnd.left;
					if(m_rcDottedInWnd.right > rcReal.right) {
						m_rcDottedInWnd.right = rcReal.right;
					}
				} else {
					m_rcDottedInWnd.left = EDIT_DEFAULT_WIDTH + m_rcDottedInWnd.right;
					if(m_rcDottedInWnd.left < rcReal.left) {
						m_rcDottedInWnd.left = rcReal.left;
					}
				}
			}
			//
			if(abs( m_rcDottedInWnd.bottom - m_rcDottedInWnd.top ) < EDIT_DEFAULT_HEIGHT) {
				if(m_rcDottedInWnd.bottom > m_rcDottedInWnd.top ) {
					m_rcDottedInWnd.bottom = EDIT_DEFAULT_HEIGHT + m_rcDottedInWnd.top;
					if(m_rcDottedInWnd.bottom > rcReal.bottom) {
						m_rcDottedInWnd.bottom = rcReal.bottom;
					}
				} else {
					m_rcDottedInWnd.top = EDIT_DEFAULT_HEIGHT + m_rcDottedInWnd.bottom;
					if(m_rcDottedInWnd.top < rcReal.top) {
						m_rcDottedInWnd.top = rcReal.top;
					}
				}
			}
			::SetFocus( m_hWndEdit );
		}
	}
	
	_moveEdit( true );
	//
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	HDC hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
	//
}

void ImageEditor::_moveEdit( bool bVisible ) {
	if( !m_bShowEditDlg )
		return;
	
	if(bVisible) {
		::MoveWindow( m_hWndEdit, min( m_rcDottedInWnd.left, m_rcDottedInWnd.right ) + 1, min( m_rcDottedInWnd.top, m_rcDottedInWnd.bottom ) + 1, abs( m_rcDottedInWnd.right - m_rcDottedInWnd.left ) - 1, abs( m_rcDottedInWnd.bottom - m_rcDottedInWnd.top ) - 1, TRUE );
		_initEdit();
		::ShowWindow( m_hWndEdit, SW_NORMAL );
	} else {
		::ShowWindow( m_hWndEdit, SW_HIDE );
		_pushback();
	}
}

void ImageEditor::_initEdit() {
	::SetWindowText( m_hWndEdit, _T( "" ) );
	//
	LOGFONT lFont;
	HFONT hFont;

	::memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, m_dlgProperty.getFontFaceName().c_str() );
	lFont.lfHeight = m_dlgProperty.getFontSize();
	hFont = ::CreateFontIndirect( &lFont );
	::SendMessage( m_hWndEdit, WM_SETFONT, (WPARAM)(hFont), NULL );
	HDC hDC = ::GetDC( m_hWndEdit );
	::SetTextColor( hDC, m_dlgProperty.getTextClr() );
	::ReleaseDC( m_hWndEdit, hDC );
	//
	//::DeleteObject( hFont );
}

void ImageEditor::_pushback() {
	if((m_uFlag & BTN_STATUS_DOWN) == BTN_STATUS_DOWN) {
		if((m_uFlag & EDIT_INPUT_OVER) != EDIT_INPUT_OVER) {
			return;
		}
	}
	m_uFlag &= ~EDIT_INPUT_OVER;
	//
	TCHAR szMsg[MAX_PATH] = {0};
	FontInfo* pFontInfo = NULL;
	tstring_type tstrText;
	RECT rcDottedInDesktop;
	RECT rcDottedInWnd;
	POINT ptMouse;

	::GetWindowText( m_hWndEdit, szMsg, MAX_PATH );
	tstrText = szMsg;
	if(tstrText.empty()) {
		return;
	}
	pFontInfo = new FontInfo();
	pFontInfo->m_tstrText = tstrText;
	pFontInfo->m_tstrFaceName = m_dlgProperty.getFontFaceName();
	pFontInfo->m_uSizeFont = m_dlgProperty.getFontSize();
	pFontInfo->m_clrFont = m_dlgProperty.getTextClr();
	//
	::GetWindowRect( m_hWndEdit, &rcDottedInDesktop );
	ptMouse.x = rcDottedInDesktop.left;
	ptMouse.y = rcDottedInDesktop.top;
	int nWidth = rcDottedInDesktop.right - rcDottedInDesktop.left;
	int nHeight = rcDottedInDesktop.bottom - rcDottedInDesktop.top;
	::ScreenToClient( m_hWnd, &ptMouse );
	::SetRect( &rcDottedInWnd, ptMouse.x, ptMouse.y, ptMouse.x + nWidth, ptMouse.y + nHeight );
	//::SetRect( &rcDottedInWnd, 100, 100, 200, 300 );
	RECT rcInReal;//在放大过的画布里面的位置.
	YBCtrlImageStatic* pYBImage = NULL;
	pYBImage = _getImageStatic( m_uImageIdEdit );
	if(!pYBImage){
		assert( false );
	}
	int nWidthReal = (m_bRRotating ? pYBImage->getWidth() : pYBImage->getHeight() );
	int nHeightReal = ( m_bRRotating ? pYBImage->getHeight() : pYBImage->getWidth() );
	double dFraction = ( (double)nWidthReal/(double)m_nSrcWidthMax );
	rcInReal.left = int( ( m_nSrcX+ ptMouse.x - m_nDstX )*dFraction);
	rcInReal.top = int( ( m_nSrcY + ptMouse.y - m_nDstY)*dFraction);
	rcInReal.bottom = int( ( rcInReal.top + nHeight )*dFraction );
	rcInReal.right = int( ( rcInReal.left + nWidth )* dFraction );

	pFontInfo->m_rcVInC = rcInReal;
	//
	m_containerFontInfo.push_back( pFontInfo );
	m_bLoadImage = true;

	//
	_drawText();
	//
	_initEdit();
}

void ImageEditor::_drawText() {
	if(m_containerFontInfo.size() == 0)
		return;
	Gdiplus::Bitmap* pBitMap = Gdiplus::Bitmap::FromFile( m_tstrFilePath.c_str() );
	if(!pBitMap) {
		assert( false );
	}
	pBitMap->GetHBITMAP( NULL, &m_hBitmap );
	if(!m_hBitmap) {
		assert( false );
	}
	//
	YBCtrlImageStatic* pYBImage = NULL;
	unsigned int uImageId = 0;
	if(!YBImageLoader::getInstance()->loadImage( m_tstrFilePath, uImageId )) {
		assert( false );
		return;
	}
	pYBImage = _getImageStatic( uImageId );
	YBCtrlMemDC memDC( pYBImage->getWidth(), pYBImage->getHeight() );
	//循环
	containerFontInfo::iterator itBegin, itEnd;
	itEnd = m_containerFontInfo.end();

	::SelectObject( memDC.getMemDC(), m_hBitmap );
	LOGFONT lFont;
	HFONT hFont = NULL;
	FontInfo* pFontInfo = NULL;
	//
	for(itBegin = m_containerFontInfo.begin(); itBegin != itEnd; ++itBegin) {
		pFontInfo = *itBegin;
		if(pFontInfo) {
			memset( &lFont, 0, sizeof(LOGFONT) );
			_tcscpy_s( lFont.lfFaceName, pFontInfo->m_tstrFaceName.c_str() );
			lFont.lfHeight = pFontInfo->m_uSizeFont;
			HFONT hFont = ::CreateFontIndirect( &lFont );
			::SelectObject( memDC.getMemDC(), hFont );
			::SetBkMode( memDC.getMemDC(), TRANSPARENT );
			//
			::DrawText( memDC.getMemDC(), pFontInfo->m_tstrText.c_str(), -1, &pFontInfo->m_rcVInC, DT_LEFT|DT_TOP );
			//::DeleteObject( m_hBitmap );
			if(hFont) {
				::DeleteObject( hFont );
				hFont = NULL;
			}
		}
	}
	if(_tcscmp( m_tstrNewFilePath.c_str(), m_tstrTmpFilePath.c_str() ) == 0) {
		m_tstrTmpFilePath = m_tstrNewFilePath2;
	} else if(_tcscmp( m_tstrNewFilePath2.c_str(), m_tstrTmpFilePath.c_str() ) == 0) {
		m_tstrTmpFilePath = m_tstrNewFilePath;
	} else {
		assert( false );
	}
	if(!_save( m_hBitmap, m_tstrMimeType, m_tstrTmpFilePath )) {
		assert( false );
	}
	m_bSave = true;
	//
	//_reloadImage();
	if(!YBCTRLIMAGEDEPOSITORY_INSTANCE->removeImage( m_uImageIdEdit )) {
		assert( false );
	}
	if(m_bLoadImage) {
		if(!YBImageLoader::getInstance()->loadImage( m_tstrTmpFilePath, m_uImageIdEdit )) {
			assert( false );
		}
	}
	m_bLoadImage = false;
	// 	//
	RECT rcClient;
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC1( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC1.getMemDC() );

	HDC hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, (rcClient.right - rcClient.left), (rcClient.bottom - rcClient.top), memDC1.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );

	if(pBitMap) {
		delete pBitMap;
		pBitMap = NULL;
	}
}

ybctrl::YBCtrlImageStatic* ImageEditor::_getImageStatic( unsigned int uImageIdBk ) {
	YBCtrlImageBase* pYBImageBase = NULL;

	if(!YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageIdBk, &pYBImageBase )) {
		assert( false );
		return NULL;
	}
	if(!pYBImageBase || pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC) {
		assert( false );
		return NULL;
	}
	return (YBCtrlImageStatic*)pYBImageBase;
}

void ImageEditor::_drawStatus2( HDC hDC ) {
	_drawBk( hDC );
	if(m_bMouseWheel) {
		_drawWheelBmp( hDC );
	} else {
		_drawDefaultBmp( hDC );
	}
}
void ImageEditor::_drawStatus( HDC hDC ) {
// 	_drawBk( hDC );
// 	if(m_bDrawLBtn) {
// 		_drawDragBmp( hDC );
// 	} else {
// 		if(m_bMouseWheel) {
// 			_drawWheelBmp( hDC );
// 		} else {
// 			_drawDefaultBmp( hDC );
// 		}
// 	}
// 	_drawDotted( hDC );
		_drawBk( hDC );
		if(m_bMouseWheel) {
			_drawWheelBmp( hDC );
		} else {
			_drawDefaultBmp( hDC );
		}
		_drawDotted( hDC );
		//
		//_drawText( hDC );
}

void ImageEditor::_drawDotted( HDC hDC ) {
	if(!m_bShowEditDlg) {
		::ShowWindow( m_hWndEdit, SW_HIDE );
		return;
	}
	HPEN hPenOld = NULL;
	hPenOld = (HPEN)::SelectObject( hDC, m_hPenDotted );
	::MoveToEx( hDC, m_rcDottedInWnd.left, m_rcDottedInWnd.top, NULL );
	::LineTo( hDC, m_rcDottedInWnd.right, m_rcDottedInWnd.top );
	::LineTo( hDC, m_rcDottedInWnd.right, m_rcDottedInWnd.bottom );
	::LineTo( hDC, m_rcDottedInWnd.left, m_rcDottedInWnd.bottom );
	::LineTo( hDC, m_rcDottedInWnd.left, m_rcDottedInWnd.top );
	::SelectObject( hDC, hPenOld );
}

void ImageEditor::_drawBk( HDC hDC ) {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	::FillRect( hDC, &rcClient, m_hBrushBk );
}

void ImageEditor::_drawDefaultBmp( HDC hDC ) {
	YBCtrlImageStatic* pYBImage = NULL;
	::memset( &m_rcBigZoomBmp, 0, sizeof(RECT) );
	//
	if( m_uImageIdEdit == 0)
		return;
	//_reloadImage();
	pYBImage = _getImageStatic( m_uImageIdEdit );
	if(!pYBImage)
		return;

	if(m_bRRotating) {
		pYBImage->getImage()->RotateFlip( Gdiplus::Rotate90FlipNone );
		m_bRRotating = false;
		if(m_bBBRectangle) {
			m_bBBRectangle = false;
		} else {
			m_bBBRectangle = true;
		}
	}

	unsigned int uMemWidth = m_bBBRectangle ? pYBImage->getWidth() : pYBImage->getHeight();
	unsigned int uMemHeight = m_bBBRectangle ? pYBImage->getHeight() : pYBImage->getWidth();
	int nSrcWidthMax = uMemWidth;
	int nSrcHeightMax = uMemHeight;
	int nDstWidthMax = m_rcBigBmpMax.right - m_rcBigBmpMax.left;
	int nDstHeightMax = m_rcBigBmpMax.bottom - m_rcBigBmpMax.top;
	//
	m_nSrcWidthMax = uMemWidth;
	m_nSrcHeightMax = uMemHeight;
	//
	if(nSrcWidthMax <= nDstWidthMax && nSrcHeightMax <= nDstHeightMax) {
		m_nSrcX = 0;
		m_nSrcY = 0;
		m_nSrcWidth = nSrcWidthMax;
		m_nSrcHeight = nSrcHeightMax;
		m_nDstX = m_rcBigBmpMax.left + (nDstWidthMax - m_nSrcWidth) / 2;
		m_nDstY = m_rcBigBmpMax.top + (nDstHeightMax - m_nSrcHeight) / 2;
		m_nDstWidth = m_nSrcWidth;
		m_nDstHeight = m_nSrcHeight;
	} else if(nSrcWidthMax > nDstWidthMax && nSrcHeightMax <= nDstHeightMax) {
		m_nSrcX = (nSrcWidthMax - nDstWidthMax) / 2;
		m_nSrcY = 0;
		m_nSrcWidth = nDstWidthMax;
		m_nSrcHeight = nSrcHeightMax;
		m_nDstX = m_rcBigBmpMax.left;
		m_nDstY = (nDstHeightMax - nSrcHeightMax) / 2 + m_rcBigBmpMax.top;
		m_nDstWidth = nDstWidthMax;
		m_nDstHeight = m_nSrcHeight;
	} else if(nSrcWidthMax <= nDstWidthMax && nSrcHeightMax > nDstHeightMax) {
		m_nSrcX = 0;
		m_nSrcY = (nSrcHeightMax - nDstHeightMax) / 2;
		m_nSrcWidth = nSrcWidthMax;
		m_nSrcHeight = nDstHeightMax;
		m_nDstX = m_rcBigBmpMax.left + (nDstWidthMax - nSrcWidthMax) / 2;
		m_nDstY = m_rcBigBmpMax.top;
		m_nDstWidth = m_nSrcWidth;
		m_nDstHeight = nDstHeightMax;
	} else if(nSrcWidthMax > nDstWidthMax && nSrcHeightMax > nDstHeightMax) {
		m_nSrcX = (nSrcWidthMax - nDstWidthMax) / 2;
		m_nSrcY = (nSrcHeightMax - nDstHeightMax) / 2;
		m_nSrcWidth = nDstWidthMax;
		m_nSrcHeight = nDstHeightMax;
		m_nDstX = m_rcBigBmpMax.left;
		m_nDstY = m_rcBigBmpMax.top;
		m_nDstWidth = nDstWidthMax;
		m_nDstHeight = nDstHeightMax;
	}

	YBCtrlMemDC memDC( uMemWidth, uMemHeight );
	Gdiplus::Graphics graphics( memDC.getMemDC() );
	GdipBitBlt( &graphics, pYBImage->getImage(), 0, 0, uMemWidth, uMemHeight, 0, 0, uMemWidth, uMemHeight );
	YBCtrlMemDC memDC2( nSrcWidthMax, nSrcHeightMax );
	::StretchBlt( memDC2.getMemDC(), 0, 0, nSrcWidthMax, nSrcHeightMax, memDC.getMemDC(), 0, 0, uMemWidth, uMemHeight, SRCCOPY );
	//
	//::BitBlt( hDC, m_nDstX, m_nDstY, m_nDstWidth, m_nDstHeight, memDC2.getMemDC(), m_nSrcX, m_nSrcY, SRCCOPY );
	BackGroundBitBlt( hDC, m_nDstX, m_nDstY, m_nDstWidth, m_nDstHeight, memDC2.getMemDC(),
		m_nSrcX, m_nSrcY, m_nSrcWidth, m_nSrcHeight, 0, 0, 0, 0 );
// 	Gdiplus::Graphics graphics2( hDC );
// 	GdipBitBlt( &graphics2, pYBImage->getImage(), m_nDstX, m_nDstY, m_nDstWidth, m_nDstHeight, m_nSrcX, m_nSrcY, m_nSrcWidth, m_nSrcHeight );
}

void ImageEditor::_drawDragBmp( HDC hDC ) {
	if( m_bRRotating )
		return;
	YBCtrlImageStatic* pYBImage = NULL;
	//_reloadImage();
	pYBImage = _getImageStatic( m_uImageIdEdit );
	if(!pYBImage)
		return;

	int nSrcWidthMax;
	int nSrcHeightMax;
	unsigned int uMemWidth = m_bBBRectangle ? pYBImage->getWidth() : pYBImage->getHeight();
	unsigned int uMemHeight = m_bBBRectangle ? pYBImage->getHeight() : pYBImage->getWidth();
	int nDstWidthMax = m_rcBigBmpMax.right - m_rcBigBmpMax.left;
	int nDstHeightMax = m_rcBigBmpMax.bottom - m_rcBigBmpMax.top;
	if(m_bMouseWheel) {
		nSrcWidthMax = m_rcBigZoomBmp.right - m_rcBigZoomBmp.left;
		nSrcHeightMax = m_rcBigZoomBmp.bottom - m_rcBigZoomBmp.top;
	} else {
		nSrcWidthMax = uMemWidth;
		nSrcHeightMax = uMemHeight;
	}
	m_nSrcWidthMax = nSrcWidthMax;
	m_nSrcHeightMax = nSrcHeightMax;
	YBCtrlMemDC memDC( uMemWidth, uMemHeight );
	Gdiplus::Graphics graphics( memDC.getMemDC() );
	GdipBitBlt( &graphics, pYBImage->getImage(), 0, 0, uMemWidth, uMemHeight, 0, 0, uMemWidth, uMemHeight );
	YBCtrlMemDC memDC2( nSrcWidthMax, nSrcHeightMax );
	::StretchBlt( memDC2.getMemDC(), 0, 0, nSrcWidthMax, nSrcHeightMax, memDC.getMemDC(), 0, 0, uMemWidth, uMemHeight, SRCCOPY );
	//
	if((nSrcWidthMax - m_nSrcX) < m_nDstWidth) {
		m_nSrcX = nSrcWidthMax - m_nDstWidth;
	}
	if((nSrcHeightMax - m_nSrcY) < m_nDstHeight) {
		m_nSrcY = nSrcHeightMax - m_nDstHeight;
	}
	m_nSrcX = max( m_nSrcX, 0 );
	m_nSrcY = max( m_nSrcY, 0 );
	BackGroundBitBlt( hDC, m_nDstX, m_nDstY, m_nDstWidth, m_nDstHeight, memDC2.getMemDC(),
		m_nSrcX, m_nSrcY, m_nSrcWidth, m_nSrcHeight, 0, 0, 0, 0 );
}

void ImageEditor::_drawWheelBmp( HDC hDC ) {
	if( m_bRRotating )
		return;
	YBCtrlImageStatic* pYBImage = NULL;

	//_reloadImage();
	pYBImage = _getImageStatic( m_uImageIdEdit );
	if(!pYBImage)
		return;

	int nSrcWidthMax = m_rcBigZoomBmp.right - m_rcBigZoomBmp.left;
	int nSrcHeightMax = m_rcBigZoomBmp.bottom - m_rcBigZoomBmp.top;
	int nDstWidthMax = m_rcBigBmpMax.right - m_rcBigBmpMax.left;
	int nDstHeightMax = m_rcBigBmpMax.bottom - m_rcBigBmpMax.top;
	//
	if(nSrcWidthMax <= nDstWidthMax && nSrcHeightMax <= nDstHeightMax) {
		m_nSrcX = 0;
		m_nSrcY = 0;
		m_nSrcWidth = nSrcWidthMax;
		m_nSrcHeight = nSrcHeightMax;
		m_nDstX = m_rcBigBmpMax.left + (nDstWidthMax - m_nSrcWidth) / 2;
		m_nDstY = m_rcBigBmpMax.top + (nDstHeightMax - m_nSrcHeight) / 2;
		m_nDstWidth = m_nSrcWidth;
		m_nDstHeight = m_nSrcHeight;
	} else if(nSrcWidthMax > nDstWidthMax && nSrcHeightMax <= nDstHeightMax) {
		m_nSrcX = (nSrcWidthMax - nDstWidthMax) / 2;
		m_nSrcY = 0;
		m_nSrcWidth = nDstWidthMax;
		m_nSrcHeight = nSrcHeightMax;
		m_nDstX = m_rcBigBmpMax.left;
		m_nDstY = (nDstHeightMax - nSrcHeightMax) / 2 + m_rcBigBmpMax.top;
		m_nDstWidth = nDstWidthMax;
		m_nDstHeight = m_nSrcHeight;
	} else if(nSrcWidthMax <= nDstWidthMax && nSrcHeightMax > nDstHeightMax) {
		m_nSrcX = 0;
		m_nSrcY = (nSrcHeightMax - nDstHeightMax) / 2;
		m_nSrcWidth = nSrcWidthMax;
		m_nSrcHeight = nDstHeightMax;
		m_nDstX = m_rcBigBmpMax.left + (nDstWidthMax - nSrcWidthMax) / 2;
		m_nDstY = m_rcBigBmpMax.top;
		m_nDstWidth = m_nSrcWidth;
		m_nDstHeight = nDstHeightMax;
	} else if(nSrcWidthMax > nDstWidthMax && nSrcHeightMax > nDstHeightMax) {
		m_nSrcX = (nSrcWidthMax - nDstWidthMax) / 2;
		m_nSrcY = (nSrcHeightMax - nDstHeightMax) / 2;
		m_nSrcWidth = nDstWidthMax;
		m_nSrcHeight = nDstHeightMax;
		m_nDstX = m_rcBigBmpMax.left;
		m_nDstY = m_rcBigBmpMax.top;
		m_nDstWidth = nDstWidthMax;
		m_nDstHeight = nDstHeightMax;
	}

	unsigned int uMemWidth = m_bBBRectangle ? pYBImage->getWidth() : pYBImage->getHeight();
	unsigned int uMemHeight = m_bBBRectangle ? pYBImage->getHeight() : pYBImage->getWidth();
	m_nSrcWidthMax = nSrcWidthMax;
	m_nSrcHeightMax = nSrcHeightMax;

// 	double dFraction = ((double)uMemWidth / (double)m_nSrcWidthMax );
// 	int x = (int)( dFraction * m_nSrcX );
// 	int y = (int)( dFraction * m_nSrcY);
// 
// 	Gdiplus::Graphics graphics2( hDC );
// 	GdipBitBlt( &graphics2, pYBImage->getImage(), m_nDstX, m_nDstY, m_nDstWidth, m_nDstHeight, x, y, uMemWidth, uMemHeight );

	YBCtrlMemDC memDC( uMemWidth, uMemHeight );
	Gdiplus::Graphics graphics( memDC.getMemDC() );
	GdipBitBlt( &graphics, pYBImage->getImage(), 0, 0, uMemWidth, uMemHeight, 0, 0, uMemWidth, uMemHeight );
	YBCtrlMemDC memDC2( nSrcWidthMax, nSrcHeightMax );
	::StretchBlt( memDC2.getMemDC(), 0, 0, nSrcWidthMax, nSrcHeightMax, memDC.getMemDC(), 0, 0, uMemWidth, uMemHeight, SRCCOPY );
	//
	BackGroundBitBlt( hDC, m_nDstX, m_nDstY, m_nDstWidth, m_nDstHeight, memDC2.getMemDC(),
		m_nSrcX, m_nSrcY, m_nSrcWidth, m_nSrcHeight, 0, 0, 0, 0 );
}

void ImageEditor::_loadFont() {
	LOGFONT lFont;

	::memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T( "微软雅黑" ) );
	lFont.lfHeight = 18;
	m_uFontId = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(255,0,0) );
}

void ImageEditor::_reloadImage() {
	tstring_type tstrFilePath;
	//
	if(m_containerFontInfo.size() == 0) {
		tstrFilePath = m_tstrFilePath;
	} else {
		if(m_bSave) {
			tstrFilePath = m_tstrNewFilePath;
		} else {
			tstrFilePath = m_tstrFilePath;
		}
	}
	YBImageLoader::getInstance()->loadImage( tstrFilePath, m_uImageIdEdit );
}

void ImageEditor::_loadImage() {
	tstring_type tstrDirPath;
	tstring_type tstrFilePath;
	tstrDirPath = m_tstrDirPath;
	tstrDirPath += _T( "BMP\\" );
	//
	LOAD_IMAGE( _T( "YBCNType16.png" ), m_uImageIdEdit )
	m_uImageIdEdit = 0;
	//
	Gdiplus::Bitmap* pBitMap = Gdiplus::Bitmap::FromFile( tstrFilePath.c_str() );
	if(!pBitMap) {
		assert( false );
	}
	pBitMap->GetHBITMAP( NULL, &m_hBitmap );
	if(!pBitMap) {
		assert( false );
	}
	TCHAR* posTmp1 = NULL;
	posTmp1 = (TCHAR*)_tcsrchr( tstrFilePath.c_str(), _T( '.' ) );
	tstring_type tstrMimeType( posTmp1 );
	tstrMimeType = tstrMimeType.substr( 1, tstrMimeType.length() - 1 );
	m_tstrFilePath = tstrDirPath + _T( "YBCNType16_1.png" );
	//
	tstrMimeType = _T( "image/" ) + tstrMimeType;
	m_tstrMimeType = tstrMimeType;
	//
	LOAD_IMAGE( _T( "Normal.png" ), m_ubtnNor )
	LOAD_IMAGE( _T( "Hover.png" ), m_ubtnHov )
	LOAD_IMAGE( _T( "Press.png" ), m_ubtnPre )
	LOAD_IMAGE( _T( "Disable.png" ), m_ubtnDis )
}