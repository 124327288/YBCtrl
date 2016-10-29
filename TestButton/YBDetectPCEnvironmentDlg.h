#ifndef __YBDETECTPCENVIRONMENTDLG_H__
#define __YBDETECTPCENVIRONMENTDLG_H__
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "scrollwnd/YBScrollWnd.h"

class YBDetectPCEnvironmentDlg : public ybctrl::YBCtrlBase {
public:
	typedef std::basic_string< TCHAR > tstring_type;

public:
	YBDetectPCEnvironmentDlg();
	virtual ~YBDetectPCEnvironmentDlg();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE();

public:
	HWND create( int nWidth, int nHeight, HWND hWndParent, WORD wCtrlId );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	void _drawStatus( HDC hDC );
	void _drawBk( HDC hDC );
	void _drawTitle( HDC hDC );
	void _drawBlockBk( HDC hDC );
	void _drawBlockText( HDC hDC );
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageId ) const;
	void _loadImage();
	void _loadFont();
	void _clipWnd();
	void _initListbox();
	bool _getTitleRect( RECT& rcTitle ) const;
	bool _getBlockBkRect( RECT& rcBlock ) const;
	bool _getBlockNumRect( RECT& rcBlockNum ) const;
	bool _getBlockItemRect( RECT& rcBlockItem ) const;
	bool _getBlockCfgRect( RECT& rcBlockCfg ) const;
	bool _getBlockResRect( RECT& rcBlockRes ) const;
	bool _getBlockNoteRect( RECT& rcBlockNote ) const;

private:
	unsigned int m_uImageIdPCDEBk;
	unsigned int m_uImageIdPCDEBlockBk;
	unsigned int m_uImageIdPCDEBlockSpace;
	unsigned int m_uFontIdPCDETitle;
	unsigned int m_uFontIdPCDEBlockTxt;
	tstring_type m_tstrTitle;
	tstring_type m_tstrBlockNum;
	tstring_type m_tstrBlockItem;
	tstring_type m_tstrBlockCfg;
	tstring_type m_tstrBlockRes;
	tstring_type m_tstrBlockNote;
	//
	ybctrl::YBScrollWnd m_listbox4PCEDlg;
};

#endif // __YBDETECTPCENVIRONMENTDLG_H__