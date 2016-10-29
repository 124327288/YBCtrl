#ifndef __YBSPLITTER_H__
#define __YBSPLITTER_H__
#include "frame/YBCtrlBase.h"

YBCTRL_NAMESPACE_BEGIN

class YBCTRL_API YBSplitter : public YBCtrlBase {
public:
	typedef enum _ESPLITTERTYPE {
		ESPLITTER_HORIZONTAL = 0,
		ESPLITTER_VERTICAL
	} ESPLITTERTYPE;
public:
	YBSplitter( ESPLITTERTYPE eSplitterType );
	~YBSplitter();

public:
	YBCTRL_WNDMSGMAP_DECLARE()

public:
	HWND Create( HWND hWndRecv, int x, int y, int nWidth, int nHeight, int nMinSpace1, int nMinSpace2, WORD wCtrlId );
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
	int m_nStarMoveSpace;
	int m_nEndMoveSpace;
	bool m_b1stMoving;
};

YBCTRL_NAMESPACE_END
#endif //__YBSPLITTER_H__