#include "stdafx.h"
#include "ImageEditorProperty.h"
#include "frame/YBCtrlFontDepository.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlMemDC.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlImageDepository.h"
#include "ImageEditorEnvCfg.h"
#include "frame/YBCtrlMisc.h"

ImageEditorProperty::ImageEditorProperty()
: m_uImageIdBk( 0 )
, m_uImageIdIcon( 0 )
, m_uFontId( 0 )
, m_uYLinePos( 0 )
, m_uSpace( 0 )
, m_colorRef1( RGB( 255, 0, 0 ) )
, m_colorRef2( RGB( 0, 0, 0 ) )
, m_uIndex( 0 )
, m_bSave( false )
, m_bCBB1Change( false )
, m_uCurrIndex( 0 )
{
	::memset( &m_rcEdit, 0, sizeof(RECT) );
	m_hPenEdit = ::CreatePen( PS_SOLID, 2, RGB(224,224,224) );
	m_hPenLine = ::CreatePen( PS_SOLID, 1, RGB(224,224,224) );
	m_hBrush = ::CreateSolidBrush( RGB(224,224,224) );
	_loadFont();
}

ImageEditorProperty::~ImageEditorProperty() {
	if(m_hBrush){
		::DeleteObject( m_hBrush );
		m_hBrush = NULL;
	}
	if(m_hPenEdit) {
		::DeleteObject( m_hPenEdit );
		m_hPenEdit = NULL;
	}
	if(m_hPenLine) {
		::DeleteObject( m_hPenLine );
		m_hPenLine = NULL;
	}
}

YBCTRL_WNDMSGMAP_BEGIN( ImageEditorProperty, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_CTLCOLOREDIT, _onWM_CTLCOLOREDIT )
YBCTRL_WNDMSGMAP_END()

HWND ImageEditorProperty::create( int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance ) {
	HWND hWnd;

	hWnd = _createHWND( NULL, WS_CHILD|WS_VISIBLE, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, NULL );
	if(hWnd) {
		::SetRect( &m_rcEdit, 10, SPACE_EDITOR_CONTROLBAR_HEIGHT + 8, nWidth - 10, SPACE_EDITOR_CONTROLBAR_HEIGHT + 88 );
		m_hWndEdit = ::CreateWindowEx( NULL, _T("Edit"), _T(""), WS_VISIBLE|WS_CHILD|ES_MULTILINE|ES_WANTRETURN,
			m_rcEdit.left, m_rcEdit.top, m_rcEdit.right - m_rcEdit.left, m_rcEdit.bottom - m_rcEdit.top, hWnd, (HMENU)EDIT_PROPERTY_ID, hInstance, NULL );
		m_uYLinePos = m_rcEdit.bottom + 11;
		m_uSpace = (nHeight - m_uYLinePos) / 11;

		m_rbtnHorArrangement.Create( 0, WS_VISIBLE | WS_CHILD, 10, m_uYLinePos + 1, 70, m_uSpace - 2, hWnd, (HMENU)RADIO_BTN_HOR_ARRANGEMENT, hInstance );
		m_rbtnVerArrangement.Create( 0, WS_VISIBLE | WS_CHILD, nWidth/2 + 10, m_uYLinePos + 1, 70, m_uSpace - 2, hWnd, (HMENU)RADIO_BTN_VER_ARRAGGEMENT, hInstance );
		m_rbtnMultilineOutput.Create( 0, WS_VISIBLE | WS_CHILD, 10, m_uYLinePos + m_uSpace + 1, 70, m_uSpace -2, hWnd, (HMENU)RADIO_BTN_MULTTLE_LINE_OUTPUT, hInstance );
		m_rbtnSingleOutput.Create( 0, WS_VISIBLE | WS_CHILD, nWidth/2 + 10, m_uYLinePos + m_uSpace + 1, 70, m_uSpace - 2, hWnd, (HMENU)RADIO_BTN_SINGLE_OUTPUT, hInstance );
		//
		_init();
		m_hWndCBBox1 = ::CreateWindow( _T( "Combobox" ), _T( "" ), WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL, m_rcColor1.left, m_uYLinePos+m_uSpace*2+1, 70, m_uSpace + 50, hWnd, (HMENU)COMBOBOX1_ID, NULL, NULL );
		m_hWndCBBox2 = ::CreateWindow( _T( "Combobox" ), _T( "" ), WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL, m_rcColor1.left, m_uYLinePos+m_uSpace*5+1, 70, m_uSpace + 50, hWnd, (HMENU)COMBOBOX2_ID, NULL, NULL );
		m_hWndCBBox3 = ::CreateWindow( _T( "Combobox" ), _T( "" ), WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL, m_rcColor1.left, m_uYLinePos+m_uSpace*7+1, 70, m_uSpace + 50, hWnd, (HMENU)COMBOBOX3_ID, NULL, NULL );
		m_hWndCBBox4= ::CreateWindow( _T( "Combobox" ), _T( "" ), WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL, m_rcColor1.left, m_uYLinePos+m_uSpace*9+1, 70, m_uSpace + 50, hWnd, (HMENU)COMBOBOX4_ID, NULL, NULL );
		m_hWndEditXPos = ::CreateWindow( _T( "Edit" ), _T( "" ), WS_VISIBLE | WS_CHILD | ES_WANTRETURN|ES_NUMBER,
			m_rcColor1.left + 1, m_uYLinePos+m_uSpace*3+4, 65, m_uSpace - 8, hWnd, (HMENU)EDIT_XPOS_ID, hInstance, NULL );
		m_hWndEditYPos = ::CreateWindow( _T( "Edit" ), _T( "" ), WS_VISIBLE | WS_CHILD | ES_WANTRETURN | ES_NUMBER,
			m_rcColor1.left + 1, m_uYLinePos + m_uSpace * 4 + 4, 65, m_uSpace - 8, hWnd, (HMENU)EDIT_YPOS_ID, hInstance, NULL );
		_reinit();
	}
	m_wordCtrlId = (WORD)hMenu;

	return hWnd;
}

