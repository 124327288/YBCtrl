#ifndef __YBCTRLMENUITEMS_H__
#define __YBCTRLMENUITEMS_H__
#include "menu/YBCtrlMenu.h"
#include "frame/YBCtrlImageStatic.h"

YBCTRL_NAMESPACE_BEGIN

class YBCTRL_API YBCtrlMIDefault : public YBCtrlMenuItemBase{
public:
	YBCtrlMIDefault();
	virtual ~YBCtrlMIDefault();

protected:
	virtual bool isDisable();
	virtual bool canClick();
	virtual void setMouseHoverFlag( bool bHover );
	virtual void drawStatus( HDC hDC, const RECT& rcItem ) const;
	virtual unsigned int getHeight() const;
	//
	virtual bool isDivide2Part();
	virtual bool isDivide3Part();
	virtual void setMouseHoverInPart1Flag( bool bMouseHoverInPart1 );
	virtual void setMouseHoverInPart2Flag( bool bMouseHoverInPart2 );
	virtual void setNormalInPart1Flag( bool bMouseNormalInPart1 );
	virtual void setNormalInPart2Flag( bool bMouseNormalInPart2 );
	virtual void setMouseHoverInPart3Flag( bool bMouseHoverInPart3 );
	virtual bool CanClickFlagInPart1();
	virtual bool CanClickFlagInPart2();
	virtual bool CanClickFlagInPart3();
	virtual bool isDividePara1Disable();
	virtual bool isDividePara2Disable();
	virtual bool isDividePara3Disable();
	virtual bool isDivide2PartCenter();

public:
	void setFontId( unsigned int uFontIdTxt );
	void setCanClickFlag( bool bCanClick );
	void setDisableFlag( bool bDisable );
	void setDisableInPart1Flag( bool bDisableInPart1 );
	void setDisableInPart2Flag( bool bDisableInPart2 );

protected:
	virtual void _drawBk( HDC hDC, const RECT& rcItem ) const;
	virtual void _drawIcon( HDC hDC, const RECT& rcItem ) const;
	virtual void _drawText( HDC hDC, const RECT& rcItem ) const;
	virtual void _drawDivideLine( HDC hDC, const RECT& rcItem ) const;
	virtual void _drawSelf( HDC hDC, const RECT& rcItem ) const {};
	bool _getIconRect( const RECT& rcItem, RECT& rcIcon ) const;
	bool _getTextRect( const RECT& rcItem, RECT& rcText ) const;
	YBCtrlImageStatic* _getImageStatic( unsigned int uImageId ) const;
	bool isHover( ) const;
public:
	unsigned int m_uFlag;
	unsigned int m_uImageIdBk;
	unsigned int m_uImageIdHover;
	unsigned int m_uImageIdIcon;
	unsigned int m_uImageIdDisable;
	unsigned int m_uFontIdTxt;
	unsigned int m_uItemHeight;
	bool m_bDrawDivideLine;

protected:
	unsigned int m_uIconLSpace;
	mutable unsigned int m_uTxtLSpace;
	unsigned int m_uTxtRSpace;
	unsigned int m_uTxtTSpace;
	unsigned int m_uTxtBSpace;
	HPEN m_hPenDivideLine;
	unsigned int m_uLineLSpace;
	unsigned int m_uLineRSpace;

public:
#pragma warning(push)
#pragma warning(disable:4251)
	tstring_type m_tstrTxt;
#pragma warning(pop)
}; 

YBCTRL_NAMESPACE_END
#endif //__YBCTRLMENUITEMS_H__