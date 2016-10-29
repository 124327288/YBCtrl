#ifndef __YBNWSCROLLBAR_H__
#define __YBNWSCROLLBAR_H__
#include "frame/YBCtrlEnvCfg.h"
#include "frame/YBCtrlBase.h"

namespace Gdiplus{
	class Image;
}

YBCTRL_NAMESPACE_BEGIN
class YBCtrlImageBase;
class YBCtrlImageStatic;

class YBCTRL_API YBNWScrollBar{
public:
	typedef enum _ENUMDIRECTIONTYPE {
		EDIRECTION_HORIZONTAL = 0, // horizontal
		EDIRECTION_VERTICAL // vertical
	} ENUMDIRECTIONTYPE;
	typedef enum _ENUMTARROWSTATUS{     // Start Arrow of four status
		ESARROW_STATUS_PRENORMAL = 0,
		ESARROW_STATUS_NORMAL = 1,
		ESARROW_STATUS_HOVER = 2,
		ESARROW_STATUS_PRESS = 3
	}ENUMTARROWSTATUS;
	typedef enum _ENUMBARROWSTATUS{     // End Arrow of four status
		EEARROW_STATUS_PRENORMAL = 0,
		EEARROW_STATUS_NORMAL = 1,
		EEARROW_STATUS_HOVER = 2,
		EEARROW_STATUS_PRESS = 3
	}ENUMBARROWSTATUS;
	typedef enum _ENUMTHUMBSTATUS{
		EFT_STATUS_NORMAL = 0,
		EFT_STATUS_HOVER = 1,
		EFT_STATUS_PRESS = 2
	}ENUMTHUMBSTATUS;

public:
	YBNWScrollBar( ENUMDIRECTIONTYPE eDirection = EDIRECTION_HORIZONTAL );
	virtual ~YBNWScrollBar();

public:
	void drawBk( HDC hDC );
	void drawSArrow( HDC hDC );
	void drawEArrow( HDC hDC );
	void drawFT( HDC hDC );
	void drawStatus( HDC hDC );
	//
	void setRectBk( RECT& rcBk );
	void setHWND( HWND hWnd );
	void setCtrlID( WORD SArrowWord, WORD EArrowWord );
	void getCtrlId( WORD& SArrowWord, WORD& EArrowWord ) const ;
	void setImageIdBk( unsigned int uImageId );
	void setImageIdTArrow( ENUMTARROWSTATUS eArrowStatus, unsigned int uImageId );
	void setImageIdBArrow( ENUMBARROWSTATUS eArrowStatus, unsigned int uImageId );
	void setImageIdFT( ENUMTHUMBSTATUS eFtStatus, unsigned int uImageId );
	void getPos() const;
	void setRange( int nMinPos, int nMaxPos );
	void setStep( int uSteps );
	void setFTCurHeight( unsigned int nHeight );
	void setWidth( double dDisToTopPage, double dFullPage );
	void setLimitBarWidth( unsigned int uMaxBar, unsigned int uMinBar );
	//
	void onLBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void onLBUTTONUP( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void onMOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void onPAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void onMOUSEWHEEL( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );
	void onTIMER( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	YBCtrlImageStatic* _getImageStatic( unsigned int uImageId );
	bool _getImageIdSArrow( ENUMTARROWSTATUS eArrowStatus, unsigned int& uImageId );
	bool _getImageIdEArrow( ENUMBARROWSTATUS eArrowStatus, unsigned int& uImageId );
	bool _getImageIdFT( ENUMTHUMBSTATUS eFtStatus, unsigned int& uImageId );
	HWND _getHWND(){ return m_hWnd; }
	double _calcDisToSArrow( double dFullScrollBar );
	double _calcDisToSPage( double dDisToSArrow );

private:
	HWND m_hWnd;
	unsigned int m_uFlag;
	ENUMTARROWSTATUS m_eSArrowStatus;
	ENUMBARROWSTATUS m_eEArrowStatus;
	ENUMTHUMBSTATUS m_eFtStatus;
	unsigned int m_uImageIdBk;
	unsigned int m_uSArrowTSpaced;
	unsigned int m_uEArrowTSpaced;
	unsigned int m_uArrowWidth;
	unsigned int m_uArrowHeight;
	//ArrowImageId
	unsigned int m_uImageIdSArrPreNor;
	unsigned int m_uImageIdSArrNor;
	unsigned int m_uImageIdSArrHov;
	unsigned int m_uImageIdSArrPre;
	unsigned int m_uImageIdEArrNor;
	unsigned int m_uImageIdEArrPreNor;
	unsigned int m_uImageIdEArrHov;
	unsigned int m_uImageIdEArrPre;
	// FT Imaged
	unsigned int m_uImageIdFtNor;
	unsigned int m_uImageIdFtHov;
	unsigned int m_uImageIdFtPre;
	WORD m_SArrowCtrlId;
	WORD m_EArrowCtrlId;
	RECT m_rcBk;
	RECT m_rcSArrow;
	RECT m_rcEArrow;
	RECT m_rcFT;
	unsigned int m_uMaxBar;
	unsigned int m_uMinBar;
	int m_uSteps;
	unsigned int m_uFTHeight;
	HWND m_hWndSetCaptureOld;
	double m_dDisToTopPage;
	double m_dFullPage;
	//
	ENUMDIRECTIONTYPE m_eDirection;
};

YBCTRL_NAMESPACE_END
#endif //__YBNWSCROLLBAR_H__