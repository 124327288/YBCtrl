#ifndef __YBTOOLTIP_H__
#define __YBTOOLTIP_H__
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlMemDC.h"
#include <deque>

namespace Gdiplus{
	class Image;
}

YBCTRL_NAMESPACE_BEGIN
class YBCtrlImageBase;
class YBCtrlImageStatic;

//
class YBCTRL_API YBToolTip : public YBCtrlBase {
public:
	typedef std::basic_string< TCHAR > tstring_type;
	typedef std::deque< unsigned int > charwidth_container_type;
	typedef std::deque< unsigned int > brchar_container_type;

public:
	~YBToolTip();
	static YBToolTip* getInstance();

public:
	YBCTRL_WNDMSGMAP_DECLARE()

public:
	void updateTipText( const tstring_type& tstrText, HWND hWndDest );
	void hide();

private:
	YBToolTip();

private:
	void _onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_TIMER( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_MOUSEMOVE( YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	HWND _createWnd();
	void _drawStatus( HDC hDC );
	void _drawText( HDC hDC );
	void _drawBk( HDC hDC );
	void _drawDefaultBk( HDC hDC );
	void _calcAllCharWidth();
	void _calcWndSpace( int pointx, int pointy, int& newpointx, int& newpointy ) const;
	void _movewnd( int pointx, int pointy );
	bool _calcWidth();
	bool _calcHeight();
	bool _calcCharCount( unsigned int uCharIndexBegin, unsigned int uWidth, unsigned int& uCharCountCalc );
	YBCtrlImageStatic* _getImageStatic( unsigned int uImageId );
	void _loadFont();
	unsigned int _calcEveryPartWidth( unsigned int uIndexPart );
	unsigned int _calcEveryPartCharCount( unsigned int uIndexPart );

private:
	unsigned int m_uWidth;
	unsigned int m_uCharWidth;
	unsigned int m_uHeight;
	unsigned int m_uLineHeight;
	unsigned int m_uFontId;
	unsigned int m_uImageBk;
	unsigned int m_uExitTimeMS;
	YBCtrlMemDC* m_pMemDC;
	HPEN m_hPen;
	HBRUSH m_hBrushBk;

#pragma warning( push )
#pragma warning (disable:4251 )
	tstring_type m_tstrText;
	charwidth_container_type m_containerCharWidth;
	brchar_container_type m_containerBrCharIndex;
#pragma warning( pop )

};

YBCTRL_NAMESPACE_END
#endif //__YBTOOLTIP_H__