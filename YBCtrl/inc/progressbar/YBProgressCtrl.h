#ifndef __YBPROGRESSCTRL_H__
#define __YBPROGRESSCTRL_H__
#include "frame/YBCtrlBase.h"
#include "button/YBButton.h"
#include <string.h>

namespace Gdiplus{
	class Image;
}

YBCTRL_NAMESPACE_BEGIN
class YBCtrlImageBase;
class YBCtrlImageStatic;

class YBCTRL_API YBProgressCtrl : public YBCtrlBase{
public:
	typedef enum _ENUMTHUMBSTATUS{
		ETHUMB_STATUS_NORMAL = 0,
		ETHUMB_STATUS_HOVER =1,
		ETHUMB_STATUS_PRESS =2
	}ENUMTHUMBSTATUS;
public:
	YBProgressCtrl();
	virtual ~YBProgressCtrl();

public:
	YBCTRL_WNDMSGMAP_DECLARE()

public:
	HWND Create( DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance );
	void SetImageId( unsigned int uImageIdBK, unsigned int uForGndImageId );
	void SetImageId( ENUMTHUMBSTATUS eThumbStatus, unsigned int uImageId );
	void SetProgressCtrlSpaced( unsigned int uLSpaced, unsigned int uTSpaced, unsigned int uRSpaced, unsigned int uBSpaced );
	void setRange( int nLower, int nUpper );
	void setPos( unsigned int nPos );
	unsigned int GetPos();
	unsigned int GetMaxPos(){ return m_nMaxPos; };
	unsigned int GetMinPos(){ return m_nMinPos; };

private:
	void _onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONUP( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEMOVE( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSELEAVE( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_SETCURSOR( YBCtrlWndMsg* pYBCtrlWndMsg );
	bool _getImageId( ENUMTHUMBSTATUS eThumbStatus, unsigned int& uImageId );
	YBCtrlImageBase* _getImageBase( unsigned int uImageId );
	YBCtrlImageStatic* _getImageStatic( unsigned int uImageId );
	//
	void _drawStatus( HDC hDC );
	void _drawBK( HDC hDC );
	void _drawForeGnd( HDC hDC );
	void _drawThumbBtn( HDC hDC );

private:
	unsigned int m_uFlag;
	WORD m_wordCtrlID;
	unsigned int m_uImageIdBK;
	unsigned int m_uImageIdFG;
	unsigned int m_uImageIdThumbNor;
	unsigned int m_uImageIdThumbHov;
	unsigned int m_uImageIdThumbPre;
	unsigned int m_uLFixed;
	unsigned int m_uTFixed;
	unsigned int m_uRFixed;
	unsigned int m_uBFixed;
	unsigned int m_unForeGndPtX;
	int m_nMaxPos;
	int m_nMinPos;
	ENUMTHUMBSTATUS m_eThumbStatus;
//	ybctrl::YBThumb m_thumbBtn;
	HWND m_hWndSetCaptureOld;
	unsigned int m_uThumbWidth;
	unsigned int m_uLSpaced;
	unsigned int m_uTSpaced;
	unsigned int m_uRSpaced;
	unsigned int m_uBSpaced;
};


YBCTRL_NAMESPACE_END

#endif //__YBPROGRESSCTRL_H__