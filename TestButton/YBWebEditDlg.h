#ifndef __YBWEBEDITDLG_H__
#define __YBWEBEDITDLG_H__
#include "dialog/YBModalDialog.h"
#include "radiobtn/YBRadioBtn.h"
#include "button/YBButton.h"

#define YBWEBEDITID_BTN_OK (0x00000501)
#define YBWEBEDITID_BTN_CANCEL (0x00000502)
#define YBWEBEDITID_BTN_CLOSE (0x00000503)
#define YBWEBEDITID_RADIOBTN_BODYTEXT (0x00000504)
#define YBWEBEDITID_RADIOBTN_FULLTEXT (0x00000505)

class YBWebEditDlg : public ybctrl::YBModalDialog {
public:
	typedef std::basic_string< TCHAR >tstring_type;

public:
	YBWebEditDlg();
	~YBWebEditDlg();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	void setbSelect( bool bSelect );
	void setEnable( bool bEnable );
	int getWEType();

private:
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_KEYDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

protected:
	virtual void _createUI( HWND hWndModalDlg, HINSTANCE hInst );
	virtual unsigned int _getWndHeight() const;
	virtual unsigned int _getWndWidth() const;
	void _drawSelf( HDC hDC );

private:
	void _drawTitle( HDC hDC );
	void _loadImage();
	void _loadFont();
	void _clipWnd();
	void _clipWnd4Btn();
	bool _getTitleRect( RECT& rcTitle ) const;

private:
	unsigned int m_uImageIdWEDBk;
	ybctrl::YBButton m_btnOk;
	ybctrl::YBButton m_btnCancel;
	ybctrl::YBButton m_btnClose;
	ybctrl::YBRadioBtn m_rb4BodyText;
	ybctrl::YBRadioBtn m_rb4FullText;
	tstring_type m_tstrTitle;
	unsigned int m_uFontIdTitle;
	bool m_bEnable;
	bool m_bSelect;
	// ok Button;
	unsigned int m_uFontIdBtn;
	unsigned int m_uImageIdWEDBtnNor;
	unsigned int m_uImageIdWEDBtnHor;
	unsigned int m_uImageIdWEDBtnPre;
	// close Button
	unsigned int m_uImageIdWEDCBtnNor;
	unsigned int m_uImageIdWEDCBtnHor;
	unsigned int m_uImageIdWEDCBtnPre;
	// radio Button
	unsigned int m_uImageIdWEDRBBk;
	unsigned int m_uFontIdRadio;
	unsigned int m_uImageIdWEDRBNor;
	unsigned int m_uImageIdWEDRBHor;
	unsigned int m_uImageIdWEDRBPre;
	unsigned int m_uImageIdWEDRBDisable;
	unsigned int m_uImageIdWEDNRBDisable;
	unsigned int m_uImageIdWEDNRBNor;
	unsigned int m_uImageIdWEDNRBHor;
	unsigned int m_uImageIdWEDNRBPre;
};

#endif //__YBWEBEDITDLG_H__