#ifndef __YBFILETPDLG_H__
#define __YBFILETPDLG_H__
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "button/YBButton.h"
#include "progressbar/YBProgressBar.h"

class YBFileTPDlg : public ybctrl::YBCtrlBase {
public:
	typedef enum _EFILETPTYPE{
		EFILETP_UPLOAD = 0,
		EFILETP_DOWNLOAD = 1
	}EFILETPTYPE;

public:
	typedef std::basic_string< TCHAR > tstring_type;

public:
	YBFileTPDlg( EFILETPTYPE eFileTPType );
	virtual ~YBFileTPDlg();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( int x, int y, int uWidth, int uHeight, HWND hWndParent, HINSTANCE hInstance );
	void setTitleText( const tstring_type& tstrTitleTxt );
	void setPos( __int64 i64PosCur, __int64 i64PosMax );
	void updateStatus();
	void onFinished( bool bSuccess );
	EFILETPTYPE getType() { return m_eFileTPType; };

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_SIZE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_TIMER( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageId );
	bool _getTitleTextRect( RECT& rcTitleText ) const;
	bool _getPBRect( RECT& rcPB ) const;
	void _drawStatus( HDC hDC );
	void _drawBk( HDC hDC );
	void _drawFrameLine( HDC hDC );
	void _drawIcon( HDC hDC );
	void _drawTitleText( HDC hDC );
	void _loadImage();
	void _loadFont();
	void _init();
	void _calcLocation();
	void _createAllControl();

private:
	EFILETPTYPE m_eFileTPType;
	ybctrl::YBProgressBar m_progressBar;
	tstring_type m_tstrTitleText;
	unsigned int m_uImageIdBk;
	unsigned int m_uFontIdTxt;
	unsigned int m_uFontIdPBTxt;
	// Icon
	unsigned int m_uImageIdPBBk;
	unsigned int m_uImageIdPBFT;
	unsigned int m_uImageIdIconUL;
	unsigned int m_uImageIdIconDL;
	unsigned int m_uIconLSpace;
	HPEN m_hPen;
};

#endif //__YBFILETPDLG_H__