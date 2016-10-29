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
	tstring_type m_tstrText;//�ı�����.
	tstring_type m_tstrFaceName;//��������
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
	void _drawDefaultBmp( HDC hDC );//�༭��ͼƬ��Ĭ�ϻ滭
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
	unsigned int m_uImageIdEdit; //���б༭��ͼƬ
	HBRUSH m_hBrushBk;			//����������ˢ
	RECT m_rcBigBmpMax;			//��ʾ�༭ͼ�ı���������.	
	RECT m_rcBigZoomBmp;		//��ʾ�༭ͼ�Ŵ������ͼ������
	//
	bool m_bDrawLBtn;	  //true��ʾ�϶�ͼƬ�¼�.
	bool m_bMouseWheel;//true��ʾ�����������¼�.
	bool m_bRRotating;	  //true��ʾ���������ת��ť.
	bool m_bBBRectangle; //true��ʾͼƬ������ڷš������ǰ���.
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
	POINT m_ptLBtnDown; //��סͼƬʱ������(����ڱ༭ͼƬ�����Ͻ�)
	//
	ImageEditorHeader m_dlgHeader;
	ImageEditorControlBar m_dlgControlBar;
	ImageEditorProperty m_dlgProperty;
	//
	ybctrl::YBButton m_btnNext;//���Կ��ϵ���һҳ��ť
	ybctrl::YBButton m_btnPre;//���Կ��ϵ���һҳ��ť
	ybctrl::YBButton m_btnDelete;//���Կ��ϵ�ɾ����ť.
	ybctrl::YBButton m_btnAdd;//���Կ��ϵ�������ť��
	ybctrl::YBButton m_btnSave;//���Կ��ϵı��水ť
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
	HWND m_hWndEdit;//�����
	bool m_bShowEditDlg;//�Ƿ���ʾ�����
	bool m_bMoveCaret;//Ϊ��ʱ��ʾ�������߿򱻰��������.
	bool m_bShowEditDlgOld;
	HPEN m_hPenDotted;//�������ı߿�����
	RECT m_rcDottedInWnd;//���ο��ڴ����ϵ�λ��.
	//RECT m_rcDottedInReal;//���ο����ڴ�DC�ϵ�λ��.
	//
	containerFontInfo m_containerFontInfo;
	bool m_bSave;
	bool m_bLoadImage;
};

#endif //__IMAGEEDITOR_H__