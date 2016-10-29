#ifndef __PICTURECTRL_H__
#define __PICTURECTRL_H__
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "button/YBButton.h"
#include <deque>

class PictureCtrl : public ybctrl::YBCtrlBase {
public:
	typedef std::basic_string< TCHAR > tstring_type;
	typedef std::deque< unsigned int > containerImageIndex;
public:
	typedef enum _ENUMRECTSTATUS {
		ERECT_STATUS_NONE = 0,
		ERECT_STATUS_BOTTOM = 1,
		ERECT_STATUS_MIDDLE = 2,
		ERECT_STATUS_TOP = 3
	} ENUMRECTSTATUS;
public:
	PictureCtrl( const tstring_type& tstrAppPath );
	~PictureCtrl();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( int x, int y, int uWidth, int uHeight, HWND hWndParent );
	void setBigBmpSpace( unsigned int uLSpace = 20, unsigned int uTSpace = 20, unsigned int uWidth = 100, unsigned int uHeight = 100 );
	void loadImage( const void *buf, size_t size );
	void loadImage( tstring_type tstrImageFilePath );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONUP( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	void _drawStatus( HDC hDC );
	void _drawBk( HDC hDC );
	void _drawBigBmp( HDC hDC );
	void _drawZoomBmp( HDC hDC );
	void _drawTopBmp( HDC hDC );
	void _drawMiddleBmp( HDC hDC );
	void _drawBottomBmp( HDC hDC );
	ybctrl::YBCtrlImageStatic* _getImageStatic(unsigned int uImageId);
	bool _getBigBmpRect( RECT& rcBigBmp );
	bool _getBottomBmpRect( RECT& rcBottomBmp );
	bool _getTopBmpRect( RECT& rcTopBmp );
	bool _getMiddleBmpRect( RECT& rcMiddlBmp );
	bool _getBigBmpIndex( unsigned int& uImageId );
	bool _getTopBmpIndex( unsigned int& uImageId );
	bool _getMiddleBmpIndex( unsigned int& uImageId );
	bool _getBottomBmpIndex( unsigned int& uImageId );

private:
	void _loadImage();
	void _loadFont();
	void _clipWnd();

private:
	containerImageIndex m_containerImageIndex;
	ENUMRECTSTATUS m_eRectStatus;
	tstring_type m_tstrAppPath;
	unsigned int m_uImageIdBk;
	RECT m_rcBigBmp;
	RECT m_rcBigBmpBk;
	POINT m_ptMouse;
	ybctrl::YBButton m_btnRotating;
	unsigned int m_uImageBtnNor;
	unsigned int m_uImageBtnHov;
	unsigned int m_uImageBtnPre;
	unsigned int m_uFontIdBtn;
	bool m_bRotating;
	bool m_bShowZoomBmp;
};

#endif //__PICTURECTRL_H__