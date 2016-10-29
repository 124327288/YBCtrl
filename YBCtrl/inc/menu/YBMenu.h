#ifndef __YBMENU_H__
#define __YBMENU_H__
#include "frame/YBCtrlBase.h"
#include <string>
#include <map>
#include <deque>

YBCTRL_NAMESPACE_BEGIN

namespace Gdiplus{
	class Image;
	class Graphics;
}

class YBCtrlImageBase;
class YBCtrlImageStatic;

class YBCTRL_API YBMenu : public YBCtrlBase{
public:
	struct YBMenuItem{
		std::basic_string< TCHAR > m_tstrText;
		WORD m_wordCtrlId;
		bool m_bShowChildWindow;
	};
	typedef std::deque< YBMenuItem* > mi_container_type;
	typedef std::map< HWND, YBMenu* > hwndYBMenu_map_type;
	typedef std::basic_string< TCHAR >tstring_type;
public:
	YBMenu();
	virtual ~YBMenu();

public:
	YBCTRL_WNDMSGMAP_DECLARE();

public:
	HWND Create( int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance );
	void SetBkImage( unsigned int uBkImageId );
	void SetFontId( unsigned int uFontId );
	BOOL InsertMenuItem( YBMenuItem* pYBMenuItem );

private:
	void _onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_KILLFOCUS( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _drawBk( HDC hDC );
	YBCtrlImageStatic* _getBkImageStatic( unsigned int uBkImageId );
	void _clearAllMenuItem();
	BOOL _calcItemRect( unsigned int uIndex, RECT& rcItemRect );
	void _drawText( HDC hDC );

private:
	unsigned int m_uBkImageId;
	unsigned int m_uFontId;

#pragma warning(push)
#pragma warning(disable:4251)
	mi_container_type m_containerMenuItem;
	hwndYBMenu_map_type g_mapHWNDMenu;
#pragma warning(pop)
};

YBCTRL_NAMESPACE_END
#endif //__YBMENU_H__