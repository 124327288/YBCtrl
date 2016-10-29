#ifndef __YBITDLGFILEUPLOAD_H__
#define __YBITDLGFILEUPLOAD_H__
#include "frame\YBCtrlBase.h"
#include "button\YBButtonX.h"
#include "button\YBButton.h"
#include "static\YBStatic.h"
#include "checkbox\YBCheckBox.h"
#include "frame\YBCtrlImageBase.h"
#include "frame\YBCtrlImageStatic.h"
#include "dialog\YBModalDialog.h"


#define YBCTRLID_SAVE (0x00000100)
#define YBCTRLID_CANCEL (0x00000200)
#define YBCTRLID_CLOSE (0x00000300)
#define YBCTRLID_TEXT_TITLE (0x00000301)
#define YBCTRLID_TEXT_SAVE (0x00000302)
#define YBCTRLID_CHECK_FILE (0x00000303)
#define YBCTRLID_CHECK_DATABANK (0x00000304)
#define YBCTRLID_CHECK_LOCAL (0x00000305)
#define YBCTRLID_CHECK_PHOTO (0x00000306)
#define YBCTRLID_CHECK_CHAT (0x00000307)
#define YBCTRLID_EDIT (0x00000305)

class YBITDlgFileUpLoad : public ybctrl::YBModalDialog{
public:
	typedef std::basic_string< TCHAR > tstring_type;
public:
	YBITDlgFileUpLoad();
	virtual ~YBITDlgFileUpLoad();

public:
	void setFileName( const tstring_type& tstrFileName );
	tstring_type getFileName() const;
	bool getCheckState( unsigned int uCheckBoxID );

protected:
	virtual void _createUI( HWND hWndModalDlg, HINSTANCE hInst );
	virtual unsigned int _getWndHeight() const;
	virtual unsigned int _getWndWidth() const;

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

private:
	void _loadImage();
	void _loadFont();

private:
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_DESTROY( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_KEYDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _drawBk( HDC hDC );
	ybctrl::YBCtrlImageBase* _getTitleBkImageBase( unsigned int uBkImageId );
	ybctrl::YBCtrlImageStatic* _getTitleBkImageStatic( unsigned int uBkImageId );

private:
	unsigned int m_uImageYBITFULBk;
	ybctrl::YBButton m_btnSave;
	ybctrl::YBButton m_btnCancel;
	ybctrl::YBButton m_btnClose;
	ybctrl::YBCheckBox m_checkFileOf;
	ybctrl::YBCheckBox m_checkDataBank;
	ybctrl::YBCheckBox m_checkLocal;
	ybctrl::YBCheckBox m_checkPhoto;
	ybctrl::YBCheckBox m_checkChat;
	ybctrl::YBStatic m_staticTxtTittle;
	ybctrl::YBStatic m_staticTxtSave;
	HWND m_hEditFileName; // 文件名的那个编辑框窗口
	tstring_type m_tstrFileName; // 文件名缓存
	unsigned int m_uImageIdDisable;
	unsigned int m_uImageIdNDisable;
	unsigned int m_uImageIdBtnNor;
	unsigned int m_uImageIdBtnHov;
	unsigned int m_uImageIdBtnPre;
	unsigned int m_uImageIdBtnXNor;
	unsigned int m_uImageIdBtnXHov;
	unsigned int m_uImageIdBtnXPre;
	unsigned int m_uImageIdTittleBk;
	unsigned int m_uImageIdWhite;
	unsigned int m_uFontIdTxt;
	unsigned int m_uFontIdbtn;
	//check box
	unsigned int m_uFontIdCheckBox;
	unsigned int m_uImageIdCheckBk;
	unsigned int m_uImageIdNoCheckNor;
	unsigned int m_uImageIdNoCheckHov;
	unsigned int m_uImageIdNoCheckPre;
	unsigned int m_uImageIdCheckNor;
	unsigned int m_uImageIdCheckHov;
	unsigned int m_uImageIdCheckPre;
	//
	HPEN m_hPenEditFrame;
};


#endif // __YBITDLGFILEUPLOAD_H__