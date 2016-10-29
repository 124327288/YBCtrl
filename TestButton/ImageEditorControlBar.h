#ifndef __IMAGEEDITORCONTROLBAR_H__
#define __IMAGEEDITORCONTROLBAR_H__
#include "frame/YBCtrlBase.h"
#include "button/YBButton.h"
#include "progressbar/YBProgressCtrl.h"

class ImageEditorControlBar : public ybctrl::YBButton {
public:
	ImageEditorControlBar();
	~ImageEditorControlBar();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance );
	void setPath( const tstring_type& tstrDirPath );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	void _loadImage();
	void _init();
	void _drawStatus( HDC hDC );

private:
	tstring_type m_tstrDirPath;
	WORD m_wordCtrlID;
	HBRUSH m_hBrushBk;
	ybctrl::YBProgressCtrl m_dlgProCtrl;
	ybctrl::YBButton m_btnBig;		//放大按钮.
	ybctrl::YBButton m_btnSmall;	//缩小按钮.
	ybctrl::YBButton m_btnRotat;	//旋转按钮.
	ybctrl::YBButton m_btnCompare;	//对比按钮.
	ybctrl::YBButton m_btnPreview;	//预览按钮.
	ybctrl::YBButton m_btnUndo;		//撤销按钮.
	ybctrl::YBButton m_btnRedo;		//重做.
	ybctrl::YBButton m_btnRetore;		//还原按钮.
	// progress ctrl 
	unsigned int m_uProCtrlImageIdBK;
	unsigned int m_uProCtrlImageId;
	unsigned int m_uImageIdPCNormal;
	unsigned int m_uImageIdPCHover;
	unsigned int m_uImageIdPCPress;
	//button Image id.
	unsigned int m_uImageIdBtnBigNor;
	unsigned int m_uImageIdBtnBigHov;
	unsigned int m_uImageIdBtnBigPre;
	unsigned int m_uImageIdBtnSmallNor;
	unsigned int m_uImageIdBtnSmallHov;
	unsigned int m_uImageIdBtnSmallPre;
	unsigned int m_uImageIdBtnRotatNor;
	unsigned int m_uImageIdBtnRotatHov;
	unsigned int m_uImageIdBtnRotatPre;
	unsigned int m_uImageIdBtnCompareNor;
	unsigned int m_uImageIdBtnCompareHov;
	unsigned int m_uImageIdBtnComparePre;
	unsigned int m_uImageIdBtnPreviewNor;
	unsigned int m_uImageIdBtnPreviewHov;
	unsigned int m_uImageIdBtnPreviewPre;
	unsigned int m_uImageIdBtnUndoNor;
	unsigned int m_uImageIdBtnUndoHov;
	unsigned int m_uImageIdBtnUndoPre;
	unsigned int m_uImageIdBtnRedoNor;
	unsigned int m_uImageIdBtnRedoHov;
	unsigned int m_uImageIdBtnRedoPre;
	unsigned int m_uImageIdBtnRestoreNor;
	unsigned int m_uImageIdBtnRestoreHov;
	unsigned int m_uImageIdBtnRestorePre;
};

#endif //__IMAGEEDITORCONTROLBAR_H__