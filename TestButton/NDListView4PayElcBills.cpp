#include "stdafx.h"
#include "frame/YBCtrlMemDC.h"
#include "NDListView4PayElcBills.h"
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

NDView4PayElcBillInner::NDView4PayElcBillInner()
: ybctrl::YBViewInner(new NDView4PayElcBillContent()){
	setContentXPosLT(0);
	setContentYPosLT(0);
}

NDView4PayElcBillInner::~NDView4PayElcBillInner(){

}

YBCTRL_WNDMSGMAP_BEGIN(NDView4PayElcBillInner, YBViewInner)
YBCTRL_WNDMSGMAP_END()

////////////////////////////////////////////////////////////

NDView4PayElcBillContent::NDView4PayElcBillContent()
: m_uWidth(520)
, m_uItemHeight(26)
, m_pMemDC(new ybctrl::YBCtrlMemDC(m_uWidth, NDView4PayElcBillContent::getHeight()))
, m_hBrushItemOdd(NULL)
, m_hBrushItemEven(NULL)
{
	_loadFont();
	m_hBrushItemOdd = ::CreateSolidBrush(COLOR_YBLB4DDC_ODD_ITEMBK_DEFAULT);
	m_hBrushItemEven = ::CreateSolidBrush(COLOR_YBLB4DDC_EVEN_ITEMBK_DEFAULT);
	m_hBrushItemBK = ::CreateSolidBrush(COLOR_YBLB4DDC_ITEMBK_DEFAULT);
	//
	NDListViewPayElcBillSubItem* pListViewSubItem = NULL;
	NDListViewPayElcBillColumn* pListViewColumn = NULL;
	TCHAR szMsgBox[100];
	for (int nColumn = 0; nColumn < 5; ++nColumn) {
		pListViewColumn = new NDListViewPayElcBillColumn();
		pListViewColumn->m_uIndex = nColumn;
		pListViewColumn->m_uWidth = 40;
		insertColumn(nColumn, pListViewColumn);
		for (int nLine = 0; nLine < 10; ++nLine) {
			pListViewSubItem = new NDListViewPayElcBillSubItem();
			_stprintf_s(szMsgBox, _T("%d_Test"), nLine);
			pListViewSubItem->m_uSubItemIndex = nColumn;
			pListViewSubItem->m_uItemIndex = nLine;
			pListViewSubItem->m_tstrItemText = szMsgBox;
			insertItem( nLine, nColumn, pListViewSubItem );
		}
	}

}

NDView4PayElcBillContent::~NDView4PayElcBillContent(){
	if (m_hBrushItemBK) {
		::DeleteObject(m_hBrushItemBK);
		m_hBrushItemBK = NULL;
	}
	if (m_hBrushItemEven) {
		::DeleteObject(m_hBrushItemEven);
		m_hBrushItemEven = NULL;
	}
	if (m_hBrushItemOdd) {
		::DeleteObject(m_hBrushItemOdd);
		m_hBrushItemOdd = NULL;
	}

	if (m_pMemDC) {
		delete m_pMemDC;
		m_pMemDC = NULL;
	}
}

void NDView4PayElcBillContent::_clearAllItem() {
	index2SubItem_map_type::iterator itIndex2SubItem, iendIndex2SubItem;
	NDListViewPayElcBillSubItem* pListViewItem = NULL;
	NDListViewIndex* pListViewIndex = NULL;

	iendIndex2SubItem = m_mapIndex2SubItem.end();
	for (itIndex2SubItem = m_mapIndex2SubItem.begin(); itIndex2SubItem != iendIndex2SubItem; ++itIndex2SubItem) {
		pListViewItem = itIndex2SubItem->second;
		pListViewIndex = itIndex2SubItem->first;
		if (pListViewIndex) {
			delete pListViewIndex;
			pListViewIndex = NULL;
		}
		if (pListViewItem) {
			delete pListViewItem;
			pListViewItem = NULL;
		}
	}
	m_mapIndex2SubItem.clear();
	m_containerListViewIndex.clear();
	//
	listviewcolumn_container_type::iterator itBegin, itEnd;
	itEnd = m_containerListViewColumns.end();
	for (itBegin = m_containerListViewColumns.begin(); itBegin != itEnd; ++itBegin) {
		NDListViewPayElcBillColumn* pListViewColumn = (*itBegin);
		if (!pListViewColumn)
			continue;
		delete pListViewColumn;
		pListViewColumn = NULL;
	}
	m_containerListViewColumns.clear();
}

