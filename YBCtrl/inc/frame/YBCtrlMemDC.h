#ifndef __YBCTRLMEMDC_H__
#define __YBCTRLMEMDC_H__
#include "YBCtrlEnvCfg.h"
YBCTRL_NAMESPACE_BEGIN
//
class YBCTRL_API YBCtrlMemDC {
public:
	YBCtrlMemDC( unsigned int uWidth, unsigned int uHeight );
	virtual ~YBCtrlMemDC();

public:
	void resize( unsigned int uWidth, unsigned int uHeight );
	HDC getMemDC() const { return m_hMemDC; }
	unsigned int getWidth() const { return m_uWidth; }
	unsigned int getHeight() const { return m_uHeight; }
	HBITMAP getBitMap() { return m_hMemBitmap; }

private:
	void _init( unsigned int uWidth, unsigned int uHeight );
	void _unInit();

private:
	HDC m_hMemDC;
	HBITMAP m_hMemBitmap;
	HBITMAP m_hMemBitmapOld;
	unsigned int m_uWidth;
	unsigned int m_uHeight;
};

YBCTRL_NAMESPACE_END
#endif //__YBCTRLMEMDC_H__