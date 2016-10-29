#ifndef __YBCHECKBOX4UD_H__
#define __YBCHECKBOX4UD_H__
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlImageStatic.h"
#include <string>

#define WM_VOLBTNCHECK (WM_USER + 100)

namespace Gdiplus{
	class Image;
}

namespace ybctrl{
	class YBCtrlImageBase;
	class YBCtrlImageStatic;
}

class YBCheckBox4UD : public ybctrl::YBCtrlBase{
public:
	typedef std::basic_string< TCHAR > tstring_type;
public:
	typedef enum _EBOX4UDSTATUS{
		EBOX_STATUS_CHECK = 0,
		EBOX_STATUS_NOCHECK = 1,
	}EBOX4UDSTATUS;
	typedef enum _ENUMBTNSTATUS{
		EBTN_STATUS_NORMAL = 0,
		EBTN_STATUS_HOVER = 1,
		EBTN_STATUS_PRESS = 2
	}EBTNSTATUS;
	typedef std::basic_string< TCHAR > tstring_type;

public:
	YBCheckBox4UD();
	virtual~YBCheckBox4UD();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance );
	void setCheckStatus( bool bChecked );
	bool isCheck();
	void setText( tstring_type tstrText );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONUP( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSELEAVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageId );
	bool _getBoxImageId( EBOX4UDSTATUS eBoxStatus, unsigned int& uImageId );
	bool _getBtnImageId( EBTNSTATUS eBtnStatus, unsigned int& uImageId );
	void _drawBox( HDC hDC );
	void _drawBtn( HDC hDC );
	void _drawStatus( HDC hDC );
	void _loadImage();
	void _loadFont();
	bool _getDrawTextRect( RECT& rcDrawText );
	void _drawText( HDC hDC );
	void _drawTriangle( HDC hDC );

private:
	WORD m_wordCtrlId;
	unsigned int m_uFontId;
	RECT m_rcBox;
	unsigned int m_uFlag;
	EBOX4UDSTATUS m_eBoxStatus;
	EBTNSTATUS m_eBtnStatus;
	unsigned int m_uImageIdBoxNoCheck;
	unsigned int m_uImageIdBoxCheck;
	unsigned int m_uImageIdBtnNor;
	unsigned int m_uImageIdBtnHov;
	unsigned int m_uImageIdBtnPre;

#pragma warning( push )
#pragma warning (disable:4251 )
	tstring_type m_tstrText;
#pragma warning( pop )
};

#endif //__YBCHECKBOX_H__