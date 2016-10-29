#include "stdafx.h"
#include "YBView4InstallFile.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlFontDepository.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlMisc.h"
#include "frame/YBCtrlImageStatic.h"
#include <assert.h>
//
USING_NAMESPACE_YBCTRL

#define COLOR_YBLB4WSAWMENU_ODD_ITEMBK_DEFAULT (RGB(200, 238, 249))
#define COLOR_YBLB4WSAWMENU_EVEN_ITEMBK_DEFAULT (RGB(168, 232, 250))
#define COLOR_YBLB4WSAWMENU_SELECTED_ITEMBK_DEFAULT (RGB(254, 237, 181))
#define COLOR_YBLB4WSAWMENU_ITEMBK_DEFAULT (RGB(255,255,255))
#define COLOR_YBLB4WSAWMENU_VIEWBK_DEFAULT (RGB(122,122,122))

#define YBWSAWMENU_INDEX_INVALID (0xFFFFFFFF)
	//
#define YBWSAWMENU_FLAG_NONE (0x00000000)
#define YBWSAWMENU_FLAG_INHOVER (0x00000001)
	//////////////////////////////////////////////////////////////////////////
	YBListBoxInstallFileItem::YBListBoxInstallFileItem()
	: m_tstrItemContent( _T("") )
	, m_uImageIdNor( YBCTRL_IMAGE_ID_INVALID )
	, m_uImageIdHov( YBCTRL_IMAGE_ID_INVALID )
	, m_uItemId( YBCTRL_MENU_ID_INVALID )
	, m_pBtnChk( new YBCheckBox() )
{
	// checkbox 资源
	//m_pBtnChk->setImage( YBCheckBox::ECHECK_STATUS_NORMAL, YBOS_Common::m_pResourceLoader->m_IdBtnChkNor );
	//m_pBtnChk->setImage( YBCheckBox::ECHECK_STATUS_HOVER, YBOS_Common::m_pResourceLoader->m_IdBtnChkHov );
	//m_pBtnChk->setImage( YBCheckBox::ECHECK_STATUS_PRESS, YBOS_Common::m_pResourceLoader->m_IdBtnChkPre );
	//m_pBtnChk->setImage( YBCheckBox::ECHECK_STATUS_DISABLE, YBOS_Common::m_pResourceLoader->m_IdBtnChkDis );
	//m_pBtnChk->setImage( YBCheckBox::ENOCHECK_STATUS_NORMAL, YBOS_Common::m_pResourceLoader->m_IdBtnNoChkNor );
	//m_pBtnChk->setImage( YBCheckBox::ENOCHECK_STATUS_HOVER, YBOS_Common::m_pResourceLoader->m_IdBtnNoChkHov );
	//m_pBtnChk->setImage( YBCheckBox::ENOCHECK_STATUS_PRESS, YBOS_Common::m_pResourceLoader->m_IdBtnNoChkPre );
	//m_pBtnChk->setImage( YBCheckBox::ENOCHECK_STATUS_DISABLE, YBOS_Common::m_pResourceLoader->m_IdBtnNoChkDis );
	//m_pBtnChk->setBKImageID( YBOS_Common::m_pResourceLoader->m_IdBtnChkBK );
	//m_pBtnChk->setCheckLSpace( 0 );
}

YBListBoxInstallFileItem::~YBListBoxInstallFileItem() {
	if ( m_pBtnChk ) {
		if ( ::IsWindow( m_pBtnChk->getHWND() ) ) {
			::DestroyWindow( m_pBtnChk->getHWND() );
		}
		delete m_pBtnChk;
		m_pBtnChk = NULL;
	}
}

unsigned int YBListBoxInstallFileItem::getItemId() const{
	assert( m_uItemId != YBCTRL_MENU_ID_INVALID );
	return m_uItemId;
}

void YBListBoxInstallFileItem::_setItemId( unsigned int uItemId ){
	assert( m_uItemId == YBCTRL_MENU_ID_INVALID );
	m_uItemId = uItemId;
}

