#ifndef __YBLISTBOX4UDFOLDER_H__
#define __YBLISTBOX4UDFOLDER_H__
#include "view/YBViewContentBase.h"
#include "view/YBViewInner.h"
#include "scrollbar/YBNWScrollBar.h"
#include <deque>
//
namespace ybctrl {
	class YBCtrlMemDC;
	class YBNWScrollBar;
	class YBButton;
	class YBCheckBox;
}
//
class YBView4UDFolderInner : public ybctrl::YBViewInner {
public:
	YBView4UDFolderInner();
	virtual ~YBView4UDFolderInner();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()
	
public:

};
//
struct YBListBoxFolderItem {
	YBListBoxFolderItem();
	~YBListBoxFolderItem();
	std::basic_string< TCHAR > m_tstrItemText;
	bool m_bCheckStatus;
	unsigned int m_uImageId;
	WORD m_wBtnCtrlId;
	//
	ybctrl::YBButton* m_pBtnBrowser; //ä¯ÀÀ°´Å¥
	ybctrl::YBCheckBox* m_pBtnChk; //check box
	
};
//
class YBView4UDFolderContent : public ybctrl::YBViewContentBase {
public:
	typedef std::deque< YBListBoxFolderItem* > listboxitem_container_type;
public:
	YBView4UDFolderContent( ybctrl::YBViewInner* pViewInner );
	virtual ~YBView4UDFolderContent();

public:
	void insertItem( unsigned int uIndexItem, YBListBoxFolderItem* pListBoxItem );
	unsigned int getItemCount() const { return m_containerListBoxItems.size(); }
	YBListBoxFolderItem* getItem( unsigned int uIndexItem );
	void setItemBkClr( COLORREF clrOdd, COLORREF clrEven );
	void setWidth( unsigned int uWidthNew );
	void clearAllItem();
	bool HitTest( const POINT& ptInContent, unsigned int& uIndexItem );

	// interface of YBViewContentBase
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
	ybctrl::YBViewInner* m_pViewInner;
	unsigned int m_uWidth;
	unsigned int m_uItemHeight;
	listboxitem_container_type m_containerListBoxItems;
	ybctrl::YBCtrlMemDC* m_pMemDC;
	HBRUSH m_hBrushItemOdd; //  ÆæÊýItemµÄ±³¾°É«»­Ë¢
	HBRUSH m_hBrushItemEven; // Å¼ÊýItemµÄ±³¾°É«»­Ë¢
	HBRUSH m_hBrushItemBK; // ÄÚÈÝ±³¾°É«»­Ë¢
	unsigned int m_uFontIdItem;
};


#endif //__YBLISTBOX4UDFOLDER_H__