void ImageEditorProperty::_onWM_CTLCOLOREDIT( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_CTLCOLOREDIT );
	HWND hEditCtrl = (HWND)pYBCtrlWndMsg->m_lParam;
	HDC hEditDC = (HDC)pYBCtrlWndMsg->m_wParam;

	if(hEditCtrl == m_hWndEdit) {
		::SetBkMode( hEditDC, TRANSPARENT );
		::SetTextColor( hEditDC, RGB(255,0,0) );
	}
}

void ImageEditorProperty::_reinit() {
	const YBCtrlFont* pYBFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontId );
	if(!pYBFont) {
		assert( false );
	}
	//edit
	::SendMessage( m_hWndEdit, WM_SETFONT, (WPARAM)(pYBFont->m_hFont), NULL );
	::SendMessage( m_hWndEditXPos, WM_SETFONT, (WPARAM)(pYBFont->m_hFont), NULL );
	::SendMessage( m_hWndEditYPos, WM_SETFONT, (WPARAM)(pYBFont->m_hFont), NULL );
	::SetWindowText( m_hWndEditXPos, _T( "10" ) );
	::SetWindowText( m_hWndEditYPos, _T( "10" ) );
	::SetWindowText( m_hWndEdit, _T( "" ) );
	//Combobox
	::SendMessage( m_hWndCBBox1, WM_SETFONT, (WPARAM)(pYBFont->m_hFont), NULL );
	::SendMessage( m_hWndCBBox2, WM_SETFONT, (WPARAM)(pYBFont->m_hFont), NULL );
	::SendMessage( m_hWndCBBox3, WM_SETFONT, (WPARAM)(pYBFont->m_hFont), NULL );
	::SendMessage( m_hWndCBBox4, WM_SETFONT, (WPARAM)(pYBFont->m_hFont), NULL );
	tstring_type tstrText = _T( "宋体" );
	::SendMessage( m_hWndCBBox1, CB_SETITEMHEIGHT, (WPARAM)-1, (LPARAM)20 );
	::SendMessage( m_hWndCBBox2, CB_SETITEMHEIGHT, (WPARAM)-1, (LPARAM)20 );
	::SendMessage( m_hWndCBBox3, CB_SETITEMHEIGHT, (WPARAM)-1, (LPARAM)20 );
	::SendMessage( m_hWndCBBox4, CB_SETITEMHEIGHT, (WPARAM)-1, (LPARAM)20 );
	::SendMessage( m_hWndCBBox1, CB_ADDSTRING, 0, (LPARAM)(_T( "宋体" )) );
	::SendMessage( m_hWndCBBox1, CB_ADDSTRING, 0, (LPARAM)(_T( "微软雅黑" )) );
	::SendMessage( m_hWndCBBox2, CB_ADDSTRING, 0, (LPARAM)(_T( "20" )) );
	::SendMessage( m_hWndCBBox2, CB_ADDSTRING, 0, (LPARAM)(_T( "21" )) );
	::SendMessage( m_hWndCBBox3, CB_ADDSTRING, 0, (LPARAM)(_T( "0" )) );
	::SendMessage( m_hWndCBBox3, CB_ADDSTRING, 0, (LPARAM)(_T( "1" )) );
	::SendMessage( m_hWndCBBox4, CB_ADDSTRING, 0, (LPARAM)(_T( "是" )) );
	::SendMessage( m_hWndCBBox4, CB_ADDSTRING, 0, (LPARAM)(_T( "否" )) );
	::SendMessage( m_hWndCBBox1, CB_SETCURSEL, 0, 0 );
	::SendMessage( m_hWndCBBox2, CB_SETCURSEL, 0, 0 );
	::SendMessage( m_hWndCBBox3, CB_SETCURSEL, 0, 0 );
	::SendMessage( m_hWndCBBox4, CB_SETCURSEL, 0, 0 );
//
	::InvalidateRect( m_rbtnVerArrangement.getHWND(), NULL, FALSE );
	::InvalidateRect( m_rbtnHorArrangement.getHWND(), NULL, FALSE );
	::InvalidateRect( m_rbtnMultilineOutput.getHWND(), NULL, FALSE );
	::InvalidateRect( m_rbtnSingleOutput.getHWND(), NULL, FALSE );
	::InvalidateRect( m_hWndCBBox1, NULL, FALSE );
	::InvalidateRect( m_hWndCBBox2, NULL, FALSE );
	::InvalidateRect( m_hWndCBBox3, NULL, FALSE );
	::InvalidateRect( m_hWndCBBox4, NULL, FALSE );
	::InvalidateRect( m_hWndEdit, NULL, FALSE );
	::InvalidateRect( m_hWndEditXPos, NULL, FALSE );
	::InvalidateRect( m_hWndEditYPos, NULL, FALSE );
}

