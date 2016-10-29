#ifndef __YBSCROLLBAR_H__
#define __YBSCROLLBAR_H__
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlImageStatic.h"
YBCTRL_NAMESPACE_BEGIN

class YBCTRL_API YBScrollBar : public YBCtrlBase {
public:
	typedef enum _ENUMDIRECTIONTYPE {
		EDIRECTION_HORIZONTAL = 0, // horizontal
		EDIRECTION_VERTICAL // vertical
	} ENUMDIRECTIONTYPE;
	typedef enum _ENUMARROWSTATUS {
		EARROW_STATUS_NORMAL = 0,
		EARROW_STATUS_OUTHOVER = 1,
		EARROW_STATUS_INHOVER = 2,
		EARROW_STATUS_PRESS = 3,
	}EARROWSTATUS;
	typedef enum _ENUMTHUMBSTATUS{
		ETHUMB_STATUS_NORMAL = 0,
		ETHUMB_STATUS_HOVER = 1,
		ETHUMB_STATUS_PRESS = 2
	}ENUMTHUMBSTATUS;

public:
	YBScrollBar( ENUMDIRECTIONTYPE eDirectType, unsigned int uLenStartBtn = 16, unsigned int uLenEndBtn = 16 );
	virtual ~YBScrollBar();

public:
	YBCTRL_WNDMSGMAP_DECLARE()
	
public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent, WORD wCtrlID, HINSTANCE hInstance );
	void setImageIdBk( unsigned int uImageId );
	void setImageIdStartArrow( EARROWSTATUS eArrowStatus, unsigned int uImageId );
	void setImageIdEndArrow( EARROWSTATUS eArrowStatus, unsigned int uImageId );
	void setImageIdThumb( ENUMTHUMBSTATUS eThumbStatus, unsigned int uImageId );
	void setLenThumbBtn( unsigned int uLenThumbBtn ) { m_uLenThumbBtn = uLenThumbBtn; };
	void setValRange( int nValMin, int nValMax );
	void getValRange( int& nValMin, int& nValMax );
	int getCurVal() const { return m_nValCur; }
	void setCurVal( int nValCur ) { m_nValCur = nValCur; }
	int getPosRange() const;
	int getTotalLen() const;
	bool getThick( unsigned int& uScrollBarThick ) const;
	
private:
	void _drawStatus( HDC hDC );
	void _drawStartArrow( HDC hDC );
	void _drawEndArrow( HDC hDC );
	void _drawBk( HDC hDC );
	void _drawThumb( HDC hDC );
	//
	bool _getImageIdStartArrow( EARROWSTATUS eArrowStatus, unsigned int& uImageId );
	bool _getImageIdEndArrow( EARROWSTATUS eArrowStatus, unsigned int& uImageId );
	bool _getImageIdThumb( ENUMTHUMBSTATUS eThumbStatus, unsigned int& uImageId );
	bool _getStartBtnRect( RECT& rcStartBtn );
	bool _getEndBtnRect( RECT& rcEndBtn );
	bool _getStartSpaceRect( RECT& rcStartSpace );
	bool _getEndSpaceRect( RECT& rcEndSpace );
	bool _getThumbBtnRect( RECT& rcThumbBtn );
	YBCtrlImageStatic* _getImageStatic( unsigned int uImageId );
	void _moveThumbBtn( POINT ptMouseNow );

private:
	void _onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEMOVE( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSELEAVE( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_LBUTTONUP( YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	ENUMDIRECTIONTYPE m_eDirectType;
	EARROWSTATUS m_eStartArrowStatus;
	EARROWSTATUS m_eEndArrowStatus;
	ENUMTHUMBSTATUS m_eThumbStatus;
	unsigned int m_uFlag;
	WORD m_wordCtrlId;
	int m_nValMin;
	int m_nValMax;
	int m_nValCur;
	POINT m_ptLBtnDownOnThumb;
	int m_nValLBtnDownOnThumb;
	//
	unsigned int m_uLenThumbBtn;
	unsigned int m_uLenStartBtn;
	unsigned int m_uLenEndBtn;
	// BK 
	unsigned int m_uImageIdBk;
	// FT 
	unsigned int m_uImageIdThumbNor;
	unsigned int m_uImageIdThumbHov;
	unsigned int m_uImageIdThumbPre;
	// Start Arrow
	unsigned int m_uImageIdStartArrNor;
	unsigned int m_uImageIdStartArrOutHov;
	unsigned int m_uImageIdStartArrInHov;
	unsigned int m_uImageIdStartArrPre;
	// End Arrow
	unsigned int m_uImageIdEndArrNor;
	unsigned int m_uImageIdEndArrOutHov;
	unsigned int m_uImageIdEndArrInHov;
	unsigned int m_uImageIdEndArrPre;
};

YBCTRL_NAMESPACE_END
#endif //__YBSCROLLBAR_H__