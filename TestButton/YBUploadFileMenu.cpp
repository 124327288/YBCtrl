#include "stdafx.h"
#include "YBUploadFileMenu.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlFontDepository.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlMisc.h"
#include "frame/YBCtrlImageStatic.h"
#include <assert.h>
//
USING_NAMESPACE_YBCTRL

#define COLOR_YBLB4ULFileMENU_ODD_ITEMBK_DEFAULT (RGB(200, 238, 249))
#define COLOR_YBLB4ULFileMENU_EVEN_ITEMBK_DEFAULT (RGB(168, 232, 250))
#define COLOR_YBLB4ULFileMENU_SELECTED_ITEMBK_DEFAULT (RGB(254, 237, 181))
#define COLOR_YBLB4ULFileMENU_ITEMBK_DEFAULT (RGB(255,255,255))
#define COLOR_YBLB4ULFileMENU_VIEWBK_DEFAULT (RGB(122,122,122))

#define YBULFileMENU_INDEX_INVALID (0xFFFFFFFF)
	//
#define YBULFileMENU_FLAG_NONE (0x00000000)
#define YBULFileMENU_FLAG_INHOVER (0x00000001)
	//////////////////////////////////////////////////////////////////////////
YBListBoxULFileMenuItem::YBListBoxULFileMenuItem()
: m_tstrItemContent( _T("") )
, m_tstrItemState( _T("") )
, m_uItemId( YBCTRL_MENU_ID_INVALID )
{

}

YBListBoxULFileMenuItem::~YBListBoxULFileMenuItem() {

}


unsigned int YBListBoxULFileMenuItem::getItemId() const{
	assert( m_uItemId != YBCTRL_MENU_ID_INVALID );
	return m_uItemId;
}

void YBListBoxULFileMenuItem::_setItemId( unsigned int uItemId ){
	assert( m_uItemId == YBCTRL_MENU_ID_INVALID );
	m_uItemId = uItemId;
}

//////////////////////////////////////////////////////////////////////////
#pragma warning( push )
#pragma warning (disable:4355 )
YBView4ULFileMenuInner::YBView4ULFileMenuInner( unsigned int uContentWidth, unsigned int uItemHeight )
	: ybctrl::YBViewInner( new YBView4ULFileMenuContent( uContentWidth, uItemHeight, this ) ) {
		setBkClr( COLOR_YBLB4ULFileMENU_VIEWBK_DEFAULT );
		setContentXPosLT( 0 );
		setContentYPosLT( 0 );
}
#pragma warning( pop )

YBView4ULFileMenuInner::~YBView4ULFileMenuInner() {

}

YBCTRL_WNDMSGMAP_BEGIN( YBView4ULFileMenuInner, YBViewInner )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
	YBCTRL_ONWNDMSG( WM_MOUSELEAVE, _onWM_MOUSELEAVE )
	YBCTRL_WNDMSGMAP_END()

	void YBView4ULFileMenuInner::_onWM_COMMAND(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
		YBCTRLMSG_ENTER_CHECK( WM_COMMAND );

}

void YBView4ULFileMenuInner::_onWM_MOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	POINT ptInV;
	POINT ptInC;
	unsigned int uItemIndexMove;
	TRACKMOUSEEVENT trackMouseEvt;
	YBView4ULFileMenuContent* pView4ULFileMenuContent = NULL;

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
	pView4ULFileMenuContent = (YBView4ULFileMenuContent*)getViewContentObj();
	if ( pView4ULFileMenuContent ) {
		if ( pView4ULFileMenuContent->moveTest( ptInC, uItemIndexMove ) ) {
			pView4ULFileMenuContent->setSelItem( uItemIndexMove );		
		} else {
			pView4ULFileMenuContent->setItemInvalid( YBULFileMENU_INDEX_INVALID );
		}
	}

}

void YBView4ULFileMenuInner::_onWM_MOUSELEAVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSELEAVE );

	YBView4ULFileMenuContent* pView4UPFileMenuContent = NULL;
	pView4UPFileMenuContent = (YBView4ULFileMenuContent*)getViewContentObj();

	for ( unsigned int index = 0; index < pView4UPFileMenuContent->getItemCount(); ++index ) {
		pView4UPFileMenuContent->setItemInvalid( YBULFileMENU_INDEX_INVALID );
	}
	::InvalidateRect( m_hWnd, NULL, FALSE );
}

