#ifndef __YBCTRLTASK4GIF_H__
#define __YBCTRLTASK4GIF_H__
#include "taskEngine/YBTaskEngine.h"
#include "YBCtrlEnvCfg.h"
#include "frame/YBCtrlMemDC.h"

USING_NAMESPACE_YBCTRL
//
namespace Gdiplus {
	class Image;
	class PropertyItem;
}
//
class YBCtrlTask4Gif : public YBITTaskBase {
public:
	YBCtrlTask4Gif( Gdiplus::Image* pImage );
	~YBCtrlTask4Gif();

public:
	void Run();

private:
	void _init();

public:
	YBCtrlMemDC* m_pMemDC;

private:
	Gdiplus::Image* m_pImage;
	//
	unsigned int m_uFrameCount;
	unsigned int m_uFramePos;
	Gdiplus::PropertyItem* m_pPropertyItem;
};

#endif //__YBCTRLTASK4GIF_H__