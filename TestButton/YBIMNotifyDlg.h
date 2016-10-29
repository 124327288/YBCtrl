#ifndef __YBIMNOTIFYDLG_H__
#define __YBIMNOTIFYDLG_H__
#include "frame\YBCtrlBase.h"
#include "button\YBButton.h"
#include <string>
#include "tchar.h"

class CYBIMNotifyDlg : public ybctrl::YBCtrlBase {
public:
	typedef std::basic_string< TCHAR > tstring_type;
	enum ENUMSTATE {
		ESTATE_DOWNLOAD = 0, // 确定下载, 折叠状态
		ESTATE_INSTALL // 确定安装，展开状态
	};
public:
	CYBIMNotifyDlg();
	virtual ~CYBIMNotifyDlg();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND Create( int x, int y, HINSTANCE hInstance );
	ybctrl::YBButton* getBtnInstall();
	ybctrl::YBButton* getBtnCancel();
	//top
	void setTextTop( const tstring_type& tstrText );
	//middle
	void setTextMiddle( const tstring_type& tstrText );
	//under
	void setTextUnder( const tstring_type& tstrText );
	//time
	void setTextTime( tstring_type uMinute, tstring_type uSecond );
	//
	void drawState();
protected:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_KEYDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_SETCURSOR( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onBtnInstallClick();

private:
	bool _getBtnInstallRect( RECT& rcBtnInstall );
	bool _getBtnCancelRect( RECT& rcBtnCancel );
	void _drawState( HDC hDC );
	void _drawBK( HDC hDC );
	void _drawTextTop( HDC hDC );
	void _drawTextMiddle( HDC hDC );
	void _drawTextUnder( HDC hDC );
	void _drawTime( HDC hDC );
	ybctrl::YBCtrlImageStatic* _getBKImageObj();
	unsigned int _getBKImageId();
	bool _getDrawTextRect( RECT& rcDrawText );
	bool _getDrawTextRectMid( RECT& rcDrawText );
	bool _getDrawTextRectUnder( RECT& rcDrawText );
	bool _getDrawTextRectTime( RECT& rcDrawText, unsigned int uPartIndex );
	void _reset();
	void _loadImage();
	void _loadFont();
	void _init();

private:
	unsigned int m_uTxtSpaceL;
	unsigned int m_uTxtSpaceT;
	unsigned int m_uTxtSpaceR;
	unsigned int m_uTxtHeight;;
	//
	unsigned int m_uMTxtSpaceL;
	unsigned int m_uMTxtSpaceT;
	unsigned int m_uMTxtSpaceR;
	unsigned int m_uMTxtSpaceB;
	
	//
	unsigned int m_uUTxtSpaceL;
	unsigned int m_uUTxtHeight;
	unsigned int m_uUTxtSpaceR;
	unsigned int m_uUTxtSpaceB;
	//
	unsigned int m_fontIdBtnTxt;
	unsigned int m_fontIdBtnTxtTop;
	unsigned int m_fontIdBtnTxtMid;
	unsigned int m_fontIdBtnTxtUnder;
	unsigned int m_fontIdBtnLoad;
	unsigned int m_fontIdTime;
	//
	ENUMSTATE m_eState;
	unsigned int m_uImageDownloadBK; // 下载时背景图片
	unsigned int m_uImageInstallBK; // 安装时背景图片
	ybctrl::YBButton m_btnInstall; // 确定安装按钮
	ybctrl::YBButton m_btnCancel; // 取消按钮
	unsigned int m_uImageIdYBIMNDBtnNormal;
	unsigned int m_uImageIdYBIMNDBtnHover;
	unsigned int m_uImageIdYBIMNDBtnPress;
	//
	tstring_type m_tstrText;
	tstring_type m_tstrTextMid;
	tstring_type m_tstrTextUnder;
	tstring_type m_tstrTextTime;
	tstring_type m_tstrTextMinute;
	tstring_type m_tstrTextSecond;
	tstring_type m_uMinute;
	tstring_type m_uSecond;
};


#endif //__YBIMNOTIFYDLG_H__