#ifndef __YBLISTBOX4DDFOLDER_H__
#define __YBLISTBOX4DDFOLDER_H__
#include "view/YBViewContentBase.h"
#include "view/YBViewInner.h"
#include "scrollbar/YBScrollBar.h"
#include <deque>

namespace ybctrl{
	class YBCtrlMemDC;
}

class YBView4DDFolderInner : public ybctrl::YBViewInner{
public:
	YBView4DDFolderInner();
	~YBView4DDFolderInner();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

};

struct YBListBoxFolderDDItem{
	std::basic_string< TCHAR > m_tstrItemText;
	bool m_bCheckStatus;
	unsigned int m_uImageId;
	WORD m_wBtnCtrlId;
};
//

class YBView4DDFolderContent : public ybctrl::YBViewContentBase{
public:
	typedef std::deque< YBListBoxFolderDDItem* > listboxitem_container_type;
public:
	YBView4DDFolderContent();
	virtual ~YBView4DDFolderContent();

public:
	void insertItem( unsigned int uIndexItem, YBListBoxFolderDDItem* pListBoxItem );
	void setItemBkClr( COLORREF clrOdd, COLORREF clrEven );
	void setWidth( unsigned int uWidthNew );

protected:
	virtual unsigned int getWidth() const;
	virtual unsigned int getHeight() const;
	virtual void _drawStatus( HDC hDC, ybctrl::YBViewInner* pYBViewInner ) const;

private:
	void _drawBK( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner ) const;
	void _drawAllItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner ) const;
	void _drawItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner, unsigned int uItemIndex ) const;
	bool _getItemRect( unsigned int uIndex, RECT& rcItem ) const;
	void _clearAllItem();
	void _drawItemText( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const;
	void _loadFont();
	
private:
	unsigned int m_uWidth;
	unsigned int m_uItemHeight;
	listboxitem_container_type m_containerListBoxItems;
	ybctrl::YBCtrlMemDC* m_pMemDC;
	HBRUSH m_hBrushItemOdd;
	HBRUSH m_hBrushItemEven;
	HBRUSH m_hBrushItemBK;
	unsigned int m_uFontIdItem;
};

#endif //__YBLISTBOX4DDFOLDER_H__