//////////////////////////////////////////////////////////////////////////

YBView4ULFileMenuContent::YBView4ULFileMenuContent( unsigned int uContentWidth, unsigned int uItemHeight, ybctrl::YBViewInner* pViewInner )
	: m_pViewInner( pViewInner )
	, m_uWidth( uContentWidth )
	, m_uItemHeight( uItemHeight )
	, m_pMemDC( new ybctrl::YBCtrlMemDC( m_uWidth, YBView4ULFileMenuContent::getHeight() ) )
	, m_hBrushItemOdd( NULL )
	, m_hBrushItemEven( NULL )
	, m_hBrushItemSel( NULL )
	, m_hBrushItemBK( NULL )
	, m_uSelItemIndex( YBULFileMENU_INDEX_INVALID )
	, m_uSelInItemIndex( YBULFileMENU_INDEX_INVALID )
	, m_uFontIdItem( YBCTRL_FONT_ID_INVALID )
{
	_loadFont();
	m_hBrushItemOdd = ::CreateSolidBrush( COLOR_YBLB4ULFileMENU_ODD_ITEMBK_DEFAULT );
	m_hBrushItemEven = ::CreateSolidBrush( COLOR_YBLB4ULFileMENU_EVEN_ITEMBK_DEFAULT );
	m_hBrushItemSel = ::CreateSolidBrush( COLOR_YBLB4ULFileMENU_SELECTED_ITEMBK_DEFAULT );
	m_hBrushItemBK = ::CreateSolidBrush( COLOR_YBLB4ULFileMENU_ITEMBK_DEFAULT );
}

YBView4ULFileMenuContent::~YBView4ULFileMenuContent() {
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

void YBView4ULFileMenuContent::clearAllItem() {
	//
	_clearAllItem();
	//
	m_uSelItemIndex = YBULFileMENU_INDEX_INVALID;
}

void YBView4ULFileMenuContent::_clearAllItem() {
	listboxitem_container_type containerListBoxItemsNeedDel;
	listboxitem_container_type::iterator itLBItem, iendLBItem;
	YBListBoxULFileMenuItem* pLBItem = NULL;

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

unsigned int YBView4ULFileMenuContent::getWidth() const {
	return m_uWidth;
}

unsigned int YBView4ULFileMenuContent::getHeight() const {
	return m_uItemHeight * m_containerListBoxItems.size();
}

void YBView4ULFileMenuContent::_drawStatus( HDC hDC, ybctrl::YBViewInner* pYBViewInner ) const {
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

void YBView4ULFileMenuContent::_drawBK( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner ) const{
	HBRUSH hBrushBK = NULL;
	RECT rcCInC;

	::SetRect( &rcCInC, 0, 0, getWidth(), getHeight() );
	::FillRect( hDC, &rcCInC, m_hBrushItemBK );
}

void YBView4ULFileMenuContent::_drawAllItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner ) const {
	if ( m_containerListBoxItems.size() == 0 )
		return;
	unsigned int uItemCount;

	uItemCount = m_containerListBoxItems.size();
	for ( unsigned int uItemIndex = 0; uItemIndex<uItemCount; ++uItemIndex ) {
		_drawItem( hDC, rcNeedShowInContent, pYBViewInner, uItemIndex );
	}
}

void YBView4ULFileMenuContent::_drawItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner, unsigned int uItemIndex ) const {
	assert( uItemIndex < m_containerListBoxItems.size() );
	RECT rcItem;
	RECT rcDest;
	YBListBoxULFileMenuItem* pItemMenu = NULL;

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
		::FillRect( hDC, &rcItem, (uItemIndex%2) ? m_hBrushItemEven : m_hBrushItemOdd );
	}

	// drawItemText
	_drawItemText( hDC, rcItem, uItemIndex );
	// drawIcon
//	_drawIcon( hDC, uItemIndex );
	// drawNumber 123456789
	_drawItemNumber( hDC, rcItem, uItemIndex );
	// drawIconText ¡°½øÈë¡±
	_drawIconText( hDC, rcItem, uItemIndex );
}

