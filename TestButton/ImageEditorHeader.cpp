#include "stdafx.h"
#include "ImageEditorHeader.h"
#include "loader/YBImageLoader.h"
#include "ImageEditorEnvCfg.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlImageBase.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlMisc.h"
#include "frame/YBCtrlFontDepository.h"

#define COLOR_BK_TOP (RGB(16,74,124))
#define COLOR_BK_BOTTOM (RGB(232,232,232))

USING_NAMESPACE_YBCTRL

ImageEditorHeader::ImageEditorHeader()
: m_uImageIdEdtTextIcon( 0 )
, m_uImageIdEdtImagebtnNor( 0 )
, m_uImageIdEdtImagebtnHov( 0 )
, m_uImageIdEdtImagebtnPre( 0 )
, m_uImageIdEdtTextbtnNor( 0 )
, m_uImageIdEdtTextbtnHov( 0 )
, m_uImageIdEdtTextbtnPre( 0 )
, m_uImageIdEdtVediobtnNor( 0 )
, m_uImageIdEdtVediobtnHov( 0 )
, m_uImageIdEdtVediobtnPre( 0 )
, m_uImageIdScrPartNor( 0 )
, m_uImageIdScrPartHov( 0 )
, m_uImageIdScrPartPre( 0 )
, m_uImageIdOpenbtnNor( 0 )
, m_uImageIdOpenbtnHov( 0 )
, m_uImageIdOpenbtnPre( 0 )
, m_uImageIdClearbtnNor( 0 )
, m_uImageIdClearbtnHov( 0 )
, m_uImageIdClearbtnPre( 0 )
, m_uImageIdDeletebtnNor( 0 )
, m_uImageIdDeletebtnHov( 0 )
, m_uImageIdDeletebtnPre( 0 )
, m_uImageIdSavebtnNor( 0 )
, m_uImageIdSavebtnHov( 0 )
, m_uImageIdSavebtnPre( 0 )
, m_uImageIdSaveAsbtnNor( 0 )
, m_uImageIdSaveAsbtnHov( 0 )
, m_uImageIdSaveAsbtnPre( 0 ) {
	::memset( &m_rcEdtImageBtn, 0, sizeof(RECT) );
	::memset( &m_rcEdtTextBtn, 0, sizeof(RECT) );
	::memset( &m_rcEdtVedioBtn, 0, sizeof(RECT) );
	::memset( &m_rcScreenPartBtn, 0, sizeof(RECT) );
	::memset( &m_rcOpenBtn, 0, sizeof(RECT) );
	::memset( &m_rcClearBtn, 0, sizeof(RECT) );
	::memset( &m_rcDeleteBtn, 0, sizeof(RECT) );
	::memset( &m_rcSaveBtn, 0, sizeof(RECT) );
	::memset( &m_rcSaveAsBtn, 0, sizeof(RECT) );
	m_hBrushBkTop = ::CreateSolidBrush( COLOR_BK_TOP );
	m_hBrushBkBottom = ::CreateSolidBrush( COLOR_BK_BOTTOM );
	_loadFont();
}

ImageEditorHeader::~ImageEditorHeader() {
	if(m_hBrushBkTop) {
		::DeleteObject( m_hBrushBkTop );
		m_hBrushBkTop = NULL;
	}
	if(m_hBrushBkBottom){
		::DeleteObject( m_hBrushBkBottom );
		m_hBrushBkBottom = NULL;
	}
}