void NDView4PayElcBillContent::insertItem(unsigned int uIndexItem, unsigned int uIndexSubItem, NDListViewPayElcBillSubItem* pListViewItem) {
	unsigned int uLineCount = 0;
	unsigned int uColumnCount = 0;

	uColumnCount = m_containerListViewColumns.size();
	if (uColumnCount == 0) {
		assert( false );
		return;
	}
	uLineCount = m_containerListViewIndex.size() / uColumnCount;
	if (uIndexItem > uLineCount || uIndexSubItem > uColumnCount) {
		assert(false);
		return;
	}
	NDListViewIndex* pListViewIndex = new NDListViewIndex();
	pListViewIndex->m_uLineIndex = pListViewItem->m_uItemIndex;
	pListViewIndex->m_uColumnIndex = pListViewItem->m_uSubItemIndex;
	
	m_containerListViewIndex.push_back(pListViewIndex);
	m_mapIndex2SubItem[ pListViewIndex ] = pListViewItem;
}

void NDView4PayElcBillContent::insertColumn(unsigned int uIndexColumn, NDListViewPayElcBillColumn* pListViewColumn) {
	if (uIndexColumn > m_containerListViewColumns.size()) {
		assert(false);
		return;
	}
	m_containerListViewColumns.insert(m_containerListViewColumns.begin() + uIndexColumn, pListViewColumn);
}

unsigned int NDView4PayElcBillContent::getWidth() const {
	return m_uWidth;
}

unsigned int NDView4PayElcBillContent::getHeight() const {
	return m_uItemHeight * _getLineCount();
}

unsigned int NDView4PayElcBillContent::_getLineCount() const {
	unsigned int uColumnCount = 0;

	uColumnCount = m_containerListViewColumns.size();
	if (uColumnCount == 0) {
		return 0;
	}
	return m_containerListViewIndex.size() / uColumnCount;
}

void NDView4PayElcBillContent::_drawBK(HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner) const{
	HBRUSH hBrushBK = NULL;
	RECT rcCInC;

	::SetRect(&rcCInC, 0, 0, getWidth(), getHeight());
	::FillRect(hDC, &rcCInC, m_hBrushItemBK);
}

void NDView4PayElcBillContent::_drawAllItem(HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner) const {
	if (m_mapIndex2SubItem.size() == 0)
		return;
	unsigned int uLineCount = _getLineCount();	
	unsigned int uColumnCount = m_containerListViewColumns.size();

	for (unsigned int uLineIndex = 0; uLineIndex < uLineCount; ++uLineIndex) {
		for (unsigned int uColumnIndex = 0; uColumnIndex < uColumnCount; ++uColumnIndex) {
			_drawSubItem(hDC, rcNeedShowInContent, pYBViewInner, uLineIndex, uColumnIndex);
		}
	}
}

void NDView4PayElcBillContent::_drawSubItemText(HDC hDC, RECT& rcSubItem, unsigned int uItemIndex, unsigned int uSubItemIndex) const{
	HFONT hFontOld = NULL;
	NDListViewPayElcBillSubItem* pListViewSubItem = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;

	pListViewSubItem = _getSubItem(uItemIndex, uSubItemIndex);
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont(m_uFontIdItem);
	if (pYBCtrlFont){
		hFontOld = (HFONT)::SelectObject(hDC, pYBCtrlFont->m_hFont);
		iBkModeOld = ::SetBkMode(hDC, TRANSPARENT);
		clrTxtOld = ::SetTextColor(hDC, pYBCtrlFont->m_clrTxt);

		::DrawText(hDC, pListViewSubItem->m_tstrItemText.c_str(), pListViewSubItem->m_tstrItemText.length(), &rcSubItem, DT_LEFT | DT_VCENTER);
		::SetTextColor(hDC, clrTxtOld);
		::SetBkMode(hDC, iBkModeOld);
		::SelectObject(hDC, hFontOld);
	}

}

void NDView4PayElcBillContent::_drawSubItem(HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner, unsigned int uItemIndex, unsigned int uSubItemIndex) const {
	RECT rcSubItem;

	if (!_getSubItemRect(uItemIndex, uSubItemIndex, rcSubItem)) {
		return;
	}

	rcSubItem.bottom -= 1;
	// draw item bk
	::FillRect(hDC, &rcSubItem, (uItemIndex % 2)?m_hBrushItemEven:m_hBrushItemOdd);

	_drawSubItemText(hDC, rcSubItem, uItemIndex, uSubItemIndex);
	// 	// 
	// 	m_containerListViewItems[ uItemIndex ].m_tstrItemText = _T("XXX");
}

bool NDView4PayElcBillContent::_getSubItemRect(unsigned int uItemIndex, unsigned int uSubItemIndex, RECT& rcSubItem) const{
	unsigned int uLineCount = _getLineCount();
	unsigned int uColumnCount = m_containerListViewColumns.size();
	RECT rcSubItemIndexPre;
	if (uItemIndex >= uLineCount || uSubItemIndex >= uColumnCount )
		return false;
	if (uSubItemIndex == 0){
		rcSubItemIndexPre.right = 0;
	} else{
		if (!_getSubItemRect(uItemIndex, --uSubItemIndex, rcSubItemIndexPre))
			return false;
	}
	rcSubItem.left = rcSubItemIndexPre.right;
	rcSubItem.right = rcSubItem.left + _getSubItemWidth(uSubItemIndex);
	rcSubItem.top = uItemIndex*m_uItemHeight;
	rcSubItem.bottom = (uItemIndex + 1)*m_uItemHeight;
	return true;
}

