#ifndef __IMAGEEDITORDLG2_H__
#define __IMAGEEDITORDLG2_H__
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "ImageEditorHeader.h"
#include "ImageEditorControlBar.h"
#include "ImageEditorProperty.h"
#include "button/YBButtonX.h"
#include <deque>

struct EditFontInfo {
	tstring_type m_tstrText;//�ı�����.
	tstring_type m_tstrFaceName;//��������
	COLORREF m_clrFont;
	unsigned int m_uSizeFont;
	RECT m_rcText;
	unsigned int m_uLineCount;
};
//
struct TextTuCeng {
	YBButtonX* m_pYBButtonX;
	tstring_type m_tstrText;
	unsigned int m_uIndex = 0;
	unsigned int m_uCtrlID = 0;//��ʾbuttonx�Ŀؼ�ID.
};
class ImageEditorDlg : public ybctrl::YBCtrlBase {
public:
	typedef std::deque< EditFontInfo* > containerFontInfo;
	typedef std::deque< TextTuCeng* > containerTextTC;
public:
	ImageEditorDlg();
	~ImageEditorDlg();

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
	void _onWM_COLOREDIT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageIdBk );

private:
	void _drawStatus( HDC hDC );
	void _drawStatus2( HDC hDC );
	void _drawBk( HDC hDC );
	void _drawCanvas( HDC hDC );
	void _drawImage( HDC hDC );
	void _drawTextBox( HDC hDC );//���ı���
	void _drawText( HDC hDC );

private:
	void _loadImage();
	void _initImageSpace();
	void _showTextBox( bool bShow );
	void _initEdit();
	void _pushback();
	bool _saveImage( HBITMAP hBitmap, tstring_type tstrMimeType, tstring_type tstrFileName );
	bool _getEncoderClsid( tstring_type tstrMimeType, CLSID* pClsid );
	void _calcLineSpace();
private:
	static LRESULT CALLBACK WndProcNew( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	containerFontInfo m_containerFontInfo;
	containerTextTC m_containerTextTC;
	unsigned int m_uFlag;
	unsigned int m_uImageId;
	RECT m_rcCanvas;
	RECT m_rcImage;
	RECT m_rcTxtBox;
	//
	ImageEditorHeader m_dlgHeader;
	ImageEditorControlBar m_dlgControlBar;
	ImageEditorProperty m_dlgProperty;
	//
	tstring_type m_tstrMimeType;//��ʾͼƬ�ĺ�׺�� Image//png
	tstring_type m_tstrDirPath;//��Ŀ¼·��.
	//
	POINT m_ptMouse;//��¼��갴��ȥʱ���λ��.
	unsigned int m_uBMPBSpace;//��¼��갴��ʱ��ͼƬ�ײ��ľ���.
	unsigned int m_uBMPRSpace;//��¼��갴��ʱ��ͼƬ�ұߵľ���.
	unsigned int m_uTBLSpace;
	unsigned int m_uTBTSpace;
	HWND m_hWndCaptureOld;
	HWND m_hWndEdit;
	bool m_bHide;
	bool m_bIsShow;
	bool m_bDrag;
	bool m_bMoveAll;
	POINT m_ptLBtnDownOld;
	bool m_bReturn;
	unsigned int m_uImageIdTCNor;
	unsigned int m_uImageIdTCPre;
};

#endif //__IMAGEEDITORDLG_H__