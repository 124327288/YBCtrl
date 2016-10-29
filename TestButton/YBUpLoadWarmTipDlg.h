#ifndef __YBUPLOADWARMTIP_H__
#define __YBUPLOADWARMTIP_H__
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "progressbar/YBProgressBar.h"
#include "button/YBButton.h"
#include <deque>

class YBUpLoadWarmTipDlg : public ybctrl::YBCtrlBase {
public:
	typedef std::basic_string < TCHAR > tstring_type;
	typedef std::deque < unsigned int > charwidth_container_type;
public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	YBUpLoadWarmTipDlg();
	~YBUpLoadWarmTipDlg();

public:
	HWND trackPopupWTDlg( HWND hWndParent );
	void setPos( __int64 i64PosCur, __int64 i64PosMax );
	void updateStatus();
	void setULFileInfo( const tstring_type& tstrULFileNumber, const tstring_type& tstrULFileSize, const tstring_type& tstrULFileTime );
	void setULFileTime( const tstring_type& tstrHour, const tstring_type& tstrMinute, const tstring_type& tstrSecond );
	void setULFileSFNumber( const tstring_type& tstrSuccessNumber, const tstring_type& tstrFailNumber );
	void setULFileResult( bool bSuccess );

private:
	void _onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_NCHITTEST( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_COMMAND( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageId );
	void _drawStatus( HDC hDC );
	void _drawBk( HDC hDC );
	void _drawFFITxt1( HDC hDC );
	void _drawDFITxt1( HDC hDC );
	void _drawFFITxt2( HDC hDC );
	void _drawDFITxt2( HDC hDC );
	void _drawFFITxt3( HDC hDC );
	void _drawDFITxt3( HDC hDC );
	void _drawFFITxt4( HDC hDC );
	void _drawDlgTitle( HDC hDC );
	void _drawTimeTxt( HDC hDC );
	void _drawTimeBk( HDC hDC );
	void _drawTime( HDC hDC );
	void _drawUints4Time( HDC hDC );
	void _drawResultListBk( HDC hDC );
	void _drawIcon4SuccessAndFailed( HDC hDC );
	void _drawResultListTxt( HDC hDC );
	void _drawBubble( HDC hDC );
	void _drawBubbleTxt( HDC hDC );
	void _drawULProgress( HDC hDC );
	void _drawULComplete( HDC hDC );
	void _loadImage();
	void _loadFont();
	void _init();
	void _clipWnd();
	void _clearAll();
	void _movewindow();
	//
	bool _calcCharCount( charwidth_container_type containerCharWidth, unsigned int uCharIndexBegin, unsigned int uWidth, unsigned int& uCharCountCalc );
	bool _getFADFileInfoTxtRect( RECT& rcFITxt ) const;
	unsigned int _calcAllCharWidth( const tstring_type& tstrTxt, charwidth_container_type& containerCharWidth );
	//
	bool _getTitleRect( RECT& rcTitle ) const;
	bool _getTimeTxtRect( RECT& rcTimeTxt ) const;
	bool _getHourTxtRect( RECT& rcHourTxt ) const;
	bool _getMinuteTxtRect( RECT& rcMinuteTxt ) const;
	bool _getSecondTxtRect( RECT& rcSecondTxt ) const;
	bool _getUints4Hour( RECT& rcUints4Hour ) const;
	bool _getUints4Minute( RECT& rcUints4Minute ) const;
	bool _getUints4Second( RECT& rcUints4Second ) const;
	bool _getResultListRect( RECT& rcResultList ) const;
	bool _getIcon4Success( RECT& rcIconSuccess );
	bool _getIcon4Failed( RECT& rcIconFailed );
	bool _getLeftBracket( RECT& rcLeftBracket ) const;
	bool _getSuccessRect( RECT& rcSuccess ) const;
	bool _getSuccessNumberRect( RECT& rcNumber4Success );
	bool _getFailedRect( RECT& rcFailed );
	bool _getFailedNumberRect( RECT& rcNumber4Failed );
	bool _getRightBracket( RECT& rcRightBracket );
	bool _getBubbleRect( RECT& rcBubble ) const;
	bool _getBubbleTextRect( RECT& rcBubbleText ) const;
	bool _getULCompleteTxt( RECT& rcCompleteTxt ) const;

private:
	unsigned int m_uFlag;
	charwidth_container_type m_containerFixedCharWidth1;
	charwidth_container_type m_containerDynamicCharWidth1;
	charwidth_container_type m_containerFixedCharWidth2;
	charwidth_container_type m_containerDynamicCharWidth2;
	charwidth_container_type m_containerFixedCharWidth3;
	charwidth_container_type m_containerDynamicCharWidth3;
	charwidth_container_type m_containerFixedCharWidth4;
	charwidth_container_type m_containerSingleRowChar;
	charwidth_container_type m_containerULSuccessNumber;
	charwidth_container_type m_containerULFailedNumber;
	unsigned int m_uRowIndex;
	tstring_type m_tstrFFITxt1;
	tstring_type m_tstrDFITxt1;
	tstring_type m_tstrFFITxt2;
	tstring_type m_tstrDFITxt2;
	tstring_type m_tstrFFITxt3;
	tstring_type m_tstrDFITxt3;
	tstring_type m_tstrFFITxt4;
	tstring_type m_tstrTitle;
	tstring_type m_tstrTimeTxt;
	tstring_type m_tstrTimeHourTxt;
	tstring_type m_tstrTimeMinuteTxt;
	tstring_type m_tstrTimeSecondTxt;
	tstring_type m_tstrUints4Hour;
	tstring_type m_tstrUints4Minute;
	tstring_type m_tstrUints4Second;
	tstring_type m_tstrLeftBracket;
	tstring_type m_tstrRightBracket;
	tstring_type m_tstrSuccessTxt;
	tstring_type m_tstrFailedTxt;
	tstring_type m_tstrSuccessNumber;
	tstring_type m_tstrFailedNumber;
	tstring_type m_tstrPBProgressTxt;
	tstring_type m_tstrULCompleteTxt;
	unsigned int m_uFontIdULCompleteTxt;
	unsigned int m_uImageIdULWTDlgBk;
	unsigned int m_uFontIdFixedFileInfo;
	unsigned int m_uFontIdDynamicFileInfo;
	unsigned int m_uLineHeight;
	unsigned int m_uSingleRowCharWidth;
	//
	unsigned int m_uFontIdTitle;
	unsigned int m_uFontIdTimeTxt;
	unsigned int m_uFontIdTime;
	unsigned int m_uImageIdULWTDlgTimeBk;
	HBRUSH m_hBrushULWTDlgRLBk;
	unsigned int m_uIconULSuccess;
	unsigned int m_uIconULFailed;
	unsigned int m_uFontIdULWTDlgRLTxt;
	//
	ybctrl::YBProgressBar m_progressBar;
	unsigned int m_uImageIdULWTDPBBk;
	unsigned int m_uImageIdULWTDPBFT;
	unsigned int m_uImageIdULWTDBubble;
	unsigned int m_uFontIdULWTDBubbleTxt;
	//
	ybctrl::YBButton m_btnCancelUL;
	ybctrl::YBButton m_btnContinueUL;
	ybctrl::YBButton m_btnReturnCD;
	unsigned int m_uFontIdULWTDBtn;
	unsigned int m_uImageIdULWTDBtnNor;
	unsigned int m_uImageIdULWTDBtnHov;
	unsigned int m_uImageIdULWTDBtnPre;
};

#endif //__YBUPLOADWARMTIP_H__