YBCTRL_WNDMSGMAP_BEGIN( ImageEditorHeader, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
YBCTRL_WNDMSGMAP_END()

void ImageEditorHeader::setPath( const tstring_type& tstrDirPath ) {
	m_tstrDirPath = tstrDirPath;
	_loadImage();
}

HWND ImageEditorHeader::create( HWND hWndParent, HMENU hMenu, HINSTANCE hInstance ) {
	HWND hWnd;
	RECT rcParentClient;

	::GetClientRect( hWndParent, &rcParentClient );
	hWnd = _createHWND( NULL, WS_CHILD|WS_VISIBLE, 0, 0, rcParentClient.right - rcParentClient.left, 90, hWndParent, hMenu, hInstance, NULL );
	if(hWnd) {
		_init();
		m_btnEditorImage.Create( NULL, WS_VISIBLE|WS_CHILD, m_rcEdtImageBtn.left, m_rcEdtImageBtn.top, m_rcEdtImageBtn.right - m_rcEdtImageBtn.left, m_rcEdtImageBtn.bottom - m_rcEdtImageBtn.top, hWnd, (HMENU)BTN_EDITOR_IMAGE_ID, NULL );
		m_btnEditorText.Create( NULL, WS_VISIBLE | WS_CHILD, m_rcEdtTextBtn.left, m_rcEdtTextBtn.top, m_rcEdtTextBtn.right - m_rcEdtTextBtn.left, m_rcEdtTextBtn.bottom - m_rcEdtTextBtn.top, hWnd, (HMENU)BTN_EDITOR_TEXT_ID, NULL );
		m_btnEditorVedio.Create( NULL, WS_VISIBLE | WS_CHILD, m_rcEdtVedioBtn.left, m_rcEdtVedioBtn.top, m_rcEdtVedioBtn.right - m_rcEdtVedioBtn.left, m_rcEdtVedioBtn.bottom - m_rcEdtVedioBtn.top, hWnd, (HMENU)BTN_EDITOR_VEDIO_ID, NULL );
		m_btnScreenPart.Create( NULL, WS_VISIBLE | WS_CHILD, m_rcScreenPartBtn.left, m_rcScreenPartBtn.top, m_rcScreenPartBtn.right - m_rcScreenPartBtn.left, m_rcScreenPartBtn.bottom - m_rcScreenPartBtn.top, hWnd, (HMENU)BTN_SCREEN_PART_ID, NULL );
		//
		m_btnOpen.Create( NULL, WS_VISIBLE | WS_CHILD, m_rcOpenBtn.left, m_rcOpenBtn.top, m_rcOpenBtn.right - m_rcOpenBtn.left, m_rcOpenBtn.bottom - m_rcOpenBtn.top, hWnd, (HMENU)BTN_HEADER_OPEN_ID, NULL );
		m_btnClear.Create( NULL, WS_VISIBLE | WS_CHILD, m_rcClearBtn.left, m_rcClearBtn.top, m_rcClearBtn.right - m_rcClearBtn.left, m_rcClearBtn.bottom - m_rcClearBtn.top, hWnd, (HMENU)BTN_HEADER_CLEAR_ID, NULL );
		m_btnDelete.Create( NULL, WS_VISIBLE | WS_CHILD, m_rcDeleteBtn.left, m_rcDeleteBtn.top, m_rcDeleteBtn.right - m_rcDeleteBtn.left, m_rcDeleteBtn.bottom - m_rcDeleteBtn.top, hWnd, (HMENU)BTN_HEADER_DELETE_ID, NULL );
		m_btnSave.Create( NULL, WS_VISIBLE | WS_CHILD, m_rcSaveBtn.left, m_rcSaveBtn.top, m_rcSaveBtn.right - m_rcSaveBtn.left, m_rcSaveBtn.bottom - m_rcSaveBtn.top, hWnd, (HMENU)BTN_HEADER_SAVE_ID, NULL );
		m_btnSaveAs.Create( NULL, WS_VISIBLE | WS_CHILD, m_rcSaveAsBtn.left, m_rcSaveAsBtn.top, m_rcSaveAsBtn.right - m_rcSaveAsBtn.left, m_rcSaveAsBtn.bottom - m_rcSaveAsBtn.top, hWnd, (HMENU)BTN_HEADER_SAVEAS_ID, NULL );
	}
	m_btnEditorImage.pushDown();
	m_wordCtrlId = (WORD)hMenu;
	return hWnd;
}

void ImageEditorHeader::_onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg ) {
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

void ImageEditorHeader::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	RECT rcClient;
	PAINTSTRUCT ps;
	HDC hPaintDC;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void ImageEditorHeader::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCode = HIWORD( pYBCtrlWndMsg->m_wParam );
	switch(wCtrlId) {
		case BTN_EDITOR_IMAGE_ID:{
						m_btnEditorText.popUp();
						m_btnEditorVedio.popUp();
						m_btnScreenPart.popUp();
		}
		break;
		case BTN_EDITOR_TEXT_ID:{
						m_btnEditorImage.popUp();
						m_btnEditorVedio.popUp();
						m_btnScreenPart.popUp();
		}
		break;
		case BTN_EDITOR_VEDIO_ID:{
						m_btnEditorImage.popUp();
						m_btnEditorText.popUp();
						m_btnScreenPart.popUp();
		}
		break;
		case BTN_SCREEN_PART_ID:{
						m_btnEditorImage.popUp();
						m_btnEditorText.popUp();
						m_btnEditorVedio.popUp();
		}
		break;
		case BTN_HEADER_OPEN_ID:{
						::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, MAKELONG( m_wordCtrlId, BTN_HEADER_CMD_OPEN ), (LPARAM)m_hWnd );
		}
		break;
		case BTN_HEADER_CLEAR_ID:{
						::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, MAKELONG( m_wordCtrlId, BTN_HEADER_CMD_CLEAR ), (LPARAM)m_hWnd );
		}
		break;
		case BTN_HEADER_DELETE_ID:{
						::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, MAKELONG( m_wordCtrlId, BTN_HEADER_CMD_DELETE ), (LPARAM)m_hWnd );
		}
		break;
		case BTN_HEADER_SAVE_ID:{
						::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, MAKELONG( m_wordCtrlId, BTN_HEADER_CMD_SAVE ), (LPARAM)m_hWnd );
		}
		break;
		case BTN_HEADER_SAVEAS_ID:{
						::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, MAKELONG( m_wordCtrlId, BTN_HEADER_CMD_SAVEAS ), (LPARAM)m_hWnd );
		}
		break;
		default:
			break;
	}
}

