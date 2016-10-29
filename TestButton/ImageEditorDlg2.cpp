#include "stdafx.h"
#include "ImageEditorDlg2.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlMisc.h"
#include "ImageEditorEnvCfg.h"
#include "frame/YBCtrlFontDepository.h"

#define SIDE_LENGTH (3)
#define EDITOR_NONE (0x00000000)
#define IMAGE_EDITOR_TYPE (0x00000001) //判断是否是编辑图片还是文字.
#define ENABLE_EDITOR_IMAGE (0x00000002) //判断是否可以对画布进行编辑.
#define MOUSEMOVE_IN_BMP_BOTTOM (0x00000010) //判断鼠标是否按在图片最下面区域的中间位置,该位置可以向上向下拖动
#define MOUSEMOVE_IN_BMP_RIGHT (0x00000020)    //..................................................右.........................................................向左向右.......
#define MOUSEMOVE_IN_BMP_BRCORNER (0x00000040) //............................................右下角................................................左上左下.......
#define MOUSEMOVE_IN_TEXTBOX_LTOP (0x00000080)
#define MOUSEMOVE_IN_TEXTBOX_TOP (0x00000400)
#define MOUSEMOVE_IN_TEXTBOX_RTOP (0x00000800)
#define MOUSEMOVE_IN_TEXTBOX_LEFT (0x00001000)
#define MOUSEMOVE_IN_TEXTBOX_RIGHT (0x00002000)
#define MOUSEMOVE_IN_TEXTBOX_LBOTTOM (0x00004000)
#define MOUSEMOVE_IN_TEXTBOX_BOTTOM (0x00008000)
#define MOUSEMOVE_IN_TEXTBOX_RBOTTOM (0x00010000)
#define MOUSEMOVE_IN_TEXTBOX_BOTH (0x00020000)
//
//#define BTN_STATUS_DOWN (0x00000200) //判断鼠标是否按下.
#define BTN_STATUS_MOVING (0x00020000) //判断鼠标是否是拖动
//
#define TC_BTNX_WIDTH (40)
#define TC_BTNX_HEIGHT (20)
#define TC_BTNX_WSPACE (15) //表示左右间隔
#define TC_BTNX_HSPACE (10)//表示上下间隔
#define TC_LPOS (20)
#define TC_RPOS (20)
#define TC_TPOS (95)

WNDPROC m_EditWndProcOld;
ImageEditorDlg* g_pEditorDlg = NULL;

ImageEditorDlg::ImageEditorDlg()
: m_uFlag( EDITOR_NONE )
, m_hWndCaptureOld( NULL )
, m_uImageId( 0 )
, m_uBMPBSpace( 0 )
, m_bHide( false )
, m_bDrag( false )
, m_bIsShow( false )
, m_bReturn( false )
, m_bMoveAll( false )
, m_uBMPRSpace( 0 )
, m_tstrMimeType( _T( "" ) )
, m_tstrDirPath( _T( "" ) ) {
	::memset( &m_rcImage, 0, sizeof(RECT) );
	::memset( &m_rcTxtBox, 0, sizeof(RECT) );
	::SetRect( &m_rcCanvas, 220, 150, 860,560 );
	::memset( &m_ptMouse, 0, sizeof(POINT) );
}

ImageEditorDlg::~ImageEditorDlg() {

}


HWND ImageEditorDlg::create( int x, int y, int nWidth, int nHeight, HWND hWndParent, HINSTANCE hInstance ) {
	HWND hWnd;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	hWnd = _createHWND( NULL, dwStyle, x, y, nWidth, nHeight, hWndParent, (HMENU)321, hInstance, NULL );
	if(hWnd) {
		m_dlgHeader.create( hWnd, (HMENU)HEADER_DIALOG_ID, hInstance );
		int nHeight2 = m_rcCanvas.top - SPACE_EDITOR_CONTROLBAR_HEIGHT - 2;
		m_dlgControlBar.create( m_rcCanvas.left, nHeight2, m_rcCanvas.right - m_rcCanvas.left, SPACE_EDITOR_CONTROLBAR_HEIGHT, hWnd, (HMENU)CONTROLBAR_DIALOG_ID, hInstance );
		m_dlgProperty.create( 15, nHeight2, m_rcCanvas.left - 15 - 10, m_rcCanvas.bottom - nHeight2, hWnd, (HMENU)PROPERTY_DIALOG_ID, hInstance );
		m_hWndEdit = ::CreateWindowEx( NULL, _T( "Edit" ), _T( "" ), WS_CHILD | ES_MULTILINE | ES_WANTRETURN, 0, 0, EDIT_DEFAULT_WIDTH, EDIT_DEFAULT_HEIGHT, hWnd, (HMENU)EDIT_BOX_ID, hInstance, NULL );
	}
	m_EditWndProcOld = (WNDPROC)::SetWindowLong( m_hWndEdit, GWL_WNDPROC, (LONG)WndProcNew );
	g_pEditorDlg = this;
	//
	return hWnd;
}

