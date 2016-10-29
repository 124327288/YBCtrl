#include "stdafx.h"
#include "dialog/YBModalDialog.h"
#include "YBITMsgBox.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlImageBase.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlMisc.h"
#include "frame/YBCtrlMemDC.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlFontDepository.h"
#include <assert.h>

#define YBITMB_STATIC_TEXT (_T(""))

USING_NAMESPACE_YBCTRL

YBITMsgBox::YBITMsgBox(  bool bAutoExit, unsigned int uExitTimeMS )
: ybctrl::YBModalDialog( bAutoExit, uExitTimeMS )
, m_uImageIdBK( YBCTRL_IMAGE_ID_INVALID )
, m_uLFixed( 2 )
, m_uRFixed( 2 )
, m_uBFixed( 2 )
, m_uTFixed( 2 )
, m_uImageIdBtnNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnPre( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnXNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnXHov( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdBtnXPre( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdStaticTxt( YBCTRL_IMAGE_ID_INVALID )
, m_uFontIdStaticTxt( YBCTRL_FONT_ID_INVALID )
, m_uFontIdbtn( YBCTRL_FONT_ID_INVALID )
, m_tstrTittleText( _T("ÇëÊäÈë±êÌâ£º") )
, m_tstrText( _T("°´¡°OK¡±°´Å¥¹Ø±Õ¶Ô»°¿ò") )
, m_tstrOkText( _T("OK") )
, m_uLSpace( 90 )
, m_uTSpace( 90 )
, m_uWidthText( 260 )
, m_uHeightText( 55 )
, m_uMBWidth( 420 )
, m_uMBHeight( 200 )
{
	_loadImage();
	_loadFont();
}

YBITMsgBox::~YBITMsgBox(){

}

YBCTRL_WNDMSGMAP_BEGIN( YBITMsgBox, YBModalDialog )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_NCHITTEST, _onWM_NCHITTEST )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_WNDMSGMAP_END()

void YBITMsgBox::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );

	PAINTSTRUCT ps;
	RECT rcClient;
	HDC hPaintDC;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );

	YBModalDialog::_drawBK( memDC.getMemDC() );
	_drawBK( memDC.getMemDC() );
	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void YBITMsgBox::_onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_NCHITTEST );

	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = HTCAPTION;
}


void YBITMsgBox::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
	WORD wCtrlId = LOWORD( pYBCtrlWndMsg->m_wParam );
	WORD wEvtCoed = HIWORD( pYBCtrlWndMsg->m_wParam );

	switch( wCtrlId ){
	case YBITMSGBOX_BTN_CLOSE:
		{
			endModal( ::GetDlgCtrlID( m_btnClose.getHWND() ) );
		}
		break;
	case YBITMSGBOX_BTN_OK:
		{
			endModal( ::GetDlgCtrlID( m_btnOK.getHWND() ) );
		}
		break;
	default:
		break;
	}
}

unsigned int YBITMsgBox::_getWndHeight() const{ return m_uMBHeight; }

unsigned int YBITMsgBox::_getWndWidth() const{ return m_uMBWidth; }