void ImageEditorHeader::_loadFont() {
	LOGFONT lFont;

	::memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T( "Î¢ÈíÑÅºÚ" ) );
	lFont.lfHeight = 22;
	m_uFontIdTop = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(255,255,255) );
	//
	::memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T( "Î¢ÈíÑÅºÚ" ) );
	lFont.lfHeight = 16;
	m_uFontIdBottom = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );
}

void ImageEditorHeader::_loadImage() {
	tstring_type tstrDirPath;
	tstring_type tstrFilePath;

	tstrDirPath = m_tstrDirPath;
	tstrDirPath += _T( "BMP\\" );
	//
	LOAD_IMAGE( _T( "editorHeaderIcon.png" ), m_uImageIdEdtTextIcon )
	LOAD_IMAGE( _T( "editorHeaderImageNor.png" ), m_uImageIdEdtImagebtnNor )
	LOAD_IMAGE( _T( "editorHeaderImagePre.png" ), m_uImageIdEdtImagebtnHov )
	LOAD_IMAGE( _T( "editorHeaderImagePre.png" ), m_uImageIdEdtImagebtnPre )
	LOAD_IMAGE( _T( "editorHeaderTextNor.png" ), m_uImageIdEdtTextbtnNor )
	LOAD_IMAGE( _T( "editorHeaderTextPre.png" ), m_uImageIdEdtTextbtnHov )
	LOAD_IMAGE( _T( "editorHeaderTextPre.png" ), m_uImageIdEdtTextbtnPre )
	LOAD_IMAGE( _T( "editorHeaderVedioNor.png" ), m_uImageIdEdtVediobtnNor )
	LOAD_IMAGE( _T( "editorHeaderVedioPre.png" ), m_uImageIdEdtVediobtnHov )
	LOAD_IMAGE( _T( "editorHeaderVedioPre.png" ), m_uImageIdEdtVediobtnPre )
	LOAD_IMAGE( _T( "editorHeaderScrPartNor.png" ), m_uImageIdScrPartNor )
	LOAD_IMAGE( _T( "editorHeaderScrPartPre.png" ), m_uImageIdScrPartHov )
	LOAD_IMAGE( _T( "editorHeaderScrPartPre.png" ), m_uImageIdScrPartPre )
	LOAD_IMAGE( _T( "editorHeaderOpenNor.png" ), m_uImageIdOpenbtnNor )
	LOAD_IMAGE( _T( "editorHeaderOpenHov.png" ), m_uImageIdOpenbtnHov )
	LOAD_IMAGE( _T( "editorHeaderOpenPre.png" ), m_uImageIdOpenbtnPre )
	LOAD_IMAGE( _T( "editorHeaderClearNor.png" ), m_uImageIdClearbtnNor )
	LOAD_IMAGE( _T( "editorHeaderClearHov.png" ), m_uImageIdClearbtnHov )
	LOAD_IMAGE( _T( "editorHeaderClearPre.png" ), m_uImageIdClearbtnPre )
	LOAD_IMAGE( _T( "editorHeaderDeleteNor.png" ), m_uImageIdDeletebtnNor )
	LOAD_IMAGE( _T( "editorHeaderDeleteHov.png" ), m_uImageIdDeletebtnHov )
	LOAD_IMAGE( _T( "editorHeaderDeletePre.png" ), m_uImageIdDeletebtnPre )
	LOAD_IMAGE( _T( "editorHeaderSaveNor.png" ), m_uImageIdSavebtnNor )
	LOAD_IMAGE( _T( "editorHeaderSaveHov.png" ), m_uImageIdSavebtnHov )
	LOAD_IMAGE( _T( "editorHeaderSavePre.png" ), m_uImageIdSavebtnPre )
	LOAD_IMAGE( _T( "editorHeaderSaveAsNor.png" ), m_uImageIdSaveAsbtnNor )
	LOAD_IMAGE( _T( "editorHeaderSaveAsHov.png" ), m_uImageIdSaveAsbtnHov )
	LOAD_IMAGE( _T( "editorHeaderSaveAsPre.png" ), m_uImageIdSaveAsbtnPre )
}

