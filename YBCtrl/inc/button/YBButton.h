#ifndef __YBBUTTON_H__
#define __YBBUTTON_H__
#include "frame\YBCtrlBase.h"
#include "frame\YBCtrlDefs.h"
#include <deque>
//
namespace Gdiplus {
	class Image;
}
//
YBCTRL_NAMESPACE_BEGIN
class YBCtrlImageBase;
class YBCtrlImageStatic;
//
class YBCTRL_API YBButton : public YBCtrlBase {
public:
	typedef std::basic_string< TCHAR > tstring_type;
	typedef std::deque< tstring_type > tstrTxt_container_type;  //»»ÐÐ·û"\n".
public:
	typedef enum _ENUMBTNSTATUS {
		EBTN_STATUS_NORMAL = 0,
		EBTN_STATUS_HOVER = 1,
		EBTN_STATUS_PRESS = 2,
		EBTN_STATUS_DISABLE = 3
	} ENUMBTNSTATUS;
public:
	YBButton();
	virtual ~YBButton();

public:
	YBCTRL_WNDMSGMAP_DECLARE()

public:
	HWND Create( DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance );
	void setImage( ENUMBTNSTATUS eBtnStatus, unsigned int uImageID );
	void setFixedSpace( unsigned int uLFixed, unsigned int uTFixed, unsigned int uRFixed, unsigned int uBFixed );
	void setTextSpace( unsigned int uTxtSpaceL, unsigned int uTxtSpaceT, unsigned int uTxtSpaceR, unsigned int uTxtSpaceB );
	void setText( const tstring_type& tstrText );
	tstring_type getText() const { return m_tstrText; }
	void setFontId( unsigned int uFontId );
	void enable( bool bEnable );
	void setLIconSize( unsigned int uWidth, unsigned int uHeight );
	void setLIconSpace( unsigned int uLSpace, unsigned int uTSpace );
	void setRIconSize( unsigned int uWidth, unsigned int uHeight );
	void setRIconSpace( unsigned int uRSpace, unsigned int uTSpace );
	void setImageLIcon( unsigned int uLIconIdNor, unsigned int uLIconIdHov, unsigned int uLIconIdPre );
	void setImageRIcon( unsigned int uRIconIdNor, unsigned int uRIconIdHov, unsigned int uRIconIdPre );
	void setIcon( HICON hIcon, unsigned int uWidth, unsigned int uHeight );
	void setIconSpace( unsigned int uIconSpaceL ) { m_uIconSpaceL = uIconSpaceL; }
	void bDrawLIcon( bool bDrawLIcon );
	void bDrawRIcon( bool bDrawRIcon );
	void setTxtVcenter( bool bVcenter );
	void updateStatus();
	void addToolTip( bool bEnable );
	void setTipText( bool bTipText, const tstring_type& tstrTipText );
	void setHovTxtClr( COLORREF clrTxt = RGB( 143, 70, 1 ) );
	void setPreTxtClr(COLORREF clrTxt = RGB(143, 70, 1));
	void setDisTxtClr( COLORREF clrTxt = YBCOLOR_GRAY );
	void popUp();
	void pushDown();

private:
	void _onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDBLCLK( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONUP( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEMOVE( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSELEAVE( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_KEYDOWN( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_DESTROY( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _reset();

private:
	void _drawStatus( HDC hDC, ENUMBTNSTATUS eBtnStatus );
	void _drawLIcon( HDC hDC );
	void _drawRIcon( HDC hDC );
	void _drawIcon( HDC hDC );
	void _drawText( HDC hDC );
	//void _drawForeGround( HDC hDC );
	Gdiplus::Image* _getGPImage( ENUMBTNSTATUS eBtnStatus );
	YBCtrlImageStatic* _getYBImageStatic( ENUMBTNSTATUS eBtnStatus );
	YBCtrlImageBase* _getYBImageBase( ENUMBTNSTATUS eBtnStatus );
	YBCtrlImageStatic* _getImageStatic( unsigned int uImageId ) const;
	unsigned int _getImageID( ENUMBTNSTATUS eBtnStatus );
	bool _getDrawTextRect( RECT& rcDrawText );
	void _calcEveryPartTxt();
	void _calcLineHeight();
	void _getDrawIconRect( RECT& rcDrawIcon );

private:
	unsigned int m_uBtnFlag;
	ENUMBTNSTATUS m_eBtnStatus;
	unsigned int m_uImageIDBtnNormal;
	unsigned int m_uImageIDBtnHover;
	unsigned int m_uImageIDBtnPress;
	unsigned int m_uImageIDBtnDisable;
	unsigned int m_uLFixed;
	unsigned int m_uTFixed;
	unsigned int m_uRFixed;
	unsigned int m_uBFixed;
	unsigned int m_uBtnSpace;
	WORD m_wordCtrlID;
	unsigned int m_uTxtSpaceL;
	unsigned int m_uTxtSpaceT;
	unsigned int m_uTxtSpaceR;
	unsigned int m_uTxtSpaceB;
	unsigned int m_uFontId;
	// Icon
	unsigned int m_uImageIdLIconNor;
	unsigned int m_uImageIdLIconHov;
	unsigned int m_uImageIdLIconPre;
	unsigned int m_uImageIdRIconNor;
	unsigned int m_uImageIdRIconHov;
	unsigned int m_uImageIdRIconPre;
	unsigned int m_uLIconWidth;
	unsigned int m_uLIconHeight;
	unsigned int m_uLIconLSpace;
	unsigned int m_uLIconTSpace;
	unsigned int m_uRIconWidth;
	unsigned int m_uRIconHeight;
	unsigned int m_uRIconRSpace;
	unsigned int m_uRIconTSpace;
	bool m_bTipText;
	bool m_bVcenter;
	bool m_bSetHovTxtClr;
	bool m_bSetPreTxtClr;
	bool m_bSetImageIcon;
	COLORREF m_rgbHovTxtClr;
	COLORREF m_rgbPreTxtClr;
	COLORREF m_rgbDisTxtClr;
	unsigned int m_uLineHeight;
	HICON m_hIcon;
	SIZE m_sizeIcon;
	unsigned int m_uIconSpaceL;

#pragma warning( push )
#pragma warning (disable:4251 )
	tstring_type m_tstrText;
	tstring_type m_tstrTipText;
	tstrTxt_container_type m_containerTxt;
#pragma warning( pop )
};

YBCTRL_NAMESPACE_END
#endif //__YBBUTTON_H__