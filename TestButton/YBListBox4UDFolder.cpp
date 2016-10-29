#include "stdafx.h"
#include "YBListBox4UDFolder.h"
#include "frame/YBCtrlMemDC.h"
#include "scrollbar/YBNWScrollBar.h"
#include "frame/YBCtrlFontDepository.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame\YBCtrlDefs.h"
#include "button\YBButton.h"
#include "checkbox\YBCheckBox.h"
#include <assert.h>
//
#define COLOR_YBLB4UDC_ODD_ITEMBK_DEFAULT (RGB(223, 227, 230))
#define COLOR_YBLB4UDC_EVEN_ITEMBK_DEFAULT (RGB(197, 202, 206))
#define COLOR_YBLB4UDC_ITEMBK_DEFAULT (RGB(255,255,255))
//
#define YBLB4UDFOLDER_FLAG_NONE (0)
USING_NAMESPACE_YBCTRL;
//////////////////////////////////////////////////////////////////////////
YBListBoxFolderItem::YBListBoxFolderItem()
: m_tstrItemText( _T("") )
, m_bCheckStatus( false )
, m_uImageId( YBCTRL_IMAGE_ID_INVALID )
, m_wBtnCtrlId( 0 )
, m_pBtnBrowser( new YBButton() )
, m_pBtnChk( new YBCheckBox() ) {

}

YBListBoxFolderItem::~YBListBoxFolderItem() {
	if ( m_pBtnChk ) {
		delete m_pBtnChk;
		m_pBtnChk = NULL;
	}
	if ( m_pBtnBrowser ) {
		delete m_pBtnBrowser;
		m_pBtnBrowser = NULL;
	}
}
//////////////////////////////////////////////////////////////////////////
#pragma warning( push )
#pragma warning (disable:4355 )
YBView4UDFolderInner::YBView4UDFolderInner()
: ybctrl::YBViewInner( new YBView4UDFolderContent( this ) )
{
	setContentXPosLT( 0 );
	setContentYPosLT( 0 );
}
#pragma warning( pop )

YBView4UDFolderInner::~YBView4UDFolderInner() {

}

YBCTRL_WNDMSGMAP_BEGIN( YBView4UDFolderInner, YBViewInner )
YBCTRL_WNDMSGMAP_END()

//////////////////////////////////////////////////////////////////////////
YBView4UDFolderContent::YBView4UDFolderContent( ybctrl::YBViewInner* pViewInner )
: m_pViewInner( pViewInner )
, m_uWidth( 520 )
, m_uItemHeight( 26 )
, m_pMemDC( new ybctrl::YBCtrlMemDC( m_uWidth, YBView4UDFolderContent::getHeight() ) )
, m_hBrushItemOdd( NULL )
, m_hBrushItemEven( NULL )
{
	_loadFont();
	//
	m_hBrushItemOdd = ::CreateSolidBrush( COLOR_YBLB4UDC_ODD_ITEMBK_DEFAULT );
	m_hBrushItemEven = ::CreateSolidBrush( COLOR_YBLB4UDC_EVEN_ITEMBK_DEFAULT );
	m_hBrushItemBK = ::CreateSolidBrush( COLOR_YBLB4UDC_ITEMBK_DEFAULT );
}

