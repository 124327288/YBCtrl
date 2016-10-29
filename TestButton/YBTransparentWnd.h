#ifndef __YBTRANSPARENTWND_H__
#define __YBTRANSPARENTWND_H__
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlEnvCfg.h"
//
class YBTransparentWnd : public ybctrl::YBCtrlBase {
public:
	YBTransparentWnd();
	virtual ~YBTransparentWnd();

public:
	HWND create( unsigned int x, unsigned int y, int nWidth, int nHeight );

private:
	void _drawBk( HDC hDC );
	void _setTransparent( unsigned int uPtDstX, unsigned int uPtDstY );

private:
	HBRUSH m_hBrushBk;

};

#endif // __YBTRANSPARENTWND_H__