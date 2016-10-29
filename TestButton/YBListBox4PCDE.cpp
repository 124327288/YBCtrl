#include "stdafx.h"
#include "YBListBox4PCDE.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlFontDepository.h"

USING_NAMESPACE_YBCTRL

YBView4PCDEInner::YBView4PCDEInner( unsigned int uContentWidth, unsigned int uItemHeight )
: ybctrl::YBViewInner( new YBView4PCDEContent( uContentWidth, uItemHeight, this ) ) {
	setContentXPosLT( 0 );
	setContentYPosLT( 0 );
}

YBView4PCDEInner::~YBView4PCDEInner() {

}

YBListBoxPCDEItem::YBListBoxPCDEItem()
: m_tstrNum( _T("") )
, m_tstrItem( _T("") )
, m_tstrCfg( _T("") )
, m_tstrRes( _T("") )
, m_tstrNote( _T("") ) {

}

YBListBoxPCDEItem::~YBListBoxPCDEItem() {

}
//////////////////////////////////////////////////////////////////////////
#define COLOR_YBLB4PCDE_ODD_ITEMBK_DEFAULT ( RGB(223, 227, 230) )
#define COLOR_YBLB4PCDE_EVEN_ITEMBK_DEFAULT ( RGB(192, 202, 206) )
#define COLOR_YBLB4PCDE_ITEMBK_DEFAULT ( RGB(255, 255, 255) )
//////////////////////////////////////////////////////////////////////////
YBView4PCDEContent::YBView4PCDEContent( unsigned int uContentWidth, unsigned int uItemHeight, ybctrl::YBViewInner* pViewInner )
: m_pViewInner( pViewInner )
, m_uWidth( uContentWidth )
, m_uItemHeight( uItemHeight )
, m_pMemDC( new YBCtrlMemDC( m_uWidth, YBView4PCDEContent::getHeight() ) ) {
	_loadFont();
	m_hBrushItemEven = ::CreateSolidBrush( COLOR_YBLB4PCDE_EVEN_ITEMBK_DEFAULT );
	m_hBrushItemOdd = ::CreateSolidBrush( COLOR_YBLB4PCDE_ODD_ITEMBK_DEFAULT );
	m_hBrushItemBk = ::CreateSolidBrush( COLOR_YBLB4PCDE_ITEMBK_DEFAULT );
}

YBView4PCDEContent::~YBView4PCDEContent() {
	if ( m_hBrushItemBk ) {
		::DeleteObject( m_hBrushItemBk );
		m_hBrushItemBk = NULL;
	}
	if ( m_hBrushItemEven ) {
		::DeleteObject( m_hBrushItemEven );
		m_hBrushItemEven = NULL;
	}
	if ( m_hBrushItemOdd ) {
		::DeleteObject( m_hBrushItemOdd );
		m_hBrushItemOdd = NULL;
	}
}

void YBView4PCDEContent::clearAllItem() {
	_clearAllItem();
}

void YBView4PCDEContent::_clearAllItem() {
	listboxitem_container_type::iterator itLBItem, iendLBItem;
	YBListBoxPCDEItem* pListBoxItem = NULL;

	iendLBItem = m_containerListboxItems.end();
	for ( itLBItem = m_containerListboxItems.begin(); itLBItem != iendLBItem; ++itLBItem ) {
		pListBoxItem = (*itLBItem);
		delete pListBoxItem;
		pListBoxItem = NULL;
	}
	m_containerListboxItems.clear();
}

void YBView4PCDEContent::insertItem( unsigned int uIndexItem, YBListBoxPCDEItem* pListBoxItem ) {
	if ( !pListBoxItem || !m_pViewInner ||
		 !::IsWindow( m_pViewInner->getHWND() ) ||
		 uIndexItem > m_containerListboxItems.size() ) {
		assert( false );
		return;
	}
	//
	m_containerListboxItems.insert( m_containerListboxItems.begin() + uIndexItem, pListBoxItem );
}

unsigned int YBView4PCDEContent::getWidth() const {
	return m_uWidth;
}

unsigned int YBView4PCDEContent::getHeight() const {
	return m_uItemHeight*m_containerListboxItems.size();
}

void YBView4PCDEContent::_drawStatus( HDC hDC, ybctrl::YBViewInner* pViewInner ) const {
	if ( !pViewInner )
		return;
	RECT rcDestInContent;
	RECT rcDestInView;

	if ( !pViewInner->getContentUpdateRect( rcDestInContent, rcDestInView ) )
		return;
	//
	m_pMemDC->resize( getWidth(), getHeight() );
	_drawBk( m_pMemDC->getMemDC(), rcDestInContent, pViewInner );
	_drawAllItem( m_pMemDC->getMemDC(), rcDestInContent, pViewInner );

	::BitBlt( hDC, rcDestInView.left, rcDestInView.top, ( rcDestInView.right - rcDestInView.left ), ( rcDestInView.bottom - rcDestInView.top ),
			  m_pMemDC->getMemDC(), 0, 0, SRCCOPY );
}

void YBView4PCDEContent::setWidth( unsigned int uWidthNew ) {
	if ( m_uWidth == uWidthNew )
		return;
	m_uWidth = uWidthNew;
}

void YBView4PCDEContent::_loadFont() {
	LOGFONT lFont;

	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 21;

	m_uFontIdItem = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB(1, 75, 122) );
}

void YBView4PCDEContent::_drawBk( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pViewInner ) const {
	HBRUSH hBrushBK=NULL;
	RECT rcCInC;

	::SetRect( &rcCInC, 0, 0, getWidth(), getHeight() );
	::FillRect( hDC, &rcCInC, m_hBrushItemBk );
}

void YBView4PCDEContent::_drawAllItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pViewInner ) const {
	if( m_containerListboxItems.size() == 0 )
	return;
	unsigned int uItemCount;

	uItemCount = m_containerListboxItems.size();
	for( unsigned int uItemIndex=0; uItemIndex < uItemCount; ++uItemIndex ) {
		_drawItem( hDC, rcNeedShowInContent, pViewInner, uItemIndex );
	}
}

void YBView4PCDEContent::_drawItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner, unsigned int uItemIndex ) const {
	RECT rcItem;
	RECT rcDest;
	YBListBoxPCDEItem* pListBoxItem = NULL;

	if ( !_getItemRect( uItemIndex, rcItem ) ) {
		return;
	}

	if( !::IntersectRect( &rcDest, &rcItem, &rcNeedShowInContent ) ) {
		return; // do not intersect
	}

	// draw item bk
	rcItem.bottom -= 1;
	::FillRect( hDC, &rcItem, ( uItemIndex % 2 )?m_hBrushItemEven:m_hBrushItemOdd );
}

bool YBView4PCDEContent::_getItemRect( unsigned int uIndex, RECT& rcItem ) const {
	if ( uIndex >= m_containerListboxItems.size() )
		return false;
	rcItem.left = 0;
	rcItem.right = getWidth();
	rcItem.top = uIndex*m_uItemHeight;
	rcItem.bottom = ( uIndex + 1 )*m_uItemHeight;
	return true;
}