ybctrl::YBCtrlImageStatic* YBView4ULFileMenuContent::_getImageStatic( unsigned int uImageId ) const{
	YBCtrlImageBase* pImageBase = NULL;
	YBCtrlImageStatic* pImageStatic = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pImageBase ) )
		return NULL;
	if( !pImageBase || ( pImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pImageBase;
}

void YBView4ULFileMenuContent::setWidth( unsigned int uWidthNew ) {
	if ( m_uWidth == uWidthNew )
		return;
	m_uWidth = uWidthNew;
}

void YBView4ULFileMenuContent::_drawItemText( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const{
	HFONT hFontOld = NULL;
	const YBListBoxULFileMenuItem* pListBoxItem = NULL;
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

void YBView4ULFileMenuContent::_drawIconText( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const {
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcIcon;
	const YBListBoxULFileMenuItem* pListBoxItem = NULL;

	getHitRect( uItemIndex, rcIcon );
	pListBoxItem = m_containerListBoxItems[ uItemIndex ];
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdItem );
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

		::DrawText( hDC, pListBoxItem->m_tstrItemState.c_str(), pListBoxItem->m_tstrItemState.length(), &rcIcon, DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

void YBView4ULFileMenuContent::_drawItemNumber( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const {
	HFONT hFontOld = NULL;
	const YBListBoxULFileMenuItem* pListBoxItem = NULL;
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

bool YBView4ULFileMenuContent::_getItemRect( unsigned int uIndex, RECT& rcItem ) const {
	if ( uIndex >= m_containerListBoxItems.size() )
		return false;
	rcItem.left = 0;
	rcItem.right = getWidth();
	rcItem.top = uIndex*m_uItemHeight;
	rcItem.bottom = ( uIndex + 1 )*m_uItemHeight;
	return true;
}


bool YBView4ULFileMenuContent::_getNumberRect( unsigned int uIndex, RECT& rcNumber ) const {
	if ( uIndex >= m_containerListBoxItems.size() )
		return false;
	RECT rcClient;

	::GetClientRect( m_pViewInner->getHWND(), &rcClient );
	rcNumber.left = 0;
	rcNumber.right = rcNumber.left + 55*( rcClient.right - rcClient.left )/280;
	rcNumber.top = uIndex*m_uItemHeight + 2;
	rcNumber.bottom = ( uIndex + 1 )*m_uItemHeight - 2;

	return true;
}
bool YBView4ULFileMenuContent::_getContentRect( unsigned int uIndex, RECT& rcContent ) const {
	if ( uIndex >= m_containerListBoxItems.size() )
		return false;
	RECT rcClient;

	::GetClientRect( m_pViewInner->getHWND(), &rcClient );
	rcContent.left = rcClient.left + 55*( rcClient.right - rcClient.left )/280;
	rcContent.right = rcContent.left + 17*( rcClient.right - rcClient.left )/28;
	rcContent.top = uIndex*m_uItemHeight + 2;
	rcContent.bottom = ( uIndex + 1 )*m_uItemHeight - 2;

	return true;
}

bool YBView4ULFileMenuContent::getHitRect( unsigned int uIndex, RECT& rcHit ) const {
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

bool YBView4ULFileMenuContent::insertItem( unsigned int uIndexItem, YBListBoxULFileMenuItem* pListBoxItem, item_id_type& itemId ) {
	if (!m_pViewInner ||
		!pListBoxItem || 
		!::IsWindow( m_pViewInner->getHWND() ) ||
		uIndexItem > m_containerListBoxItems.size() ) {
			assert( false );
			return false;
	}
	//
	itemId = _allocalItemId();
	pListBoxItem->_setItemId( itemId );
	//
	m_containerListBoxItems.insert( m_containerListBoxItems.begin() + uIndexItem, pListBoxItem );

	return true;
}

bool YBView4ULFileMenuContent::push_front( YBListBoxULFileMenuItem* pListBoxItem, item_id_type& itemId ) {
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

void YBView4ULFileMenuContent::_loadFont(){
	LOGFONT lFont;

	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 20;

	m_uFontIdItem = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 8, 43, 77 ) );
}

bool YBView4ULFileMenuContent::moveTest( const POINT& ptInContent, unsigned int& uIndexItem ) {
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

bool YBView4ULFileMenuContent::setSelItem( unsigned int uIndexItem ) {
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

void YBView4ULFileMenuContent::setItemInvalid( unsigned int uIndexItem ) {
	m_uSelItemIndex = uIndexItem;
}

void YBView4ULFileMenuContent::deleteMenuItemByIndex( unsigned int uIndex ) {
	YBListBoxULFileMenuItem* pLB4ULFileMenuItem = NULL;
	YBListBoxULFileMenuItem* pLB4ULFileMenuItemTmp = NULL;

	pLB4ULFileMenuItem = m_containerListBoxItems[ uIndex ];
	if( !pLB4ULFileMenuItem ) {
		assert( false );
		return;
	}

	listboxitem_container_type::iterator itLB4ULFileItem, iendLB4ULFileItem;
	iendLB4ULFileItem = m_containerListBoxItems.end();
	for ( itLB4ULFileItem = m_containerListBoxItems.begin(); itLB4ULFileItem != iendLB4ULFileItem; ++itLB4ULFileItem ) {
		pLB4ULFileMenuItemTmp = *itLB4ULFileItem;
		if ( pLB4ULFileMenuItemTmp == pLB4ULFileMenuItem ) {
			delete pLB4ULFileMenuItem;
			m_containerListBoxItems.erase( itLB4ULFileItem );
			break;
		}
	}
}

void YBView4ULFileMenuContent::deleteMenuItemById( unsigned int uItemId ) {
	if ( !_hasItemId( uItemId ) ) {
		assert( false );
		return;
	}
	YBListBoxULFileMenuItem* pLB4ULFileMenuItem = NULL;
	listboxitem_container_type::iterator itLB4ULFileItem, iendLB4ULFileItem;

	iendLB4ULFileItem = m_containerListBoxItems.end();
	for ( itLB4ULFileItem = m_containerListBoxItems.begin(); itLB4ULFileItem != iendLB4ULFileItem; ++itLB4ULFileItem ) {
		pLB4ULFileMenuItem = *itLB4ULFileItem;
		if ( pLB4ULFileMenuItem->getItemId() == uItemId ) {
			delete pLB4ULFileMenuItem;
			m_containerListBoxItems.erase( itLB4ULFileItem );
			break;
		}
	}

}

YBListBoxULFileMenuItem* YBView4ULFileMenuContent::getItemById( item_id_type itemId ) {
	unsigned int uItemIndex;
	YBListBoxULFileMenuItem* pLB4ULFileMenuItem = NULL;

	itemId2Index( itemId, uItemIndex );
	pLB4ULFileMenuItem = m_containerListBoxItems[ uItemIndex ];

	return pLB4ULFileMenuItem;
}

YBView4ULFileMenuContent::item_id_type YBView4ULFileMenuContent::_allocalItemId() const {
	static item_id_type s_idBase = YBCTRL_MENU_ID_BASE;
	return ++s_idBase;
}

bool YBView4ULFileMenuContent::itemId2Index( item_id_type idItem, unsigned int& uIndex ) const {
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

bool YBView4ULFileMenuContent::index2ItemId( unsigned int uIndex, item_id_type& idItem ) const {
	if ( m_containerListBoxItems.size() == 0 ) {
		return false;
	}
	YBListBoxULFileMenuItem* pLB4ULFileMenuItem = NULL;

	pLB4ULFileMenuItem = m_containerListBoxItems[ uIndex ];
	idItem = pLB4ULFileMenuItem->getItemId();

	return true;
}

bool YBView4ULFileMenuContent::hasItemId( unsigned int uItemId ) const {
	return _hasItemId( uItemId );
}

bool YBView4ULFileMenuContent::_hasItemId( unsigned int uItemId ) const {
	listboxitem_container_type::const_iterator citItem, ciendItem;

	ciendItem = m_containerListBoxItems.end();
	for ( citItem = m_containerListBoxItems.begin(); citItem != ciendItem; ++citItem ) {
		if ( (*citItem)->getItemId() == uItemId ) {
			return true;
		}
	}

	return false;
}