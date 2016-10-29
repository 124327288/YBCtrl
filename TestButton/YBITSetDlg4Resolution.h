#ifndef __YBITSETDLG4RESOLUTION_H__
#define __YBITSETDLG4RESOLUTION_H__
#include "dialog/YBModalDialog.h"
#include "radiobtn/YBRadioBtn.h"
#include "button/YBButton.h"

#define YBITDLG4RESOLUTION_RADIONBTN_ID (0x00000501)
#define YBITDLG4RESOLUTION_OKBUTTON_ID (0x00000502)

class YBITSetDlg4Resolution : public ybctrl::YBModalDialog {
public:
	typedef std::basic_string< TCHAR >tstring_type;

public:
	YBITSetDlg4Resolution();
	~YBITSetDlg4Resolution();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	bool isSelected();

private:
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

protected:
	virtual void _createUI( HWND hWndModalDlg, HINSTANCE hInst );
	virtual unsigned int _getWndHeight() const;
	virtual unsigned int _getWndWidth() const;

private:
	void _loadImage();
	void _loadFont();

private:
	unsigned int m_uImageIdBk;
	ybctrl::YBRadioBtn m_radioBtn4Res;
	ybctrl::YBButton m_btn4Res;
	// radio button
	unsigned int m_uImageIdRdBtnBk;
	unsigned int m_uFontIdRadio;
	unsigned int m_uImageIdRdBtnNor;
	unsigned int m_uImageIdRdBtnHor;
	unsigned int m_uImageIdRdBtnPre;
	unsigned int m_uImageIdRdBtnDisable;
	unsigned int m_uImageIdNRdBtnDisable;
	unsigned int m_uImageIdNRdBtnNor;
	unsigned int m_uImageIdNRdBtnHor;
	unsigned int m_uImageIdNRdBtnPre;
	// ok Button;
	unsigned int m_uFontIdBtn;
	unsigned int m_uImageIdBtnNor;
	unsigned int m_uImageIdBtnHor;
	unsigned int m_uImageIdBtnPre;

};

#endif //__YBITSETDLG4RESOLUTION_H__