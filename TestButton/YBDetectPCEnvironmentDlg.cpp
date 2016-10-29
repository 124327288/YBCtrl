#include "stdafx.h"
#include "YBDetectPCEnvironmentDlg.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlMisc.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlFontDepository.h"
#include "YBListBox4PCDE.h"

#define YBDPCEDLG_BLOCK_WIDTH ( 761 )
#define YBDPCEDLG_BLOCKNUM_WIDTH ( 54 )
#define YBDPCEDLG_BLOCKITEM_WIDTH ( 100 )
#define YBDPCEDLG_BLOCKCFG_WIDTH ( 101 )
#define YBDPCEDLG_BLOCKRES_WIDTH ( 108 )
#define YBDPCEDLG_BLOCKNOTE_WIDTH ( 400 )

USING_NAMESPACE_YBCTRL

#define BINDING_IMAGE_FNAME2ID( filename, imageid ) \
	do { \
	tstring_type tstrImageDirPath; \
	tstring_type tstrImageFilePath; \
	TCHAR szModulePath[MAX_PATH]; \
	TCHAR* posTmp = NULL; \
	::GetModuleFileName( NULL, szModulePath, MAX_PATH ); \
	posTmp = _tcsrchr( szModulePath, _T('\\')); \
	if ( !posTmp ) { \
	assert( false ); \
	return; \
	} \
	posTmp += 1; \
	*posTmp = 0; \
	tstrImageDirPath = szModulePath; \
	tstrImageDirPath += _T("Image\\"); \
	tstrImageFilePath = tstrImageDirPath; \
	tstrImageFilePath += filename; \
	if ( !ybctrl::YBImageLoader::getInstance()->loadImage( tstrImageFilePath, imageid ) ) { \
	assert( false ); \
	return; \
	} \
	} while( 0 )


YBDetectPCEnvironmentDlg::YBDetectPCEnvironmentDlg()
: m_tstrTitle( _T("电子律师安装环境检测") )
, m_tstrBlockNum( _T("序号") )
, m_tstrBlockItem( _T("项目") )
, m_tstrBlockCfg( _T("配置") )
, m_tstrBlockRes( _T("检测结果") )
, m_tstrBlockNote( _T("备注") )
, m_listbox4PCEDlg( new YBView4PCDEInner( 300, 26 ), 25, 26 ) {
	_loadFont();
	_loadImage();
}

YBDetectPCEnvironmentDlg::~YBDetectPCEnvironmentDlg() {

}

YBCTRL_WNDMSGMAP_BEGIN( YBDetectPCEnvironmentDlg, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_ONWNDMSG( WM_NCHITTEST, _onWM_NCHITTEST )
YBCTRL_WNDMSGMAP_END()

HWND YBDetectPCEnvironmentDlg::create( int nWidth, int nHeight, HWND hWndParent, WORD wCtrlId ) {
	HWND hWnd;
	int nScreenWidth = ::GetSystemMetrics( SM_CXSCREEN );
	int nScreenHeight = ::GetSystemMetrics( SM_CYSCREEN );
	int x = ( nScreenWidth - nWidth )/2;
	int y = ( nScreenHeight - nHeight )/2;

	hWnd = _createHWND( 0, WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_CHILD|WS_VISIBLE, x, y, nWidth, nHeight, hWndParent, (HMENU)wCtrlId, NULL, NULL );
	if ( hWnd ) {
		_clipWnd();
		_initListbox();
	}
	return hWnd;
}

void YBDetectPCEnvironmentDlg::_initListbox() {
	m_listbox4PCEDlg.create( 34, 113, 761, 349, m_hWnd, 300, NULL );
	if(m_listbox4PCEDlg.getHWND()) {
		YBView4PCDEContent* pContentObj = (YBView4PCDEContent*)m_listbox4PCEDlg.getViewInner()->getViewContentObj();
		if(pContentObj) {
			pContentObj->setWidth( 761 );
		}
	}
	YBView4PCDEContent* pViewContent4WSAWMenu=NULL;
	YBListBoxPCDEItem* pListBoxMenuItem=NULL;
	TCHAR szMsgBoxMenu[100];
	pViewContent4WSAWMenu = (YBView4PCDEContent*)m_listbox4PCEDlg.getViewInner()->getViewContentObj();
	for(int i=0; i < 30; ++i) {
		pListBoxMenuItem = new YBListBoxPCDEItem();
		_stprintf_s( szMsgBoxMenu, _T( "%d" ), i + 1 );
		pViewContent4WSAWMenu->insertItem( i, pListBoxMenuItem );
	}
	m_listbox4PCEDlg.updateStatus();
	::ShowWindow( m_listbox4PCEDlg.getHWND(), SW_NORMAL );
}

void YBDetectPCEnvironmentDlg::_clipWnd() {
	HRGN hRgn;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	hRgn = ::CreateRoundRectRgn( 0, 0, rcClient.right, rcClient.bottom, 5, 5 );
	::SetWindowRgn( m_hWnd, hRgn, TRUE );
	::DeleteObject( hRgn );
}

void YBDetectPCEnvironmentDlg::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_PAINT );
	PAINTSTRUCT ps;
	RECT rcClient;
	HDC hPaintDC;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );

	hPaintDC = ::BeginPaint( m_hWnd, &ps );
	::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( m_hWnd, &ps );
}