void ImageEditorHeader::_init() {
	//m_btnEditorImage.setFontId( m_uFontIdTop );
	//m_btnEditorImage.setText( _T("Í¼Æ¬±à¼­") );
	m_btnEditorImage.enableBtnX( false );
	m_btnEditorImage.setImage( YBButtonX::EBTN_STATUS_NORMAL, m_uImageIdEdtImagebtnNor );
	m_btnEditorImage.setImage( YBButtonX::EBTN_STATUS_HOVER, m_uImageIdEdtImagebtnHov );
	m_btnEditorImage.setImage( YBButtonX::EBTN_STATUS_PRESS, m_uImageIdEdtImagebtnPre );
	m_btnEditorImage.setImage( YBButtonX::EBTNX_STATUS_NORMAL, m_uImageIdEdtImagebtnPre );
	m_btnEditorImage.setImage( YBButtonX::EBTNX_STATUS_HOVER, m_uImageIdEdtImagebtnPre );
	m_btnEditorImage.setImage( YBButtonX::EBTNX_STATUS_PRESS, m_uImageIdEdtImagebtnPre );
	//
	//m_btnEditorText.setFontId( m_uFontIdTop );
	//m_btnEditorText.setText( _T("ÎÄ×Ö±à¼­") );
	m_btnEditorText.enableBtnX( false );
	m_btnEditorText.setImage( YBButtonX::EBTN_STATUS_NORMAL, m_uImageIdEdtTextbtnNor );
	m_btnEditorText.setImage( YBButtonX::EBTN_STATUS_HOVER, m_uImageIdEdtTextbtnHov );
	m_btnEditorText.setImage( YBButtonX::EBTN_STATUS_PRESS, m_uImageIdEdtTextbtnPre );
	m_btnEditorText.setImage( YBButtonX::EBTNX_STATUS_NORMAL, m_uImageIdEdtImagebtnPre );
	m_btnEditorText.setImage( YBButtonX::EBTNX_STATUS_HOVER, m_uImageIdEdtImagebtnPre );
	m_btnEditorText.setImage( YBButtonX::EBTNX_STATUS_PRESS, m_uImageIdEdtImagebtnPre );
	//
	//m_btnEditorVedio.setFontId( m_uFontIdTop );
	//m_btnEditorVedio.setText( _T( "ÊÓÆµÔ¤ÀÀ" ) );
	m_btnEditorVedio.enableBtnX( false );
	m_btnEditorVedio.setImage( YBButtonX::EBTN_STATUS_NORMAL, m_uImageIdEdtVediobtnNor );
	m_btnEditorVedio.setImage( YBButtonX::EBTN_STATUS_HOVER, m_uImageIdEdtVediobtnHov );
	m_btnEditorVedio.setImage( YBButtonX::EBTN_STATUS_PRESS, m_uImageIdEdtVediobtnPre );
	m_btnEditorVedio.setImage( YBButtonX::EBTNX_STATUS_NORMAL, m_uImageIdEdtImagebtnPre );
	m_btnEditorVedio.setImage( YBButtonX::EBTNX_STATUS_HOVER, m_uImageIdEdtImagebtnPre );
	m_btnEditorVedio.setImage( YBButtonX::EBTNX_STATUS_PRESS, m_uImageIdEdtImagebtnPre );
	//
	//m_btnScreenPart.setFontId( m_uFontIdTop );
	//m_btnScreenPart.setText( _T( "ÆÁÄ»·ÖÇø" ) );
	m_btnScreenPart.enableBtnX( false );
	m_btnScreenPart.setImage( YBButtonX::EBTN_STATUS_NORMAL, m_uImageIdScrPartNor );
	m_btnScreenPart.setImage( YBButtonX::EBTN_STATUS_HOVER, m_uImageIdScrPartHov );
	m_btnScreenPart.setImage( YBButtonX::EBTN_STATUS_PRESS, m_uImageIdScrPartPre );
	m_btnScreenPart.setImage( YBButtonX::EBTNX_STATUS_NORMAL, m_uImageIdEdtImagebtnPre );
	m_btnScreenPart.setImage( YBButtonX::EBTNX_STATUS_HOVER, m_uImageIdEdtImagebtnPre );
	m_btnScreenPart.setImage( YBButtonX::EBTNX_STATUS_PRESS, m_uImageIdEdtImagebtnPre );
	//
	//m_btnOpen.setFontId( m_uFontIdBottom );
	//m_btnOpen.setText( _T("´ò¿ª"));
	m_btnOpen.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdOpenbtnNor );
	m_btnOpen.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdOpenbtnHov );
	m_btnOpen.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdOpenbtnPre );
	//
	//m_btnClear.setFontId( m_uFontIdBottom );
	//m_btnClear.setText( _T( "Çå³ý" ) );
	m_btnClear.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdClearbtnNor );
	m_btnClear.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdClearbtnHov );
	m_btnClear.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdClearbtnPre );
	//
	//m_btnDelete.setFontId( m_uFontIdBottom );
	//m_btnDelete.setText( _T( "É¾³ý" ) );
	m_btnDelete.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdDeletebtnNor );
	m_btnDelete.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdDeletebtnHov );
	m_btnDelete.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdDeletebtnPre );
	//
	//m_btnSave.setFontId( m_uFontIdBottom );
	//m_btnSave.setText( _T( "±£´æ" ) );
	m_btnSave.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdSavebtnNor );
	m_btnSave.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdSavebtnHov );
	m_btnSave.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdSavebtnPre );
	//
	//m_btnSaveAs.setFontId( m_uFontIdBottom );
	//m_btnSaveAs.setText( _T( "Áí´æ" ) );
	m_btnSaveAs.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdSaveAsbtnNor );
	m_btnSaveAs.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdSaveAsbtnHov );
	m_btnSaveAs.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdSaveAsbtnPre );
	//read config file.
	tstring_type tstrFilePath;

	tstrFilePath = m_tstrDirPath;
	tstrFilePath += CONFIG_FILENAME;
	//
	m_rcEdtImageBtn.left = ::GetPrivateProfileInt( _T( "EditorImageBtnRect" ), _T( "left" ), 40, tstrFilePath.c_str() );
	m_rcEdtImageBtn.top = ::GetPrivateProfileInt( _T( "EditorImageBtnRect" ), _T( "top" ), 20, tstrFilePath.c_str() );
	m_rcEdtImageBtn.right = m_rcEdtImageBtn.left + ::GetPrivateProfileInt( _T( "EditorImageBtnRect" ), _T( "nWidth" ), 120, tstrFilePath.c_str() );
	m_rcEdtImageBtn.bottom = m_rcEdtImageBtn.top + ::GetPrivateProfileInt( _T( "EditorImageBtnRect" ), _T( "nHeight" ), 40, tstrFilePath.c_str() );
	//
	m_rcEdtTextBtn.left = ::GetPrivateProfileInt( _T( "EditorTextBtnRect" ), _T( "left" ), 190, tstrFilePath.c_str() );
	m_rcEdtTextBtn.top = ::GetPrivateProfileInt( _T( "EditorTextBtnRect" ), _T( "top" ), 20, tstrFilePath.c_str() );
	m_rcEdtTextBtn.right = m_rcEdtTextBtn.left + ::GetPrivateProfileInt( _T( "EditorTextBtnRect" ), _T( "nWidth" ), 120, tstrFilePath.c_str() );
	m_rcEdtTextBtn.bottom = m_rcEdtTextBtn.top + ::GetPrivateProfileInt( _T( "EditorTextBtnRect" ), _T( "nHeight" ), 40, tstrFilePath.c_str() );
	//
	m_rcEdtVedioBtn.left = ::GetPrivateProfileInt( _T( "EditorVedioBtnRect" ), _T( "left" ), 340, tstrFilePath.c_str() );
	m_rcEdtVedioBtn.top = ::GetPrivateProfileInt( _T( "EditorVedioBtnRect" ), _T( "top" ), 20, tstrFilePath.c_str() );
	m_rcEdtVedioBtn.right = m_rcEdtVedioBtn.left + ::GetPrivateProfileInt( _T( "EditorVedioBtnRect" ), _T( "nWidth" ), 120, tstrFilePath.c_str() );
	m_rcEdtVedioBtn.bottom = m_rcEdtVedioBtn.top + ::GetPrivateProfileInt( _T( "EditorVedioBtnRect" ), _T( "nHeight" ), 40, tstrFilePath.c_str() );
	//
	m_rcScreenPartBtn.left = ::GetPrivateProfileInt( _T( "ScreenPartBtnRect" ), _T( "left" ), 490, tstrFilePath.c_str() );
	m_rcScreenPartBtn.top = ::GetPrivateProfileInt( _T( "ScreenPartBtnRect" ), _T( "top" ), 20, tstrFilePath.c_str() );
	m_rcScreenPartBtn.right = m_rcScreenPartBtn.left + ::GetPrivateProfileInt( _T( "ScreenPartBtnRect" ), _T( "nWidth" ), 120, tstrFilePath.c_str() );
	m_rcScreenPartBtn.bottom = m_rcScreenPartBtn.top + ::GetPrivateProfileInt( _T( "ScreenPartBtnRect" ), _T( "nHeight" ), 40, tstrFilePath.c_str() );
	//right/bottom
	RECT rcClient;

	if(!m_hWnd) {
		assert( false );
	}
	::GetClientRect( m_hWnd, &rcClient );
	m_rcSaveAsBtn.right = ::GetPrivateProfileInt( _T( "SaveAsBtnRect" ), _T( "right" ), 40, tstrFilePath.c_str() );
	m_rcSaveAsBtn.bottom = ::GetPrivateProfileInt( _T( "SaveAsBtnRect" ), _T( "bottom" ), 2, tstrFilePath.c_str() );
	m_rcSaveAsBtn.right = rcClient.right - m_rcSaveAsBtn.right;
	m_rcSaveAsBtn.bottom = rcClient.bottom - m_rcSaveAsBtn.bottom;
	m_rcSaveAsBtn.left = m_rcSaveAsBtn.right - ::GetPrivateProfileInt( _T( "SaveAsBtnRect" ), _T( "nWidth" ), 30, tstrFilePath.c_str() );
	m_rcSaveAsBtn.top = m_rcSaveAsBtn.bottom - ::GetPrivateProfileInt( _T( "SaveAsBtnRect" ), _T( "nHeight" ), 28, tstrFilePath.c_str() );
	//right/bottom
	m_rcSaveBtn.right = ::GetPrivateProfileInt( _T( "SaveBtnRect" ), _T( "right" ), 80, tstrFilePath.c_str() );
	m_rcSaveBtn.bottom = ::GetPrivateProfileInt( _T( "SaveBtnRect" ), _T( "bottom" ), 2, tstrFilePath.c_str() );
	m_rcSaveBtn.right = rcClient.right - m_rcSaveBtn.right;
	m_rcSaveBtn.bottom = rcClient.bottom - m_rcSaveBtn.bottom;
	m_rcSaveBtn.left = m_rcSaveBtn.right - ::GetPrivateProfileInt( _T( "SaveBtnRect" ), _T( "nWidth" ), 30, tstrFilePath.c_str() );
	m_rcSaveBtn.top = m_rcSaveBtn.bottom - ::GetPrivateProfileInt( _T( "SaveBtnRect" ), _T( "nHeight" ), 28, tstrFilePath.c_str() );
	//right/bottom
	m_rcDeleteBtn.right = ::GetPrivateProfileInt( _T( "DeleteBtnRect" ), _T( "right" ), 120, tstrFilePath.c_str() );
	m_rcDeleteBtn.bottom = ::GetPrivateProfileInt( _T( "DeleteBtnRect" ), _T( "bottom" ), 2, tstrFilePath.c_str() );
	m_rcDeleteBtn.right = rcClient.right - m_rcDeleteBtn.right;
	m_rcDeleteBtn.bottom = rcClient.bottom - m_rcDeleteBtn.bottom;
	m_rcDeleteBtn.left = m_rcDeleteBtn.right - ::GetPrivateProfileInt( _T( "DeleteBtnRect" ), _T( "nWidth" ), 30, tstrFilePath.c_str() );
	m_rcDeleteBtn.top = m_rcDeleteBtn.bottom - ::GetPrivateProfileInt( _T( "DeleteBtnRect" ), _T( "nHeight" ), 28, tstrFilePath.c_str() );
	//right/bottom
	m_rcClearBtn.right = ::GetPrivateProfileInt( _T( "ClearBtnRect" ), _T( "right" ), 160, tstrFilePath.c_str() );
	m_rcClearBtn.bottom = ::GetPrivateProfileInt( _T( "ClearBtnRect" ), _T( "bottom" ), 2, tstrFilePath.c_str() );
	m_rcClearBtn.right = rcClient.right - m_rcClearBtn.right;
	m_rcClearBtn.bottom = rcClient.bottom - m_rcClearBtn.bottom;
	m_rcClearBtn.left = m_rcClearBtn.right - ::GetPrivateProfileInt( _T( "ClearBtnRect" ), _T( "nWidth" ), 30, tstrFilePath.c_str() );
	m_rcClearBtn.top = m_rcClearBtn.bottom - ::GetPrivateProfileInt( _T( "ClearBtnRect" ), _T( "nHeight" ), 28, tstrFilePath.c_str() );
	//right/bottom
	m_rcOpenBtn.right = ::GetPrivateProfileInt( _T( "OpenBtnRect" ), _T( "right" ), 200, tstrFilePath.c_str() );
	m_rcOpenBtn.bottom = ::GetPrivateProfileInt( _T( "OpenBtnRect" ), _T( "bottom" ), 2, tstrFilePath.c_str() );
	m_rcOpenBtn.right = rcClient.right - m_rcOpenBtn.right;
	m_rcOpenBtn.bottom = rcClient.bottom - m_rcOpenBtn.bottom;
	m_rcOpenBtn.left = m_rcOpenBtn.right - ::GetPrivateProfileInt( _T( "OpenBtnRect" ), _T( "nWidth" ), 30, tstrFilePath.c_str() );
	m_rcOpenBtn.top = m_rcOpenBtn.bottom - ::GetPrivateProfileInt( _T( "OpenBtnRect" ), _T( "nHeight" ), 28, tstrFilePath.c_str() );
}