void ImageEditorProperty::_init() {
	m_rbtnHorArrangement.setbeSelect( true );
	m_rbtnHorArrangement.setText( _T( "水平排列" ) );
	m_rbtnHorArrangement.setRadioBtnLSpace( 6 );
	m_rbtnHorArrangement.setFontId( m_uFontId );
	m_rbtnHorArrangement.setRadioBtnLSpace( 4 );
	m_rbtnHorArrangement.setImageIdBk( m_uImageIdWEDRBBk );
	m_rbtnHorArrangement.setImage( YBRadioBtn::ENORADIOBTN_STATUS_DISABLE, m_uImageIdWEDNRBDisable );
	m_rbtnHorArrangement.setImage( YBRadioBtn::ERADIOBTN_STATUS_DISABLE, m_uImageIdWEDRBDisable );
	m_rbtnHorArrangement.setImage( YBRadioBtn::ENORADIOBTN_STATUS_NORMAL, m_uImageIdWEDNRBNor );
	m_rbtnHorArrangement.setImage( YBRadioBtn::ENORADIOBTN_STATUS_HOVER, m_uImageIdWEDNRBHor );
	m_rbtnHorArrangement.setImage( YBRadioBtn::ENORADIOBTN_STATUS_PRESS, m_uImageIdWEDNRBPre );
	m_rbtnHorArrangement.setImage( YBRadioBtn::ERADIOBTN_STATUS_NORMAL, m_uImageIdWEDRBNor );
	m_rbtnHorArrangement.setImage( YBRadioBtn::ERADIOBTN_STATUS_HOVER, m_uImageIdWEDRBHor );
	m_rbtnHorArrangement.setImage( YBRadioBtn::ERADIOBTN_STATUS_PRESS, m_uImageIdWEDRBPre );
	//
	m_rbtnVerArrangement.setText( _T( "竖直排列" ) );
	m_rbtnVerArrangement.setFontId( m_uFontId );
	m_rbtnVerArrangement.setRadioBtnLSpace( 6 );
	m_rbtnVerArrangement.setImageIdBk( m_uImageIdWEDRBBk );
	m_rbtnVerArrangement.setImage( YBRadioBtn::ENORADIOBTN_STATUS_DISABLE, m_uImageIdWEDNRBDisable );
	m_rbtnVerArrangement.setImage( YBRadioBtn::ERADIOBTN_STATUS_DISABLE, m_uImageIdWEDRBDisable );
	m_rbtnVerArrangement.setImage( YBRadioBtn::ENORADIOBTN_STATUS_NORMAL, m_uImageIdWEDNRBNor );
	m_rbtnVerArrangement.setImage( YBRadioBtn::ENORADIOBTN_STATUS_HOVER, m_uImageIdWEDNRBHor );
	m_rbtnVerArrangement.setImage( YBRadioBtn::ENORADIOBTN_STATUS_PRESS, m_uImageIdWEDNRBPre );
	m_rbtnVerArrangement.setImage( YBRadioBtn::ERADIOBTN_STATUS_NORMAL, m_uImageIdWEDRBNor );
	m_rbtnVerArrangement.setImage( YBRadioBtn::ERADIOBTN_STATUS_HOVER, m_uImageIdWEDRBHor );
	m_rbtnVerArrangement.setImage( YBRadioBtn::ERADIOBTN_STATUS_PRESS, m_uImageIdWEDRBPre );
	//
	m_rbtnSingleOutput.setbeSelect( true );
	m_rbtnSingleOutput.setText( _T( "单行输出" ) );
	m_rbtnSingleOutput.setFontId( m_uFontId );
	m_rbtnSingleOutput.setRadioBtnLSpace( 4 );
	m_rbtnSingleOutput.setImageIdBk( m_uImageIdWEDRBBk );
	m_rbtnSingleOutput.setImage( YBRadioBtn::ENORADIOBTN_STATUS_DISABLE, m_uImageIdWEDNRBDisable );
	m_rbtnSingleOutput.setImage( YBRadioBtn::ERADIOBTN_STATUS_DISABLE, m_uImageIdWEDRBDisable );
	m_rbtnSingleOutput.setImage( YBRadioBtn::ENORADIOBTN_STATUS_NORMAL, m_uImageIdWEDNRBNor );
	m_rbtnSingleOutput.setImage( YBRadioBtn::ENORADIOBTN_STATUS_HOVER, m_uImageIdWEDNRBHor );
	m_rbtnSingleOutput.setImage( YBRadioBtn::ENORADIOBTN_STATUS_PRESS, m_uImageIdWEDNRBPre );
	m_rbtnSingleOutput.setImage( YBRadioBtn::ERADIOBTN_STATUS_NORMAL, m_uImageIdWEDRBNor );
	m_rbtnSingleOutput.setImage( YBRadioBtn::ERADIOBTN_STATUS_HOVER, m_uImageIdWEDRBHor );
	m_rbtnSingleOutput.setImage( YBRadioBtn::ERADIOBTN_STATUS_PRESS, m_uImageIdWEDRBPre );
	//
	m_rbtnMultilineOutput.setText( _T( "多行输出" ) );
	m_rbtnMultilineOutput.setFontId( m_uFontId );
	m_rbtnMultilineOutput.setRadioBtnLSpace( 6 );
	m_rbtnMultilineOutput.setImageIdBk( m_uImageIdWEDRBBk );
	m_rbtnMultilineOutput.setImage( YBRadioBtn::ENORADIOBTN_STATUS_DISABLE, m_uImageIdWEDNRBDisable );
	m_rbtnMultilineOutput.setImage( YBRadioBtn::ERADIOBTN_STATUS_DISABLE, m_uImageIdWEDRBDisable );
	m_rbtnMultilineOutput.setImage( YBRadioBtn::ENORADIOBTN_STATUS_NORMAL, m_uImageIdWEDNRBNor );
	m_rbtnMultilineOutput.setImage( YBRadioBtn::ENORADIOBTN_STATUS_HOVER, m_uImageIdWEDNRBHor );
	m_rbtnMultilineOutput.setImage( YBRadioBtn::ENORADIOBTN_STATUS_PRESS, m_uImageIdWEDNRBPre );
	m_rbtnMultilineOutput.setImage( YBRadioBtn::ERADIOBTN_STATUS_NORMAL, m_uImageIdWEDRBNor );
	m_rbtnMultilineOutput.setImage( YBRadioBtn::ERADIOBTN_STATUS_HOVER, m_uImageIdWEDRBHor );
	m_rbtnMultilineOutput.setImage( YBRadioBtn::ERADIOBTN_STATUS_PRESS, m_uImageIdWEDRBPre );
	//
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	::SetRect( &m_rcColor1, rcClient.right / 2 + 10, m_uYLinePos + m_uSpace * 6 + 5, rcClient.right / 2 + 10 + 35, m_uYLinePos + m_uSpace * 7 - 5 );
	::SetRect( &m_rcColor2, rcClient.right / 2 + 10, m_uYLinePos + m_uSpace * 8 + 5, rcClient.right / 2 + 10 + 35, m_uYLinePos + m_uSpace * 9 - 5 );
}

