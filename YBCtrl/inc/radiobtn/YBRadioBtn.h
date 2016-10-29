#ifndef __YBRADIOBTN_H__
#define __YBRADIOBTN_H__
#include "frame/YBCtrlBase.h"
#include <string>

namespace Gdiplus{
	class Image;
}

YBCTRL_NAMESPACE_BEGIN
class YBCtrlImageBase;
class YBCtrlImageStatic;

class YBCTRL_API YBRadioBtn : public YBCtrlBase{
public:
	typedef enum _ERADIOBTNSTATUS{
		ENORADIOBTN_STATUS_NORMAL = 0,
		ENORADIOBTN_STATUS_HOVER = 1,
		ENORADIOBTN_STATUS_PRESS = 2,
		ERADIOBTN_STATUS_NORMAL = 3,
		ERADIOBTN_STATUS_HOVER = 4,
		ERADIOBTN_STATUS_PRESS = 5,
		ENORADIOBTN_STATUS_DISABLE = 6,
		ERADIOBTN_STATUS_DISABLE = 7
	}ERADIOBTNSTATUS;
	typedef std::basic_string< TCHAR > tstring_type;

public:
	YBRadioBtn();
	~YBRadioBtn();

public:
	YBCTRL_WNDMSGMAP_DECLARE()

public:
	HWND Create( DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance );
	void setImageIdBk( unsigned int uImageIdBK );
	void setImage( ERADIOBTNSTATUS eRadioBtnStatus, unsigned int uImageId );
	void setTextSpace( unsigned int uTxtSpaceL, unsigned int uTxtSpaceT, unsigned int uTxtSpaceR, unsigned int uTxtSpaceB );
	void setRadioBtnLSpace( unsigned int uRdBtnIconSL );
	void setText( const tstring_type& tstrText );
	void setFontId( unsigned int uFontId );
	void InitialState();
	void setbeSelect( bool bSelected );
	bool isSelect();
	void setEnable( bool bEnable );
	void updateStatus();
	//void drawStatus( HDC hDC );

private:
	void _onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEMOVE( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONUP( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSELEAVE( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _drawStatus( HDC hDC );
	void _drawBK( HDC hDC );
	void _drawRadio( HDC hDC );
	void _drawText( HDC hDC );
	bool _getImageID( ERADIOBTNSTATUS eRadioBtnStatus, unsigned int& uImageId );
	YBCtrlImageBase* _getBKImageBase( unsigned int uImageIdBK );
	YBCtrlImageStatic* _getBKImageStatic( unsigned int uImageIdBK );
	Gdiplus::Image* _getBKGPImage( unsigned int uImageIdBK );
	void _getRadioBtnRect( RECT& rcRdBtnClient );
	bool _getDrawTextRect( RECT& rcDrawText );

private:
	ERADIOBTNSTATUS m_eRadioBtnStatus;
	unsigned int m_uNoRdBtnIIdNormal;
	unsigned int m_uNoRdBtnIIdHover;
	unsigned int m_uNoRdBtnIIdPress;
	unsigned int m_uNoRdBtnIIdDisable;
	unsigned int m_uRdBtnIIdNormal;
	unsigned int m_uRdBtnIIdHover;
	unsigned int m_uRdBtnIIdPress;
	unsigned int m_uRdBtnIIdDisable;
	unsigned int m_uRdBtnIIdBK;
	unsigned int m_uLFixed;
	unsigned int m_uTFixed;
	unsigned int m_uRFixed;
	unsigned int m_uBFixed;
	unsigned int m_uRdBtnIconSL;
	unsigned int m_uRdBtnIconWidth;
	unsigned int m_uRdBtnIconHeight;
	unsigned int m_uRdBtnFlag;
	unsigned int m_uTxtSpaceL;
	unsigned int m_uTxtSpaceT;
	unsigned int m_uTxtSpaceR;
	unsigned int m_uTxtSpaceB;
	unsigned int m_uFontId;
	WORD m_wordCtrlID;
#pragma warning(push)
#pragma warning(disable:4251)
	tstring_type m_tstrText;
#pragma warning(pop)
};

YBCTRL_NAMESPACE_END

#endif //__YBRADIOBTN_H__