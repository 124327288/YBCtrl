#ifndef __YBFSMENUHEADER_H__
#define __YBFSMENUHEADER_H__
#include "frame/YBCtrlBase.h"
#include "scrollwnd/YBScrollWnd.h"

namespace ybctrl {
	class YBCtrlImageStatic;
}

class YBFSMenuHeader : public ybctrl::YBCtrlBase {
public:
	typedef std::basic_string< TCHAR >tstring_type;

public:
	YBFSMenuHeader();
	virtual ~YBFSMenuHeader();
	
public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance );
	void setText( tstring_type tstrFront, tstring_type tstrMiddle, tstring_type tstrLast );
	void setHeaderImageId( unsigned int uImageHeaderId );
	void setFontId( unsigned int uFontId );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageId ) const;
	void _drawStatus( HDC hDC );
	void _drawBk( HDC hDC );
	void _drawFrontTxt( HDC hDC );
	void _drawMiddleTxt( HDC hDC );
	void _drawLastTxt( HDC hDC );
	void _drawLine( HDC hDC );
	bool _getFrontTxtRect( RECT& rcFrontTxt );
	bool _getMiddleTxtRect( RECT& rcMiddleTxt );
	bool _getLastTxtRect( RECT& rcLastTxt );

private:
	unsigned int m_uImageIdBk;
	unsigned int m_uFontIdTxt;
	unsigned int m_uHeaderHeight;
	tstring_type m_tstrFront;
	tstring_type m_tstrMiddle;
	tstring_type m_tstrLast;
	HPEN m_hPen;

};

#endif //__YBFSMENUHEADER_H__