#ifndef __YBITMSGBOX_H__
#define __YBITMSGBOX_H__
#include "dialog/YBModalDialog.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlImageBase.h"
#include "static/YBStatic.h"
#include "frame/YBCtrlBase.h"
#include "button/YBButton.h"

#define YBITMSGBOX_BTN_CLOSE (0x00000100)
#define YBITMSGBOX_BTN_OK (0x00000101)
#define YBITMSGBOX_STATIC_TEXT (0x00000102)
#define YBITMSGBOX_STATIC_TITTLETEXT (0x00000103)

class YBITMsgBox : public ybctrl::YBModalDialog{
public:
	typedef std::basic_string< TCHAR > tstring_type;

public:
	YBITMsgBox( bool bAutoExit = false, unsigned int uExitTimeMS = 2000 );
	virtual ~YBITMsgBox();
protected:
	virtual void _createUI( HWND hWndModalDlg, HINSTANCE hInst );
	virtual unsigned int _getWndHeight() const;
	virtual unsigned int _getWndWidth() const;

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	void setText( const tstring_type& tstrText, const tstring_type& tstrTittleText, const tstring_type& tstrOkText );
	void setTextLocation( unsigned int uLSpace, unsigned int uTSpace, unsigned int uWidthText, unsigned int uHeightText );
	void setMBSize( unsigned int nMsgBoxWidth, unsigned int nMsgBoxHeight );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _drawBK( HDC hDC );
	void _loadImage();
	void _loadFont();
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageId );

private:
	unsigned int m_uImageIdBK;
	unsigned int m_uLFixed;
	unsigned int m_uTFixed;
	unsigned int m_uRFixed;
	unsigned int m_uBFixed;
	ybctrl::YBButton m_btnClose;
	ybctrl::YBButton m_btnOK;
	ybctrl::YBStatic m_staticText;
	ybctrl::YBStatic m_staticTittleText;
	unsigned int m_uImageIdStaticTxt;
	unsigned int m_uFontIdStaticTxt;
	unsigned int m_uImageIdBtnNor;
	unsigned int m_uImageIdBtnHov;
	unsigned int m_uImageIdBtnPre;
	unsigned int m_uImageIdBtnXNor;
	unsigned int m_uImageIdBtnXHov;
	unsigned int m_uImageIdBtnXPre;
	unsigned int m_uFontIdbtn;
	tstring_type m_tstrTittleText;
	tstring_type m_tstrText;
	tstring_type m_tstrOkText;
	unsigned int m_uLSpace;
	unsigned int m_uTSpace;
	unsigned int m_uWidthText;
	unsigned int m_uHeightText;
	unsigned int m_uMBWidth;
	unsigned int m_uMBHeight;
};

#endif //__YBITMSGBOX_H__