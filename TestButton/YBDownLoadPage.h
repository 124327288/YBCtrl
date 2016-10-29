#ifndef __YBDOWNLOADPAGE_H__
#define __YBDOWNLOADPAGE_H__
#include "frame/YBCtrlBase.h"

#define YBDOWNLOADPAGE_WND_ID (0x00000311)

namespace ybctrl{
	class YBCtrlImageBase;
	class YBCtrlImageStatic;
}

namespace Gdiplus{
	class Image;
}

class YBDownLoadPage : public ybctrl::YBCtrlBase{
public:
	YBDownLoadPage();
	virtual ~YBDownLoadPage();

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

#endif //__YBDOWNLOADPAGE_H__