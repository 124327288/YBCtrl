#include "stdafx.h"
#include "frame/YBCtrlMemDC.h"
#include "YBListBox4DDFolder.h"
#include "scrollbar/YBScrollBar.h"
#include "frame/YBCtrlFontDepository.h"
#include <assert.h>
//
#define COLOR_YBLB4DDC_ODD_ITEMBK_DEFAULT (RGB(223, 227, 230))
#define COLOR_YBLB4DDC_EVEN_ITEMBK_DEFAULT (RGB(197, 202, 206))
#define COLOR_YBLB4DDC_ITEMBK_DEFAULT (RGB(255,255,255))
//
USING_NAMESPACE_YBCTRL
////////////////////////////////////////////////////////////

YBView4DDFolderInner::YBView4DDFolderInner()
: ybctrl::YBViewInner( new YBView4DDFolderContent() ){
	setContentXPosLT( 0 );
	setContentYPosLT( 0 );
}

YBView4DDFolderInner::~YBView4DDFolderInner(){

}

YBCTRL_WNDMSGMAP_BEGIN( YBView4DDFolderInner, YBViewInner )
YBCTRL_WNDMSGMAP_END()

////////////////////////////////////////////////////////////

YBView4DDFolderContent::YBView4DDFolderContent()
: m_uWidth ( 520 )
, m_uItemHeight ( 26 )
, m_pMemDC( new ybctrl::YBCtrlMemDC( m_uWidth, YBView4DDFolderContent::getHeight() ) )
, m_hBrushItemOdd( NULL )
, m_hBrushItemEven( NULL )
{
	_loadFont();
	m_hBrushItemOdd = ::CreateSolidBrush( COLOR_YBLB4DDC_ODD_ITEMBK_DEFAULT );
	m_hBrushItemEven = ::CreateSolidBrush( COLOR_YBLB4DDC_EVEN_ITEMBK_DEFAULT );
	m_hBrushItemBK = ::CreateSolidBrush( COLOR_YBLB4DDC_ITEMBK_DEFAULT );
	//
	YBListBoxFolderDDItem* pListBoxItem = NULL;
	TCHAR szMsgBox[100];
	for ( int i = 0; i<10; ++i ) {
		pListBoxItem = new YBListBoxFolderDDItem();
		_stprintf_s( szMsgBox, _T("%d ¡¢ ´ó¸ç´ó"), i );
		pListBoxItem->m_tstrItemText = szMsgBox;
		pListBoxItem->m_wBtnCtrlId = 20;
		this->insertItem( i, pListBoxItem );
	}

}

YBView4DDFolderContent::~YBView4DDFolderContent(){
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

void YBView4DDFolderContent::_clearAllItem() {
	listboxitem_container_type::iterator itLBItem, iendLBItem;
	YBListBoxFolderDDItem* pLBItem = NULL;

	iendLBItem = m_containerListBoxItems.end();
	for ( itLBItem = m_containerListBoxItems.begin(); itLBItem!=iendLBItem; ++itLBItem ) {
		pLBItem = (*itLBItem);
		delete pLBItem;
		pLBItem = NULL;
	}
	m_containerListBoxItems.clear();
}

void YBView4DDFolderContent::insertItem( unsigned int uIndexItem, YBListBoxFolderDDItem* pListBoxItem ) {
	if ( uIndexItem > m_containerListBoxItems.size() ) {
		assert( false );
		return;
	}
	m_containerListBoxItems.insert( m_containerListBoxItems.begin() + uIndexItem, pListBoxItem );
}

unsigned int YBView4DDFolderContent::getWidth() const {
	return m_uWidth;
}

unsigned int YBView4DDFolderContent::getHeight() const {
	return m_uItemHeight * m_containerListBoxItems.size();
}

void YBView4DDFolderContent::_drawBK( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner ) const{
	HBRUSH hBrushBK = NULL;
	RECT rcCInC;

	::SetRect( &rcCInC, 0, 0, getWidth(), getHeight() );
	::FillRect( hDC, &rcCInC, m_hBrushItemBK );
}

bool YBView4DDFolderContent::_getItemRect( unsigned int uIndex, RECT& rcItem ) const {
	if ( uIndex >= m_containerListBoxItems.size() )
		return false;
	rcItem.left = 0;
	rcItem.right = getWidth();
	rcItem.top = uIndex*m_uItemHeight;
	rcItem.bottom = ( uIndex + 1 )*m_uItemHeight;
	return true;
}

void YBView4DDFolderContent::_drawAllItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner ) const {
	if ( m_containerListBoxItems.size() == 0 )
		return;
	unsigned int uItemCount;

	uItemCount = m_containerListBoxItems.size();
	for ( unsigned int uItemIndex = 0; uItemIndex<uItemCount; ++uItemIndex ) {
		_drawItem( hDC, rcNeedShowInContent, pYBViewInner, uItemIndex );
	}
}

void YBView4DDFolderContent::_drawItemText( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const{
	HFONT hFontOld = NULL;
	YBListBoxFolderDDItem* pListBoxItem = NULL;
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

void YBView4DDFolderContent::_drawItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner, unsigned int uItemIndex ) const {
	RECT rcItem;
	RECT rcDest;
	YBListBoxFolderDDItem* pItemFolder = NULL;

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

void YBView4DDFolderContent::setItemBkClr( COLORREF clrOdd, COLORREF clrEven ) {
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

void YBView4DDFolderContent::_drawStatus( HDC hDC, YBViewInner* pYBViewInner ) const {
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

void YBView4DDFolderContent::setWidth( unsigned int uWidthNew ) {
	if ( m_uWidth == uWidthNew )
		return;
	m_uWidth = uWidthNew;
}

void YBView4DDFolderContent::_loadFont(){
	LOGFONT lFont;

	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ") );
	lFont.lfHeight = 22;

	m_uFontIdItem = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 1, 75, 122 ) );
}