void ImageEditorHeader::_drawStatus( HDC hDC ) {
	_drawBk( hDC );
	_drawEditTextIcon( hDC );
}

void ImageEditorHeader::_drawBk( HDC hDC ) {
	RECT rcClient;
	RECT rcBottom;

	::GetClientRect( m_hWnd, &rcClient );
	::SetRect( &rcBottom, rcClient.left, m_rcEdtImageBtn.bottom, rcClient.right, rcClient.bottom );
	::FillRect( hDC, &rcClient, m_hBrushBkTop );
	::FillRect( hDC, &rcBottom, m_hBrushBkBottom );
}

void ImageEditorHeader::_drawEditTextIcon( HDC hDC ) {
	YBCtrlImageStatic* pYBImage = NULL;
	RECT rcIcon;
	RECT rcClient;

	pYBImage = _getImageStatic( m_uImageIdEdtTextIcon );
	if(!pYBImage) {
		assert( false );
		return;
	}

	::GetClientRect( m_hWnd, &rcClient );
	::SetRect( &rcIcon, m_rcEdtImageBtn.left + 18, m_rcEdtImageBtn.bottom + 3, m_rcEdtImageBtn.left + 97, m_rcEdtImageBtn.bottom + 24 );
	Gdiplus::Graphics graphics( hDC );
	GdipBitBlt( &graphics, pYBImage->getImage(), rcIcon.left, rcIcon.top, rcIcon.right - rcIcon.left, rcIcon.bottom - rcIcon.top
		, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
}

YBCtrlImageStatic* ImageEditorHeader::_getImageStatic( unsigned int uImageId ) {
	YBCtrlImageBase* pYBImage = NULL;

	if(!YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pYBImage )) {
		assert( false );
		return NULL;
	}
	if(!pYBImage || pYBImage->getType() != YBCTRL_IMAGE_TYPE_STATIC) {
		assert( false );
		return NULL;
	}
	return (YBCtrlImageStatic*)pYBImage;
}