void YBDetectPCEnvironmentDlg::_onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_NCHITTEST );
	
	pYBCtrlWndMsg->m_dwFlag = ( YBCTRLWNDMSG_FLAG_EATMSG|YBCTRLWNDMSG_FLAG_RESULT_VALID );
	pYBCtrlWndMsg->m_lRes = HTCAPTION;
}

void YBDetectPCEnvironmentDlg::_onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_ERASEBKGND );
	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	RECT rcClient;
	//
	pYBCtrlWndMsg->m_dwFlag = ( YBCTRLWNDMSG_FLAG_EATMSG | YBCTRLWNDMSG_FLAG_RESULT_VALID );
	pYBCtrlWndMsg->m_lRes = 1;

	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawStatus( memDC.getMemDC() );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
}

void YBDetectPCEnvironmentDlg::_onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );
}

void YBDetectPCEnvironmentDlg::_drawStatus( HDC hDC ) {
	_drawBk( hDC );
	_drawTitle( hDC );
	_drawBlockBk( hDC );
	_drawBlockText( hDC );
}

void YBDetectPCEnvironmentDlg::_drawBk( HDC hDC ) {
	YBCtrlImageStatic* pYBImage = NULL;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	pYBImage = _getImageStatic( m_uImageIdPCDEBk );
	if ( !pYBImage )
		return;

	BackGroundBitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pYBImage->getMemDC(),
					  0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0 );
}

void YBDetectPCEnvironmentDlg::_drawBlockText( HDC hDC ) {
	HFONT hFontOld=NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcBlockNum;
	RECT rcBlockItem;
	RECT rcBlockCfg;
	RECT rcBlockRes;
	RECT rcBlockNote;

	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdPCDEBlockTxt );
	if( !pYBCtrlFont ) {
		assert( false );
		return;
	}
	if( !_getBlockNumRect( rcBlockNum ) || !_getBlockItemRect( rcBlockItem ) || !_getBlockCfgRect( rcBlockCfg ) ||
		!_getBlockResRect( rcBlockRes ) || !_getBlockNoteRect( rcBlockNote ) ) {
		assert( false );
		return;
	}
	hFontOld=( HFONT )::SelectObject( hDC, pYBCtrlFont->m_hFont );
	iBkModeOld=::SetBkMode( hDC, TRANSPARENT );
	clrTxtOld=::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

	::DrawText( hDC, m_tstrBlockNum.c_str(), m_tstrBlockNum.length(), &rcBlockNum, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
	::DrawText( hDC, m_tstrBlockItem.c_str(), m_tstrBlockItem.length(), &rcBlockItem, DT_VCENTER|DT_CENTER|DT_SINGLELINE );
	::DrawText( hDC, m_tstrBlockCfg.c_str(), m_tstrBlockCfg.length(), &rcBlockCfg, DT_VCENTER|DT_CENTER|DT_SINGLELINE );
	::DrawText( hDC, m_tstrBlockRes.c_str(), m_tstrBlockRes.length(), &rcBlockRes, DT_VCENTER|DT_CENTER|DT_SINGLELINE );
	::DrawText( hDC, m_tstrBlockNote.c_str(), m_tstrBlockNote.length(), &rcBlockNote, DT_VCENTER|DT_CENTER|DT_SINGLELINE );
	::SetTextColor( hDC, clrTxtOld );
	::SetBkMode( hDC, iBkModeOld );
	::SelectObject( hDC, hFontOld );
}

