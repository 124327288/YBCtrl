#ifndef __YBSTATIC_H__
#define __YBSTATIC_H__
#include "frame/YBCtrlBase.h"
#include <string>
#include <deque>

namespace Gdiplus{
	class Image;
}

YBCTRL_NAMESPACE_BEGIN
class YBCtrlImageBase;
class YBCtrlImageStatic;

class YBCTRL_API YBStatic : public YBCtrlBase{
public:
	typedef std::basic_string< TCHAR > tstring_type;
	typedef std::deque< unsigned int > charwidth_container_type;
public:
	YBStatic();
	virtual ~YBStatic();

public:
	YBCTRL_WNDMSGMAP_DECLARE()

public:
	HWND Create( DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance );
	void setText( const tstring_type& tstrText );
	void setImage( unsigned int uImageId );
	void setFontId( unsigned int uFontId );
	void setLineSpace( unsigned int uLineSpace );

private:
	void _onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg );
	void _onWM_ERASEBKGND( YBCtrlWndMsg* pYBCtrlWndMsg );

private:
	void _drawBk( HDC hdc );
	void _drawText( HDC hDC );
	YBCtrlImageBase* _getImageBase( unsigned int uImageId );
	YBCtrlImageStatic* _getImageStatic( unsigned int uImageId );
	void _calcLineHeight();
	void _calcAllCharWidth();
	bool _calcCharCount( unsigned int uCharIndexBegin, unsigned int uWidth, unsigned int& uCharCountCalc );

private:
	unsigned int m_uImageId;
	unsigned int m_uFontId;
	unsigned int m_uTextLen;
	unsigned int m_uLineHeight;
	unsigned int m_uLineSpace;
#pragma warning( push )
#pragma warning (disable:4251 )
	tstring_type m_tstrText;
	charwidth_container_type m_containerCharWidth;
#pragma warning( pop )
};

YBCTRL_NAMESPACE_END
#endif //__YBSTATIC_H__