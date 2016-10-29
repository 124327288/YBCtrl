#ifndef __YBELAWDROPDOWNMENU_H__
#define __YBELAWDROPDOWNMENU_H__
#include "menu/YBCtrlMenu.h"

USING_NAMESPACE_YBCTRL

class YBElawDropDownMenu {
public:
	YBElawDropDownMenu();
	virtual ~YBElawDropDownMenu();

private:
	void _init();
	void _loadFont();
	void _loadImage();

public:
	ybctrl::YBCtrlMenu m_menu4ElawDDMenu;
	unsigned int m_uFontIdMenuTxt;
	unsigned int m_uImageIdMenuIcon;
	unsigned int m_uImageIdMenuItemBk;
	unsigned int m_uImageIdMenuHover;
	unsigned int m_uImageIdMenuItemDisable;
};

#endif //__YBELAWDROPDOWNMENU_H__