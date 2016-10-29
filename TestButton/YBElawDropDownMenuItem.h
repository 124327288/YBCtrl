#ifndef __YBELAWDROPDOWNMENUITEM_H__
#define __YBELAWDROPDOWNMENUITEM_H__
#include "menu/YBCtrlMenuItems.h"

class YBElawDropDownMenuItem : public ybctrl::YBCtrlMIDefault {
public:
	YBElawDropDownMenuItem();
	~YBElawDropDownMenuItem();

private:
	void _drawSelf( HDC hDC, const RECT& rcItem );
	virtual bool isDrawCheckIcon();
	virtual void setDrawCheckIcon( bool bDrawCheckIcon );
	bool _getCheckIconRect( const RECT& rcItem, RECT& rcCheckIcon ) const;

private:
	unsigned int m_uImageIdCheckIcon;
};


#endif //__YBELAWDROPDOWNMENUITEM_H__