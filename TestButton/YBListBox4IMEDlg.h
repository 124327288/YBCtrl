#ifndef __YBLISTBOX4IMEDLG_H__
#define __YBLISTBOX4IMEDLG_H__
#include "view/YBViewContentBase.h"
#include "view/YBViewInner.h"
#include "radiobtn/YBRadioBtn.h"
#include <deque>

namespace ybctrl {
	class YBCtrlMemDC;
	class YBCtrlImageStatic;
}

class YBView4IMEDlgInner : public ybctrl::YBViewInner {
public:
	YBView4IMEDlgInner( unsigned int uContentWidth, unsigned int uItemHeight );
	virtual ~YBView4IMEDlgInner();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	void _onWM_COMMAND(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSELEAVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEWHEEL( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

};

struct YBListBoxIMEItem {
	friend class YBView4IMEContent;

	YBListBoxIMEItem();
	~YBListBoxIMEItem();
	std::basic_string< TCHAR > m_tstrIMEName;
	unsigned int m_uImageIdIMEBtnUse;
	unsigned int m_uImageIdIMEBtnInstall;
	unsigned int m_uImageIdIMEIcon;
	unsigned int getItemId() const;
	//
	WORD m_wBtnRadioCtrlId; // check box 的 control id.
	ybctrl::YBRadioBtn* m_pBtnRadio; //radio button.

private:
	void _setItemId( unsigned int uItemId );

private:
	unsigned int m_uItemId;
	
};

class YBView4IMEContent : public  ybctrl::YBViewContentBase {
public:
	typedef std::deque< YBListBoxIMEItem* > listboxitem_container_type;
	typedef std::basic_string< TCHAR > tstring_type;
	typedef unsigned int item_id_type;

public:
	YBView4IMEContent( unsigned int uContentWidth, unsigned int uItemHeight, ybctrl::YBViewInner* pViewInner );
	virtual ~YBView4IMEContent();

public:
	bool push_front( YBListBoxIMEItem* pListBoxItem, item_id_type& itemId );
	bool insertItem( unsigned int uIndexItem, YBListBoxIMEItem* pListBoxItem, item_id_type& itemId );
	unsigned int getItemCount() const { return m_containerListBoxItems.size(); };
	void setItemBkClr( COLORREF clrOdd, COLORREF clrEven );
	void setWidth( unsigned int uWidthNew );
	void clearAllItem();
	bool moveTest( const POINT& ptInContent, unsigned int& uIndexItem );
	bool moveHitTest( const POINT& ptInContent, unsigned int& uIndexItem );
	bool setSelItem( unsigned int uIndexItem );
	void setItemInvalid( unsigned int uIndexItem );
	bool setSelInItem( unsigned int uIndexItem );
	void setInItemInvalid( unsigned int uIndexItem );
	bool getHitRect( unsigned int uIndex, RECT& rcHit ) const;
	void deleteMenuItemByIndex( unsigned int uIndex );
	void deleteMenuItemById( unsigned int uItemId );
	bool itemId2Index( item_id_type idItem, unsigned int& uIndex ) const;
	bool index2ItemId( unsigned int uIndex, item_id_type& idItem ) const;
	bool hasItemId( unsigned int uItemId ) const ;
	YBListBoxIMEItem* getItemById( item_id_type itemId );

protected:
	virtual unsigned int getWidth() const;
	virtual unsigned int getHeight() const;
	virtual void _drawStatus( HDC hDC, ybctrl::YBViewInner* pYBViewInner ) const;

private:
	void _drawBK( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner ) const;
	void _drawAllItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner ) const;
	void _drawItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner, unsigned int uItemIndex ) const;
	void _drawHitBtn( HDC hDC, unsigned int uItemIndex ) const;
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageId ) const;
	void _drawIMENameTxt( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const;
	void _drawIMENumber( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const;
	void _drawHitBtnTxt( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const;
	void _drawIcon( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const;
	bool _getItemRect( unsigned int uIndex, RECT& rcItem ) const;
	bool _getNumberRect( unsigned int uIndex, RECT& rcNumber ) const;
	bool _getIconRect( unsigned int uIndex, RECT& rcIcon ) const;
	bool _getIMENameRect( unsigned int uIndex, RECT& rcContent ) const;
	bool _getItemChkBk( unsigned int uIndex, RECT& rcChkBk ) const;
	bool _getItemBtnRadioRect( const RECT& rcItemInC, RECT& rcBtnRadioInV ) const;
	void _clearAllItem();
	void _loadFont();
	item_id_type _allocalItemId() const;
	bool _hasItemId( unsigned int uItemId ) const ;

private:
	ybctrl::YBViewInner* m_pViewInner;
	unsigned int m_uWidth;
	unsigned int m_uItemHeight;
	listboxitem_container_type m_containerListBoxItems;
	ybctrl::YBCtrlMemDC* m_pMemDC;
	HBRUSH m_hBrushItem; // Item的背景色画刷
	HBRUSH m_hBrushItemSel; // Item 被选中后的背景画刷
	HBRUSH m_hBrushItemBK; // 内容背景色画刷
	HBRUSH m_hBrushItemChkBk; // Item 上的checkbox的背景画刷
	unsigned int m_uFontIdIMEItem;
	unsigned int m_uFontIdIMEItemBtnTxt;
	unsigned int m_uSelItemIndex; //鼠标移动项索引值
	unsigned int m_uSelInItemIndex; //鼠标移动到点击区域索引值
	tstring_type m_tstrIcon; 
	//
	unsigned int m_uLSpaceIMENumber;
};

#endif //__YBLISTBOX4IMEDLG_H__	