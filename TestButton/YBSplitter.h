#ifndef __YBSPLITTER_H__
#define __YBSPLITTER_H__
#include "frame/YBCtrlBase.h"

class YBSplitter : public ybctrl::YBCtrlBase {
public:
	typedef enum _ESPLITTERTYPE {
		ESPLITTER_HORIZONTAL = 0,
		ESPLITTER_VERTICAL
	} ESPLITTERTYPE;
public:
	YBSplitter( ESPLITTERTYPE eSplitterType );
	~YBSplitter();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( HWND hWndRecv, int x, int y, int nWidth, int nHeight, int nMinSpace1, int nMinSpace2, WORD wCtrlId );
	void drawStatus();
	void setBkClr( COLORREF clrBk );
	void setMoveBkClr( COLORREF clrMoveBk );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSELEAVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONUP( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	void _drawStatus( HDC hDC );
	void _drawBk( HDC hDC );
	void _drawMoveBk( HDC hDC );
	
private:
	unsigned int m_uFlag;
	ESPLITTERTYPE m_eSplitterType;
	HBRUSH m_hBrushBk;
	HBRUSH m_hBrushMoveBk;
	HWND m_hWndRecv;
	HWND m_hWndSetCaptureOld;
	WORD m_wordCtrlID;
	int m_nXPos;
	int m_nYPos;
	int m_nMinSpace1;
	int m_nMinSpace2;
};

#endif //__YBSPLITTER_H__