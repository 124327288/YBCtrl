#ifndef __IMAGEEDITORHEADER_H__
#define __IMAGEEDITORHEADER_H__
#include "frame/YBCtrlBase.h"
#include "button/YBButton.h"
#include "button/YBButtonX.h"
#include "frame/YBCtrlImageStatic.h"

class ImageEditorHeader : public ybctrl::YBCtrlBase {
public:
	ImageEditorHeader();
	~ImageEditorHeader();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( HWND hWndParent, HMENU hMenu, HINSTANCE hInstance );
	void setPath( const tstring_type& tstrDirPath );
	unsigned int getFontId() { return m_uFontIdBottom; };

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	void _loadImage();
	void _loadFont();
	void _init();
	void _drawStatus( HDC hDC );
	void _drawBk( HDC hDC );
	void _drawEditTextIcon( HDC hDC );
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageId );

private:
	ybctrl::YBButtonX m_btnEditorImage;
	ybctrl::YBButtonX m_btnEditorText;
	ybctrl::YBButtonX m_btnEditorVedio;
	ybctrl::YBButtonX m_btnScreenPart;
	//
	ybctrl::YBButton m_btnOpen;
	ybctrl::YBButton m_btnClear;
	ybctrl::YBButton m_btnDelete;
	ybctrl::YBButton m_btnSave;
	ybctrl::YBButton m_btnSaveAs;
	//
	unsigned int m_uImageIdEdtTextIcon;
	unsigned int m_uImageIdEdtImagebtnNor;
	unsigned int m_uImageIdEdtImagebtnHov;
	unsigned int m_uImageIdEdtImagebtnPre;
	unsigned int m_uImageIdEdtTextbtnNor;
	unsigned int m_uImageIdEdtTextbtnHov;
	unsigned int m_uImageIdEdtTextbtnPre;
	unsigned int m_uImageIdEdtVediobtnNor;
	unsigned int m_uImageIdEdtVediobtnHov;
	unsigned int m_uImageIdEdtVediobtnPre;
	unsigned int m_uImageIdScrPartNor;
	unsigned int m_uImageIdScrPartHov;
	unsigned int m_uImageIdScrPartPre;
	unsigned int m_uImageIdOpenbtnNor;
	unsigned int m_uImageIdOpenbtnHov;
	unsigned int m_uImageIdOpenbtnPre;
	unsigned int m_uImageIdClearbtnNor;
	unsigned int m_uImageIdClearbtnHov;
	unsigned int m_uImageIdClearbtnPre;
	unsigned int m_uImageIdDeletebtnNor;
	unsigned int m_uImageIdDeletebtnHov;
	unsigned int m_uImageIdDeletebtnPre;
	unsigned int m_uImageIdSavebtnNor;
	unsigned int m_uImageIdSavebtnHov;
	unsigned int m_uImageIdSavebtnPre;
	unsigned int m_uImageIdSaveAsbtnNor;
	unsigned int m_uImageIdSaveAsbtnHov;
	unsigned int m_uImageIdSaveAsbtnPre;
	//
	RECT m_rcEdtImageBtn;
	RECT m_rcEdtTextBtn;
	RECT m_rcEdtVedioBtn;
	RECT m_rcScreenPartBtn;
	RECT m_rcOpenBtn;
	RECT m_rcClearBtn;
	RECT m_rcDeleteBtn;
	RECT m_rcSaveBtn;
	RECT m_rcSaveAsBtn;
	//
	HBRUSH m_hBrushBkTop;
	HBRUSH m_hBrushBkBottom;
	unsigned int  m_uFontIdTop;
	unsigned int  m_uFontIdBottom;
	//
	WORD m_wordCtrlId;
	tstring_type m_tstrDirPath;
};

#endif //__IMAGEEDITORHEADER_H__