void ImageEditorProperty::_onWM_LBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
	POINT ptMouse;
	RECT rcClient;

	ptMouse.x = LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = HIWORD( pYBCtrlWndMsg->m_lParam );

	if(::PtInRect( &m_rcColor1, ptMouse )) {
		CHOOSECOLOR cc;
		static COLORREF acrCustClr[16];
		static DWORD rgbCurrent;
		
		ZeroMemory( &cc, sizeof(cc) );
		cc.lStructSize = sizeof(cc);
		cc.lpCustColors = (LPDWORD)acrCustClr;
		cc.rgbResult = rgbCurrent;
		cc.Flags = CC_FULLOPEN | CC_RGBINIT;
		if(TRUE == ChooseColor( &cc )) {
			m_colorRef1 = cc.rgbResult;
		}
	} else if(::PtInRect( &m_rcColor2, ptMouse )) {
		CHOOSECOLOR cc2;
		static COLORREF acrCustClr2[16];
		static DWORD rgbCurrent2;

		ZeroMemory( &cc2, sizeof(cc2) );
		cc2.lStructSize = sizeof(cc2);
		cc2.lpCustColors = (LPDWORD)acrCustClr2;
		cc2.rgbResult = rgbCurrent2;
		cc2.Flags = CC_FULLOPEN | CC_RGBINIT;
		if(TRUE == ChooseColor( &cc2 )) {
			m_colorRef2 = cc2.rgbResult;
		}
	}
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	HDC hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
	::InvalidateRect( m_rbtnVerArrangement.getHWND(), NULL, FALSE );
	::InvalidateRect( m_rbtnHorArrangement.getHWND(), NULL, FALSE );
	::InvalidateRect( m_rbtnMultilineOutput.getHWND(), NULL, FALSE );
	::InvalidateRect( m_rbtnSingleOutput.getHWND(), NULL, FALSE );
	::InvalidateRect( m_hWndCBBox1, NULL, FALSE );
	::InvalidateRect( m_hWndCBBox2, NULL, FALSE );
	::InvalidateRect( m_hWndCBBox3, NULL, FALSE );
	::InvalidateRect( m_hWndCBBox4, NULL, FALSE );
	::InvalidateRect( m_hWndEdit, NULL, FALSE );
	::InvalidateRect( m_hWndEditXPos, NULL, FALSE );
	::InvalidateRect( m_hWndEditYPos, NULL, FALSE );
}

void ImageEditorProperty::_onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg ) {
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

void ImageEditorProperty::_onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	PAINTSTRUCT ps;
	RECT rcClient;
	HDC hPaintDC;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void ImageEditorProperty::_onWM_COMMAND( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );

	switch(wCtrlId) {
	case RADIO_BTN_HOR_ARRANGEMENT:{
									   if(m_rbtnVerArrangement.isSelect()) {
										   m_rbtnVerArrangement.setbeSelect( false );
									   }
	}
		break;
	case RADIO_BTN_MULTTLE_LINE_OUTPUT:{
										   if(m_rbtnSingleOutput.isSelect()) {
											   m_rbtnSingleOutput.setbeSelect( false );
										   }
	}
		break;
	case RADIO_BTN_SINGLE_OUTPUT:{
									 if(m_rbtnMultilineOutput.isSelect()) {
										 m_rbtnMultilineOutput.setbeSelect( false );
									 }
	}
		break;
	case RADIO_BTN_VER_ARRAGGEMENT: {
										if(m_rbtnHorArrangement.isSelect()) {
											m_rbtnHorArrangement.setbeSelect( false );
										}
		}
		break;
	case BTN_DELETE_ID:{

	}
		break;
	case BTN_NEXT_ID:{
						 EditorTxtProperty* pEditorTxtProperty = NULL;
						 pEditorTxtProperty = getNextPropertyDlg( m_uCurrIndex );
						// if( !pEditorTxtProperty )
	}
		break;
	case BTN_PRE_ID:{

	}
		break;
	case BTN_SAVE_ID:{
						 _save();
						 ::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, MAKELONG( m_wordCtrlId, BTN_CMD_SAVE ), (LPARAM)m_hWnd );
	}
		break;
	case COMBOBOX1_ID:{
						  if(wEvtCode == CBN_SELCHANGE) {
							  TCHAR szMsg[100] = {0};
							  ::GetWindowText( m_hWndCBBox1, szMsg, 100 );
							  tstring_type tstrText = szMsg;
							  if(_tcscmp( tstrText.c_str(), _T( "宋体" ) ) !=0 ) {
								  m_bCBB1Change = true;
							  }
						  }
	}
		break;
	case BTN_ADD_ID:{
						if(!m_bSave) {
							if(IDOK != ::MessageBox( NULL, _T( "此文字未保存，是否继续？" ), _T( "" ), MB_YESNO | MB_TOPMOST )) {
								break;
							}
							_reinit();
							break;
						}
						TCHAR szMsg[ MAX_PATH ];
						::GetWindowText( m_hWndEdit, szMsg, MAX_PATH );
						tstring_type tstrText = szMsg;
						if(tstrText.empty()) {
							if(IDOK != ::MessageBox( NULL, _T( "此文字未编辑完，是否继续？" ), _T( "" ), MB_YESNO | MB_TOPMOST )) {
								break;
							}
							_reinit();
							break;
						}
						_reinit();
						m_bSave = false;
						++m_uIndex;
						m_uCurrIndex = m_uIndex;
						::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, MAKELONG( m_wordCtrlId, BTN_CMD_ADD ), (LPARAM)m_hWnd );
	}
		break;
	default:
		break;
	}
}

