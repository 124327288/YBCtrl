#ifndef __YBVIEW4INSTALLFILE_H__
#define __YBVIEW4INSTALLFILE_H__
#include "view/YBViewContentBase.h"
#include "view/YBViewInner.h"
#include "checkbox/YBCheckBox.h"
#include <deque>

namespace ybctrl {
	class YBCtrlMemDC;
	class YBCtrlImageStatic;
}

class YBView4InstallFileInner : public ybctrl::YBViewInner {
public:
	YBView4InstallFileInner( unsigned int uContentWidth, unsigned int uItemHeight );
	virtual ~YBView4InstallFileInner();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	void _onWM_COMMAND(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSELEAVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEWHEEL( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

};

struct YBListBoxInstallFileItem{
	friend class YBView4InstallFileContent;

	YBListBoxInstallFileItem();
	~YBListBoxInstallFileItem();
	std::basic_string< TCHAR > m_tstrItemContent;
	ybctrl::YBCheckBox* m_pBtnChk; //check box.
	WORD m_wBtnChkCtrlId; // check box 的 control id.
	unsigned int m_uImageIdNor;
	unsigned int m_uImageIdHov;
	unsigned int getItemId() const;
	//
	unsigned int m_IdBtnChkNor;
	unsigned int m_IdBtnChkHov;
	unsigned int m_IdBtnChkPre;
	unsigned int m_IdBtnChkDis;
	unsigned int m_IdBtnNoChkNor;
	unsigned int m_IdBtnNoChkHov;
	unsigned int m_IdBtnNoChkPre;
	unsigned int m_IdBtnNoChkDis;
	unsigned int m_IdBtnChkBK;

private:
	void _setItemId( unsigned int uItemId );

private:
	unsigned int m_uItemId;
	
};

class YBView4InstallFileContent : public  ybctrl::YBViewContentBase {
public:
	typedef std::deque< YBListBoxInstallFileItem* > listboxitem_container_type;
	typedef std::basic_string< TCHAR > tstring_type;
	typedef unsigned int item_id_type;

public:
	YBView4InstallFileContent( unsigned int uContentWidth, unsigned int uItemHeight, ybctrl::YBViewInner* pViewInner );
	virtual ~YBView4InstallFileContent();

public:
	bool push_front( YBListBoxInstallFileItem* pListBoxItem, item_id_type& itemId );
	bool insertItem( unsigned int uIndexItem, YBListBoxInstallFileItem* pListBoxItem, item_id_type& itemId );
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
	YBListBoxInstallFileItem* getItemById( item_id_type itemId );

protected:
	virtual unsigned int getWidth() const;
	virtual unsigned int getHeight() const;
	virtual void _drawStatus( HDC hDC, ybctrl::YBViewInner* pYBViewInner ) const;

private:
	void _drawBK( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner ) const;
	void _drawAllItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner ) const;
	void _drawItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner, unsigned int uItemIndex ) const;
	void _drawIcon( HDC hDC, unsigned int uItemIndex ) const;
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageId ) const;
	void _drawItemText( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const;
	void _drawItemNumber( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const;
	void _drawIconText( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const;
	bool _getItemRect( unsigned int uIndex, RECT& rcItem ) const;
	bool _getNumberRect( unsigned int uIndex, RECT& rcNumber ) const;
	bool _getContentRect( unsigned int uIndex, RECT& rcContent ) const;
	void _clearAllItem();
	void _loadFont();
	item_id_type _allocalItemId() const;
	bool _hasItemId( unsigned int uItemId ) const ;
	bool _getItemBtnChkRect( const RECT& rcItemInC, RECT& rcBtnChkInV ) const;

private:
	ybctrl::YBViewInner* m_pViewInner;
	unsigned int m_uWidth;
	unsigned int m_uItemHeight;
	listboxitem_container_type m_containerListBoxItems;
	ybctrl::YBCtrlMemDC* m_pMemDC;
	HBRUSH m_hBrushItemOdd; //  奇数Item的背景色画刷
	HBRUSH m_hBrushItemEven; // 偶数Item的背景色画刷
	HBRUSH m_hBrushItemSel; // Item 被选中后的背景画刷
	HBRUSH m_hBrushItemBK; // 内容背景色画刷
	unsigned int m_uFontIdItem;
	unsigned int m_uSelItemIndex; //鼠标移动项索引值
	unsigned int m_uSelInItemIndex; //鼠标移动到点击区域索引值
	tstring_type m_tstrIcon;
	
};

#endif //__YBVIEW4INSTALLFILE_H__