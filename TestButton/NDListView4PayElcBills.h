#ifndef __NDLISTVIEW4PAYELCBILLS_H__
#define __NDLISTVIEW4PAYELCBILLS_H__
#include "view/YBViewContentBase.h"
#include "view/YBViewInner.h"
#include "scrollbar/YBScrollBar.h"
#include <deque>
#include <map>

namespace ybctrl{
	class YBCtrlMemDC;
}

class NDView4PayElcBillInner : public ybctrl::YBViewInner{
public:
	NDView4PayElcBillInner();
	~NDView4PayElcBillInner();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

};

struct NDListViewPayElcBillSubItem{
	std::basic_string< TCHAR > m_tstrItemText;
	bool m_bCheckStatus;
	unsigned int m_uImageId;
	unsigned int m_uItemIndex;
	unsigned int m_uSubItemIndex;
};

struct NDListViewPayElcBillColumn{
	unsigned int m_uIndex;
	unsigned int m_uWidth;
};
//
struct NDListViewIndex{
	unsigned int m_uLineIndex;
	unsigned int m_uColumnIndex;
};

class NDView4PayElcBillContent : public ybctrl::YBViewContentBase{
public:
	typedef std::deque< NDListViewIndex* > listviewindex_container_type;
	typedef std::deque< NDListViewPayElcBillColumn* > listviewcolumn_container_type;
	typedef std::map< NDListViewIndex*, NDListViewPayElcBillSubItem* > index2SubItem_map_type;

public:
	NDView4PayElcBillContent();
	virtual ~NDView4PayElcBillContent();

public:
	void insertItem(unsigned int uIndexItem, unsigned int uIndexSubItem, NDListViewPayElcBillSubItem* pListViewItem);
	void insertColumn(unsigned int uIndexColumn, NDListViewPayElcBillColumn* pListViewColumn);
	void setItemBkClr(COLORREF clrOdd, COLORREF clrEven);
	void setWidth(unsigned int uWidthNew);

protected:
	virtual unsigned int getWidth() const;
	virtual unsigned int getHeight() const;
	virtual void _drawStatus(HDC hDC, ybctrl::YBViewInner* pYBViewInner) const;

private:
	void _drawBK(HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner) const;
	void _drawAllItem(HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner) const;
	void _drawSubItem(HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner, unsigned int uItemIndex, unsigned int uSubItemIndex) const;
	bool _getSubItemRect(unsigned int uItemIndex, unsigned int uSubItemIndex, RECT& rcSubItem) const;
	void _loadFont();
	void _clearAllItem();
	void _drawSubItemText(HDC hDC, RECT& rcSubItem, unsigned int uItemIndex, unsigned int uSubItemIndex) const;
	unsigned int _getLineCount() const;
	unsigned int _getSubItemWidth( unsigned int uSubItemIndex ) const;
	NDListViewIndex* _getSubItemIndex( unsigned int uItemIndex, unsigned int uSubItemIndex ) const;
	NDListViewPayElcBillSubItem* _getSubItem( unsigned int uItemIndex, unsigned int uSubItemIndex ) const;

private:
	unsigned int m_uWidth;
	unsigned int m_uItemHeight;
	listviewindex_container_type m_containerListViewIndex;
	listviewcolumn_container_type m_containerListViewColumns;
	index2SubItem_map_type m_mapIndex2SubItem;
	ybctrl::YBCtrlMemDC* m_pMemDC;
	HBRUSH m_hBrushItemOdd;
	HBRUSH m_hBrushItemEven;
	HBRUSH m_hBrushItemBK;
	unsigned int m_uFontIdItem;
};

#endif //__NDLISTVIEW4PAYELCBILLS_H__