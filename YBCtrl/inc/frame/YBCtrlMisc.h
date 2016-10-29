#ifndef __YBCTRLMISC_H__
#define __YBCTRLMISC_H__
#include "YBCtrlEnvCfg.h"
//
namespace Gdiplus {
	class Graphics;
	class Image;
}
//
namespace ybctrl {
	class YBCtrlImageBase;
}
//
YBCTRL_NAMESPACE_BEGIN
	
bool YBCTRL_API YBImage2GPImage( YBCtrlImageBase* pYBImage, Gdiplus::Image** ppGPImage );

bool YBCTRL_API GdipStretchBlt( Gdiplus::Graphics* pGraphics, Gdiplus::Image* pImage
	, unsigned int uXDest, unsigned int uYDest, unsigned int uWidthDest, unsigned int uHeightDest
	, unsigned int uXSrc, unsigned int uYSrc, unsigned int uWidthSrc, unsigned int uHeightSrc );

bool YBCTRL_API GdipBitBlt( Gdiplus::Graphics* pGraphics, Gdiplus::Image* pImage
	, unsigned int uXDest, unsigned int uYDest, unsigned int uWidthDest, unsigned int uHeightDest
	, unsigned int uXSrc, unsigned int uYSrc, unsigned int uWidthSrc, unsigned int uHeightSrc );

void YBCTRL_API BackGroundBitBlt( HDC hDest, unsigned int xDest, unsigned int yDest, unsigned int uWidthDest, unsigned int uHeightDest
	, HDC hMemSrc, unsigned int xSrc, unsigned int ySrc, unsigned int uWidthSrc, unsigned int uHeightSrc
	, unsigned int uLFixed, unsigned int uTFixed, unsigned int uRFixed, unsigned int uBFixed );

YBCTRL_NAMESPACE_END
#endif //__YBCTRLMISC_H__