void YBITMsgBox::_createUI( HWND hWndModalDlg, HINSTANCE hInst ){
	// OK button
	m_btnOK.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnNor );
	m_btnOK.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnHov );
	m_btnOK.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnPre );
	m_btnOK.setFontId( m_uFontIdbtn );
	m_btnOK.setText( m_tstrOkText );
	m_btnOK.Create( 0, WS_VISIBLE|WS_CHILD, 310, 150, 68, 31, hWndModalDlg, (HMENU)YBITMSGBOX_BTN_OK, hInst );
	// CLOSE button
	m_btnClose.setImage( YBButton::EBTN_STATUS_NORMAL, m_uImageIdBtnXNor );
	m_btnClose.setImage( YBButton::EBTN_STATUS_HOVER, m_uImageIdBtnXHov );
	m_btnClose.setImage( YBButton::EBTN_STATUS_PRESS, m_uImageIdBtnXPre );
	m_btnClose.Create( 0, WS_VISIBLE|WS_CHILD,  400, 7, 14, 14, hWndModalDlg, (HMENU)YBITMSGBOX_BTN_CLOSE, hInst );
	// Show MsgBox context
	m_staticText.Create( 0, WS_CHILD|WS_VISIBLE, m_uLSpace, m_uTSpace, m_uWidthText, m_uHeightText, hWndModalDlg, (HMENU)YBITMSGBOX_STATIC_TEXT, hInst );
	m_staticText.setImage( m_uImageIdStaticTxt );
	m_staticText.setFontId( m_uFontIdStaticTxt );
	m_staticText.setText( m_tstrText );
	// Show MsgBox Tittle Text
	m_staticTittleText.Create( 0, WS_CHILD|WS_VISIBLE, 4, 4, 110, 25, hWndModalDlg, (HMENU)YBITMSGBOX_STATIC_TITTLETEXT, hInst );
	m_staticTittleText.setImage( m_uImageIdBK );
	m_staticTittleText.setFontId( m_uFontIdStaticTxt );
	m_staticTittleText.setText( m_tstrTittleText );
}

YBCtrlImageStatic* YBITMsgBox::_getImageStatic( unsigned int uImageId ) {
	YBCtrlImageBase* pYBImageBase = NULL;
	YBCtrlImageStatic* pImageStatic = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pYBImageBase ) )
		return NULL;

	if( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

void YBITMsgBox::_drawBK( HDC hDC ) {
	if( !getHWND() )
		return;

	RECT rcClient;
	YBCtrlImageStatic* pYBImageBK = NULL;
	::GetClientRect( m_hWnd, &rcClient );

	pYBImageBK = _getImageStatic( m_uImageIdBK );
	BackGroundBitBlt( hDC, 0, 0, ( rcClient.right - rcClient.left ), 30, 
		pYBImageBK->getMemDC(), 0, 0, pYBImageBK->getWidth(), pYBImageBK->getHeight(), 
		m_uLFixed, m_uTFixed, m_uRFixed, m_uBFixed );
}

void YBITMsgBox::_loadFont(){
	LOGFONT lFont;

	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 22;
	lFont.lfWeight = 700;
	m_uFontIdbtn = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );
	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 22;
	lFont.lfWeight = 700;
	m_uFontIdStaticTxt = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 0, 93, 147 ) );
}

void YBITMsgBox::_loadImage(){
	tstring_type tstrImageDirPath;
	tstring_type tstrImageFilePath;
	TCHAR szModulePath[MAX_PATH];
	TCHAR* posTmp = NULL;
	unsigned int idImage;

	GetModuleFileName( NULL, szModulePath, MAX_PATH );
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
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBk.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	setBKImageId( idImage, 2, 5, 5, 2 );
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgTitleBk.png");
	if( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ){
		assert(false);
	}
	m_uImageIdBK = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBtnXNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnXNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBtnXHov.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnXHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBtnXPre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnXPre = idImage;
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBtnSaveNor.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnNor = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("buttonPress.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnHov = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T("YBITDlgBtnSavePre.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdBtnPre = idImage;
	// static text BK
	tstrImageFilePath = tstrImageDirPath + _T("YBCheckBoxBk.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdStaticTxt = idImage;
}

void YBITMsgBox::setText( const tstring_type& tstrText, const tstring_type& tstrTittleText, const tstring_type& tstrOkText ){
	m_tstrText = tstrText;
	m_tstrTittleText = tstrTittleText;
	m_tstrOkText = tstrOkText;
}

void YBITMsgBox::setTextLocation( unsigned int uLSpace, unsigned int uTSpace, unsigned int uWidthText, unsigned int uHeightText ){
	m_uLFixed = uLSpace;
	m_uTFixed = uTSpace;
	m_uWidthText = uWidthText;
	m_uHeightText = uHeightText;
}

void YBITMsgBox::setMBSize( unsigned int nMsgBoxWidth, unsigned int nMsgBoxHeight ){
	m_uMBWidth = nMsgBoxWidth;
	m_uMBHeight = nMsgBoxHeight;
}