#ifndef __YBCORNERNOTIFYDLG_H__
#define __YBCORNERNOTIFYDLG_H__
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlImageBase.h"
#include "YBButton4CNDlg.h"
#include <map>

namespace ybctrl{
	class YBCtrlImageBase;
	class YBCtrlImageStatic;
	class YBCtrlImageGif;
}
class YBCornerNotifyDlg;
//
typedef void (*pfnWndDestroyProcType)( YBCornerNotifyDlg* pCNDlg, void* pParam );
//
typedef void (*pfnWndLBtnDownProcType)( YBCornerNotifyDlg* pCNDlg, void* pParam );
//
class YBCornerNotifyDlg : public ybctrl::YBCtrlBase{
public:
	typedef __int32 flag_type;
	typedef std::map< flag_type, int > flag2int_map_type;

public:
	typedef std::basic_string< TCHAR > tstring_type;

public:
	YBCornerNotifyDlg( int nType, const tstring_type& tstrTime, unsigned int uIndexCNDlg, const tstring_type& tstrTypeName, const tstring_type& tstrContent, pfnWndDestroyProcType pfnWndDestroyProc, void* pParamWndDestroy, pfnWndLBtnDownProcType pfnWndLBProc, const tstring_type& tstrClickEvtParam, unsigned int uExitTimeMS = 2000 );
	virtual ~YBCornerNotifyDlg();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( int x, int y, unsigned int uWidth, unsigned int uHeight, HWND hWndParent, HINSTANCE hInstance );
	int getCNDlgIndex() const;

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSELEAVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_TIMER( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_DESTROY( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_SETCURSOR( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onYBWM_MOUSEMOVE_BTN_CNDLG( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_SETFOCUS( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageId );
	ybctrl::YBCtrlImageGif* _getImageGif( unsigned int uImageId );
	void _drawStatus( HDC hDC );
	void _drawBk( HDC hDC );
	void _drawTimeText( HDC hDC );
	void _drawFrameLine( HDC hDC );
	void _drawContentText( HDC hDC );
	void _drawIcon( HDC hDC );
	void _drawIconText( HDC hDC );
	bool _getIconTxtRect( RECT& rcTitle );
	bool _getIconRect( RECT& rcIcon );
	bool _getContentRect( RECT& rcContent );
	bool _getTimeRect( RECT& rcTime );
	//
	void _loadFont();
	void _loadImage();
	void _init();
	void _bind2Type();
	void _unbind2Type();

private:
	flag2int_map_type s_maptype2int;
	flag_type m_flag4Type;
	int m_nIndexCNDlg;
	pfnWndDestroyProcType m_pfnWndDestroyProc;
	pfnWndLBtnDownProcType m_pfnWndLBtnDownProc;
	void* m_pParamWndDestroy;
	tstring_type m_tstrClickEvtParam;
	tstring_type m_tstrTypeName;
	tstring_type m_tstrContent;
	tstring_type m_tstrTime;
	YBButton4CNDlg m_btnNextScan;
	YBButton4CNDlg m_btnClickScan;
	YBButton4CNDlg m_btnIgnoreAll;
	unsigned int m_uExitTimeMS;
	HPEN m_hPen;
	//
	unsigned int m_uImageIdCNBk;
	unsigned int m_uImageIdCNBtnNor;
	unsigned int m_uImageIdCNBtnPre;
	unsigned int m_uFontIdCNIcon;
	unsigned int m_uFontIdCNBtn;
	unsigned int m_uFontIdCNTime;
	unsigned int m_uFontIdCNContent;
	//
	unsigned int m_uImageIdCNIcon1;
	unsigned int m_uImageIdCNIcon2;
	unsigned int m_uImageIdCNIcon3;
	unsigned int m_uImageIdCNIcon4;
	unsigned int m_uImageIdCNIcon5;
	unsigned int m_uImageIdCNIcon6;
	unsigned int m_uImageIdCNIcon7;
	unsigned int m_uImageIdCNIcon8;
	unsigned int m_uImageIdCNIcon9;
	unsigned int m_uImageIdCNIcon10;
	unsigned int m_uImageIdCNIcon11;
	unsigned int m_uImageIdCNIcon12;
	unsigned int m_uImageIdCNIcon13;
	unsigned int m_uImageIdCNIcon14;
	unsigned int m_uImageIdCNIcon15;
	unsigned int m_uImageIdCNIcon16;
	unsigned int m_uImageIdCNIcon17;
	unsigned int m_uImageIdCNIcon18;
	unsigned int m_uImageIdCNIcon19;
	unsigned int m_uImageIdCNIcon20;
	unsigned int m_uImageIdCNIcon21;
	unsigned int m_uImageIdCNIcon22;
	unsigned int m_uImageIdCNIcon23;
	unsigned int m_uImageIdCNIcon24;
	unsigned int m_uImageIdCNIcon25;
	unsigned int m_uImageIdCNIcon26;
};

#endif //__YBCORNERNOTIFYDLG_H__