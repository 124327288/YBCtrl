#ifndef __YBTOPMOSTBAR_H__
#define __YBTOPMOSTBAR_H__
#include "frame\YBCtrlBase.h"
#include <string>
#include "tchar.h"

class YBTopMostBar : public ybctrl::YBCtrlBase {
public:
	typedef std::basic_string< TCHAR > tstring_type;
public:
	YBTopMostBar();
	virtual ~YBTopMostBar();
	
public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()
	
public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent, HINSTANCE hInstance );
	bool setBkImage( const tstring_type& tstrBkImagePath );
	
protected:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_KEYDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_CLOSE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

protected:
	unsigned int m_uImageIdBK;
};


#endif //__YBTOPMOSTBAR_H__