YBView4UDFolderContent::~YBView4UDFolderContent() {
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

void YBView4UDFolderContent::clearAllItem() {
	_clearAllItem();
}

void YBView4UDFolderContent::_clearAllItem() {
	listboxitem_container_type::iterator itLBItem, iendLBItem;
	YBListBoxFolderItem* pLBItem = NULL;

	iendLBItem = m_containerListBoxItems.end();
	for ( itLBItem = m_containerListBoxItems.begin(); itLBItem!=iendLBItem; ++itLBItem ) {
		pLBItem = (*itLBItem);
		delete pLBItem;
		pLBItem = NULL;
	}
	m_containerListBoxItems.clear();
}

void YBView4UDFolderContent::insertItem( unsigned int uIndexItem, YBListBoxFolderItem* pListBoxItem ) {
	if ( !pListBoxItem 
		|| !m_pViewInner || !::IsWindow( m_pViewInner->getHWND() )
		|| uIndexItem > m_containerListBoxItems.size() ) {
		assert( false );
		return;
	}
	// 

	//
	m_containerListBoxItems.insert( m_containerListBoxItems.begin() + uIndexItem, pListBoxItem );
}

unsigned int YBView4UDFolderContent::getWidth() const {
	return m_uWidth;
}

unsigned int YBView4UDFolderContent::getHeight() const {
	return m_uItemHeight * m_containerListBoxItems.size();
}

void YBView4UDFolderContent::_drawBK( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner ) const{
	HBRUSH hBrushBK = NULL;
	RECT rcCInC;

	::SetRect( &rcCInC, 0, 0, getWidth(), getHeight() );
	::FillRect( hDC, &rcCInC, m_hBrushItemBK );
}

bool YBView4UDFolderContent::_getItemRect( unsigned int uIndex, RECT& rcItem ) const {
	if ( uIndex >= m_containerListBoxItems.size() )
		return false;
	rcItem.left = 0;
	rcItem.right = getWidth();
	rcItem.top = uIndex*m_uItemHeight;
	rcItem.bottom = ( uIndex + 1 )*m_uItemHeight;
	return true;
}

void YBView4UDFolderContent::_drawAllItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner ) const {
	if ( m_containerListBoxItems.size() == 0 )
		return;
	unsigned int uItemCount;
	
	uItemCount = m_containerListBoxItems.size();
	for ( unsigned int uItemIndex = 0; uItemIndex<uItemCount; ++uItemIndex ) {
		_drawItem( hDC, rcNeedShowInContent, pYBViewInner, uItemIndex );
	}
}

void YBView4UDFolderContent::_drawItemText( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const{
	HFONT hFontOld = NULL;
	YBListBoxFolderItem* pListBoxItem = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;

	pListBoxItem = m_containerListBoxItems[ uItemIndex ];
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdItem );
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

		::DrawText( hDC, pListBoxItem->m_tstrItemText.c_str(), pListBoxItem->m_tstrItemText.length(), &rcItem, DT_LEFT|DT_VCENTER );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}

}

void YBView4UDFolderContent::_drawItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner, unsigned int uItemIndex ) const {
	RECT rcItem;
	RECT rcDest;
	YBListBoxFolderItem* pItemFolder = NULL;

	if ( !_getItemRect( uItemIndex, rcItem ) ) {
		return;
	}

	if ( !::IntersectRect( &rcDest, &rcItem, &rcNeedShowInContent ) ) {
		return; // do not intersect
	}

	// draw item bk
	rcItem.bottom -= 1;
	::FillRect( hDC, &rcItem, (uItemIndex%2) ? m_hBrushItemEven : m_hBrushItemOdd );
	
	_drawItemText( hDC, rcItem, uItemIndex );
// 	// 
// 	m_containerListBoxItems[ uItemIndex ].m_tstrItemText = _T("XXX");
}

void YBView4UDFolderContent::setItemBkClr( COLORREF clrOdd, COLORREF clrEven ) {
	//
	if ( m_hBrushItemOdd ) {
		::DeleteObject( m_hBrushItemOdd );
		m_hBrushItemOdd = NULL;
	}
	m_hBrushItemOdd = ::CreateSolidBrush( clrOdd );
	//
	if ( m_hBrushItemEven ) {
		::DeleteObject( m_hBrushItemEven );
		m_hBrushItemEven = NULL;
	}
	m_hBrushItemEven = ::CreateSolidBrush( clrEven );

}

bool YBView4UDFolderContent::HitTest( const POINT& ptInContent, unsigned int& uIndexItem ) {
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

void YBView4UDFolderContent::_drawStatus( HDC hDC, YBViewInner* pYBViewInner ) const {
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

void YBView4UDFolderContent::setWidth( unsigned int uWidthNew ) {
	if ( m_uWidth == uWidthNew )
		return;
	m_uWidth = uWidthNew;
}

void YBView4UDFolderContent::_loadFont(){
	LOGFONT lFont;

	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 22;

	m_uFontIdItem = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 1, 75, 122 ) );
}
