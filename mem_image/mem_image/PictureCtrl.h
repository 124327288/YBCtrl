#ifndef __PICTURECTRL_H__
#define __PICTURECTRL_H__
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "button/YBButton.h"

class PictureCtrl : public ybctrl::YBCtrlBase {
public:
	typedef std::basic_string< TCHAR > tstring_type;
public:
	PictureCtrl( const tstring_type& tstrAppPath );
	~PictureCtrl();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( int x, int y, int uWidth, int uHeight, HWND hWndParent );
	void setBigBmpSpace( unsigned int uLSpace = 20, unsigned int uTSpace = 20, unsigned int uWidth = 100, unsigned int uHeight = 100 );
	void loadImage( const void *buf, size_t size );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONUP( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	void _drawStatus( HDC hDC );
	void _drawBk( HDC hDC );
	void _drawBigBmp( HDC hDC );
	void _drawZoomBmp( HDC hDC );
	ybctrl::YBCtrlImageStatic* _getImageStatic(unsigned int uImageId);
	bool _getBigBmpRect( RECT& rcBigBmp );

private:
	void _loadImage();

private:
	tstring_type m_tstrAppPath;
	unsigned int m_uImageIdBk;
	unsigned int m_uImageBigBmp;
	unsigned int m_uImageId1;
	RECT m_rcBigBmp;
	RECT m_rcBigBmpBk;
	POINT m_ptMouse;
};

#endif //__PICTURECTRL_H__