void ImageEditorProperty::showPropertyDlg() {
	EditorTxtProperty* pEditorTxtProperty = NULL;

	pEditorTxtProperty = getPropertyDlgByIndex( m_uCurrIndex );
	if(!pEditorTxtProperty) {
		return;
	}
	::SetWindowText( m_hWndEdit, (pEditorTxtProperty->m_tstrText).c_str() );
	tstring_type tstrXPos;
	tstring_type tstrYPos;
	//_itot( pEditorTxtProperty->m_uXPos, (TCHAR *)tstrXPos.c_str(), 10 );
//	_itot( pEditorTxtProperty->m_uYPos, (TCHAR *)tstrYPos.c_str(), 10 );
	::SetWindowText( m_hWndEditXPos, tstrXPos.c_str() );
	::SetWindowText( m_hWndEditYPos, tstrYPos.c_str() );
	//::SetWindowText(  )
}

EditorTxtProperty* ImageEditorProperty::getPrePropertyDlg( unsigned int uIndex ) {
	if(m_containerTxtProperty.size() == 0) {
		assert( false );
		return NULL;
	}
	EditorTxtProperty* pEditorTxtProperty = NULL;
	containerTxtProperty::iterator itBegin, itEnd;
	itEnd = m_containerTxtProperty.end();
	pEditorTxtProperty = m_containerTxtProperty[0];
	if(pEditorTxtProperty->m_uIndex == uIndex) {
		return NULL;
	}
	pEditorTxtProperty = NULL;
	for( itBegin = m_containerTxtProperty.begin(); itBegin != itEnd; ++itBegin) {
		pEditorTxtProperty = (*itBegin);
		if(pEditorTxtProperty) {
			if(pEditorTxtProperty->m_uIndex == uIndex) {
				return *(--itBegin);
			}
		}
	}
	return NULL;
}

EditorTxtProperty* ImageEditorProperty::getNextPropertyDlg( unsigned int uIndex ) {
	if(m_containerTxtProperty.size() == 0) {
		assert( false );
		return NULL;
	}
	EditorTxtProperty* pEditorTxtProperty = NULL;
	containerTxtProperty::iterator itBegin, itEnd;
	itEnd = m_containerTxtProperty.end();
	pEditorTxtProperty = *itEnd;
	if(pEditorTxtProperty->m_uIndex == uIndex) {
		return NULL;
	}
	pEditorTxtProperty = NULL;
	for(itBegin = m_containerTxtProperty.begin(); itBegin != itEnd; ++itBegin) {
		pEditorTxtProperty = (*itBegin);
		if(pEditorTxtProperty) {
			if(pEditorTxtProperty->m_uIndex == uIndex) {
				return *(++itBegin);
			}
		}
	}
	return NULL;
}

EditorTxtProperty* ImageEditorProperty::getPropertyDlgByIndex( unsigned int uIndex ) {
	if(m_containerTxtProperty.size() == 0) {
		assert( false );
		return NULL;
	}
	EditorTxtProperty* pTxtProperty = NULL;
	containerTxtProperty::iterator itBegin, itEnd;
	itEnd = m_containerTxtProperty.end();
	for(itBegin = m_containerTxtProperty.begin(); itBegin != itEnd; ++itBegin) {
		pTxtProperty = (*itBegin);
		if(!pTxtProperty) {
			assert( false );
			return NULL;
		}
		if(pTxtProperty->m_uIndex == uIndex) {
			return pTxtProperty;
		}
	}
	return NULL;
}

tstring_type ImageEditorProperty::getText() {
	tstring_type tstrText;
	TCHAR szMsg[MAX_PATH];

	::GetWindowText( m_hWndEdit, szMsg, MAX_PATH );
	tstrText = szMsg;
	return tstrText;
}
tstring_type ImageEditorProperty::getFontFaceName() {
	tstring_type tstrText;
	TCHAR szMsg[MAX_PATH];

	::GetWindowText( m_hWndCBBox1, szMsg, MAX_PATH );
	tstrText = szMsg;
	return tstrText;
}