//////////////////////////////////////////////////////////////////////////
#pragma warning( push )
#pragma warning (disable:4355 )
YBView4InstallFileInner::YBView4InstallFileInner( unsigned int uContentWidth, unsigned int uItemHeight )
	: ybctrl::YBViewInner( new YBView4InstallFileContent( uContentWidth, uItemHeight, this ) ) {
		setBkClr( COLOR_YBLB4WSAWMENU_VIEWBK_DEFAULT );
		setContentXPosLT( 0 );
		setContentYPosLT( 0 );
}
#pragma warning( pop )

YBView4InstallFileInner::~YBView4InstallFileInner() {

}

YBCTRL_WNDMSGMAP_BEGIN( YBView4InstallFileInner, YBViewInner )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
	YBCTRL_ONWNDMSG( WM_MOUSELEAVE, _onWM_MOUSELEAVE )
	YBCTRL_WNDMSGMAP_END()

	void YBView4InstallFileInner::_onWM_COMMAND(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
		YBCTRLMSG_ENTER_CHECK( WM_COMMAND );

}

void YBView4InstallFileInner::_onWM_LBUTTONDOWN(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
	POINT ptInV;
	POINT ptInC;
	unsigned int uItemIndex;
	RECT rcHit;
	YBView4InstallFileContent* pView4WSAWMenuContent = NULL;
	HWND hWndScrollWnd = NULL;

	hWndScrollWnd = ::GetParent( m_hWnd );
	ptInV.x = (__int16)LOWORD( pYBCtrlWndMsg->m_lParam );
	ptInV.y = (__int16)HIWORD( pYBCtrlWndMsg->m_lParam );

	if ( !PointView2Content( ptInV, ptInC ) )
		return;
	pView4WSAWMenuContent = (YBView4InstallFileContent*)getViewContentObj();
	if ( pView4WSAWMenuContent->moveHitTest( ptInC, uItemIndex ) ) {
		if ( !pView4WSAWMenuContent->getHitRect( uItemIndex, rcHit ) )
			return;
		if ( ::PtInRect( &rcHit, ptInC ) ) {
			::SendMessage( ::GetParent( hWndScrollWnd ), WM_COMMAND, (WPARAM)GetDlgCtrlID( hWndScrollWnd ), (LPARAM)uItemIndex );
			::InvalidateRect( m_hWnd, NULL ,FALSE );
		}
	}
}

void YBView4InstallFileInner::_onWM_MOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	POINT ptInV;
	POINT ptInC;
	unsigned int uItemIndexMove;
	unsigned int uItemIndexMoveIn;
	TRACKMOUSEEVENT trackMouseEvt;
	YBView4InstallFileContent* pView4WSAWMenuContent = NULL;

	ptInV.x = LOWORD( pYBCtrlWndMsg->m_lParam );
	ptInV.y = HIWORD( pYBCtrlWndMsg->m_lParam );
	if ( !PointView2Content( ptInV, ptInC ) )
		return;
	//
	trackMouseEvt.cbSize = sizeof( TRACKMOUSEEVENT );
	trackMouseEvt.dwFlags = TME_LEAVE;
	trackMouseEvt.hwndTrack = pYBCtrlWndMsg->m_hWnd;
	trackMouseEvt.dwHoverTime = HOVER_DEFAULT;
	//
	::TrackMouseEvent ( &trackMouseEvt );
	pView4WSAWMenuContent = (YBView4InstallFileContent*)getViewContentObj();
	if ( pView4WSAWMenuContent ) {
		if ( pView4WSAWMenuContent->moveTest( ptInC, uItemIndexMove ) ) {
			pView4WSAWMenuContent->setSelItem( uItemIndexMove );		
		} else {
			pView4WSAWMenuContent->setItemInvalid( YBWSAWMENU_INDEX_INVALID );
		}
		//
		if ( pView4WSAWMenuContent->moveHitTest( ptInC, uItemIndexMoveIn ) ) {
			pView4WSAWMenuContent->setSelInItem( uItemIndexMoveIn );
		} else {
			pView4WSAWMenuContent->setInItemInvalid( YBWSAWMENU_INDEX_INVALID );
		}
	}
}

