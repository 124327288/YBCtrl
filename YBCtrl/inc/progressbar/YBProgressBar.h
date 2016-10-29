
#ifndef __YBPROGRESSBAR_H__
#define __YBPROGRESSBAR_H__
#include "frame/YBCtrlBase.h"
#include <string.h>

namespace Gdiplus{
	class Image;
}

YBCTRL_NAMESPACE_BEGIN
class YBCtrlImageBase;
class YBCtrlImageStatic;

class YBCTRL_API YBProgressBar : public YBCtrlBase{
public:
	typedef std::basic_string< TCHAR > tstring_type;

public:
	YBProgressBar();
	virtual ~YBProgressBar();

public:
	YBCTRL_WNDMSGMAP_DECLARE()

public:
	HWND Create( DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance );
	void setImageID( unsigned int uImageIdBK, unsigned int uImageProBar );
	void setRange( __int64 nMinPos, __int64 nMaxPos );
	void getRange( __int64& nMinPos, __int64& nMaxPos ) const;
	void SetProgressBarSpaced( unsigned int uLSpaced, unsigned int uTSpaced, unsigned int uRSpaced, unsigned int uBSpaced );
	void setPos( __int64 nPos );
	void setTextAttri( unsigned int uFontId, unsigned int uTxtSpaceT, unsigned int uTxtSpaceB );
	__int64 getPos() const;
	void getCurText( tstring_type& tstrCurText );
	void setBitBltFTMode( bool bRoll );

protected:
	void _onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_TIMER( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _drawStatus( HDC hDC );
	void _drawFT( HDC hDC );
	void _drawStaticFT( HDC hDC );
	void _drawRollFT( HDC hDC );
	void _drawBk( HDC hDC );
	void _drawText( HDC hDC );
	bool _getDrawText( RECT& rcDrawText );
	void _getCurText( tstring_type& tstrCurText );
	YBCtrlImageStatic* _getImageStatic( unsigned int uImageId );
	__int64 _calcCurFTWidth( __int64 uFTImageWidth );
	
protected:
	unsigned int m_uFlag;
	unsigned int m_uImageIdBK;
	unsigned int m_uImageIdFT;
	unsigned int m_uFontIdTxt;
private:
	unsigned int m_uLFixed;
	unsigned int m_uTFixed;
	unsigned int m_uRFixed;
	unsigned int m_uBFixed;
	__int64 m_nMinPos;
	__int64 m_nMaxPos;
	__int64 m_nCurPos;
	//
	unsigned int m_uLSpaced;
	unsigned int m_uTSpaced;
	unsigned int m_uRSpaced;
	unsigned int m_uBSpaced;
	//
	unsigned int m_uTxtSpaceB;
	unsigned int m_uTxtSpaceT;
#pragma warning(push)
#pragma warning(disable:4251)
	tstring_type m_tstrText;
#pragma warning(pop)
};

YBCTRL_NAMESPACE_END


#endif //__YBPROGRESSBAR_H__