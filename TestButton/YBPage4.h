#ifndef __YBPAGE4_H__
#define __YBPAGE4_H__
#include "frame\YBCtrlBase.h"
#include "button\YBButton.h"
#include "YBTopMostBar.h"
#include "frame\YBCtrlImageBase.h"
#include "frame\YBCtrlImageStatic.h"
#include "scrollwnd\YBScrollWnd.h"
#include "YBWSAWMenuBase.h"

namespace ybctrl {
	class YBCtrlImageBase;
	class YBCtrlImageStatic;
};
namespace Gdiplus{
	class Image;
}
//
class YBPage4 : public YBTopMostBar{
public:
	YBPage4();
	virtual ~YBPage4();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE();

public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent );

private:
	void loadImage();

private:
	ybctrl::YBButton m_checkBox;
};

#endif //__YBPAGE4_H__