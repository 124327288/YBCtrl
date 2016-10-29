#ifndef __YBPopupMessageBox_H__
#define __YBPopupMessageBox_H__
#include "YBTransparentWnd.h"
#include "frame/YBCtrlImageStatic.h"
#include "button/YBButton.h"
#include "frame/YBCtrlMemDC.h"
#include <deque>
/*
 *YBMessageBox() Flags
 */
#define YBMB_FLAG_NONE			      0x00000000L
#define YBMB_OK                       0x00000001L
#define YBMB_OKCANCEL                 0x00000002L
#define YBMB_ABORTRETRYIGNORE         0x00000004L
#define YBMB_YESNOCANCEL              0x00000008L
#define YBMB_YESNO                    0x00000010L
#define YBMB_RETRYCANCEL              0x00000020L
#define YBMB_CANCELTRYCONTINUE        0x00000040L
//Icon
#define YBMB_ICONHAND				  0x00000080L
#define YBMB_ICONQUESTION             0x00000100L
#define YBMB_ICONEXCLAMATION          0x00000200L
#define YBMB_ICONASTERISK             0x00000400L
#define YBMB_ICONWARNING              YBMB_ICONEXCLAMATION
#define YBMB_ICONERROR                YBMB_ICONHAND
#define YBMB_ICONINFORMATION          YBMB_ICONASTERISK
#define YBMB_ICONSTOP                 YBMB_ICONHAND
// return value
#define YBIDOK						  1
#define YBIDCANCEL					  2
#define YBIDABORT					  3
#define YBIDRETRY					  4
#define YBIDIGNORE					  5
#define YBIDYES						  6
#define YBIDNO						  7
#define YBIDTRYAGAIN				  8
#define YBIDCONTINUE				  9
//
class YBPopupMessageBox : public ybctrl::YBCtrlBase {
public:
	typedef enum _ENUMMBSIZETYPE {
		EMBSIZE_SMALL = 0,
		EMBSIZE_BIG = 1
	} ENUMMBSIZETYPE;
public:
	typedef std::basic_string< TCHAR > tstring_type;
	typedef std::deque< unsigned int > charwidth_container_type;
	typedef std::deque< unsigned int > brchar_container_type;

private:
	YBPopupMessageBox();

public:
	static YBPopupMessageBox* getInstance();
	virtual ~YBPopupMessageBox();

public:
	int YBMessageBox( const tstring_type& tstrText, const tstring_type& tstrCaption, UINT uType );

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_KEYDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_DESTROY( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	HWND _create( HWND hWndParent );
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageId );
	void _loadImage();
	void _loadFont();
	void _init();
	void _drawStatus( HDC hDC );
	void _drawBk( HDC hDC );
	void _drawIcon( HDC hDC );
	void _drawText( HDC hDC );
	void _drawCaption( HDC hDC );
	void _clipWnd();
	void _getWndSize( ENUMMBSIZETYPE eSizeType, int& nWidth, int& nHeight );
	void _createUI();
	void _endMB( int nBtnIndex );
	void _runMessageLoop();
	void _setType( UINT uType );
	void _setBtnInfo();
	void _setBtnInfo( unsigned int uIndexBtn, bool bBtnEnable, tstring_type tstrTextBtnDef1, tstring_type tstrTextBtnDef2, tstring_type tstrTextBtnDef3 );
	bool _getIconRect( RECT& rcIcon ) const;
	bool _getTextRect( RECT& rcText ) const;
	bool _getCaptionRect( RECT& rcCaption ) const;
	unsigned int _getIconId() const;
	void _calcAllCharWidth();
	unsigned int _calcEveryPartWidth( unsigned int uIndexPart );
	unsigned int _calcEveryPartCharCount( unsigned int uIndexPart );
	bool _calcWidth();
	bool _calcHeight();
	bool _calcCharCount( unsigned int uCharIndexBegin, unsigned int uWidth, unsigned int& uCharCountCalc );

private:
	ENUMMBSIZETYPE m_eSizeType;
	tstring_type m_tstrText;
	charwidth_container_type m_containerCharWidth;
	brchar_container_type m_containerBrCharIndex;
	YBTransparentWnd m_transDlg4MB;
	ybctrl::YBButton m_btnClose;
	ybctrl::YBButton m_btnDefault1;
	ybctrl::YBButton m_btnDefault2;
	ybctrl::YBButton m_btnDefault3;
	unsigned int m_uFontIdBtnDefault;
	unsigned int m_uImageIdBk;
	unsigned int m_uImageIdBkBigSize;
	unsigned int m_uImageIdBtnCloDisable;
	unsigned int m_uImageIdBtnCloNor;
	unsigned int m_uImageIdBtnCloHov;
	unsigned int m_uImageIdBtnCloPre;
	unsigned int m_uImageIdBtnDefNor;
	unsigned int m_uImageIdBtnDefHov;
	unsigned int m_uImageIdBtnDefPre;
	unsigned int m_uImageIdIconHand;			// " x "
	unsigned int m_uImageIdIconQuestion;		// " ? "
	unsigned int m_uImageIdIconExclamation;		// " ! "
	unsigned int m_uImageIdIconAsterisk;		// " i "
	int m_nRetCode;
	unsigned int m_uFlag;
	tstring_type m_tstrCaption;
	unsigned int m_uFontIdText;
	ybctrl::YBCtrlMemDC* m_pMemDC;
	unsigned int m_uHeight;
	unsigned int m_uLineHeight;
	unsigned int m_uWidth;
	unsigned int m_uCharWidth;
};

#endif // __YBPopupMessageBox_H__