#ifndef __YBCTRLIMAGEGIF_H__
#define __YBCTRLIMAGEGIF_H__
#include "YBCtrlImageBase.h"
#include "YBCtrlDefs.h"
#include "frame/YBCtrlTask4Gif.h"

namespace Gdiplus {
	class Image;
}
//
YBCTRL_NAMESPACE_BEGIN
//
class YBITTaskEngine;
//
class YBCTRL_API YBCtrlImageGif : public YBCtrlImageBase {
public:
	YBCtrlImageGif( const tstring_type& tstrGifFilePath );
	virtual ~YBCtrlImageGif();

public:
	virtual unsigned int getType()  { return YBCTRL_IMAGE_TYPE_GIF; }
	virtual unsigned int getWidth() const { return m_uWidth; }
	virtual unsigned int getHeight() const { return m_uHeight; }

public:
	Gdiplus::Image* getImage() const { return m_pImage; }
	Gdiplus::Image* getImage() { return m_pImage; }

private:
	void _init( const tstring_type& tstrGifFilePath );
	void _uninit();

private:
	Gdiplus::Image* m_pImage;
	unsigned int m_uWidth;
	unsigned int m_uHeight;
	//
	static YBITTaskEngine* m_pEngine4DrawGif;
	YBCtrlTask4Gif* m_pTask4Gif;
};


YBCTRL_NAMESPACE_END
#endif //__YBCTRLIMAGEGIF_H__