void YBView4InstallFileInner::_onWM_MOUSELEAVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSELEAVE );

	YBView4InstallFileContent* pView4WSAWMenuContent = NULL;
	pView4WSAWMenuContent = (YBView4InstallFileContent*)getViewContentObj();

	for ( unsigned int index = 0; index < pView4WSAWMenuContent->getItemCount(); ++index ) {
		pView4WSAWMenuContent->setItemInvalid( YBWSAWMENU_INDEX_INVALID );
		pView4WSAWMenuContent->setInItemInvalid( YBWSAWMENU_INDEX_INVALID );
	}
	::InvalidateRect( m_hWnd, NULL, FALSE );
}

//////////////////////////////////////////////////////////////////////////

YBView4InstallFileContent::YBView4InstallFileContent( unsigned int uContentWidth, unsigned int uItemHeight, ybctrl::YBViewInner* pViewInner )
	: m_pViewInner( pViewInner )
	, m_uWidth( uContentWidth )
	, m_uItemHeight( uItemHeight )
	, m_pMemDC( new ybctrl::YBCtrlMemDC( m_uWidth, YBView4InstallFileContent::getHeight() ) )
	, m_hBrushItemOdd( NULL )
	, m_hBrushItemEven( NULL )
	, m_hBrushItemSel( NULL )
	, m_hBrushItemBK( NULL )
	, m_uSelItemIndex( YBWSAWMENU_INDEX_INVALID )
	, m_uSelInItemIndex( YBWSAWMENU_INDEX_INVALID )
	, m_tstrIcon( _T("进入") )
	, m_uFontIdItem( YBCTRL_FONT_ID_INVALID )
{
	_loadFont();
	m_hBrushItemOdd = ::CreateSolidBrush( COLOR_YBLB4WSAWMENU_ODD_ITEMBK_DEFAULT );
	m_hBrushItemEven = ::CreateSolidBrush( COLOR_YBLB4WSAWMENU_EVEN_ITEMBK_DEFAULT );
	m_hBrushItemSel = ::CreateSolidBrush( COLOR_YBLB4WSAWMENU_SELECTED_ITEMBK_DEFAULT );
	m_hBrushItemBK = ::CreateSolidBrush( COLOR_YBLB4WSAWMENU_ITEMBK_DEFAULT );
}

YBView4InstallFileContent::~YBView4InstallFileContent() {
	if ( m_hBrushItemSel ) {
		::DeleteObject( m_hBrushItemSel );
		m_hBrushItemSel = NULL;
	}

	if ( m_hBrushItemBK ) {
		::DeleteObject( m_hBrushItemBK );
		m_hBrushItemBK = NULL;
	}
	if ( m_hBrushItemEven ) {
		::DeleteObject( m_hBrushItemEven );
		m_hBrushItemEven = NULL;
	}
	if ( m_hBrushItemOdd ) {
		::DeleteObject( m_hBrushItemOdd );
		m_hBrushItemOdd = NULL;
	}

	if ( m_pMemDC ) {
		delete m_pMemDC;
		m_pMemDC = NULL;
	}
}

void YBView4InstallFileContent::clearAllItem() {
	//
	_clearAllItem();
	//
	m_uSelItemIndex = YBWSAWMENU_INDEX_INVALID;
}

void YBView4InstallFileContent::_clearAllItem() {
	listboxitem_container_type containerListBoxItemsNeedDel;
	listboxitem_container_type::iterator itLBItem, iendLBItem;
	YBListBoxInstallFileItem* pLBItem = NULL;

	containerListBoxItemsNeedDel = m_containerListBoxItems;
	m_containerListBoxItems.clear();
	//
	iendLBItem = containerListBoxItemsNeedDel.end();
	for ( itLBItem = containerListBoxItemsNeedDel.begin(); itLBItem!=iendLBItem; ++itLBItem ) {
		pLBItem = (*itLBItem);
		delete pLBItem;
		pLBItem = NULL;
	}
}

unsigned int YBView4InstallFileContent::getWidth() const {
	return m_uWidth;
}

unsigned int YBView4InstallFileContent::getHeight() const {
	return m_uItemHeight * m_containerListBoxItems.size();
}