void YBDetectPCEnvironmentDlg::_drawTitle( HDC hDC ) {
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcTitle;

	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdPCDETitle );
	if ( !pYBCtrlFont ) {
		assert( false );
		return;
	}
	if ( !_getTitleRect( rcTitle ) ) {
		assert( false );
		return;
	}
	hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
	iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
	clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

	::DrawText( hDC, m_tstrTitle.c_str(), m_tstrTitle.length(), &rcTitle, DT_VCENTER|DT_CENTER );
	::SetTextColor( hDC, clrTxtOld );
	::SetBkMode( hDC, iBkModeOld );
	::SelectObject( hDC, hFontOld );
}

bool YBDetectPCEnvironmentDlg::_getTitleRect( RECT& rcTitle ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcTitle.left = rcClient.left;
	rcTitle.right = rcClient.right;
	rcTitle.top = rcClient.top + 15;
	rcTitle.bottom = rcTitle.top + 40;
	return ( ( rcTitle.right > rcTitle.left ) && ( rcTitle.bottom > rcTitle.top ) );
}

void YBDetectPCEnvironmentDlg::_drawBlockBk( HDC hDC ) {
	YBCtrlImageStatic* pYBImage = NULL;
	RECT rcBlock;

	pYBImage = _getImageStatic( m_uImageIdPCDEBlockBk );
	if ( !pYBImage )
		return;
	if ( !_getBlockBkRect( rcBlock ) )
		return;

	Gdiplus::Graphics graphics( hDC );
	GdipBitBlt( &graphics, pYBImage->getImage(), rcBlock.left, rcBlock.top, rcBlock.right - rcBlock.left, rcBlock.bottom - rcBlock.top
		, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
}

bool YBDetectPCEnvironmentDlg::_getBlockBkRect( RECT& rcBlock ) const {
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	rcBlock.left = rcClient.left + 34;
	rcBlock.top = rcClient.top + 66;
	rcBlock.right = rcBlock.left + YBDPCEDLG_BLOCK_WIDTH;
	rcBlock.bottom = rcBlock.top + 46;
	return ( ( rcBlock.right > rcBlock.left ) && ( rcBlock.bottom > rcBlock.top ) );
}

bool YBDetectPCEnvironmentDlg::_getBlockNumRect( RECT& rcBlockNum ) const {
	RECT rcBlockBk;

	if ( !_getBlockBkRect( rcBlockBk ) ) {
		assert( false );
		return false;
	}
	rcBlockNum.left = rcBlockBk.left;
	rcBlockNum.right = rcBlockNum.left + YBDPCEDLG_BLOCKNUM_WIDTH;
	rcBlockNum.top = rcBlockBk.top;
	rcBlockNum.bottom = rcBlockBk.bottom;
	return ( ( rcBlockNum.right > rcBlockNum.left ) && ( rcBlockNum.bottom > rcBlockNum.top ) );
}

bool YBDetectPCEnvironmentDlg::_getBlockItemRect( RECT& rcBlockItem ) const {
	RECT rcBlockBk;

	if ( !_getBlockBkRect( rcBlockBk ) ) {
		assert( false );
		return false;
	}
	rcBlockItem.left = rcBlockBk.left + YBDPCEDLG_BLOCKNUM_WIDTH;
	rcBlockItem.right = rcBlockItem.left + YBDPCEDLG_BLOCKITEM_WIDTH;
	rcBlockItem.top = rcBlockBk.top;
	rcBlockItem.bottom = rcBlockBk.bottom;
	return ( ( rcBlockItem.right > rcBlockItem.left ) && ( rcBlockItem.bottom > rcBlockItem.top ) );
}

bool YBDetectPCEnvironmentDlg::_getBlockCfgRect( RECT& rcBlockCfg ) const {
	RECT rcBlockBk;

	if( !_getBlockBkRect( rcBlockBk ) ) {
		assert( false );
		return false;
	}
	rcBlockCfg.left = rcBlockBk.left + YBDPCEDLG_BLOCKNUM_WIDTH + YBDPCEDLG_BLOCKITEM_WIDTH;
	rcBlockCfg.right = rcBlockCfg.left + YBDPCEDLG_BLOCKCFG_WIDTH;
	rcBlockCfg.top = rcBlockBk.top;
	rcBlockCfg.bottom = rcBlockBk.bottom;
	return ( ( rcBlockCfg.right > rcBlockCfg.left ) && ( rcBlockCfg.bottom > rcBlockCfg.top ) );
}

bool YBDetectPCEnvironmentDlg::_getBlockResRect( RECT& rcBlockRes ) const {
	RECT rcBlockBk;

	if( !_getBlockBkRect( rcBlockBk ) ) {
		assert( false );
		return false;
	}
	rcBlockRes.left = rcBlockBk.left + YBDPCEDLG_BLOCKNUM_WIDTH + YBDPCEDLG_BLOCKITEM_WIDTH + YBDPCEDLG_BLOCKCFG_WIDTH;
	rcBlockRes.right = rcBlockRes.left + YBDPCEDLG_BLOCKRES_WIDTH;
	rcBlockRes.top = rcBlockBk.top;
	rcBlockRes.bottom = rcBlockBk.bottom;
	return ( ( rcBlockRes.right > rcBlockRes.left ) && ( rcBlockRes.bottom > rcBlockRes.top ) );
}

bool YBDetectPCEnvironmentDlg::_getBlockNoteRect( RECT& rcBlockNote ) const {
	RECT rcBlockBk;

	if(!_getBlockBkRect( rcBlockBk )) {
		assert( false );
		return false;
	}
	rcBlockNote.left = rcBlockBk.left + YBDPCEDLG_BLOCKNUM_WIDTH + YBDPCEDLG_BLOCKITEM_WIDTH + YBDPCEDLG_BLOCKCFG_WIDTH + YBDPCEDLG_BLOCKRES_WIDTH;
	rcBlockNote.right = rcBlockNote.left + YBDPCEDLG_BLOCKNOTE_WIDTH;
	rcBlockNote.top = rcBlockBk.top;
	rcBlockNote.bottom = rcBlockBk.bottom;
	return ( ( rcBlockNote.right > rcBlockNote.left ) && ( rcBlockNote.bottom > rcBlockNote.top ) );
}

ybctrl::YBCtrlImageStatic* YBDetectPCEnvironmentDlg::_getImageStatic( unsigned int uImageId ) const {
	YBCtrlImageBase* pYBImageBase = NULL;

	if ( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pYBImageBase ) )
		return NULL;
	if ( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

void YBDetectPCEnvironmentDlg::_loadImage() {
	BINDING_IMAGE_FNAME2ID( _T("YBPCDEBk.png"), m_uImageIdPCDEBk );
	BINDING_IMAGE_FNAME2ID( _T("YBPCDEBlockBk.png"), m_uImageIdPCDEBlockBk );
}

void YBDetectPCEnvironmentDlg::_loadFont() {
	LOGFONT lFont;

	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 35;
	m_uFontIdPCDETitle = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(22, 45, 141) );
	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T( "微软雅黑" ) );
	lFont.lfHeight = 23;
	m_uFontIdPCDEBlockTxt = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(255, 255, 255) );
}
