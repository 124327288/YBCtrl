#ifndef __YBVIEWINNER_H__
#define __YBVIEWINNER_H__
#include "frame/YBCtrlBase.h"
#include "view/YBViewContentBase.h"
YBCTRL_NAMESPACE_BEGIN
//
class YBCTRL_API YBViewInner : public YBCtrlBase {
	friend class YBScrollWnd;
public:
	YBViewInner( YBViewContentBase* pViewContent );
	virtual ~YBViewInner();

public:
	YBCTRL_WNDMSGMAP_DECLARE()
	
public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent, WORD wCtrlID, HINSTANCE hInstance );
	void setBkClr( COLORREF clrBK );
	bool getContentUpdateRect( RECT& rcDestInContent, RECT& rcDestInView );
	YBViewContentBase* getViewContentObj() { return m_pViewContent; }
	void setContentXPosLT( int nXPosLT );
	void setContentYPosLT( int nYPosLT );
	unsigned int getContentXPosLT() const { return m_nXPosContentStart; }
	unsigned int getContentYPosLT() const { return m_nYPosContentStart; }
	bool getRectContent2View( const RECT& rcInContent, RECT& rcInView );
	bool getRectView2Content( const RECT& rcInView, RECT& rcInContent );
	bool PointContent2View( const POINT& ptInC, POINT& ptInV ) const;
	bool PointView2Content( const POINT& ptInV, POINT& ptInC ) const;
	void enableRedraw( bool bEnable );
	bool enableRedraw();

private:
	void _onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEWHEEL( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEMOVE( YBCtrlWndMsg* pYBCtrlWndMsg );
	
private:
	void _drawStatus( HDC hDC );
	void _drawBK( HDC hDC );
	void _drawContent( HDC hDC );
	bool _getRectContent2View( const RECT& rcInContent, RECT& rcInView );
	bool _getRectView2Content( const RECT& rcInView, RECT& rcInContent );
	void _setScrollWnd( YBScrollWnd* pScrollWnd );

protected:
	YBScrollWnd* m_pScrollWnd;
private:
	DWORD m_dwFlag;
	COLORREF m_clrBK;
	YBViewContentBase* m_pViewContent;
	int m_nXPosContentStart; // 视窗内容需要显示部分左上角的X坐标
	int m_nYPosContentStart; // 视窗内容需要显示部分左上角的Y坐标
	WORD m_wCtrlId;
};




YBCTRL_NAMESPACE_END
#endif //__YBVIEWINNER_H__