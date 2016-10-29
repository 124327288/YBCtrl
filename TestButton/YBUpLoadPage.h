#ifndef __YBUPLOADPAGE_H__
#define __YBUPLOADPAGE_H__
#include "frame/YBCtrlBase.h"

#define YBUPLOADPAGE_WND_ID (0x00000301)

namespace ybctrl{
	class YBCtrlImageBase;
	class YBCtrlImageStatic;
}

namespace Gdiplus{
	class Image;
}

class YBUpLoadPage : public ybctrl::YBCtrlBase{
public:
	YBUpLoadPage();
	virtual ~YBUpLoadPage();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent );


private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _drawBk( HDC hDC );
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageId );
	void _loadImage();
	void _loadFont();
	void _init();

private:
	unsigned int m_uImageIdBk;

};

#endif //__YBUPLOADPAGE_H__