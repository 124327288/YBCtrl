#ifndef __YBSCROLLWND_H__
#define __YBSCROLLWND_H__
#include "frame/YBCtrlEnvCfg.h"
#include "frame/YBCtrlBase.h"
YBCTRL_NAMESPACE_BEGIN
class YBViewInner;
class YBScrollBar;
//
#define YBSW_SBTHICK_DEFAULT (20)
#define YBSW_STEPMOVE_DEFAULT (20)
//
class YBCTRL_API YBScrollWnd : public YBCtrlBase {
public:
	YBScrollWnd( YBViewInner* pViewInner, 
		unsigned int uScrollBarThick = YBSW_SBTHICK_DEFAULT, 
		unsigned int uStepMove = YBSW_STEPMOVE_DEFAULT );
	virtual ~YBScrollWnd();

public:
	YBCTRL_WNDMSGMAP_DECLARE()
	
public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent, WORD wCtrlID, HINSTANCE hInstance );
	YBViewInner* getViewInner() { return m_pViewInner; }
	int setScrollBarPos();
	YBScrollBar* getScrollBarH() { return m_pScrollBarH; }
	YBScrollBar* getScrollBarV() { return m_pScrollBarV; }
	void updateStatus();
	void reset();
	void enableRedraw( bool bReDraw );
	bool enableRedraw();
	void checkIsNeedScrollBar( bool& bNeedScrollBarH, bool& bNeedScrollBarV );

private:
	void getViewInnerRect( RECT& rcViewInner, bool bNeedScrollBarH, bool bNeedScrollBarV );
	void getScrollBarHRect( RECT& rcScrollBarH, bool bNeedScrollBarV );
	void getScrollBarVRect( RECT& rcScrollBarV, bool bNeedScrollBarH );
	void onScrollBarEventH( WORD wCtlEvt, LPARAM lParam );
	void onScrollBarEventV( WORD wCtlEvt, LPARAM lParam );
	void onViewInnerEvent ( WORD wCtrlEvt, LPARAM lParam );
	void adjustSBThumbLenH();
	void adjustSBThumbLenV();
	void adjustContentLTPos();
	int _calcSBValH( int nXPosLT );
	int _calcSBValV( int nYPosLT );

private:
	void _onWM_COMMAND( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_SIZE( YBCtrlWndMsg* pYBCtrlWndMsg );

protected:
	unsigned int m_uFlag;
	YBViewInner* m_pViewInner;
	YBScrollBar* m_pScrollBarH; // 水平滚动条
	YBScrollBar* m_pScrollBarV; // 垂直滚动条
	unsigned int m_uScrollBarThick; // 滚动条厚度
	unsigned int m_uStepMove; // 最小移动单位像素
	HBRUSH m_hBrushBK; // 背景画刷

};

YBCTRL_NAMESPACE_END
#endif //__YBSCROLLWND_H__