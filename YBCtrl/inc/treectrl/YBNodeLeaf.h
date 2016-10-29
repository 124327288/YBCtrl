#ifndef __YBNODELEAF_H__
#define __YBNODELEAF_H__
#include "treectrl/YBNodeBase.h"
#include "frame/YBCtrlDefs.h"

YBCTRL_NAMESPACE_BEGIN

class YBCTRL_API YBNodeLeaf : public YBNodeBase {
public:
	YBNodeLeaf();
	virtual ~YBNodeLeaf();

protected:
	virtual void _calcPosition( unsigned int uXPosStart, unsigned int uYPosStart );
	virtual void _onWndMsg( HWND hWnd, unsigned int uMessage, WPARAM wParam, LPARAM lParam, PARAMONWNDMSG* pParamOnWndMsg );
	virtual void _drawSelf( HDC hDC ) const;

private:
	virtual unsigned int _getHeight() const;
	virtual unsigned int _getWidth() const;
	virtual void _drawStatus( HDC hDC ) const;
	
private:
	void _onEvtLeafNodeDBClk( HWND hWnd, PARAMONWNDMSG* pParamOnWndMsg );

protected:
	void _drawText( HDC hDC ) const;

public:
	unsigned int m_uFontIdTextNor;
#pragma warning(push)
#pragma warning(disable:4251)
	tstring_type m_tstrText;
#pragma warning(pop)

protected:
	unsigned int m_uWidth;
	unsigned int m_uHeight;
};

YBCTRL_NAMESPACE_END

#endif //__YBNODELEAF_H__