unsigned int ImageEditorProperty::getFontSize() {
	tstring_type tstrText;
	unsigned int uFontSize;
	TCHAR szMsg[MAX_PATH];

	::GetWindowText( m_hWndCBBox2, szMsg, MAX_PATH );
	tstrText = szMsg;
	uFontSize = _ttoi( tstrText.c_str() );
	return uFontSize;
}

COLORREF ImageEditorProperty::getTextClr() {
	return m_colorRef1;
}

void ImageEditorProperty::_save() {
	TCHAR szMsg[MAX_PATH];
	TCHAR szMsgXPos[MAX_PATH];
	TCHAR szMsgYPos[MAX_PATH];
	TCHAR szMsgFaceName[MAX_PATH];
	TCHAR szMsgSizeFont[MAX_PATH];
	TCHAR szMsgStroke[MAX_PATH];
	TCHAR szMsgTransparent[MAX_PATH];
	tstring_type tstrTxt;
	tstring_type tstrXPos;
	tstring_type tstrYPos;
	tstring_type tstrFaceName;
	tstring_type tstrSizeFont;
	tstring_type tstrStroke;
	tstring_type tstrTransparent;
	::GetWindowText( m_hWndEdit, szMsg, MAX_PATH );
	tstrTxt = szMsg;
	if(tstrTxt.empty()) {
		::MessageBox( NULL, _T( "输入文字不能为空!" ), _T( "图片文字编辑" ), MB_OK|MB_TOPMOST );
		return;
	}
	//
	::GetWindowText( m_hWndEditXPos, szMsgXPos, MAX_PATH );
	tstrXPos = szMsgXPos;
	if(tstrXPos.empty()) {
		::MessageBox( NULL, _T( "起始坐标x不能为空!" ), _T( "图片文字编辑" ), MB_OK|MB_TOPMOST );
		return;
	}
	//
	::GetWindowText( m_hWndEditYPos, szMsgYPos, MAX_PATH );
	tstrYPos = szMsgYPos;
	if(tstrYPos.empty()) {
		::MessageBox( NULL, _T( "起始坐标y不能为空!" ), _T( "图片文字编辑" ), MB_OK|MB_TOPMOST );
		return;
	}
	//
	::GetWindowText( m_hWndCBBox1, szMsgFaceName, MAX_PATH );
	tstrFaceName = szMsgFaceName;
	::GetWindowText( m_hWndCBBox2, szMsgSizeFont, MAX_PATH );
	tstrSizeFont = szMsgSizeFont;
	::GetWindowText( m_hWndCBBox3, szMsgStroke, MAX_PATH );
	tstrStroke = szMsgStroke;
	::GetWindowText( m_hWndCBBox4, szMsgTransparent, MAX_PATH );
	tstrTransparent = szMsgTransparent;
	//
	EditorTxtProperty* pEditorTxtProperty = new EditorTxtProperty();
	pEditorTxtProperty->m_tstrText = tstrTxt;
	pEditorTxtProperty->m_bVerArrangeMent = m_rbtnVerArrangement.isSelect();
	pEditorTxtProperty->m_tstrFaceName = tstrFaceName;
	pEditorTxtProperty->m_uXPos = _ttoi( tstrXPos.c_str() );
	pEditorTxtProperty->m_uYPos = _ttoi( tstrYPos.c_str() );
	pEditorTxtProperty->m_uSizeFont = _ttoi( tstrSizeFont.c_str() );
	pEditorTxtProperty->m_clrFont = m_colorRef1;
	pEditorTxtProperty->m_uSizeStroke = _ttoi( tstrStroke.c_str() );
	pEditorTxtProperty->m_clrStroke = m_colorRef2;
	pEditorTxtProperty->m_bTransparent = (_tcscmp( tstrTransparent.c_str(), _T( "是" ) ) == 0) ? true : false;
	//pEditorTxtProperty->m_nAngleRotat;旋转角度
	pEditorTxtProperty->m_uIndex = m_uIndex;
	m_containerTxtProperty.push_back( pEditorTxtProperty );
	//
	m_bSave = true;
}

void ImageEditorProperty::_clearAll() {

}

ybctrl::YBCtrlImageStatic* ImageEditorProperty::_getImageStatic( unsigned int uImageIdBk ) {
	YBCtrlImageBase* pYBImage = NULL;

	if(!YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageIdBk, &pYBImage )) {
		assert( false );
		return NULL;
	}
	if(!pYBImage || pYBImage->getType() != YBCTRL_IMAGE_TYPE_STATIC) {
		assert( false );
		return NULL;
	}
	return (YBCtrlImageStatic*)pYBImage;
}

void ImageEditorProperty::setPath( const tstring_type& tstrDirPath ) {
	m_tstrDirPath = tstrDirPath;
	_loadImage();
}

void ImageEditorProperty::_loadImage() {
	tstring_type tstrDirPath;
	tstring_type tstrFilePath;

	tstrDirPath = m_tstrDirPath;
	tstrDirPath += _T( "BMP\\" );
	//
	LOAD_IMAGE( _T( "editorPropertyDlgBk.png" ), m_uImageIdBk )
	LOAD_IMAGE( _T( "editorPropertyDlgHead.png" ), m_uImageIdIcon )
	// Radio btn
	LOAD_IMAGE( _T( "YBWEDRBNor.png" ), m_uImageIdWEDRBNor )
	LOAD_IMAGE( _T( "YBWEDRBHov.png" ), m_uImageIdWEDRBHor )
	LOAD_IMAGE( _T( "YBWEDRBPre.png" ), m_uImageIdWEDRBPre )
	LOAD_IMAGE( _T( "YBWEDRBDis.png" ), m_uImageIdWEDRBDisable )
	LOAD_IMAGE( _T( "YBWEDNRBDis.png" ), m_uImageIdWEDNRBDisable )
	LOAD_IMAGE( _T( "YBWEDNRBNor.png" ), m_uImageIdWEDNRBNor )
	LOAD_IMAGE( _T( "YBWEDNRBHov.png" ), m_uImageIdWEDNRBHor )
	LOAD_IMAGE( _T( "YBWEDNRBPre.png" ), m_uImageIdWEDNRBPre )
	LOAD_IMAGE( _T( "YBWEDRBBk.png" ), m_uImageIdWEDRBBk )
}