unsigned int NDView4PayElcBillContent::_getSubItemWidth(unsigned int uSubItemIndex) const {
	if (uSubItemIndex >= m_containerListViewColumns.size()) {
		assert( false );
		return 0;
	}
	listviewcolumn_container_type::const_iterator itBegin, itEnd;
	itEnd = m_containerListViewColumns.end();
	for( itBegin = m_containerListViewColumns.begin(); itBegin != itEnd; ++itBegin ) {
		if( uSubItemIndex != (*itBegin)->m_uIndex )
			continue;
		return (*itBegin)->m_uWidth;
	}
	return 0;
}

NDListViewPayElcBillSubItem* NDView4PayElcBillContent::_getSubItem(unsigned int uItemIndex, unsigned int uSubItemIndex) const {
	if (uItemIndex >= _getLineCount() || uSubItemIndex >= m_containerListViewColumns.size()) {
		assert( false );
		return NULL;
	}
	NDListViewIndex* pListViewItemIndex = _getSubItemIndex(uItemIndex, uSubItemIndex);
	if (!pListViewItemIndex)
		return NULL;
	index2SubItem_map_type::const_iterator itIndex2SubItem;
	itIndex2SubItem = m_mapIndex2SubItem.find(pListViewItemIndex);
	if (itIndex2SubItem == m_mapIndex2SubItem.end())
		return NULL;

	return itIndex2SubItem->second;
}

NDListViewIndex* NDView4PayElcBillContent::_getSubItemIndex(unsigned int uItemIndex, unsigned int uSubItemIndex) const {
	if (uItemIndex >= _getLineCount() || uSubItemIndex >= m_containerListViewColumns.size()) {
		assert(false);
		return NULL;
	}
	listviewindex_container_type::const_iterator itBegin, itEnd;
	itEnd = m_containerListViewIndex.end();
	for (itBegin = m_containerListViewIndex.begin(); itBegin != itEnd; ++itBegin) {
		NDListViewIndex* pListViewItemIndex = (*itBegin);
		if (!pListViewItemIndex)
			continue;
		if (pListViewItemIndex->m_uLineIndex == uItemIndex && pListViewItemIndex->m_uColumnIndex == uSubItemIndex)
			return pListViewItemIndex;
	}
	return NULL;
}

void NDView4PayElcBillContent::setItemBkClr(COLORREF clrOdd, COLORREF clrEven) {
	//
	if (m_hBrushItemOdd) {
		::DeleteObject(m_hBrushItemOdd);
		m_hBrushItemOdd = NULL;
	}
	m_hBrushItemOdd = ::CreateSolidBrush(clrOdd);
	//
	if (m_hBrushItemEven) {
		::DeleteObject(m_hBrushItemEven);
		m_hBrushItemEven = NULL;
	}
	m_hBrushItemEven = ::CreateSolidBrush(clrEven);

}

void NDView4PayElcBillContent::_drawStatus(HDC hDC, YBViewInner* pYBViewInner) const {
	if (!pYBViewInner)
		return;
	RECT rcDestInContent;
	RECT rcDestInView;

	if (!pYBViewInner->getContentUpdateRect(rcDestInContent, rcDestInView))
		return;
	//
	m_pMemDC->resize(getWidth(), getHeight());
	_drawBK(m_pMemDC->getMemDC(), rcDestInContent, pYBViewInner);
	_drawAllItem(m_pMemDC->getMemDC(), rcDestInContent, pYBViewInner);

	::BitBlt(hDC, rcDestInView.left, rcDestInView.top, (rcDestInView.right - rcDestInView.left), (rcDestInView.bottom - rcDestInView.top),
		m_pMemDC->getMemDC(), rcDestInContent.left, rcDestInContent.top, SRCCOPY);

}

void NDView4PayElcBillContent::setWidth(unsigned int uWidthNew) {
	if (m_uWidth == uWidthNew)
		return;
	m_uWidth = uWidthNew;
}

void NDView4PayElcBillContent::_loadFont(){
	LOGFONT lFont;

	memset(&lFont, 0, sizeof(LOGFONT));
	_tcscpy_s(lFont.lfFaceName, _T("Î¢ÈíÑÅºÚ"));
	lFont.lfHeight = 22;

	m_uFontIdItem = YBCtrlFontDepository::getInstance()->insertLOGFONT(lFont, RGB(1, 75, 122));
}


