#ifndef __YBITDLGFILEUPLOAD_H__
#define __YBITDLGFILEUPLOAD_H__
#include "frame\YBCtrlBase.h"
#include "button\YBButtonX.h"
#include "button\YBButton.h"
#include "static\YBStatic.h"
#include "checkbox\YBCheckBox.h"
#include "frame\YBCtrlImageBase.h"
#include "frame\YBCtrlImageStatic.h"

namespace Gdiplus{
	class Image;
}

class YBITDlgFileUpLoadRaw : public ybctrl::YBCtrlBase{
public:
	YBITDlgFileUpLoadRaw();
	virtual ~YBITDlgFileUpLoadRaw();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND Create( int x, int y, int nWidth, int nHeight, HWND hWndParent, HINSTANCE hInstance );
// 	ybctrl::YBButtonX* getBtnXTitle();
// 	ybctrl::YBButton* getBtnSave();
// 	ybctrl::YBButton* getBtnCancel();

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _drawBk( HDC hDC );
	ybctrl::YBCtrlImageBase* _getBkImageBase( unsigned int uBkImageId );
	ybctrl::YBCtrlImageStatic* _getBkImageStatic( unsigned int uBkImageId );
	ybctrl::YBCtrlImageBase* _getTitleBkImageBase( unsigned int uBkImageId );
	ybctrl::YBCtrlImageStatic* _getTitleBkImageStatic( unsigned int uBkImageId );
	void _loadImage();
	void _loadFont();

private:
	bool _getEncoderClsid( tstring_type tstrMimeType, CLSID* pClsid );
	bool _save( HBITMAP hBitmap, tstring_type tstrMimeType, tstring_type tstrFileName );

private:
	unsigned int m_uBkImageId;
	unsigned int m_uTitleBkImageId;
	ybctrl::YBButton m_btnClose;
	ybctrl::YBButton m_btnSave;
	ybctrl::YBButton m_btnCancel;
	ybctrl::YBCheckBox m_checkFileOf;
	ybctrl::YBCheckBox m_checkDataBank;
	ybctrl::YBStatic m_staticTxtTittle;
	ybctrl::YBStatic m_staticTxtSave;
	// button
	unsigned int m_uFontIdBtn;
	unsigned int m_uImageIdBtnXNor;
	unsigned int m_uImageIdBtnXHov;
	unsigned int m_uImageIdBtnXPre;
	unsigned int m_uImageIdBtnSaveNor;
	unsigned int m_uImageIdBtnSaveHov;
	unsigned int m_uImageIdBtnSavePre;
	unsigned int m_uImageIdBtnCancelNor;
	unsigned int m_uImageIdBtnCancelHov;
	unsigned int m_uImageIdBtnCancelPre;
	// check box
	unsigned int m_uFontIdCheckBox;
	unsigned int m_uImageIdCheckBk;
	unsigned int m_uImageIdNoCheckNor;
	unsigned int m_uImageIdNoCheckHov;
	unsigned int m_uImageIdNoCheckPre;
	unsigned int m_uImageIdCheckNor;
	unsigned int m_uImageIdCheckHov;
	unsigned int m_uImageIdCheckPre;
	// static text
	unsigned int m_uImageIdStaTxtTittle;
	unsigned int m_uImageIdStaTxtSave;
	unsigned int m_uFontIdTxt;
};

#endif // __YBITDLGFILEUPLOAD_H__