LRESULT ImageEditorDlg::WndProcNew( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	switch(uMsg) {
	case WM_KEYDOWN:{
						if(wParam == VK_RETURN) {
							g_pEditorDlg->m_bReturn = true;
						}
// 						LOGFONT lFont;
// 						HFONT hFont;
// 						
// 						::memset( &lFont, 0, sizeof(LOGFONT) );
// 						_tcscpy_s( lFont.lfFaceName, g_pEditorDlg->m_dlgProperty.getFontFaceName().c_str() );
// 						//::MessageBox( NULL, g_pEditorDlg->m_dlgProperty.getFontFaceName().c_str(), NULL, MB_OK );
// 						lFont.lfHeight = g_pEditorDlg->m_dlgProperty.getFontSize();
// 						hFont = ::CreateFontIndirect( &lFont );
// 						::SendMessage( g_pEditorDlg->m_hWndEdit, WM_SETFONT, (WPARAM)(hFont), NULL );
// 						::InvalidateRect( g_pEditorDlg->m_hWndEdit, NULL, FALSE );
// 						::DeleteObject( hFont );
	}
		break;
	case WM_MOVE:{
					 int xPos = LOWORD( lParam );
					 int yPos = HIWORD( lParam );
					 RECT rcClient;
					 RECT rcEdit;
					 ::GetClientRect( g_pEditorDlg->m_hWndEdit, &rcEdit );
					 ::GetClientRect( g_pEditorDlg->m_hWnd, &rcClient );
					 unsigned int uWidth = rcEdit.right - rcEdit.left;
					 unsigned int uHeight = rcEdit.bottom - rcEdit.top;
					 YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
					 g_pEditorDlg->_drawStatus2( memDC.getMemDC() );
					 YBCtrlMemDC memDC2( uWidth, uHeight);
					 ::BitBlt( memDC2.getMemDC(), 0, 0, uWidth, uHeight, memDC.getMemDC(), xPos, yPos, SRCCOPY );
					 //
					 HDC hDC = ::GetDC( g_pEditorDlg->m_hWndEdit );
					 ::BitBlt( hDC, 0, 0, uWidth, uHeight, memDC2.getMemDC(), 0, 0, SRCCOPY );
	}
		break;
	case WM_SIZE:{
					 int nWidth = LOWORD( lParam );
					 int nHeight = HIWORD( lParam );
					 RECT rcClient;
					 ::GetClientRect( g_pEditorDlg->m_hWnd, &rcClient );
					 YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
					 g_pEditorDlg->_drawStatus2( memDC.getMemDC() );
					 int xPos = min( g_pEditorDlg->m_rcTxtBox.left, g_pEditorDlg->m_rcTxtBox.right );
					 int yPos = min( g_pEditorDlg->m_rcTxtBox.top, g_pEditorDlg->m_rcTxtBox.bottom );
					 YBCtrlMemDC memDC2( nWidth, nHeight);
					 ::BitBlt( memDC2.getMemDC(), 0, 0, nWidth, nHeight, memDC.getMemDC(), xPos, yPos, SRCCOPY );
					 //
					 HDC hDC = ::GetDC( g_pEditorDlg->m_hWndEdit );
					 ::BitBlt( hDC, 0, 0, nWidth, nHeight, memDC2.getMemDC(), 0, 0, SRCCOPY );
	}
		break;
	case WM_ERASEBKGND:{
						   RECT rcEdit;
						   ::GetClientRect( g_pEditorDlg->m_hWndEdit, &rcEdit );
						   int nWidth = rcEdit.right - rcEdit.left;
						   int nHeight = rcEdit.bottom - rcEdit.top;

						   HDC hDCDst = (HDC)wParam;
						   HDC hDCSrc = ::GetDC( g_pEditorDlg->m_hWnd );
						   YBCtrlMemDC memDC( nWidth, nHeight );
						   int xPos = min( g_pEditorDlg->m_rcTxtBox.left, g_pEditorDlg->m_rcTxtBox.right );
						   int yPos = min( g_pEditorDlg->m_rcTxtBox.top, g_pEditorDlg->m_rcTxtBox.bottom );
						   ::BitBlt( memDC.getMemDC(), 0, 0, nWidth, nHeight, hDCSrc, xPos, yPos, SRCCOPY );
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
	return ::CallWindowProc( m_EditWndProcOld, hwnd, uMsg, wParam, lParam );
}

void ImageEditorDlg::_calcLineSpace() {
	Gdiplus::SolidBrush     brush( Gdiplus::Color( 124, 25, 28 ) );


	Gdiplus::RectF bound;
	SIZE sz;
	/*
	HDC hDC = gs.GetHDC();
	SetTextCharacterExtra( hDC, 0 );
	SetTextJustification( hDC, 0, 1 );
	gs.ReleaseHDC( hDC );


	gs.SetSmoothingMode( Gdiplus::SmoothingModeAntiAlias );
	gs.SetTextRenderingHint( Gdiplus::TextRenderingHintAntiAliasGridFit );
*/
	int emSize = 20;
	Gdiplus::StringFormat   *strFmt = Gdiplus::StringFormat::GenericTypographic()->Clone();
	Gdiplus::FontFamily  fontFamily( L"微软雅黑" );


	//行间距计算  
	Gdiplus::REAL linespace = fontFamily.GetLineSpacing( Gdiplus::FontStyleRegular );
	Gdiplus::REAL ascent = fontFamily.GetCellAscent( Gdiplus::FontStyleRegular );
	Gdiplus::REAL descent = fontFamily.GetCellDescent( Gdiplus::FontStyleRegular );
	Gdiplus::REAL emheight = fontFamily.GetEmHeight( Gdiplus::FontStyleRegular );

	Gdiplus::Font    font( &fontFamily, emSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel );
//	Gdiplus::REAL    ftheight = font.GetHeight( &gs );
	Gdiplus::REAL    ftsize = font.GetSize();


	INT16 ascentp = ftsize * ascent / emheight;
	INT16 descentp = ftsize * descent / emheight;
	INT16 linespacep = ftsize * linespace / emheight;
	INT16 gapline = linespacep - ascentp - descentp;

}

void ImageEditorDlg::setPath( const tstring_type& tstrDirPath ) {
	m_tstrDirPath = tstrDirPath;
	_loadImage();
	//
	m_dlgControlBar.setPath( tstrDirPath );
	m_dlgHeader.setPath( tstrDirPath );
	m_dlgProperty.setPath( tstrDirPath );
}

YBCTRL_WNDMSGMAP_BEGIN( ImageEditorDlg, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
	YBCTRL_ONWNDMSG( WM_MOUSEWHEEL, _onWM_MOUSEWHEEL )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_LBUTTONUP, _onWM_LBUTTONUP )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	//YBCTRL_ONWNDMSG( WM_SETCURSOR, _onWM_SETCURSOR )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_CTLCOLOREDIT, _onWM_COLOREDIT )
YBCTRL_WNDMSGMAP_END()

void ImageEditorDlg::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
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

void ImageEditorDlg::_onWM_LBUTTONUP( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONUP );
	POINT ptMouse;
	RECT rcClient;
	YBCtrlImageStatic* pYBImage = NULL;
	unsigned int uWidthImage = 0;
	unsigned int uHeightImage = 0;
	ptMouse.x = LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = HIWORD( pYBCtrlWndMsg->m_lParam );
	//根据鼠标位置设置光标形状
	m_bDrag = false;
	if(::PtInRect( &m_rcCanvas, ptMouse )) {
		if((m_uFlag & IMAGE_EDITOR_TYPE) == IMAGE_EDITOR_TYPE){
			if((m_uFlag & ENABLE_EDITOR_IMAGE) == ENABLE_EDITOR_IMAGE){
				::SetCursor( ::LoadCursor( NULL, IDC_IBEAM ) );
				if( ( (m_uFlag & BTN_STATUS_DOWN) == BTN_STATUS_DOWN ) && !m_bHide ) { //设置文本框位置.
					if( ( (m_uFlag & MOUSEMOVE_IN_TEXTBOX_LTOP) != MOUSEMOVE_IN_TEXTBOX_LTOP) 
						&& ((m_uFlag & MOUSEMOVE_IN_TEXTBOX_TOP) != MOUSEMOVE_IN_TEXTBOX_TOP)
						&& ((m_uFlag & MOUSEMOVE_IN_TEXTBOX_RTOP) != MOUSEMOVE_IN_TEXTBOX_RTOP)
						&& ((m_uFlag & MOUSEMOVE_IN_TEXTBOX_LEFT) != MOUSEMOVE_IN_TEXTBOX_LEFT)
						&& ((m_uFlag & MOUSEMOVE_IN_TEXTBOX_RIGHT) != MOUSEMOVE_IN_TEXTBOX_RIGHT)
						&& ((m_uFlag & MOUSEMOVE_IN_TEXTBOX_LBOTTOM) != MOUSEMOVE_IN_TEXTBOX_LBOTTOM)
						&& ((m_uFlag & MOUSEMOVE_IN_TEXTBOX_BOTTOM) != MOUSEMOVE_IN_TEXTBOX_BOTTOM)
						&& ((m_uFlag & MOUSEMOVE_IN_TEXTBOX_RBOTTOM) != MOUSEMOVE_IN_TEXTBOX_RBOTTOM)
						&& ((m_uFlag & MOUSEMOVE_IN_BMP_BRCORNER) != MOUSEMOVE_IN_BMP_BRCORNER)
						&& ((m_uFlag & MOUSEMOVE_IN_BMP_RIGHT) != MOUSEMOVE_IN_BMP_RIGHT)
						&& ((m_uFlag & MOUSEMOVE_IN_BMP_BOTTOM) != MOUSEMOVE_IN_BMP_BOTTOM)
						&& !m_bMoveAll
						){
						if(ptMouse.x > m_ptMouse.x) {
							m_rcTxtBox.left = m_ptMouse.x;
						} else {
							m_rcTxtBox.left = ptMouse.x;
						}
						if(ptMouse.y > m_ptMouse.y) {
							m_rcTxtBox.top = m_ptMouse.y;
						} else {
							m_rcTxtBox.top = ptMouse.y;
						}
						//
						unsigned int uWidth = abs( m_ptMouse.x - ptMouse.x );
						unsigned int uHeight = abs( m_ptMouse.y - ptMouse.y );
						uWidth = max( uWidth, EDIT_DEFAULT_WIDTH );
						uHeight = max( uHeight, EDIT_DEFAULT_HEIGHT );
						m_rcTxtBox.right = m_rcTxtBox.left + uWidth;
						m_rcTxtBox.bottom = m_rcTxtBox.top + uHeight;
					}
				}
				if(m_bHide){
					m_bIsShow = false;
					_showTextBox( false );
				} else {
					m_bIsShow = true;
					_showTextBox( true );
				}
				//
			}
		}
	} else {
		::SetCursor( ::LoadCursor( NULL, IDC_ARROW ) );
	}
	m_bMoveAll = false;
	//
	m_uFlag &= ~MOUSEMOVE_IN_BMP_BOTTOM;
	m_uFlag &= ~MOUSEMOVE_IN_BMP_RIGHT;
	m_uFlag &= ~MOUSEMOVE_IN_BMP_BRCORNER;
	m_uFlag &= ~BTN_STATUS_DOWN;
	m_uFlag &= ~MOUSEMOVE_IN_TEXTBOX_BOTH;
	m_uFlag &= ~MOUSEMOVE_IN_TEXTBOX_LTOP;
	m_uFlag &= ~MOUSEMOVE_IN_TEXTBOX_TOP;
	m_uFlag &= ~MOUSEMOVE_IN_TEXTBOX_RTOP;
	m_uFlag &= ~MOUSEMOVE_IN_TEXTBOX_LEFT;
	m_uFlag &= ~MOUSEMOVE_IN_TEXTBOX_RIGHT;
	m_uFlag &= ~MOUSEMOVE_IN_TEXTBOX_LBOTTOM;
	m_uFlag &= ~MOUSEMOVE_IN_TEXTBOX_BOTTOM;
	m_uFlag &= ~MOUSEMOVE_IN_TEXTBOX_RBOTTOM;
	//
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	HDC hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
	//
	::SetCapture( m_hWndCaptureOld );
}

void ImageEditorDlg::_onWM_MOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	TRACKMOUSEEVENT trackMouseEvt;
	RECT rcClient;
	POINT ptMouse;
	RECT rcBMPBottom;
	RECT rcBMPRight;
	RECT rcBMPBRCorner;

	unsigned int uSideLength = 7;
	::SetRect( &rcBMPBottom, m_rcCanvas.left + (m_rcImage.right - m_rcImage.left - uSideLength) / 2, m_rcImage.bottom, m_rcCanvas.left + (m_rcImage.right - m_rcImage.left - uSideLength) / 2 + uSideLength, m_rcImage.bottom + uSideLength );
	::SetRect( &rcBMPRight, m_rcImage.right, m_rcCanvas.top + (m_rcImage.bottom - m_rcImage.top - uSideLength) / 2, m_rcImage.right + uSideLength, m_rcCanvas.top + (m_rcImage.bottom - m_rcImage.top - uSideLength) / 2 + uSideLength );
	::SetRect( &rcBMPBRCorner, m_rcImage.right, m_rcImage.bottom, m_rcImage.right + uSideLength, m_rcImage.bottom + uSideLength );
	//画矩形框 8个
	RECT rcTxtBox;
	::SetRect( &rcTxtBox, m_rcTxtBox.left - 2, m_rcTxtBox.top - 2, m_rcTxtBox.right + 2, m_rcTxtBox.bottom + 2 );
	unsigned int uOutSideSpace = 2;
	unsigned int uInsideSpace = 3;
	unsigned int uTBXPos = rcTxtBox.left + (rcTxtBox.right - rcTxtBox.left) / 2;//上下中间点X坐标
	unsigned int uLRYPos = rcTxtBox.top + (rcTxtBox.bottom - rcTxtBox.top) / 2;//左右中间点Y坐标
	RECT rcLTop, rcTop, rcRTop, rcLeft, rcLBottom, rcBottom, rcRBottom, rcRight;
	::SetRect( &rcLTop, rcTxtBox.left - uOutSideSpace, rcTxtBox.top - uOutSideSpace, rcTxtBox.left + uInsideSpace, rcTxtBox.top + uInsideSpace );
	::SetRect( &rcTop, uTBXPos - uInsideSpace, rcTxtBox.top - uOutSideSpace, uTBXPos + uOutSideSpace, rcTxtBox.top + uInsideSpace );
	::SetRect( &rcRTop, rcTxtBox.right - uInsideSpace, rcTxtBox.top - uOutSideSpace, rcTxtBox.right + uOutSideSpace, rcTxtBox.top + uInsideSpace );
	::SetRect( &rcLBottom, rcTxtBox.left - uOutSideSpace, rcTxtBox.bottom - uInsideSpace, rcTxtBox.left + uInsideSpace, rcTxtBox.bottom + uOutSideSpace );
	::SetRect( &rcBottom, uTBXPos - uInsideSpace, rcTxtBox.bottom - uOutSideSpace, uTBXPos + uOutSideSpace, rcTxtBox.bottom + uInsideSpace );
	::SetRect( &rcRBottom, rcTxtBox.right - uInsideSpace, rcTxtBox.bottom - uInsideSpace, rcTxtBox.right + uOutSideSpace, rcTxtBox.bottom + uOutSideSpace );
	::SetRect( &rcLeft, rcTxtBox.left - uOutSideSpace, uLRYPos - uOutSideSpace, rcTxtBox.left + uInsideSpace, uLRYPos + uInsideSpace );
	::SetRect( &rcRight, rcTxtBox.right - uInsideSpace, uLRYPos - uOutSideSpace, rcTxtBox.right + uOutSideSpace, uLRYPos + uInsideSpace );
	ptMouse.x = (__int16)LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = (__int16)HIWORD( pYBCtrlWndMsg->m_lParam );
	//
	trackMouseEvt.cbSize = sizeof(TRACKMOUSEEVENT);
	trackMouseEvt.dwFlags = TME_LEAVE;
	trackMouseEvt.hwndTrack = pYBCtrlWndMsg->m_hWnd;
	trackMouseEvt.dwHoverTime = HOVER_DEFAULT;
	::TrackMouseEvent( &trackMouseEvt );
	//根据鼠标位置设置光标形状
	if(::PtInRect( &m_rcCanvas, ptMouse )) {
		if((m_uFlag & IMAGE_EDITOR_TYPE) == IMAGE_EDITOR_TYPE){ //判断是否是图片编辑
			if((m_uFlag & ENABLE_EDITOR_IMAGE ) == ENABLE_EDITOR_IMAGE ){ //判断是否可以进行编辑.
				::SetCursor( ::LoadCursor( NULL, IDC_IBEAM ) );
				if( (m_uFlag & BTN_STATUS_DOWN) == BTN_STATUS_DOWN && !m_bHide ) { //设置文本框位置.
					m_bDrag = true;
					m_bIsShow = true;
					//
					m_uFlag |= BTN_STATUS_MOVING;
					//
					if((m_uFlag & MOUSEMOVE_IN_TEXTBOX_LTOP) == MOUSEMOVE_IN_TEXTBOX_LTOP){
						m_rcTxtBox.left = ptMouse.x + m_uTBLSpace;
						m_rcTxtBox.top = ptMouse.y + m_uTBTSpace;
						m_rcTxtBox.left = min( m_rcTxtBox.right, max( m_rcTxtBox.left, m_rcCanvas.left ) );
						m_rcTxtBox.top = min( m_rcTxtBox.bottom, max( m_rcTxtBox.top, m_rcCanvas.top ) );
						m_bDrag = false;
						_showTextBox( true );
					} else if((m_uFlag & MOUSEMOVE_IN_TEXTBOX_TOP) == MOUSEMOVE_IN_TEXTBOX_TOP){
						m_rcTxtBox.top = ptMouse.y + m_uTBTSpace;
						m_rcTxtBox.top = min( m_rcTxtBox.bottom, max( m_rcTxtBox.top, m_rcCanvas.top ) );
						m_bDrag = false;
						_showTextBox( true );
					} else if((m_uFlag & MOUSEMOVE_IN_TEXTBOX_RTOP) == MOUSEMOVE_IN_TEXTBOX_RTOP){
						m_rcTxtBox.right = ptMouse.x - m_uTBLSpace;
						m_rcTxtBox.top = ptMouse.y + m_uTBTSpace;
						m_rcTxtBox.right = max( m_rcTxtBox.left, min( m_rcTxtBox.right, m_rcCanvas.right ) );
						m_rcTxtBox.top = min( m_rcTxtBox.bottom, max( m_rcTxtBox.top, m_rcCanvas.top ) );
						m_bDrag = false;
						_showTextBox( true );
					} else if((m_uFlag & MOUSEMOVE_IN_TEXTBOX_LEFT) == MOUSEMOVE_IN_TEXTBOX_LEFT){
						m_rcTxtBox.left = ptMouse.x + m_uTBTSpace;
						m_rcTxtBox.left = min( m_rcTxtBox.right, max( m_rcTxtBox.left, m_rcCanvas.left ) );
						m_bDrag = false;
						_showTextBox( true );
					} else if((m_uFlag & MOUSEMOVE_IN_TEXTBOX_RIGHT) == MOUSEMOVE_IN_TEXTBOX_RIGHT){
						m_rcTxtBox.right = ptMouse.x - m_uTBLSpace;
						m_rcTxtBox.right = max( m_rcTxtBox.left, min( m_rcTxtBox.right, m_rcCanvas.right ) );
						m_bDrag = false;
						_showTextBox( true );
					} else if((m_uFlag & MOUSEMOVE_IN_TEXTBOX_LBOTTOM) == MOUSEMOVE_IN_TEXTBOX_LBOTTOM){
						m_rcTxtBox.left = ptMouse.x + m_uTBLSpace;
						m_rcTxtBox.bottom = ptMouse.y - m_uTBTSpace;
						m_rcTxtBox.left = min( m_rcTxtBox.right, max( m_rcTxtBox.left, m_rcCanvas.left ) );
						m_rcTxtBox.bottom = max( m_rcTxtBox.top, min( m_rcTxtBox.bottom, m_rcCanvas.bottom ) );
						m_bDrag = false;
						_showTextBox( true );
					} else if((m_uFlag & MOUSEMOVE_IN_TEXTBOX_BOTTOM) == MOUSEMOVE_IN_TEXTBOX_BOTTOM){
						m_rcTxtBox.bottom = ptMouse.y - m_uTBTSpace;
						m_rcTxtBox.bottom = max( m_rcTxtBox.top, min( m_rcTxtBox.bottom, m_rcCanvas.bottom ) );
						m_bDrag = false;
						_showTextBox( true );
					} else if((m_uFlag & MOUSEMOVE_IN_TEXTBOX_RBOTTOM) == MOUSEMOVE_IN_TEXTBOX_RBOTTOM){
						m_rcTxtBox.right = ptMouse.x - m_uTBLSpace;
						m_rcTxtBox.bottom = ptMouse.y - m_uTBTSpace;
						m_rcTxtBox.right = max( m_rcTxtBox.left, min( m_rcTxtBox.right, m_rcCanvas.right ) );
						m_rcTxtBox.bottom = max( m_rcTxtBox.top, min( m_rcTxtBox.bottom, m_rcCanvas.bottom ) );
						m_bDrag = false;
						_showTextBox( true );
					} else if( m_bMoveAll ) {
						int dx = ptMouse.x - m_ptLBtnDownOld.x;
						int dy = ptMouse.y - m_ptLBtnDownOld.y;
						m_ptLBtnDownOld = ptMouse;
						m_bDrag = false;
						::OffsetRect( &m_rcTxtBox, dx, dy );
						m_rcTxtBox.left = max( m_rcCanvas.left, min( m_rcCanvas.right, m_rcTxtBox.left ) );
						m_rcTxtBox.right = min( m_rcCanvas.right, max( m_rcCanvas.left, m_rcTxtBox.right ) );
						m_rcTxtBox.top = max( m_rcCanvas.top, min( m_rcCanvas.bottom, m_rcTxtBox.top ) );
						m_rcTxtBox.bottom = min( m_rcCanvas.bottom, max( m_rcCanvas.top, m_rcTxtBox.bottom ) );
						_showTextBox( true );
// 						if((rcTxtBox.left >= m_rcCanvas.left) && (rcTxtBox.right <= m_rcCanvas.right)
// 							&& (rcTxtBox.top >= m_rcCanvas.top) && (rcTxtBox.bottom <= m_rcCanvas.bottom)) {
// 							int dx = ptMouse.x - m_ptLBtnDownOld.x;
// 							int dy = ptMouse.y - m_ptLBtnDownOld.y;
// 							m_ptLBtnDownOld = ptMouse;
// 							m_bDrag = false;
// 							::OffsetRect( &m_rcTxtBox, dx, dy );
// 							_showTextBox( true );
// 						}
 					} else {
						if(ptMouse.x > m_ptMouse.x) {
							m_rcTxtBox.left = m_ptMouse.x;
						} else {
							m_rcTxtBox.left = ptMouse.x;
						}
						if(ptMouse.y > m_ptMouse.y) {
							m_rcTxtBox.top = m_ptMouse.y;
						} else {
							m_rcTxtBox.top = ptMouse.y;
						}
						//
						unsigned int uWidth = abs( m_ptMouse.x - ptMouse.x );
						unsigned int uHeight = abs( m_ptMouse.y - ptMouse.y );
						if(!m_bDrag) {
							uWidth = max( uWidth, EDIT_DEFAULT_WIDTH );
							uHeight = max( uHeight, EDIT_DEFAULT_HEIGHT );
						}
						m_rcTxtBox.right = m_rcTxtBox.left + uWidth;
						m_rcTxtBox.bottom = m_rcTxtBox.top + uHeight;
					}
				}
			}
		}
	} else {
		::SetCursor( ::LoadCursor( NULL, IDC_ARROW  ));
	}
	if(::PtInRect( &rcBMPBottom, ptMouse )) {
		::SetCursor( ::LoadCursor( NULL, IDC_SIZENS ) );
	} else if( ::PtInRect( &rcBMPRight, ptMouse )) {
		::SetCursor( ::LoadCursor( NULL, IDC_SIZEWE) );
	} else if(::PtInRect( &rcBMPBRCorner, ptMouse )) {
		::SetCursor( ::LoadCursor( NULL, IDC_SIZENWSE ) );
	}
	//拉动文本框时
	if(m_bIsShow) {
		if(::PtInRect( &rcLTop, ptMouse )) {
			::SetCursor( ::LoadCursor( NULL, IDC_SIZENWSE ) );
		} else if(::PtInRect( &rcTop, ptMouse )) {
			::SetCursor( ::LoadCursor( NULL, IDC_SIZENS ) );
		} else if(::PtInRect( &rcRTop, ptMouse )) {
			::SetCursor( ::LoadCursor( NULL, IDC_SIZENESW ) );
		} else if(::PtInRect( &rcLeft, ptMouse )) {
			::SetCursor( ::LoadCursor( NULL, IDC_SIZEWE ) );
		} else if(::PtInRect( &rcRight, ptMouse )) {
			::SetCursor( ::LoadCursor( NULL, IDC_SIZEWE ) );
		} else if(::PtInRect( &rcLBottom, ptMouse )) {
			::SetCursor( ::LoadCursor( NULL, IDC_SIZENESW ) );
		} else if(::PtInRect( &rcBottom, ptMouse )) {
			::SetCursor( ::LoadCursor( NULL, IDC_SIZENS ) );
		} else if(::PtInRect( &rcRBottom, ptMouse )) {
			::SetCursor( ::LoadCursor( NULL, IDC_SIZENWSE ) );
		} else if(::PtInRect( &rcTxtBox, ptMouse )) {
			::SetCursor( ::LoadCursor( NULL, IDC_SIZEALL ) );
		}
	}

	//拉动图片放大区域的位置.
	if((m_uFlag & MOUSEMOVE_IN_BMP_BOTTOM) == MOUSEMOVE_IN_BMP_BOTTOM) {
		m_rcImage.bottom = ptMouse.y - m_uBMPBSpace;
		m_rcImage.bottom = min( m_rcCanvas.bottom, max( m_rcCanvas.top + 30, ptMouse.y ) );
	} else if((m_uFlag & MOUSEMOVE_IN_BMP_RIGHT) == MOUSEMOVE_IN_BMP_RIGHT) {
		m_rcImage.right = ptMouse.x - m_uBMPRSpace;
		m_rcImage.right = min( m_rcCanvas.right, max( m_rcCanvas.left + 30, ptMouse.x ) );
	} else if((m_uFlag & MOUSEMOVE_IN_BMP_BRCORNER) == MOUSEMOVE_IN_BMP_BRCORNER) {
		m_rcImage.right = ptMouse.x - m_uBMPRSpace;
		m_rcImage.bottom = ptMouse.y - m_uBMPBSpace;
		m_rcImage.right = min( m_rcCanvas.right, max( m_rcCanvas.left + 30, ptMouse.x ) );
		m_rcImage.bottom = min( m_rcCanvas.bottom, max( m_rcCanvas.top + 30, ptMouse.y ) );
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

void ImageEditorDlg::_onWM_MOUSEWHEEL( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEWHEEL );
	__int16 i16WheelDelta = HIWORD( pYBCtrlWndMsg->m_wParam );
	if( ( m_uFlag & IMAGE_EDITOR_TYPE ) != IMAGE_EDITOR_TYPE )
		return;
	int uWidthOld = m_rcImage.right - m_rcImage.left;
	int uHeightOld = m_rcImage.bottom - m_rcImage.top;
	int uWidthMax = m_rcCanvas.right - m_rcCanvas.left;
	int uHeightMax = m_rcCanvas.bottom - m_rcCanvas.top;
	//
	switch(i16WheelDelta){
	case 120:
	{
				int uWidthNew = int( uWidthOld*1.1 + 0.5 );
				int uHeightNew = int( uHeightOld*1.1 + 0.5 );
				uWidthNew = min( uWidthMax, uWidthNew );
				uHeightNew = min( uHeightNew, uHeightMax );
				::SetRect( &m_rcImage, m_rcCanvas.left, m_rcCanvas.top, m_rcCanvas.left + uWidthNew, m_rcCanvas.top + uHeightNew );
	}
		break;
	case -120:
	{
				 int uWidthNew = int( uWidthOld*0.9 + 0.5 );
				 int uHeightNew = int( uHeightOld*0.9 + 0.5 );
				 uWidthNew = max( uWidthNew, 20 );
				 uHeightNew = max( uHeightNew, 20 );
				 ::SetRect( &m_rcImage, m_rcCanvas.left, m_rcCanvas.top, m_rcCanvas.left + uWidthNew, m_rcCanvas.top + uHeightNew );
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

void ImageEditorDlg::_onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
	HDC hDC;
	RECT rcClient;
	RECT rcBMPBottom;
	RECT rcBMPRight;
	RECT rcBMPBRCorner;
	unsigned int uSideLength = 7;

	::SetRect( &rcBMPBottom, m_rcCanvas.left + (m_rcImage.right - m_rcImage.left - uSideLength) / 2, m_rcImage.bottom, m_rcCanvas.left + (m_rcImage.right - m_rcImage.left - uSideLength) / 2 + uSideLength, m_rcImage.bottom + uSideLength );
	::SetRect( &rcBMPRight, m_rcImage.right, m_rcCanvas.top + (m_rcImage.bottom - m_rcImage.top - uSideLength) / 2, m_rcImage.right + uSideLength, m_rcCanvas.top + (m_rcImage.bottom - m_rcImage.top - uSideLength) / 2 + uSideLength );
	::SetRect( &rcBMPBRCorner, m_rcImage.right, m_rcImage.bottom, m_rcImage.right + uSideLength, m_rcImage.bottom + uSideLength );
	//画矩形框 8个
	RECT rcTxtBox;
	::SetRect( &rcTxtBox, m_rcTxtBox.left - 2, m_rcTxtBox.top - 2, m_rcTxtBox.right + 2, m_rcTxtBox.bottom + 2 );
	unsigned int uOutSideSpace = 2;
	unsigned int uInsideSpace = 3;
	unsigned int uTBXPos = rcTxtBox.left + (rcTxtBox.right - rcTxtBox.left) / 2;//上下中间点X坐标
	unsigned int uLRYPos = rcTxtBox.top + (rcTxtBox.bottom - rcTxtBox.top) / 2;//左右中间点Y坐标
	RECT rcLTop, rcTop, rcRTop, rcLeft, rcLBottom, rcBottom, rcRBottom, rcRight;
	::SetRect( &rcLTop, rcTxtBox.left - uOutSideSpace, rcTxtBox.top - uOutSideSpace, rcTxtBox.left + uInsideSpace, rcTxtBox.top + uInsideSpace );
	::SetRect( &rcTop, uTBXPos - uInsideSpace, rcTxtBox.top - uOutSideSpace, uTBXPos + uOutSideSpace, rcTxtBox.top + uInsideSpace );
	::SetRect( &rcRTop, rcTxtBox.right - uInsideSpace, rcTxtBox.top - uOutSideSpace, rcTxtBox.right + uOutSideSpace, rcTxtBox.top + uInsideSpace );
	::SetRect( &rcLBottom, rcTxtBox.left - uOutSideSpace, rcTxtBox.bottom - uInsideSpace, rcTxtBox.left + uInsideSpace, rcTxtBox.bottom + uOutSideSpace );
	::SetRect( &rcBottom, uTBXPos - uInsideSpace, rcTxtBox.bottom - uOutSideSpace, uTBXPos + uOutSideSpace, rcTxtBox.bottom + uInsideSpace );
	::SetRect( &rcRBottom, rcTxtBox.right - uInsideSpace, rcTxtBox.bottom - uInsideSpace, rcTxtBox.right + uOutSideSpace, rcTxtBox.bottom + uOutSideSpace );
	::SetRect( &rcLeft, rcTxtBox.left - uOutSideSpace, uLRYPos - uOutSideSpace, rcTxtBox.left + uInsideSpace, uLRYPos + uInsideSpace );
	::SetRect( &rcRight, rcTxtBox.right - uInsideSpace, uLRYPos - uOutSideSpace, rcTxtBox.right + uOutSideSpace, uLRYPos + uInsideSpace );
	m_ptMouse.x = LOWORD( pYBCtrlWndMsg->m_lParam );
	m_ptMouse.y = HIWORD( pYBCtrlWndMsg->m_lParam );
	m_ptLBtnDownOld = m_ptMouse;
	m_bDrag = false;
	//根据鼠标位置设置光标形状
	if(::PtInRect( &m_rcCanvas, m_ptMouse )) {
		if((m_uFlag & IMAGE_EDITOR_TYPE) == IMAGE_EDITOR_TYPE){
			if((m_uFlag & ENABLE_EDITOR_IMAGE) == ENABLE_EDITOR_IMAGE){
				::SetCursor( ::LoadCursor( NULL, IDC_IBEAM ) );	//设置鼠标形状
				m_uFlag |= BTN_STATUS_DOWN;						//判断鼠标是否按下.
				m_uFlag &= ~BTN_STATUS_MOVING;
			}
		}
	} else {
		::SetCursor( ::LoadCursor( NULL, IDC_ARROW ) );
		m_uFlag &= ~BTN_STATUS_DOWN;
	}
	if(m_bIsShow) {
		//拉动文本框时
		if(::PtInRect( &rcLTop, m_ptMouse )) {
			m_uFlag |= MOUSEMOVE_IN_TEXTBOX_LTOP;
			::SetCursor( ::LoadCursor( NULL, IDC_SIZENWSE ) );
			m_uTBLSpace = m_ptMouse.x - m_rcTxtBox.left;
			m_uTBTSpace = m_ptMouse.y - m_rcTxtBox.top;
		} else if(::PtInRect( &rcTop, m_ptMouse )) {
			m_uFlag |= MOUSEMOVE_IN_TEXTBOX_TOP;
			::SetCursor( ::LoadCursor( NULL, IDC_SIZENS ) );
			m_uTBTSpace = m_ptMouse.y - m_rcTxtBox.top;
		} else if(::PtInRect( &rcRTop, m_ptMouse )) {
			m_uFlag |= MOUSEMOVE_IN_TEXTBOX_RTOP;
			::SetCursor( ::LoadCursor( NULL, IDC_SIZENESW ) );
			m_uTBLSpace = m_ptMouse.x - m_rcTxtBox.right;
			m_uTBTSpace = m_ptMouse.y - m_rcTxtBox.top;
		} else if(::PtInRect( &rcLeft, m_ptMouse )) {
			m_uFlag |= MOUSEMOVE_IN_TEXTBOX_LEFT;
			::SetCursor( ::LoadCursor( NULL, IDC_SIZEWE ) );
			m_uTBLSpace = m_ptMouse.x - m_rcTxtBox.left;
		} else if(::PtInRect( &rcRight, m_ptMouse )) {
			m_uFlag |= MOUSEMOVE_IN_TEXTBOX_RIGHT;
			::SetCursor( ::LoadCursor( NULL, IDC_SIZEWE ) );
			m_uTBLSpace = m_ptMouse.x - m_rcTxtBox.right;
		} else if(::PtInRect( &rcLBottom, m_ptMouse )) {
			m_uFlag |= MOUSEMOVE_IN_TEXTBOX_LBOTTOM;
			::SetCursor( ::LoadCursor( NULL, IDC_SIZENESW ) );
			m_uTBLSpace = m_ptMouse.x - m_rcTxtBox.left;
			m_uTBTSpace = m_ptMouse.y - m_rcTxtBox.bottom;
		} else if(::PtInRect( &rcBottom, m_ptMouse )) {
			m_uFlag |= MOUSEMOVE_IN_TEXTBOX_BOTTOM;
			::SetCursor( ::LoadCursor( NULL, IDC_SIZENS ) );
			m_uTBTSpace = m_ptMouse.y - m_rcTxtBox.bottom;
		} else if(::PtInRect( &rcRBottom, m_ptMouse )) {
			m_uFlag |= MOUSEMOVE_IN_TEXTBOX_RBOTTOM;
			::SetCursor( ::LoadCursor( NULL, IDC_SIZENWSE ) );
			m_uTBLSpace = m_ptMouse.x - m_rcTxtBox.right;
			m_uTBTSpace = m_ptMouse.y - m_rcTxtBox.bottom;
		} else if(::PtInRect( &rcTxtBox, m_ptMouse )){
			//m_uFlag |= MOUSEMOVE_IN_TEXTBOX_BOTH;
			m_bMoveAll = true;
			::SetCursor( ::LoadCursor( NULL, IDC_SIZEALL ) );
		} else {
			::SetCursor( ::LoadCursor( NULL, IDC_IBEAM ) );
			m_bHide = true;
			m_bIsShow = false;
			//_showTextBox( false );
		}
	} else {
// 		//拉动文本框时
// 		if(::PtInRect( &rcLTop, m_ptMouse )) {
// 			m_uFlag |= MOUSEMOVE_IN_TEXTBOX_LTOP;
// 			::SetCursor( ::LoadCursor( NULL, IDC_SIZENWSE ) );
// 			m_uTBLSpace = m_ptMouse.x - m_rcTxtBox.left;
// 			m_uTBTSpace = m_ptMouse.y - m_rcTxtBox.top;
// 		} else if(::PtInRect( &rcTop, m_ptMouse )) {
// 			m_uFlag |= MOUSEMOVE_IN_TEXTBOX_TOP;
// 			::SetCursor( ::LoadCursor( NULL, IDC_SIZENS ) );
// 			m_uTBTSpace = m_ptMouse.y - m_rcTxtBox.top;
// 		} else if(::PtInRect( &rcRTop, m_ptMouse )) {
// 			m_uFlag |= MOUSEMOVE_IN_TEXTBOX_RTOP;
// 			::SetCursor( ::LoadCursor( NULL, IDC_SIZENESW ) );
// 			m_uTBLSpace = m_ptMouse.x - m_rcTxtBox.right;
// 			m_uTBTSpace = m_ptMouse.y - m_rcTxtBox.top;
// 		} else if(::PtInRect( &rcLeft, m_ptMouse )) {
// 			m_uFlag |= MOUSEMOVE_IN_TEXTBOX_LEFT;
// 			::SetCursor( ::LoadCursor( NULL, IDC_SIZEWE ) );
// 			m_uTBLSpace = m_ptMouse.x - m_rcTxtBox.left;
// 		} else if(::PtInRect( &rcRight, m_ptMouse )) {
// 			m_uFlag |= MOUSEMOVE_IN_TEXTBOX_RIGHT;
// 			::SetCursor( ::LoadCursor( NULL, IDC_SIZEWE ) );
// 			m_uTBLSpace = m_ptMouse.x - m_rcTxtBox.right;
// 		} else if(::PtInRect( &rcLBottom, m_ptMouse )) {
// 			m_uFlag |= MOUSEMOVE_IN_TEXTBOX_LBOTTOM;
// 			::SetCursor( ::LoadCursor( NULL, IDC_SIZENESW ) );
// 			m_uTBLSpace = m_ptMouse.x - m_rcTxtBox.left;
// 			m_uTBTSpace = m_ptMouse.y - m_rcTxtBox.bottom;
// 		} else if(::PtInRect( &rcBottom, m_ptMouse )) {
// 			m_uFlag |= MOUSEMOVE_IN_TEXTBOX_BOTTOM;
// 			::SetCursor( ::LoadCursor( NULL, IDC_SIZENS ) );
// 			m_uTBTSpace = m_ptMouse.y - m_rcTxtBox.bottom;
// 		} else if(::PtInRect( &rcRBottom, m_ptMouse )) {
// 			m_uFlag |= MOUSEMOVE_IN_TEXTBOX_RBOTTOM;
// 			::SetCursor( ::LoadCursor( NULL, IDC_SIZENWSE ) );
// 			m_uTBLSpace = m_ptMouse.x - m_rcTxtBox.right;
// 			m_uTBTSpace = m_ptMouse.y - m_rcTxtBox.bottom;
// 
// 		} else if( ::PtInRect( &rcTxtBox, m_ptMouse ) ){
// 			//m_uFlag |= MOUSEMOVE_IN_TEXTBOX_BOTH;
// 			m_bMoveAll = true;
// 			::SetCursor( ::LoadCursor( NULL, IDC_SIZEALL ) );
//  		} else {
// 			m_bIsShow = false;
// 			m_bHide = false;
// 		}
		m_bIsShow = false;
		m_bHide = false;
	}
	//拉动图片时
	if(::PtInRect( &rcBMPBottom, m_ptMouse )) {
		m_uFlag |= MOUSEMOVE_IN_BMP_BOTTOM;
		::SetCursor( ::LoadCursor( NULL, IDC_SIZENS ) );
		m_uBMPBSpace = m_ptMouse.y - m_rcImage.bottom;
	} else if(::PtInRect( &rcBMPRight, m_ptMouse )) {
		m_uFlag |= MOUSEMOVE_IN_BMP_RIGHT;
		::SetCursor( ::LoadCursor( NULL, IDC_SIZEWE ) );
		m_uBMPRSpace = m_ptMouse.x - m_rcImage.right;
	} else if(::PtInRect( &rcBMPBRCorner, m_ptMouse )) {
		m_uFlag |= MOUSEMOVE_IN_BMP_BRCORNER;
		::SetCursor( ::LoadCursor( NULL, IDC_SIZENWSE ) );
		m_uBMPRSpace = m_ptMouse.x - m_rcImage.right;
		m_uBMPBSpace = m_ptMouse.y - m_rcImage.bottom;
	}

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
	//
	m_hWndCaptureOld = ::SetCapture( m_hWnd );
}

void ImageEditorDlg::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );
	switch(wCtrlId) {
	case EDIT_BOX_ID:{
						 if(wEvtCode == EN_CHANGE) {
							 int a = 0; 
						 }
	}
		break;
	case HEADER_DIALOG_ID:{
							  switch(wEvtCode) {
							  case BTN_HEADER_CMD_SAVE:{

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
}

void ImageEditorDlg::_onWM_COLOREDIT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_CTLCOLOREDIT );
	HWND hEditCtrl = (HWND)pYBCtrlWndMsg->m_lParam;
	HDC hEditDC = (HDC)pYBCtrlWndMsg->m_wParam;
	RECT rcEdit;

	::GetClientRect( m_hWndEdit, &rcEdit );
	if(hEditCtrl == m_hWndEdit) {
		//HDC hDC = ::GetDC( m_hWnd );
		int xPos = min( m_rcTxtBox.left, m_rcTxtBox.right );
		int yPos = min( m_rcTxtBox.top, m_rcTxtBox.bottom );
		RECT rcClient;
		::GetClientRect( m_hWnd, &rcClient );
		YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
		_drawStatus2( memDC.getMemDC() );

		::BitBlt( hEditDC, 0, 0, rcEdit.right - rcEdit.left, rcEdit.bottom - rcEdit.top, memDC.getMemDC(), xPos, yPos, SRCCOPY );
		//::ReleaseDC( m_hWnd, hDC );
		::SetTextColor( hEditDC, m_dlgProperty.getTextClr() );
		::SetBkMode( hEditDC, TRANSPARENT );
		SetTextCharacterExtra( hEditDC, 2 );
		int nPos = ::SendMessage( m_hWndEdit, EM_GETSEL, 0, 0 );
		WORD wPosStart = LOWORD( nPos );
		WORD wPosEnd = HIWORD( nPos );
		if((wPosStart != wPosEnd) && ::IsWindowVisible( m_hWndEdit )) {
			TCHAR szMsg[100] = { 0 };
			::GetWindowText( m_hWndEdit, szMsg, 100 );
			//::SetWindowText( m_hWndEdit, szMsg );
		}
		if(m_bReturn) {
			m_bReturn = false;
		}
	}
	int i = GetTextCharacterExtra( hEditDC );
	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG | YBCTRLWNDMSG_FLAG_RESULT_VALID);

}

void ImageEditorDlg::_onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
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

ybctrl::YBCtrlImageStatic* ImageEditorDlg::_getImageStatic( unsigned int uImageIdBk ) {
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

void ImageEditorDlg::_drawStatus2( HDC hDC ) {
	_drawBk( hDC );
	_drawCanvas( hDC );
	_drawImage( hDC );
	_drawText( hDC );
}

void ImageEditorDlg::_drawStatus( HDC hDC ) {
	SetTextCharacterExtra( hDC, 10 );
	_drawBk( hDC );
	_drawCanvas( hDC );
	_drawImage( hDC );
	_drawText( hDC );
	_drawTextBox( hDC );
}

void ImageEditorDlg::_drawBk( HDC hDC ) {
	RECT rcClient;
	HBRUSH hBrushBk;

	hBrushBk = ::CreateSolidBrush( RGB(203, 212, 228) );
	::GetClientRect( m_hWnd, &rcClient );
	::FillRect( hDC, &rcClient, hBrushBk );
	::DeleteObject( hBrushBk );
}

void ImageEditorDlg::_drawCanvas( HDC hDC ) {
	HBRUSH hBrushBk;

	hBrushBk = ::CreateSolidBrush( RGB( 0, 0, 0 ) );
	::FillRect( hDC, &m_rcCanvas, hBrushBk );
	::DeleteObject( hBrushBk );
}

void ImageEditorDlg::_drawText( HDC hDC ) {
	if(m_containerFontInfo.size() == 0)
		return;
	EditFontInfo* pFontInfo = NULL;
	containerFontInfo::iterator itBegin, itEnd;
	itEnd = m_containerFontInfo.end();
	HFONT hFontOld = NULL;
	int iBkModeOld;
	COLORREF clrTxtOld;
	for(itBegin = m_containerFontInfo.begin(); itBegin != itEnd; ++itBegin) {
		pFontInfo = (*itBegin);
		if(pFontInfo) {
			LOGFONT lFont;
			memset( &lFont, 0, sizeof(LOGFONT) );
			_tcscpy_s( lFont.lfFaceName, pFontInfo->m_tstrFaceName.c_str() );
			lFont.lfHeight = pFontInfo->m_uSizeFont;
			HFONT hFont = ::CreateFontIndirect( &lFont );
			hFontOld = (HFONT)::SelectObject( hDC, hFont );
			iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
			clrTxtOld = ::SetTextColor( hDC, pFontInfo->m_clrFont );
			::DrawText( hDC, pFontInfo->m_tstrText.c_str(), pFontInfo->m_tstrText.length(), &pFontInfo->m_rcText, DT_LEFT );
			::SetTextColor( hDC, clrTxtOld );
			::SetBkMode( hDC, iBkModeOld );
			::SelectObject( hDC, hFontOld );
			::DeleteObject( hFont );
			hFont = NULL;
		}
	}
}

void ImageEditorDlg::_drawImage( HDC hDC ) {
	if( ( m_uFlag & IMAGE_EDITOR_TYPE ) != IMAGE_EDITOR_TYPE )
		return;
	YBCtrlImageStatic* pYBImage = NULL;
	pYBImage = _getImageStatic( m_uImageId );
	if(!pYBImage) {
		assert( false );
		return;
	}
	Gdiplus::Graphics graphics( hDC );
	GdipBitBlt( &graphics, pYBImage->getImage(), m_rcImage.left, m_rcImage.top,
		m_rcImage.right - m_rcImage.left, m_rcImage.bottom - m_rcImage.top, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
	//画下图片的最下面、最右边、右下角的三个小矩形.
	RECT rcBMPBottom;
	RECT rcBMPRight;
	RECT rcBMPBRCorner;

	::SetRect( &rcBMPBottom, m_rcCanvas.left + ( m_rcImage.right - m_rcImage.left - SIDE_LENGTH )/2, m_rcImage.bottom, m_rcCanvas.left + ( m_rcImage.right - m_rcImage.left - SIDE_LENGTH )/2 + SIDE_LENGTH, m_rcImage.bottom + SIDE_LENGTH );
	::SetRect( &rcBMPRight, m_rcImage.right, m_rcCanvas.top + ( m_rcImage.bottom - m_rcImage.top - SIDE_LENGTH )/2, m_rcImage.right + SIDE_LENGTH, m_rcCanvas.top + ( m_rcImage.bottom - m_rcImage.top - SIDE_LENGTH )/2 + SIDE_LENGTH );
	::SetRect( &rcBMPBRCorner, m_rcImage.right, m_rcImage.bottom, m_rcImage.right + SIDE_LENGTH, m_rcImage.bottom + SIDE_LENGTH );
	HPEN hPen = ::CreatePen( PS_SOLID, 1, RGB( 255, 255, 255 ) );
	HPEN hPenOld = (HPEN)::SelectObject( hDC, hPen );
	::Rectangle( hDC, rcBMPBottom.left, rcBMPBottom.top, rcBMPBottom.right, rcBMPBottom.bottom );
	::Rectangle( hDC, rcBMPRight.left, rcBMPRight.top, rcBMPRight.right, rcBMPRight.bottom );
	::Rectangle( hDC, rcBMPBRCorner.left, rcBMPBRCorner.top, rcBMPBRCorner.right, rcBMPBRCorner.bottom );
	::SelectObject( hDC, hPenOld );
	::DeleteObject( hPen );
}

void ImageEditorDlg::_drawTextBox( HDC hDC ) {
	if( ( m_uFlag & ENABLE_EDITOR_IMAGE ) != ENABLE_EDITOR_IMAGE )
		return;
	if(((m_uFlag & MOUSEMOVE_IN_BMP_BOTTOM) == MOUSEMOVE_IN_BMP_BOTTOM) ||
		((m_uFlag & MOUSEMOVE_IN_BMP_RIGHT) == MOUSEMOVE_IN_BMP_RIGHT) ||
		((m_uFlag & MOUSEMOVE_IN_BMP_BRCORNER) == MOUSEMOVE_IN_BMP_BRCORNER)) {
		return;
	}
	if( !m_bIsShow )
		return;
	HPEN hPen;
	HPEN hPenOld;

	if( m_bDrag ) {
		//鼠标按着时候且edit不可见的文本框
		//hPen = ::CreatePen( PS_DOT, 0, RGB( 95, 110, 107 ) );
		hPen = ::CreatePen( PS_SOLID, 0, RGB( 255, 0, 0 ) );
		hPenOld = (HPEN)::SelectObject( hDC, hPen );
		::MoveToEx( hDC, m_rcTxtBox.left - 1, m_rcTxtBox.top, NULL );
		::LineTo( hDC, m_rcTxtBox.right, m_rcTxtBox.top );
		::LineTo( hDC, m_rcTxtBox.right, m_rcTxtBox.bottom );
		::LineTo( hDC, m_rcTxtBox.left - 1, m_rcTxtBox.bottom );
		::LineTo( hDC, m_rcTxtBox.left - 1, m_rcTxtBox.top );
		::SelectObject( hDC, hPenOld );
		::DeleteObject( hPen );
	} else {
		//鼠标按着时候的文本框
		if( !::IsWindowVisible( m_hWndEdit ) )
			return;
		RECT rcTxtBox;
		::SetRect( &rcTxtBox, m_rcTxtBox.left - 2, m_rcTxtBox.top - 2, m_rcTxtBox.right + 2, m_rcTxtBox.bottom + 2 );
		//画白线
		hPen = ::CreatePen( PS_SOLID, 1, RGB( 255, 255, 255 ) );
		hPenOld = (HPEN)::SelectObject( hDC, hPen );
		::MoveToEx( hDC, rcTxtBox.left, rcTxtBox.top, NULL );
		::LineTo( hDC, rcTxtBox.right, rcTxtBox.top );
		::LineTo( hDC, rcTxtBox.right, rcTxtBox.bottom );
		::LineTo( hDC, rcTxtBox.left, rcTxtBox.bottom );
		::LineTo( hDC, rcTxtBox.left, rcTxtBox.top );
		::SelectObject( hDC, hPenOld );
		::SelectObject( hDC, hPenOld );
		::DeleteObject( hPen );
		//画蓝线
		hPen = ::CreatePen( PS_DOT, 0, RGB( 51, 153, 255 ) );
		hPenOld = (HPEN)::SelectObject( hDC, hPen );
		::MoveToEx( hDC, rcTxtBox.left, rcTxtBox.top, NULL );
		::LineTo( hDC, rcTxtBox.right, rcTxtBox.top );
		::LineTo( hDC, rcTxtBox.right, rcTxtBox.bottom );
		::LineTo( hDC, rcTxtBox.left, rcTxtBox.bottom );
		::LineTo( hDC, rcTxtBox.left, rcTxtBox.top );
		::SelectObject( hDC, hPenOld );
		::SelectObject( hDC, hPenOld );
		::DeleteObject( hPen );
		//画矩形框 8个
		unsigned int uOutSideSpace = 2;
		unsigned int uInsideSpace = 3;
		unsigned int uTBXPos = rcTxtBox.left + (rcTxtBox.right - rcTxtBox.left) / 2;//上下中间点X坐标
		unsigned int uLRYPos = rcTxtBox.top + (rcTxtBox.bottom - rcTxtBox.top) / 2;//左右中间点Y坐标
		RECT rcLTop, rcTop, rcRTop, rcLeft, rcLBottom, rcBottom, rcRBottom, rcRight;
		::SetRect( &rcLTop, rcTxtBox.left - uOutSideSpace, rcTxtBox.top - uOutSideSpace, rcTxtBox.left + uInsideSpace, rcTxtBox.top + uInsideSpace );
		::SetRect( &rcTop, uTBXPos - uInsideSpace, rcTxtBox.top - uOutSideSpace, uTBXPos + uOutSideSpace, rcTxtBox.top + uInsideSpace );
		::SetRect( &rcRTop, rcTxtBox.right - uInsideSpace, rcTxtBox.top - uOutSideSpace, rcTxtBox.right + uOutSideSpace, rcTxtBox.top + uInsideSpace );
		::SetRect( &rcLBottom, rcTxtBox.left - uOutSideSpace, rcTxtBox.bottom - uInsideSpace, rcTxtBox.left + uInsideSpace, rcTxtBox.bottom + uOutSideSpace );
		::SetRect( &rcBottom, uTBXPos - uInsideSpace, rcTxtBox.bottom - uOutSideSpace, uTBXPos + uOutSideSpace, rcTxtBox.bottom + uInsideSpace );
		::SetRect( &rcRBottom, rcTxtBox.right - uInsideSpace, rcTxtBox.bottom - uInsideSpace, rcTxtBox.right + uOutSideSpace, rcTxtBox.bottom + uOutSideSpace );
		::SetRect( &rcLeft, rcTxtBox.left - uOutSideSpace, uLRYPos - uOutSideSpace, rcTxtBox.left + uInsideSpace, uLRYPos + uInsideSpace );
		::SetRect( &rcRight, rcTxtBox.right - uInsideSpace, uLRYPos - uOutSideSpace, rcTxtBox.right + uOutSideSpace, uLRYPos + uInsideSpace );
		HBRUSH hBrush = ::CreateSolidBrush( RGB( 255, 255, 255 ) );
		hPen = ::CreatePen( PS_SOLID, 1, RGB( 85, 85, 85 ) );
		hPenOld = (HPEN)::SelectObject( hDC, hPen );
		::FillRect( hDC, &rcLTop, hBrush );
		::FillRect( hDC, &rcTop, hBrush );
		::FillRect( hDC, &rcRTop, hBrush );
		::FillRect( hDC, &rcLeft, hBrush );
		::FillRect( hDC, &rcLBottom, hBrush );
		::FillRect( hDC, &rcBottom, hBrush );
		::FillRect( hDC, &rcRBottom, hBrush );
		::FillRect( hDC, &rcRight, hBrush );
		::Rectangle( hDC, rcLTop.left, rcLTop.top, rcLTop.right, rcLTop.bottom );
		::Rectangle( hDC, rcTop.left, rcTop.top, rcTop.right, rcTop.bottom );
		::Rectangle( hDC, rcRTop.left, rcRTop.top, rcRTop.right, rcRTop.bottom );
		::Rectangle( hDC, rcLeft.left, rcLeft.top, rcLeft.right, rcLeft.bottom );
		::Rectangle( hDC, rcLBottom.left, rcLBottom.top, rcLBottom.right, rcLBottom.bottom );
		::Rectangle( hDC, rcBottom.left, rcBottom.top, rcBottom.right, rcBottom.bottom );
		::Rectangle( hDC, rcRBottom.left, rcRBottom.top, rcRBottom.right, rcRBottom.bottom );
		::Rectangle( hDC, rcRight.left, rcRight.top, rcRight.right, rcRight.bottom );
		::SelectObject( hDC, hPen );
		::DeleteObject( hPen );
		::DeleteObject( hBrush );
	}
}

void ImageEditorDlg::_showTextBox( bool bShow ) {
	if(((m_uFlag & MOUSEMOVE_IN_BMP_BOTTOM) == MOUSEMOVE_IN_BMP_BOTTOM) ||
		((m_uFlag & MOUSEMOVE_IN_BMP_RIGHT) == MOUSEMOVE_IN_BMP_RIGHT) ||
		((m_uFlag & MOUSEMOVE_IN_BMP_BRCORNER) == MOUSEMOVE_IN_BMP_BRCORNER)) {
		::ShowWindow( m_hWndEdit, SW_HIDE );
		return;
	}
	if(bShow) {
		::MoveWindow( m_hWndEdit, m_rcTxtBox.left, m_rcTxtBox.top, m_rcTxtBox.right - m_rcTxtBox.left, m_rcTxtBox.bottom - m_rcTxtBox.top, FALSE );
		_initEdit();
		::SetFocus( m_hWndEdit );
		::ShowWindow( m_hWndEdit, SW_NORMAL );
	} else {
		::SendMessage( m_hWndEdit, WM_KILLFOCUS, 0, 0 );
		_pushback();
		::ShowWindow( m_hWndEdit, SW_HIDE );
	}
}

void ImageEditorDlg::_initEdit() {
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
	::DeleteObject( hFont );
}

void ImageEditorDlg::_pushback() {
	if( m_bIsShow || m_bDrag )
		return;
	//
	TCHAR szMsg[MAX_PATH] = { 0 };
	EditFontInfo* pFontInfo = NULL;
	tstring_type tstrText;
	RECT rcEditInWnd;
	RECT rcEdit;
	RECT rcEditInCanvas;
	POINT ptMouse;

	::GetWindowText( m_hWndEdit, szMsg, MAX_PATH );
	tstrText = szMsg;
	if(tstrText.empty()) {
		return;
	}
	pFontInfo = new EditFontInfo();
	pFontInfo->m_tstrText = tstrText;
	pFontInfo->m_tstrFaceName = m_dlgProperty.getFontFaceName();
	pFontInfo->m_uSizeFont = m_dlgProperty.getFontSize();
	pFontInfo->m_clrFont = m_dlgProperty.getTextClr();
	//
	::GetWindowRect( m_hWndEdit, &rcEdit );
	ptMouse.x = rcEdit.left;
	ptMouse.y = rcEdit.top;
	int nWidth = rcEdit.right - rcEdit.left;
	int nHeight = rcEdit.bottom - rcEdit.top;
	::ScreenToClient( m_hWnd, &ptMouse );
	::SetRect( &rcEditInWnd, ptMouse.x, ptMouse.y, ptMouse.x + nWidth, ptMouse.y + nHeight );
	//
// 	rcEditInCanvas.left = rcEditInWnd.left - m_rcCanvas.left;
// 	rcEditInCanvas.top = rcEditInWnd.top - m_rcCanvas.top;
// 	rcEditInCanvas.bottom = rcEditInCanvas.top + nHeight;
// 	rcEditInCanvas.right = rcEditInCanvas.left + nWidth;
// 	//
	if(m_dlgProperty.isCBB1Change() ) {
		::OffsetRect( &rcEditInWnd, 2, 0 );
	}
   	    pFontInfo->m_rcText = rcEditInWnd;
// 	pFontInfo->m_nXPos = rcEditInWnd.left;
// 	pFontInfo->m_nYPos = rcEditInWnd.top;
// 	pFontInfo->m_pMemDC = new YBCtrlMemDC( nWidth, nHeight );
// 	HDC hDCEdit = ::GetDC( m_hWndEdit );
// 	::BitBlt( pFontInfo->m_pMemDC->getMemDC(), 0, 0, nWidth, nHeight, hDCEdit, 0, 0, SRCCOPY );
// 	::ReleaseDC( m_hWndEdit, hDCEdit );
		//
// 		int nLineCount = ::SendMessage( m_hWndEdit, EM_GETLINECOUNT, 0, 0 );
// 		TCHAR szBuffer[100];
// 		((WORD*)szBuffer)[0] = 100;
// 		for(unsigned int i = 0; i < nLineCount; ++i) {
// 			((WORD*)szBuffer)[0] = 100;
// 			::SendMessage( m_hWndEdit, EM_GETLINE, i, (LPARAM)szBuffer );
// 			tstrText = szBuffer;
// 		}
// 		int nLineCount = ::SendMessage( m_hWndEdit, EM_GETLINECOUNT, 0, 0 );
// 		TCHAR szBuffer[100];
// 	::SendMessage( m_hWndEdit, WM_GETTEXT, (WPARAM)tstrText.length(), (LPARAM)szBuffer );
// 	tstrText = szBuffer;
	m_containerFontInfo.push_back( pFontInfo );
	int nCount = m_containerFontInfo.size();
	//TCHAR szMsg[100] = {0};
	_stprintf_s( szMsg, _T( "%d个文字\r\n" ), nCount );
	::OutputDebugString( szMsg );
	//
//	_initEdit();
	//增加图层
	static unsigned int s_btnId = 0;
	s_btnId++;
	int nCountTC = m_containerTextTC.size();
	//TCHAR szMsg[100] = {0};
	_stprintf_s( szMsg, _T( "%d个图层按钮" ), nCountTC );
	::OutputDebugString( szMsg );
	::OutputDebugString( _T("AB:") );
	// 	::MessageBox( NULL, szMsg, NULL, MB_OK );
	RECT rcClient;
	::GetClientRect( m_hWnd, &rcClient );
	//表示一行能容纳的最多图层按钮.
	int nMaxBtnX = ((rcClient.right - rcClient.left - TC_LPOS - TC_RPOS) / (TC_BTNX_WSPACE + TC_BTNX_WIDTH));
	//表示第几行图层区.
	int nIndexTC = nCountTC / nMaxBtnX;
	TextTuCeng* pTextTuCent = new TextTuCeng();
	pTextTuCent->m_pYBButtonX = new YBButtonX();
	pTextTuCent->m_pYBButtonX->Create( NULL, WS_VISIBLE | WS_CHILD, TC_LPOS + nCountTC*(TC_BTNX_WIDTH + TC_BTNX_WSPACE),
		TC_TPOS + (TC_BTNX_HEIGHT + TC_BTNX_HSPACE)*nIndexTC,
		TC_BTNX_WIDTH, TC_BTNX_HEIGHT, m_hWnd, (HMENU)(1000 + s_btnId), NULL );
	pTextTuCent->m_uCtrlID = 1000 + s_btnId;
	pTextTuCent->m_pYBButtonX->enableBtnX( false );
	pTextTuCent->m_pYBButtonX->setFontId( m_dlgHeader.getFontId() );
	pTextTuCent->m_pYBButtonX->setText( tstrText );
	pTextTuCent->m_pYBButtonX->setImage( YBButtonX::EBTN_STATUS_NORMAL, m_uImageIdTCNor );
	pTextTuCent->m_pYBButtonX->setImage( YBButtonX::EBTN_STATUS_HOVER, m_uImageIdTCNor );
	pTextTuCent->m_pYBButtonX->setImage( YBButtonX::EBTN_STATUS_PRESS, m_uImageIdTCPre );
	pTextTuCent->m_pYBButtonX->setImage( YBButtonX::EBTNX_STATUS_NORMAL, m_uImageIdTCNor );
	pTextTuCent->m_pYBButtonX->setImage( YBButtonX::EBTNX_STATUS_HOVER, m_uImageIdTCNor );
	pTextTuCent->m_pYBButtonX->setImage( YBButtonX::EBTNX_STATUS_PRESS, m_uImageIdTCNor );
	pTextTuCent->m_tstrText = tstrText;
	m_containerTextTC.push_back( pTextTuCent );
	HDC hDCEdit = ::GetDC( m_hWndEdit );
	int i = GetTextCharacterExtra( hDCEdit );
	::ReleaseDC( m_hWndEdit, hDCEdit );
	
	hDCEdit = ::GetDC( m_hWnd );
	i = GetTextCharacterExtra( hDCEdit );
	::ReleaseDC( m_hWndEdit, hDCEdit );
}

void ImageEditorDlg::_loadImage() {
	tstring_type tstrDirPath;
	tstring_type tstrFilePath;
	tstrDirPath = m_tstrDirPath;
	tstrDirPath += _T( "BMP\\" );
	//
	m_uFlag |= IMAGE_EDITOR_TYPE;
	m_uFlag |= ENABLE_EDITOR_IMAGE;
	LOAD_IMAGE( _T( "YBCNType16.png" ), m_uImageId )
	LOAD_IMAGE( _T( "Normal.png" ), m_uImageIdTCNor )
	LOAD_IMAGE( _T( "Press.png" ), m_uImageIdTCPre )
	
	_initImageSpace();
}

void ImageEditorDlg::_initImageSpace() {
	if((m_uFlag & IMAGE_EDITOR_TYPE) != IMAGE_EDITOR_TYPE) 
		return;
	YBCtrlImageStatic* pYBImage = NULL;
	pYBImage = _getImageStatic( m_uImageId );
	if(!pYBImage) {
		assert( false );
		return;
	}
	unsigned int uCanvasWidth = m_rcCanvas.right - m_rcCanvas.left;
	unsigned int uCanvasHeight = m_rcCanvas.bottom - m_rcCanvas.top;
	unsigned int uImageWidth = pYBImage->getWidth();
	unsigned int uImageHeight = pYBImage->getHeight();
	unsigned int uWidth = min( uCanvasWidth, uImageWidth );
	unsigned int uHeight = min( uCanvasHeight, uImageHeight );
	::SetRect( &m_rcImage, m_rcCanvas.left, m_rcCanvas.top, m_rcCanvas.left + uWidth, m_rcCanvas.top + uHeight );
}

bool ImageEditorDlg::_saveImage( HBITMAP hBitmap, tstring_type tstrMimeType, tstring_type tstrFileName ) {
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

bool ImageEditorDlg::_getEncoderClsid( tstring_type tstrMimeType, CLSID* pClsid ) {
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