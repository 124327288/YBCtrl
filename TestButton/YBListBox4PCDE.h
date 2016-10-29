#ifndef __YBLISTBOX4PCDE_H__
#define __YBLISTBOX4PCDE_H__
#include "view/YBViewContentBase.h"
#include "view/YBViewInner.h"
#include "frame/YBCtrlMemDC.h"
#include <deque>

class YBView4PCDEInner : public ybctrl::YBViewInner {
public:
	YBView4PCDEInner( unsigned int uContentWidth, unsigned int uItemHeight );
	~YBView4PCDEInner();

};
//
struct YBListBoxPCDEItem {
	YBListBoxPCDEItem();
	~YBListBoxPCDEItem();
	std::basic_string< TCHAR > m_tstrNum; //ÐòÌ–
	std::basic_string< TCHAR > m_tstrItem; //í—Ä¿.
	std::basic_string< TCHAR > m_tstrCfg; //ÅäÖÃ.
	std::basic_string< TCHAR > m_tstrRes; //™zœy½Y¹û.
	std::basic_string< TCHAR > m_tstrNote; //‚äÔ].
};

class YBView4PCDEContent : public ybctrl::YBViewContentBase {
public:
	typedef std::deque< YBListBoxPCDEItem* > listboxitem_container_type;

public:
	YBView4PCDEContent( unsigned int uContentWidth, unsigned int uItemHeight, ybctrl::YBViewInner* pViewInner );
	virtual ~YBView4PCDEContent();

public:
	void insertItem( unsigned int uIndexItem, YBListBoxPCDEItem* pListBoxItem );
	unsigned int getItemCount() const{ return m_containerListboxItems.size(); }
	void setWidth( unsigned int uWidthNew );
	void clearAllItem();
	void setItemBkClr( COLORREF clrOdd, COLORREF clrEven );
	YBListBoxPCDEItem* getItem( unsigned int uIndexItem );

protected:
	virtual unsigned int getWidth() const;
	virtual unsigned int getHeight() const;
	virtual void _drawStatus( HDC hDC, ybctrl::YBViewInner* pViewInner ) const;

private:
	void _drawBk( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pViewInner ) const;
	void _drawAllItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pViewInner ) const;
	void _drawItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pViewInner, unsigned int uIndexItem ) const;
	bool _getItemRect( unsigned int uIndex, RECT& rcItem ) const;
	void _clearAllItem();
	void _loadFont();

private:
	listboxitem_container_type m_containerListboxItems;
	ybctrl::YBViewInner* m_pViewInner;
	unsigned int m_uWidth;
	unsigned int m_uItemHeight;
	ybctrl::YBCtrlMemDC* m_pMemDC;
	HBRUSH m_hBrushItemOdd;
	HBRUSH m_hBrushItemEven;
	HBRUSH m_hBrushItemBk;
	unsigned int m_uFontIdItem;
};

#endif //__YBLISTBOX4PCDE_H__