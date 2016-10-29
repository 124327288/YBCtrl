#ifndef __YBCTRLMENU_H__
#define __YBCTRLMENU_H__
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlBase.h"
#include <deque>
//
YBCTRL_NAMESPACE_BEGIN
//
class YBCTRL_API YBCtrlMenuItemBase {
	friend class YBCtrlMenu;
public:
	YBCtrlMenuItemBase();
	virtual ~YBCtrlMenuItemBase();

protected:
	virtual bool isDisable() = 0;
	virtual bool canClick() = 0;
	virtual void setMouseHoverFlag( bool bHover ) = 0;
	virtual void drawStatus( HDC hDC, const RECT& rcItem ) const = 0;
	virtual unsigned int getHeight() const = 0;
	virtual bool isDivide2Part() = 0;
	virtual bool isDivide3Part() = 0;
	virtual void setMouseHoverInPart1Flag( bool bMouseHoverInPart1 ) = 0;
	virtual void setMouseHoverInPart2Flag( bool bMouseHoverInPart2 ) = 0;
	virtual void setMouseHoverInPart3Flag( bool bMouseHoverInPart3 ) = 0;
	virtual bool CanClickFlagInPart1() = 0;
	virtual bool CanClickFlagInPart2() = 0;
	virtual bool CanClickFlagInPart3() = 0;
	virtual bool isDividePara1Disable() = 0;
	virtual bool isDividePara2Disable() = 0;
	virtual bool isDividePara3Disable() = 0;
	virtual bool isDivide2PartCenter() = 0;
	virtual bool isDrawCheckIcon() {return false;} //为电子律师专门增加的一个接口.
	virtual void setDrawCheckIcon( bool bDrawCheckIcon ) {} //为电子律师专门增加的一个接口.
	virtual void setFlagDrawCheckIcon( bool bFlagDrawCheckIcon ) {};

public:
	unsigned int getItemId() const;

private:
	void _setItemId( unsigned int uItemId );

private:
	unsigned int m_uItemId;
};

class YBCTRL_API YBCtrlMenu : public YBCtrlBase {
public:
	typedef unsigned int item_id_type;
	typedef std::deque< YBCtrlMenuItemBase* > item_container_type;

public:
	YBCtrlMenu( unsigned int uWidth );
	virtual ~YBCtrlMenu();

public:
	YBCTRL_WNDMSGMAP_DECLARE()

public:
	HWND Create( DWORD dwExStyle, DWORD dwStyle, int x, int y,  HWND hWndMsgRecv, unsigned int uCtrlId, HINSTANCE hInstance );

public:
	bool insertItem( unsigned int uItemIndex, YBCtrlMenuItemBase* pItem, item_id_type& itemId );
	YBCtrlMenuItemBase* getItemByIndex( unsigned int uIndex );
	YBCtrlMenuItemBase* getItemById( item_id_type itItem );
	unsigned int getItemCount() const { return m_containerItem.size(); }
	bool deleteItemById( unsigned int uItemId );
	bool hasItemId( unsigned int uItemId ) const;
	bool itemId2Index( item_id_type idItem, unsigned int& uItemIndex ) const;
	bool moveTest( const POINT& ptMouse, unsigned int& uIndexItem );
	bool setHoverItem( unsigned int uIndexItem );
	//bool setHoverItem( const POINT& ptMouse, unsigned int uIndexItem );
	bool setPressItem( unsigned int uIndexItem );
	void getHoverItemIndex( unsigned int& uHoverItemIndex ) const;
	unsigned int getHeight();
	void setDivideHoverItem( const POINT& ptMouse, unsigned int uIndexItem );
	void clearAllItem();
	void setWidth( unsigned int uWidth );

protected:
	void _onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_DESTROY( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEMOVE( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSELEAVE( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_NCACTIVATE( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_SETCURSOR( YBCtrlWndMsg* pYBCtrlWndMsg );

protected:
	item_id_type _allocalItemId() const;
	void _drawBk( HDC hDC );
	void _drawAllItem( HDC hDC );
	void _drawStatus( HDC hDC );
	bool _hasItemId( unsigned int uItemId ) const;
	bool _calcItemRect( unsigned int uItemIndex, RECT& rcItem );
	bool _calcDividePart1Rect( unsigned int uItemIndex, RECT& rcDividePart1 );
	bool _calcDividePart2Rect( unsigned int uItemIndex, RECT& rcDividePart2 );
	bool _calcDividePart3Rect( unsigned int uItemIndex, RECT& rcDividePart3 );
	void _setDividePart2Flag( const POINT& ptMouse, unsigned int uIndexItem );
	void _setDividePart3Flag( const POINT& ptMouse, unsigned int uIndexItem );
	void _clearAllItem();
	void _destorySelf();

public:
	HWND m_hWndMsgRecv;
#pragma warning(push)
#pragma warning(disable:4251)
	item_container_type m_containerItem;
#pragma warning(pop)
	unsigned int m_uWidth;
	HBRUSH m_hBrushBk;
	YBCtrlMemDC* m_pMemDC;
	unsigned int m_uHoverItemIndex;
	unsigned int m_uPressItemIndexOld;
};

YBCTRL_NAMESPACE_END
#endif // __YBCTRLMENU_H__