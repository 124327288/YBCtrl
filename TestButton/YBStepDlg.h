#ifndef __YBSTEPDLG_H__
#define __YBSTEPDLG_H__
#include "frame/YBCtrlBase.h"

class YBStepDlg : public ybctrl::YBCtrlBase{
public:
	typedef std::basic_string< TCHAR >tstring_type;
public:
	YBStepDlg();
	virtual~ YBStepDlg();

public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent, HINSTANCE hInstance );

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

protected:
	void drawTextStep( HDC hDC );
	void drawTextContent( HDC hDC );

private:
	bool _getDrawTextStep( RECT& rcDrawTextStep );
	bool _getDrawTextContent( RECT& rcDrawTextContent );
	void _init();

protected:
	RECT m_rcTextStep;
	RECT m_rcTextContent;
	HBRUSH m_hBrushStep;
	HBRUSH m_hBrushContent;

#pragma warning (push)
#pragma warning (disable:4251)
	tstring_type m_tstrTextStep;
	tstring_type m_tstrTextContent;
#pragma warning (pop)

private:
	unsigned int m_uFontIdStep;
	unsigned int m_uFontIdContent;

};

#endif //__YBSTEPDLG_H__