void YBView4InstallFileContent::_drawStatus( HDC hDC, ybctrl::YBViewInner* pYBViewInner ) const {
	if ( !pYBViewInner )
		return;
	RECT rcDestInContent;
	RECT rcDestInView;

	if ( !pYBViewInner->getContentUpdateRect( rcDestInContent, rcDestInView ) )
		return;
	//
	m_pMemDC->resize( getWidth(), getHeight() );
	_drawBK( m_pMemDC->getMemDC(), rcDestInContent, pYBViewInner );
	_drawAllItem( m_pMemDC->getMemDC(), rcDestInContent, pYBViewInner );

	::BitBlt( hDC, rcDestInView.left, rcDestInView.top, (rcDestInView.right - rcDestInView.left), (rcDestInView.bottom - rcDestInView.top),
		m_pMemDC->getMemDC(), rcDestInContent.left, rcDestInContent.top, SRCCOPY );

}

void YBView4InstallFileContent::_drawBK( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner ) const{
	HBRUSH hBrushBK = NULL;
	RECT rcCInC;

	::SetRect( &rcCInC, 0, 0, getWidth(), getHeight() );
	::FillRect( hDC, &rcCInC, m_hBrushItemBK );
}

void YBView4InstallFileContent::_drawAllItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner ) const {
	if ( m_containerListBoxItems.size() == 0 )
		return;
	unsigned int uItemCount;

	uItemCount = m_containerListBoxItems.size();
	for ( unsigned int uItemIndex = 0; uItemIndex<uItemCount; ++uItemIndex ) {
		_drawItem( hDC, rcNeedShowInContent, pYBViewInner, uItemIndex );
	}
}

void YBView4InstallFileContent::_drawItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner, unsigned int uItemIndex ) const {
	assert( uItemIndex < m_containerListBoxItems.size() );
	RECT rcItem;
	RECT rcDest;
	YBListBoxInstallFileItem* pItemMenu = NULL;

	if ( !_getItemRect( uItemIndex, rcItem ) ) {
		return;
	}

	//
	pItemMenu = m_containerListBoxItems[ uItemIndex ];
	if ( !::IntersectRect( &rcDest, &rcItem, &rcNeedShowInContent ) ) {
		return; // do not intersect
	} 

	// draw item bk
	rcItem.bottom -= 1;
	if ( m_uSelItemIndex == uItemIndex ) {
		::FillRect( hDC, &rcItem, m_hBrushItemSel );
	} else {
		::FillRect( hDC, &rcItem, (uItemIndex%2) ? m_hBrushItemEven : m_hBrushItemEven );
	}

	// drawItemText
	_drawItemText( hDC, rcItem, uItemIndex );
	// drawIcon
	//_drawIcon( hDC, uItemIndex );
	// drawNumber 123456789
	_drawItemNumber( hDC, rcItem, uItemIndex );
	// drawIconText “进入”
	//_drawIconText( hDC, rcItem, uItemIndex );
	// move check box button.
	RECT rcBtnChkInV;
	_getItemBtnChkRect( rcItem, rcBtnChkInV );
	::MoveWindow( pItemMenu->m_pBtnChk->getHWND(), rcBtnChkInV.left, rcBtnChkInV.top, 
		( rcBtnChkInV.right - rcBtnChkInV.left ), ( rcBtnChkInV.bottom - rcBtnChkInV.top ), TRUE );
	::ShowWindow( pItemMenu->m_pBtnChk->getHWND(), SW_NORMAL );

}

bool YBView4InstallFileContent::_getItemBtnChkRect( const RECT& rcItemInC, RECT& rcBtnChkInV ) const {
	RECT rcItemInV;

	if ( !m_pViewInner->getRectContent2View( rcItemInC, rcItemInV ) ) {
		return false;
	}
	rcBtnChkInV.left = rcItemInV.left + 10;
	rcBtnChkInV.right = rcBtnChkInV.left + 23;
	rcBtnChkInV.top = rcItemInC.top;
	rcBtnChkInV.bottom = rcItemInC.bottom;
	return true;
}