void ImageEditorProperty::_loadFont() {
	LOGFONT lFont;

	::memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T( "微软雅黑" ) );
	lFont.lfHeight = 19;
	m_uFontId = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 0, 0, 0 ) );
}

void ImageEditorProperty::_drawStatus( HDC hDC ) {
	_drawBk( hDC );
	_drawHeadIcon( hDC );
	_drawLine( hDC );
	_drawText( hDC );
	_drawRGBBk( hDC );
}

void ImageEditorProperty::_drawRGBBk( HDC hDC ) {
	HBRUSH hBrushBk1;
	HBRUSH hBrushBk2;

	hBrushBk1 = ::CreateSolidBrush( m_colorRef1 );
	hBrushBk2 = ::CreateSolidBrush( m_colorRef2 );
	::FillRect( hDC, &m_rcColor1, hBrushBk1 );
	::FillRect( hDC, &m_rcColor2, hBrushBk2 );
	::DeleteObject( hBrushBk2 );
	::DeleteObject( hBrushBk1 );
}

void ImageEditorProperty::_drawText( HDC hDC ) {
	RECT rcTxt1;
	RECT rcTxt2;
	RECT rcTxt3;
	RECT rcTxt4;
	RECT rcTxt5;
	RECT rcTxt6;
	RECT rcTxt7;
	RECT rcTxt8;
	RECT rcTxt9;
	RECT rcClient;
	int iBkModeOld;
	COLORREF clrTxtOld;
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	tstring_type tstrTxt1 = _T( "本地字体" );
	tstring_type tstrTxt2 = _T( "起始x坐标" );
	tstring_type tstrTxt3 = _T( "起始y坐标" );
	tstring_type tstrTxt4 = _T( "文字尺寸" );
	tstring_type tstrTxt5 = _T( "颜色" );
	tstring_type tstrTxt6 = _T( "文字描边" );
	tstring_type tstrTxt7 = _T( "颜色" );
	tstring_type tstrTxt8 = _T( "是否透明" );
	tstring_type tstrTxt9 = _T( "旋转角度（°）" );
	//
	::GetClientRect( m_hWnd, &rcClient );
	::SetRect( &rcTxt1, rcClient.left + 8, m_uYLinePos + m_uSpace*2, rcClient.right, m_uYLinePos + m_uSpace*3 );
	::SetRect( &rcTxt2, rcClient.left + 8, m_uYLinePos + m_uSpace*3, rcClient.right, m_uYLinePos + m_uSpace*4 );
	::SetRect( &rcTxt3, rcClient.left + 8, m_uYLinePos + m_uSpace*4, rcClient.right, m_uYLinePos + m_uSpace*5 );
	::SetRect( &rcTxt4, rcClient.left + 8, m_uYLinePos+ m_uSpace*5, rcClient.right, m_uYLinePos + m_uSpace*6 );
	::SetRect( &rcTxt5, rcClient.left + 8, m_uYLinePos+ m_uSpace*6, rcClient.right, m_uYLinePos + m_uSpace*7 );
	::SetRect( &rcTxt6, rcClient.left + 8, m_uYLinePos+ m_uSpace*7, rcClient.right, m_uYLinePos + m_uSpace*8 );
	::SetRect( &rcTxt7, rcClient.left + 8, m_uYLinePos+ m_uSpace*8, rcClient.right, m_uYLinePos + m_uSpace*9 );
	::SetRect( &rcTxt8, rcClient.left + 8, m_uYLinePos+ m_uSpace*9, rcClient.right, m_uYLinePos + m_uSpace*10 );
	::SetRect( &rcTxt9, rcClient.left + 8, m_uYLinePos+ m_uSpace*10, rcClient.right, m_uYLinePos + m_uSpace*11 );
	// draw text
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontId );
	if(pYBCtrlFont) {
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		//clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
		::DrawText( hDC, tstrTxt1.c_str(), tstrTxt1.length(), &rcTxt1, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE );
		::DrawText( hDC, tstrTxt2.c_str(), tstrTxt2.length(), &rcTxt2, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE );
		::DrawText( hDC, tstrTxt3.c_str(), tstrTxt3.length(), &rcTxt3, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE );
		::DrawText( hDC, tstrTxt4.c_str(), tstrTxt4.length(), &rcTxt4, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE );
		::DrawText( hDC, tstrTxt5.c_str(), tstrTxt5.length(), &rcTxt5, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE );
		::DrawText( hDC, tstrTxt6.c_str(), tstrTxt6.length(), &rcTxt6, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE );
		::DrawText( hDC, tstrTxt7.c_str(), tstrTxt7.length(), &rcTxt7, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE );
		::DrawText( hDC, tstrTxt8.c_str(), tstrTxt8.length(), &rcTxt8, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE );
		::DrawText( hDC, tstrTxt9.c_str(), tstrTxt9.length(), &rcTxt9, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

void ImageEditorProperty::_drawLine( HDC hDC ) {
	RECT rcEdit;
	RECT rcClient;
	HPEN hPenOld;

	::GetClientRect( m_hWnd, &rcClient );
	hPenOld = (HPEN)::SelectObject( hDC, m_hPenEdit );
	::SetRect( &rcEdit, m_rcEdit.left - 3, m_rcEdit.top - 3, m_rcEdit.right + 3, m_rcEdit.bottom + 3 );
	::Rectangle( hDC, rcEdit.left, rcEdit.top, rcEdit.right, rcEdit.bottom );
	//
	RECT rcEdit1;
	::SetRect( &rcEdit1, m_rcColor1.left, m_uYLinePos + m_uSpace * 3 + 2, m_rcColor1.left + 68, m_uYLinePos + m_uSpace * 4 - 2 );
	::Rectangle( hDC, rcEdit1.left, rcEdit1.top, rcEdit1.right, rcEdit1.bottom );
	//
	RECT rcEdit2;
	::SetRect( &rcEdit2, m_rcColor1.left, m_uYLinePos + m_uSpace * 4 + 2, m_rcColor1.left + 68, m_uYLinePos + m_uSpace * 5 - 2 );
	::Rectangle( hDC, rcEdit2.left, rcEdit2.top, rcEdit2.right, rcEdit2.bottom );

	::SelectObject( hDC, hPenOld );
	//
	hPenOld = (HPEN)::SelectObject( hDC, m_hPenLine );
	//
	::MoveToEx( hDC, rcClient.left + 2, m_uYLinePos, NULL );
	::LineTo( hDC, rcClient.right - 2, m_uYLinePos );
	//
	::MoveToEx( hDC, rcClient.left + 2, m_uYLinePos+m_uSpace*2, NULL);
	::LineTo( hDC, rcClient.right - 2, m_uYLinePos + m_uSpace*2 );
	::MoveToEx( hDC, rcEdit.left + 8, m_uYLinePos + m_uSpace, NULL );//断线
	::LineTo( hDC, rcEdit.right - 8, m_uYLinePos + m_uSpace );
	::MoveToEx( hDC, rcClient.right/2, m_uYLinePos + 6, NULL );
	::LineTo( hDC, rcClient.right/2, m_uYLinePos + m_uSpace - 6 );
	::MoveToEx( hDC, rcClient.right / 2, m_uYLinePos + m_uSpace + 6, NULL );
	::LineTo( hDC, rcClient.right / 2, m_uYLinePos + m_uSpace*2 - 6 );
	//
	::MoveToEx( hDC, rcClient.left + 2, m_uYLinePos + m_uSpace*3, NULL );
	::LineTo( hDC, rcClient.right - 2, m_uYLinePos + m_uSpace*3 );
	//
	::MoveToEx( hDC, rcClient.left+ 2, m_uYLinePos + m_uSpace * 5, NULL );
	::LineTo( hDC, rcClient.right - 2, m_uYLinePos + m_uSpace * 5 );
	//
	::MoveToEx( hDC, rcClient.left+2, m_uYLinePos + m_uSpace * 7, NULL );
	::LineTo( hDC, rcClient.right-2, m_uYLinePos + m_uSpace * 7 );
	//
	::MoveToEx( hDC, rcClient.left + 2, m_uYLinePos + m_uSpace * 9, NULL );
	::LineTo( hDC, rcClient.right - 2, m_uYLinePos + m_uSpace * 9 );
	//
	::MoveToEx( hDC, rcClient.left + 2, m_uYLinePos + m_uSpace * 10, NULL );
	::LineTo( hDC, rcClient.right - 2, m_uYLinePos + m_uSpace * 10 );

	::SelectObject( hDC, hPenOld );
}

void ImageEditorProperty::_drawHeadIcon( HDC hDC ) {
	RECT rcClient;
	RECT rcHead;
	RECT rcIcon;
	YBCtrlImageStatic* pYBImage = NULL;

	::GetClientRect( m_hWnd, &rcClient );
	::SetRect( &rcHead, rcClient.left + 2, rcClient.top + 2, rcClient.right - 2, SPACE_EDITOR_CONTROLBAR_HEIGHT );
	::FillRect( hDC, &rcHead, m_hBrush );
	pYBImage = _getImageStatic( m_uImageIdIcon );
	if(!pYBImage) {
		assert( false );
		return;
	}
	::SetRect( &rcIcon, (rcHead.right - rcHead.left - pYBImage->getWidth())/2, rcHead.top,
		(rcHead.right - rcHead.left - pYBImage->getWidth())/2 + pYBImage->getWidth(), rcHead.top+ pYBImage->getHeight() );
	
	Gdiplus::Graphics graphics( hDC );
	GdipBitBlt( &graphics, pYBImage->getImage(), rcIcon.left, rcIcon.top, rcIcon.right - rcIcon.left, rcIcon.bottom - rcIcon.top, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
}

void ImageEditorProperty::_drawBk( HDC hDC ) {
	RECT rcClient;
	YBCtrlImageStatic* pYBImage = NULL;

	::GetClientRect( m_hWnd, &rcClient );
	pYBImage = _getImageStatic( m_uImageIdBk );
	if(!pYBImage) {
		assert( false );
		return;
	}
// 	Gdiplus::Graphics graphics( hDC );
// 	GdipBitBlt( &graphics, pYBImage->getImage(), 10, 10, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
	BackGroundBitBlt( hDC, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pYBImage->getMemDC(),
		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 2, 2, 2, 2 );
}
