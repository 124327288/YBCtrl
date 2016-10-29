#ifndef __YBITADDAPPDLG_H__
#define __YBITADDAPPDLG_H__
#include "dialog/YBModalDialog.h"
#include "button/YBButton.h"
#include "static/YBStatic.h"
#include "radiobtn/YBRadioBtn.h"

namespace ybctrl {
	class YBCtrlImageStatic;
	class YBCtrlImageBase;
};

class YBITAddAppDlg : public ybctrl::YBModalDialog {
public:
	typedef enum _ENUMAPPTYPE {
		EAPP_TYPE_LOCALAPP = 0, // local application
		EAPP_TYPE_WEBSITE = 1, // weisite application.
	} ENUMAPPTYPE;
	typedef std::basic_string< TCHAR >tstring_type;
public:
	YBITAddAppDlg();
	~YBITAddAppDlg();

public:
	ENUMAPPTYPE getAppType() const;
	tstring_type getAppTitle() const;
	tstring_type getAppLocalPath() const;
	tstring_type getWebSiteUrl() const;
	//tstring_type getAppDefExt() const;
	void setAppTitle( const tstring_type tstrAppTitle );
	void setAppLocalPath( const tstring_type tstrLocalPath );
	void setWebSiteUrl( const tstring_type tstrWebSiteUrl );
	void setDefaultType( int iType );

protected:
	virtual void _createUI( HWND hWndModalDlg, HINSTANCE hInst );
	virtual unsigned int _getWndHeight() const;
	virtual unsigned int _getWndWidth() const;

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

private:
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_CTLCOLOREDIT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	
private:
	void _loadImage();
	void _loadFont();
	void _drawLine( HDC hDC );
	bool _getFileInfo();

private:
	ENUMAPPTYPE m_eAppType;
	unsigned int m_uImageIdBk;
	unsigned int m_uFontIdSBtn;
	unsigned int m_uFontIdBBtn;
	unsigned int m_uFontIdEdit;
	unsigned int m_uFontIdRadio;
	// 
	ybctrl::YBRadioBtn m_radioBtn4CS;
	ybctrl::YBRadioBtn m_radioBtn4BS;
	ybctrl::YBButton m_btn4CS;
	ybctrl::YBButton m_btn4CSAdd;
	ybctrl::YBButton m_btn4BS;
	ybctrl::YBButton m_btn4Confirm;
	ybctrl::YBButton m_btn4Cancel;
	// small button 
	unsigned int m_uImageIdBtnNor;
	unsigned int m_uImageIdBtnHov;
	unsigned int m_uImageIdBtnPre;
	// small button2
	unsigned int m_uImageIdBtnNor2;
	unsigned int m_uImageIdBtnHov2;
	unsigned int m_uImageIdBtnPre2;
	// radio button
	unsigned int m_uImageIdRBBk;
	unsigned int m_uImageIdRBNormal;
	unsigned int m_uImageIdNRBNormal;
	tstring_type m_tstrLocalPath;
	tstring_type m_tstrFileName;
	tstring_type m_tstrDefExt;
	tstring_type m_tstrAppTitle;
	tstring_type m_tstrWebSiteUrl;
	//
	HWND m_hEditAppTitle; // 办公系统名称的输入编辑框
	HWND m_hEdit4BS;
	HWND m_hEdit4CS;
	HBRUSH m_hBrushEdit;
	HPEN m_hPenEditFrame;
	int m_iFirstWrite;
};

#endif //__YBITADDAPPDLG_H__