void YBView4InstallFileContent::_drawIcon( HDC hDC, unsigned int uItemIndex ) const {
	ybctrl::YBCtrlImageStatic* pYBImage = NULL;
	YBListBoxInstallFileItem* pListBoxItem = NULL;
	RECT rcHit;

	pListBoxItem = m_containerListBoxItems[uItemIndex];

	if ( m_uSelInItemIndex == uItemIndex ) {
		pYBImage = _getImageStatic( pListBoxItem->m_uImageIdHov );
	} else{
		pYBImage = _getImageStatic( pListBoxItem->m_uImageIdNor );
	}

	if ( !getHitRect( uItemIndex, rcHit ) ) {
		assert( false );
		return;
	}

	Gdiplus::Graphics graphics( hDC );
	GdipBitBlt( &graphics, pYBImage->getImage(), rcHit.left, rcHit.top, rcHit.right - rcHit.left, rcHit.bottom - rcHit.top
		, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
}

ybctrl::YBCtrlImageStatic* YBView4InstallFileContent::_getImageStatic( unsigned int uImageId ) const{
	YBCtrlImageBase* pImageBase = NULL;
	YBCtrlImageStatic* pImageStatic = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pImageBase ) )
		return NULL;
	if( !pImageBase || ( pImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pImageBase;
}

void YBView4InstallFileContent::setWidth( unsigned int uWidthNew ) {
	if ( m_uWidth == uWidthNew )
		return;
	m_uWidth = uWidthNew;
}

void YBView4InstallFileContent::_drawItemText( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const{
	HFONT hFontOld = NULL;
	const YBListBoxInstallFileItem* pListBoxItem = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcItemText;

	_getContentRect( uItemIndex, rcItemText );
	pListBoxItem = m_containerListBoxItems[ uItemIndex ];
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdItem );
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

		::DrawText( hDC, pListBoxItem->m_tstrItemContent.c_str(), pListBoxItem->m_tstrItemContent.length(), &rcItemText, DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}

}

void YBView4InstallFileContent::_drawIconText( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const {
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcIcon;

	getHitRect( uItemIndex, rcIcon );
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdItem );
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

		::DrawText( hDC, m_tstrIcon.c_str(), m_tstrIcon.length(), &rcIcon, DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

void YBView4InstallFileContent::_drawItemNumber( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const {
	HFONT hFontOld = NULL;
	const YBListBoxInstallFileItem* pListBoxItem = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcNumber;
	TCHAR szMsgBoxNumber[10];

	_stprintf_s( szMsgBoxNumber, _T("%d"), uItemIndex + 1 );
	_getNumberRect( uItemIndex, rcNumber );
	pListBoxItem = m_containerListBoxItems[ uItemIndex ];
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdItem );
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

		::DrawText( hDC, szMsgBoxNumber, _tcslen(szMsgBoxNumber), &rcNumber, DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

bool YBView4InstallFileContent::_getItemRect( unsigned int uIndex, RECT& rcItem ) const {
	if ( uIndex >= m_containerListBoxItems.size() )
		return false;
	rcItem.left = 0;
	rcItem.right = getWidth();
	rcItem.top = uIndex*m_uItemHeight;
	rcItem.bottom = ( uIndex + 1 )*m_uItemHeight;
	return true;
}


bool YBView4InstallFileContent::_getNumberRect( unsigned int uIndex, RECT& rcNumber ) const {
	if ( uIndex >= m_containerListBoxItems.size() )
		return false;
	RECT rcClient;

	::GetClientRect( m_pViewInner->getHWND(), &rcClient );
	rcNumber.left = 100;
	rcNumber.right = rcNumber.left + 55*( rcClient.right - rcClient.left )/280;
	rcNumber.top = uIndex*m_uItemHeight + 2;
	rcNumber.bottom = ( uIndex + 1 )*m_uItemHeight - 2;

	return true;
}
bool YBView4InstallFileContent::_getContentRect( unsigned int uIndex, RECT& rcContent ) const {
	if ( uIndex >= m_containerListBoxItems.size() )
		return false;
	RECT rcClient;

	::GetClientRect( m_pViewInner->getHWND(), &rcClient );
	rcContent.left = 100 + rcClient.left + 55*( rcClient.right - rcClient.left )/280;
	rcContent.right = rcContent.left + 17*( rcClient.right - rcClient.left )/28;
	rcContent.top = uIndex*m_uItemHeight + 2;
	rcContent.bottom = ( uIndex + 1 )*m_uItemHeight - 2;

	return true;
}

bool YBView4InstallFileContent::getHitRect( unsigned int uIndex, RECT& rcHit ) const {
	RECT rcClient;

	::GetClientRect( m_pViewInner->getHWND(), &rcClient );
	if ( uIndex >= m_containerListBoxItems.size() )
		return false;
	rcHit.left = rcClient.left + 23*( rcClient.right - rcClient.left )/28;
	rcHit.right = rcHit.left + 44*( rcClient.right - rcClient.left )/280;
	rcHit.top = uIndex*m_uItemHeight + 2;
	rcHit.bottom = ( uIndex + 1 )*m_uItemHeight - 2;

	return true;
}

bool YBView4InstallFileContent::insertItem( unsigned int uIndexItem, YBListBoxInstallFileItem* pListBoxItem, item_id_type& itemId ) {
	if (!m_pViewInner ||
		!pListBoxItem || 
		!::IsWindow( m_pViewInner->getHWND() ) ||
		!pListBoxItem->m_pBtnChk ||
		uIndexItem > m_containerListBoxItems.size() ) {
			assert( false );
			return false;
	}
	//
	itemId = _allocalItemId();
	pListBoxItem->_setItemId( itemId );
	//
	if ( !pListBoxItem->m_pBtnChk->Create( 0, WS_CHILD, 0, 0, 0, 0, m_pViewInner->getHWND(), (HMENU)pListBoxItem->m_wBtnChkCtrlId, NULL ) ) {
		assert( false );
		return false;
	}
	//
	m_containerListBoxItems.insert( m_containerListBoxItems.begin() + uIndexItem, pListBoxItem );

	return true;
}

bool YBView4InstallFileContent::push_front( YBListBoxInstallFileItem* pListBoxItem, item_id_type& itemId ) {
	if (!m_pViewInner ||
		!pListBoxItem || 
		!::IsWindow( m_pViewInner->getHWND() ) ){ 
			assert( false );
			return false;
	}

	if ( !insertItem( 0, pListBoxItem, itemId ) ) {
		assert( false );
		return false;
	}

	return true;
}

void YBView4InstallFileContent::_loadFont(){
	LOGFONT lFont;

	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 20;

	m_uFontIdItem = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 8, 43, 77 ) );
}

bool YBView4InstallFileContent::moveTest( const POINT& ptInContent, unsigned int& uIndexItem ) {
	int nItemCount = m_containerListBoxItems.size();
	RECT rcItem;

	for ( int i = 0; i!=nItemCount; ++i ) {
		if ( !_getItemRect( i, rcItem ) )
			return false;
		if ( ::PtInRect( &rcItem, ptInContent ) ) {
			uIndexItem = i;
			return true;
		}
	}

	return false;
}

bool YBView4InstallFileContent::moveHitTest( const POINT& ptInContent, unsigned int& uIndexItem ) {
	int nItemCount = m_containerListBoxItems.size();
	RECT rcHit;

	for ( int i = 0; i!=nItemCount; ++i ) {
		if ( !getHitRect( i, rcHit ) )
			return false;
		if ( ::PtInRect( &rcHit, ptInContent ) ) {
			uIndexItem = i;
			return true;
		}
	}

	return false;
}

bool YBView4InstallFileContent::setSelItem( unsigned int uIndexItem ) {
	if ( uIndexItem >= m_containerListBoxItems.size() ) {
		assert( false );
		return false;
	}
	//
	m_uSelItemIndex = uIndexItem;
	//
	if ( ::IsWindow( m_pViewInner->getHWND() ) ) {
		::InvalidateRect( m_pViewInner->getHWND(), NULL, FALSE );
	}
	return true;
}

void YBView4InstallFileContent::setItemInvalid( unsigned int uIndexItem ) {
	m_uSelItemIndex = uIndexItem;
}

bool YBView4InstallFileContent::setSelInItem( unsigned int uIndexItem ) {
	if ( uIndexItem >= m_containerListBoxItems.size() ) {
		assert( false );
		return false;
	}
	//
	m_uSelInItemIndex = uIndexItem;
	//
	if ( ::IsWindow( m_pViewInner->getHWND() ) ) {
		::InvalidateRect( m_pViewInner->getHWND(), NULL, FALSE );
	}
	return true;
}

void YBView4InstallFileContent::setInItemInvalid( unsigned int uIndexItem ) {
	m_uSelInItemIndex = uIndexItem;
}

void YBView4InstallFileContent::deleteMenuItemByIndex( unsigned int uIndex ) {
	YBListBoxInstallFileItem* pLB4WSAWMenuItem = NULL;
	YBListBoxInstallFileItem* pLB4WSAWMenuItemTmp = NULL;

	pLB4WSAWMenuItem = m_containerListBoxItems[ uIndex ];
	if( !pLB4WSAWMenuItem ) {
		assert( false );
		return;
	}

	listboxitem_container_type::iterator itLB4WSAWItem, iendLB4WSAWItem;
	iendLB4WSAWItem = m_containerListBoxItems.end();
	for ( itLB4WSAWItem = m_containerListBoxItems.begin(); itLB4WSAWItem != iendLB4WSAWItem; ++itLB4WSAWItem ) {
		pLB4WSAWMenuItemTmp = *itLB4WSAWItem;
		if ( pLB4WSAWMenuItemTmp == pLB4WSAWMenuItem ) {
			delete pLB4WSAWMenuItem;
			m_containerListBoxItems.erase( itLB4WSAWItem );
			break;
		}
	}
}

void YBView4InstallFileContent::deleteMenuItemById( unsigned int uItemId ) {
	if ( !_hasItemId( uItemId ) ) {
		assert( false );
		return;
	}
	YBListBoxInstallFileItem* pLB4WSAWMenuItem = NULL;
	listboxitem_container_type::iterator itLB4WSAWItem, iendLB4WSAWItem;

	iendLB4WSAWItem = m_containerListBoxItems.end();
	for ( itLB4WSAWItem = m_containerListBoxItems.begin(); itLB4WSAWItem != iendLB4WSAWItem; ++itLB4WSAWItem ) {
		pLB4WSAWMenuItem = *itLB4WSAWItem;
		if ( pLB4WSAWMenuItem->getItemId() == uItemId ) {
			delete pLB4WSAWMenuItem;
			m_containerListBoxItems.erase( itLB4WSAWItem );
			break;
		}
	}

}

YBListBoxInstallFileItem* YBView4InstallFileContent::getItemById( item_id_type itemId ) {
	unsigned int uItemIndex;
	YBListBoxInstallFileItem* pLB4WSAWMenuItem = NULL;

	itemId2Index( itemId, uItemIndex );
	pLB4WSAWMenuItem = m_containerListBoxItems[ uItemIndex ];

	return pLB4WSAWMenuItem;
}

YBView4InstallFileContent::item_id_type YBView4InstallFileContent::_allocalItemId() const {
	static item_id_type s_idBase = YBCTRL_MENU_ID_BASE;
	return ++s_idBase;
}

bool YBView4InstallFileContent::itemId2Index( item_id_type idItem, unsigned int& uIndex ) const {
	if ( m_containerListBoxItems.size() == 0 ) {
		return false;
	}

	listboxitem_container_type::const_iterator citItem, ciendItem;

	ciendItem = m_containerListBoxItems.end();
	for ( citItem = m_containerListBoxItems.begin(); citItem != ciendItem; ++citItem ) {
		if ( (*citItem)->getItemId() == idItem ) {
			uIndex = citItem - m_containerListBoxItems.begin();
			return true;
		}
	}

	return false;
}

bool YBView4InstallFileContent::index2ItemId( unsigned int uIndex, item_id_type& idItem ) const {
	if ( m_containerListBoxItems.size() == 0 ) {
		return false;
	}
	YBListBoxInstallFileItem* pLB4WSAWMenuItem = NULL;

	pLB4WSAWMenuItem = m_containerListBoxItems[ uIndex ];
	idItem = pLB4WSAWMenuItem->getItemId();

	return true;
}

bool YBView4InstallFileContent::hasItemId( unsigned int uItemId ) const {
	return _hasItemId( uItemId );
}

bool YBView4InstallFileContent::_hasItemId( unsigned int uItemId ) const {
	listboxitem_container_type::const_iterator citItem, ciendItem;

	ciendItem = m_containerListBoxItems.end();
	for ( citItem = m_containerListBoxItems.begin(); citItem != ciendItem; ++citItem ) {
		if ( (*citItem)->getItemId() == uItemId ) {
			return true;
		}
	}

	return false;
}