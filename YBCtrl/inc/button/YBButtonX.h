#ifndef __YBBUTTONX_H__
#define __YBBUTTONX_H__
#include "frame\YBCtrlBase.h"
#include <string>
//
namespace Gdiplus {
	class Image;
}
//
YBCTRL_NAMESPACE_BEGIN
class YBCtrlImageBase;
class YBCtrlImageStatic;
//
class YBCTRL_API YBButtonX : public YBCtrlBase {
public:
	typedef enum _ENUMBTNSTATUS {
		EBTN_STATUS_NORMAL = 0,
		EBTN_STATUS_HOVER = 1,
		EBTN_STATUS_PRESS = 2,
		EBTN_STATUS_DISABLE = 3
	} ENUMBTNSTATUS;
	typedef enum _ENUMBTNXSTATUS {
		EBTNX_STATUS_NORMAL = 0,
		EBTNX_STATUS_HOVER = 1,
		EBTNX_STATUS_PRESS = 2
	} ENUMBTNXSTATUS;
	typedef std::basic_string< TCHAR > tstring_type;
public:
	YBButtonX();
	virtual ~YBButtonX();
	
public:
	YBCTRL_WNDMSGMAP_DECLARE()
		
public:
	HWND Create( DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance );
	void setImage( ENUMBTNSTATUS eBtnStatus, unsigned int uImageID );
	void setImage( ENUMBTNXSTATUS eBtnXStatus, unsigned int uImageID );
	void setFixedSpace( unsigned int uLFixed, unsigned int uTFixed, unsigned int uRFixed, unsigned int uBFixed );
	void setTextSpace( unsigned int uTxtSpaceL, unsigned int uTxtSpaceT, unsigned int uTxtSpaceR, unsigned int uTxtSpaceB );
	void setBtnXSpace( unsigned int uBtnXRSpace, unsigned int uBtnYTSpace );
	void setText( const tstring_type& tstrText );
	void setFontId( unsigned int uFontId );
	void setIcon(HICON hIcon, unsigned int uWidth, unsigned int uHeight);
	HICON getIcon() { return m_hIcon; }
	void setIconSpace( unsigned int uIconSpaceL ) { m_uIconSpaceL = uIconSpaceL; }
	void popUp();
	void pushDown();
	// 设置 小按钮是否可用
	void enableBtnX( bool bEnable );
	// 设置 按钮是否可用
	void enableBtn( bool bEnable );
	ENUMBTNSTATUS getBtnStatus() const { return m_eBtnStatus; }
	tstring_type getText() const { return m_tstrText; }
	// 让按钮画1下自身
	void drawStatus();
	void addToolTip( bool bEnable );
	void setBtnTipTxt( const tstring_type& tstrBtnTip );
	void setBtnXTipTxt( const tstring_type& tstrBtnXTip );
	void setTextCenter( bool bCenter );
	void setHovTxtClr(COLORREF clrTxt = RGB(143, 70, 1));
	void setPreTxtClr(COLORREF clrTxt = RGB(143, 70, 1));
	void setMoveTest( bool bMoveTest );  //WM_MOUSEMOVE时发送消息
	void updateStatus();

private:
	void _drawStatus( HDC hDC, ENUMBTNSTATUS eBtnStatus, ENUMBTNXSTATUS eBtnXStatus );
	Gdiplus::Image* _getGPImage( ENUMBTNSTATUS eBtnStatus );
	Gdiplus::Image* _getGPImage( ENUMBTNXSTATUS eBtnXStatus );
	YBCtrlImageStatic* _getYBImageStatic( ENUMBTNSTATUS eBtnStatus );
	YBCtrlImageStatic* _getYBImageStatic( ENUMBTNXSTATUS eBtnXStatus );
	YBCtrlImageBase* _getYBImageBase( ENUMBTNSTATUS eBtnStatus );
	YBCtrlImageBase* _getYBImageBase( ENUMBTNXSTATUS eBtnXStatus );
	unsigned int _getImageID( ENUMBTNSTATUS eBtnStatus );
	unsigned int _getImageID( ENUMBTNXSTATUS eBtnXStatus );
	bool _getBtnXRect( RECT& rcBtnX );
	bool _getDrawTextRect( RECT& rcDrawText );
	void _getDrawIconRect( RECT& rcDrawIcon );
	bool _isHideBtnX() const;

private:
	void _onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEMOVE( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSELEAVE( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDBLCLK( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONUP( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_DESTROY( YBCtrlWndMsg* pYBCtrlWndMsg );

private: 
	unsigned int m_uBtnFlag;
	unsigned int m_uBtnXFlag;
	ENUMBTNSTATUS m_eBtnStatus;
	ENUMBTNXSTATUS m_eBtnXStatus;
	unsigned int m_uImageIDBtnNormal;
	unsigned int m_uImageIDBtnHover;
	unsigned int m_uImageIDBtnPress;
	unsigned int m_uImageIDBtnDisable;
	unsigned int m_uImageIDBtnXNormal;
	unsigned int m_uImageIDBtnXHover;
	unsigned int m_uImageIDBtnXPress;
	unsigned int m_uLFixed;
	unsigned int m_uTFixed;
	unsigned int m_uRFixed;
	unsigned int m_uBFixed;
	unsigned int m_uBtnXRSpace;
	unsigned int m_uBtnXTSpace;
	WORD m_wordCtrlID;
	unsigned int m_uTxtSpaceL;
	unsigned int m_uTxtSpaceT;
	unsigned int m_uTxtSpaceR;
	unsigned int m_uTxtSpaceB;
	bool m_bCenter;

	bool m_bSetHovTxtClr;
	bool m_bSetPreTxtClr;
	COLORREF m_rgbHovTxtClr;
	COLORREF m_rgbPreTxtClr;

#pragma warning(push)
#pragma warning(disable:4251)
	tstring_type m_tstrText;
	tstring_type m_tstrBtnXTip;
	tstring_type m_tstrBtnTip;
#pragma warning(pop)
	unsigned int m_uFontId;
	HICON m_hIcon;
	unsigned int m_uIconSpaceL;
	SIZE m_sizeIcon;
};

YBCTRL_NAMESPACE_END
#endif //__YBBUTTONX_H__