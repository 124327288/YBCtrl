#ifndef __IMAGEEDITORPROPERTY_H__
#define __IMAGEEDITORPROPERTY_H__
#include "frame/YBCtrlBase.h"
#include "radiobtn/YBRadioBtn.h"
#include <commdlg.h>
#include <deque>

USING_NAMESPACE_YBCTRL

struct EditorTxtProperty {
public:
	EditorTxtProperty() {
		m_tstrText = _T("");
		m_bVerArrangeMent = true;//是否水平排列
		m_tstrFaceName = _T("宋体");
		m_uXPos = 0;
		m_uYPos = 0;
		m_uSizeFont = 20;
		m_clrFont = RGB(255,255,255);
		m_uSizeStroke = 0;//描边
		m_clrStroke = RGB(255,255,255);
		m_bTransparent = true;
		m_nAngleRotat = 0;//旋转角度.
		m_uIndex = 0;
	}
	typedef std::basic_string<TCHAR > tstring_type;
	tstring_type m_tstrText;
	bool m_bVerArrangeMent;//是否水平排列
	tstring_type m_tstrFaceName;
	unsigned int m_uXPos;
	unsigned int m_uYPos;
	unsigned int m_uSizeFont;
	COLORREF m_clrFont;
	unsigned int m_uSizeStroke;//描边
	COLORREF m_clrStroke;
	bool m_bTransparent;
	int m_nAngleRotat;//旋转角度.
	unsigned int m_uIndex;
};

class ImageEditorProperty : public YBCtrlBase{
public:
	typedef std::basic_string< TCHAR > tstring_type;
	typedef std::deque< EditorTxtProperty* > containerTxtProperty;
public:
	ImageEditorProperty();
	~ImageEditorProperty();

public:
	YBCTRL_WNDMSGMAP_DECLARE();

public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance );
	void showPropertyDlg();
	EditorTxtProperty* getPropertyDlgByIndex( unsigned int uIndex );
	EditorTxtProperty* getPrePropertyDlg( unsigned int uIndex );
	EditorTxtProperty* getNextPropertyDlg( unsigned int uIndex );
	unsigned int getCurrIndex() { return m_uCurrIndex; };
	void setPath( const tstring_type& tstrDirPath );
	//
	tstring_type getText();
	tstring_type getFontFaceName();
	unsigned int getFontSize();
	COLORREF getTextClr();
	bool isCBB1Change() { return m_bCBB1Change; };

private:
	void _onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COMMAND( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_CTLCOLOREDIT( YBCtrlWndMsg* pYBCtrlWndMsg );
private:
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageIdBk );
	void _loadImage();
	void _loadFont();
	void _drawStatus( HDC hDC );
	void _drawBk( HDC hDC );
	void _drawHeadIcon( HDC hDC );
	void _drawLine( HDC hDC );
	void _drawText( HDC hDC );
	void _drawRGBBk( HDC hDC );
	void _init();
	void _reinit();
	void _clearAll();

private:
	void _save();

private:
	tstring_type m_tstrDirPath;
	WORD m_wordCtrlId;
	unsigned int m_uImageIdBk;
	unsigned int m_uImageIdIcon;
	unsigned int m_uFontId;
	HBRUSH m_hBrush;
	HWND m_hWndEdit;
	HWND m_hWndEditXPos;
	HWND m_hWndEditYPos;
	RECT m_rcEdit;
	HPEN m_hPenEdit;
	HPEN m_hPenLine;
	unsigned int m_uYLinePos;//输入框下面第一条线的Y坐标.
	unsigned int m_uSpace;//每行字之间的间隔距离
	YBRadioBtn m_rbtnHorArrangement;
	YBRadioBtn m_rbtnVerArrangement;
	YBRadioBtn m_rbtnSingleOutput;
	YBRadioBtn m_rbtnMultilineOutput;
	// radio Button
	unsigned int m_uImageIdWEDRBBk;
	unsigned int m_uImageIdWEDRBNor;
	unsigned int m_uImageIdWEDRBHor;
	unsigned int m_uImageIdWEDRBPre;
	unsigned int m_uImageIdWEDRBDisable;
	unsigned int m_uImageIdWEDNRBDisable;
	unsigned int m_uImageIdWEDNRBNor;
	unsigned int m_uImageIdWEDNRBHor;
	unsigned int m_uImageIdWEDNRBPre;
	//
	COLORREF m_colorRef1;
	COLORREF m_colorRef2;
	RECT m_rcColor1;
	RECT m_rcColor2;
	HWND m_hWndCBBox1;//本地字体
	HWND m_hWndCBBox2;//文字尺寸
	HWND m_hWndCBBox3;//文字描边
	HWND m_hWndCBBox4;//透明.
	unsigned int m_uIndex;
	unsigned int m_uCurrIndex;//表示当前第几个属性框
	bool m_bSave;
	bool m_bCBB1Change;
	//
public:
	containerTxtProperty m_containerTxtProperty;
};

#endif //__IMAGEEDITORPROPERTY_H__