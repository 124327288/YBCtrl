#ifndef __YBCHECKBOX_H__
#define __YBCHECKBOX_H__
#include "frame/YBCtrlBase.h"
#include <string>

namespace Gdiplus{
	class Image;
}

YBCTRL_NAMESPACE_BEGIN
class YBCtrlImageBase;
class YBCtrlImageStatic;

class YBCTRL_API YBCheckBox : public YBCtrlBase{
public:
	typedef enum _ECHECKBOXSTATUS{
		ENOCHECK_STATUS_NORMAL = 0,
		ENOCHECK_STATUS_HOVER = 1,
		ENOCHECK_STATUS_PRESS = 2,
		ECHECK_STATUS_NORMAL = 3,
		ECHECK_STATUS_HOVER = 4,
		ECHECK_STATUS_PRESS = 5,
		ECHECK_STATUS_DISABLE = 6,
		ENOCHECK_STATUS_DISABLE = 7,
	}ECHECKBOXSTATUS;
	typedef std::basic_string< TCHAR > tstring_type;

public:
	YBCheckBox();
	virtual~YBCheckBox();

public:
	YBCTRL_WNDMSGMAP_DECLARE()

public:
	HWND Create( DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance );
	void setImage( ECHECKBOXSTATUS eCheckStatus, unsigned int uImageID );
	void setBKImageID( unsigned int uImageId );
	void setCheckLSpace( unsigned int uCheckLSpace );
	void setTextSpace( unsigned int uTxtSpaceL, unsigned int uTxtSpaceT, unsigned int uTxtSpaceR, unsigned int uTxtSpaceB );
	void setText( const tstring_type& tstrText );
	void setbeChecked( bool bChecked );
	void setFontId( unsigned int uFontId );
	void setEnable( bool bEnable );
	void addToolTip( bool bEnable );
	void setToolTipTxt( const tstring_type& tstrTxt );
	bool isCheck();

private:
	void _onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEMOVE( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONUP( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSELEAVE( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_DESTROY( YBCtrlWndMsg* pYBCtrlWndMsg );
	//
	void _drawStatus( HDC hDC, ECHECKBOXSTATUS eCheckStatus );
	Gdiplus::Image* _getGPImage( ECHECKBOXSTATUS eCheckStatus );
	YBCtrlImageBase* _getImageBase( ECHECKBOXSTATUS eCheckStatus );
	YBCtrlImageStatic* _getImageStatic( ECHECKBOXSTATUS eCheckStatus );
	unsigned int _getCheckImageID( ECHECKBOXSTATUS eCheckStatus );
	void _getBoxRect( RECT& rcCheckBox );
	Gdiplus::Image* _getBKGPImage( unsigned int uImageId );
	YBCtrlImageBase* _getBKImageBase( unsigned int uImageId );
	YBCtrlImageStatic* _getBKImageStatic( unsigned int uImageId );
	bool _getDrawTextRect( RECT& rcDrawText );

private:
	ECHECKBOXSTATUS m_eCheckStatus;
	WORD m_wordCtrlId;
	unsigned int m_uImageIdBK;
	unsigned int m_uFlag;
	unsigned int m_uImageIdNCDisable;
	unsigned int m_uImageIdCDisable;
	unsigned int m_uImageIdNCNor;
	unsigned int m_uImageIdNCHov;
	unsigned int m_uImageIdNCPre;
	unsigned int m_uImageIdCNor;
	unsigned int m_uImageIdCHov;
	unsigned int m_uImageIdCPre;
	unsigned int m_uIconWidth;
	unsigned int m_uIconHeight;
	unsigned int m_uIconOriginL;
	unsigned int m_uIconOriginT;
	unsigned int m_uLFixed;
	unsigned int m_uTFixed;
	unsigned int m_uRFixed;
	unsigned int m_uBFixed;
	unsigned int m_uTxtSpaceL;
	unsigned int m_uTxtSpaceT;
	unsigned int m_uTxtSpaceR;
	unsigned int m_uTxtSpaceB;
	unsigned int m_uFontId;
	//
	bool m_bTipText;

#pragma warning( push )
#pragma warning (disable:4251 )
	tstring_type m_tstrText;
	tstring_type m_tstrToolTipTxt;
#pragma warning( pop )
};

YBCTRL_NAMESPACE_END
#endif //__YBCHECKBOX_H__