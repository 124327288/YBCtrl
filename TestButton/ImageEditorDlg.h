#ifndef __IMAGEEDITOR_H__
#define __IMAGEEDITOR_H__
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "ImageEditorHeader.h"
#include "ImageEditorControlBar.h"
#include "ImageEditorProperty.h"
#include "button/YBButton.h"
#include <deque>

struct FontInfo {
	tstring_type m_tstrText;//文本内容.
	tstring_type m_tstrFaceName;//何种字体
	COLORREF m_clrFont;
	unsigned int m_uSizeFont;
	//view in client;
	RECT m_rcVInC;
};
//
class ImageEditor : public ybctrl::YBCtrlBase {
public:
	typedef std::deque< FontInfo* > containerFontInfo;
public:
	ImageEditor();
	~ImageEditor();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE();

public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent, HINSTANCE hInstance );
	void setPath( const tstring_type& tstrDirPath );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONUP( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEWHEEL( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_SETCURSOR( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COLOREDIT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageIdBk );

private:
	void _drawStatus( HDC hDC );
	void _drawBk( HDC hDC );
	void _drawDefaultBmp( HDC hDC );//编辑的图片的默认绘画
	void _drawDragBmp( HDC hDC );
	void _drawWheelBmp( HDC hDC );
	void _drawDotted( HDC hDC );
	//
	void _drawStatus2( HDC hDC );
	void _drawText();
	void _loadImage();
	void _loadFont();
	void _init();
	void _reloadImage();
	void _moveEdit( bool bVisible );
	bool _save( HBITMAP hBitmap, tstring_type tstrMimeType, tstring_type tstrFileName );
	bool _getEncoderClsid( tstring_type tstrMimeType, CLSID* pClsid );
	void _pushback();
	void _initEdit();

private:
	static LRESULT CALLBACK WndProcNew( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	tstring_type m_tstrDirPath;
	unsigned int m_uFlag;
	unsigned int m_uImageIdEdit; //进行编辑的图片
	HBRUSH m_hBrushBk;			//整个背景画刷
	RECT m_rcBigBmpMax;			//表示编辑图的背景的区域.	
	RECT m_rcBigZoomBmp;		//表示编辑图放大后整个图的区域。
	//
	bool m_bDrawLBtn;	  //true表示拖动图片事件.
	bool m_bMouseWheel;//true表示滑动鼠标滚轮事件.
	bool m_bRRotating;	  //true表示点击向右旋转按钮.
	bool m_bBBRectangle; //true表示图片按长宽摆放。否则是按宽长.
	int m_nSrcX;
	int m_nSrcY;
	int m_nDstX;
	int m_nDstY;
	int m_nSrcWidth;
	int m_nSrcHeight;
	int m_nDstWidth;
	int m_nDstHeight;
	int m_nSrcWidthMax;
	int m_nSrcHeightMax;
	//
	int m_nMaxMultiple;
	//
	POINT m_ptMouse;
	POINT m_ptLBtnDown; //按住图片时的坐标(相对于编辑图片的左上角)
	//
	ImageEditorHeader m_dlgHeader;
	ImageEditorControlBar m_dlgControlBar;
	ImageEditorProperty m_dlgProperty;
	//
	ybctrl::YBButton m_btnNext;//属性框上的下一页按钮
	ybctrl::YBButton m_btnPre;//属性框上的上一页按钮
	ybctrl::YBButton m_btnDelete;//属性框上的删除按钮.
	ybctrl::YBButton m_btnAdd;//属性框上的新增按钮。
	ybctrl::YBButton m_btnSave;//属性框上的保存按钮
	unsigned int m_ubtnNor;
	unsigned int m_ubtnHov;
	unsigned int m_ubtnPre;
	unsigned int m_ubtnDis;
	unsigned int m_uFontId;
	//
	HBITMAP m_hBitmap;
	tstring_type m_tstrMimeType;
	tstring_type m_tstrFilePath;
	tstring_type m_tstrNewFilePath;
	tstring_type m_tstrNewFilePath2;
	tstring_type m_tstrTmpFilePath;
	//
	HWND m_hWndEdit;//输入框
	bool m_bShowEditDlg;//是否显示输入框
	bool m_bMoveCaret;//为真时表示的是虚线框被按着拉大的.
	bool m_bShowEditDlgOld;
	HPEN m_hPenDotted;//画输入框的边框虚线
	RECT m_rcDottedInWnd;//矩形框在窗体上的位置.
	//RECT m_rcDottedInReal;//矩形框在内存DC上的位置.
	//
	containerFontInfo m_containerFontInfo;
	bool m_bSave;
	bool m_bLoadImage;
};

#endif //__IMAGEEDITOR_H__