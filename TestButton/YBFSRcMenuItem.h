#ifndef __YBFSRCMENUITEM_H_
#define __YBFSRCMENUITEM_H_
#include "menu/YBCtrlMenu.h"
#include "frame/YBCtrlImageStatic.h"

class YBFSRcMenuItem : public ybctrl::YBCtrlMenuItemBase {
public:
	typedef std::basic_string< TCHAR > tstring_type;

public:
	YBFSRcMenuItem();
	~YBFSRcMenuItem();

protected:
	virtual bool isDisable();
	virtual bool canClick();
	virtual void setMouseHoverFlag( bool bHover );
	virtual void drawStatus( HDC hDC, const RECT& rcItem ) const;
	virtual unsigned int getHeight() const;
	virtual bool isDivide2Part();
	virtual void setMouseHoverInPart1Flag( bool bMouseHoverInPart1 );
	virtual void setMouseHoverInPart2Flag( bool bMouseHoverInPart2 );
	virtual bool CanClickFlagInPart1();
	virtual bool CanClickFlagInPart2();
	virtual bool CanClickFlagInPart3();
	virtual bool isDividePara1Disable();
	virtual bool isDividePara2Disable();
	virtual bool isDivide3Part();
	virtual void setMouseHoverInPart3Flag( bool bMouseHoverInPart3 );
	virtual bool isDividePara3Disable();
	virtual bool isDivide2PartCenter();

public:
	void setFontId( unsigned int uFontIdTxt );
	void setCanClickFlag( bool bCanClick );
	void setDisableFlag( bool bDisable );
	void setDividePart2Flag( bool bDivide );
	void setDividePart3Flag( bool bDivide );
	void setDisableInPart1Flag( bool bDisableInPart1 );
	void setDisableInPart2Flag( bool bDisableInPart2 );
	void setDisableInPart3Flag( bool bDisableInPart3 );
	void setCanClickInPart1Flag( bool bCanClickInPart1 );
	void setCanClickInPart2Flag( bool bCanClickInPart2 );
	void setCanClickInPart3Flag( bool bCanClickInPart3 );

private:
	void _drawBk( HDC hDC, const RECT& rcItem ) const;
	void _drawIcon( HDC hDC, const RECT& rcItem ) const;
	//void _drawIconDividePara1Text( HDC hDC, const RECT& rcDividePara1 ) const;
	//void _drawIconDividePara2Text( HDC hDC, const RECT& rcDividePara2 ) const;
	//void _drawIconDividePara3Text( HDC hDC, const RECT& rcDividePara3 ) const;
	void _drawText( HDC hDC, const RECT& rcItem ) const;
	void _drawDividePara1Text( HDC hDC, const RECT& rcItem ) const;
	void _drawDividePara2Text( HDC hDC, const RECT& rcItem ) const;
	void _drawDividePara3Text( HDC hDC, const RECT& rcItem ) const;
	void _drawDivideLine( HDC hDC, const RECT& rcItem ) const;
	void _drawNumber( HDC hDC, const RECT& rcItem ) const;
	void _drawNumberText( HDC hDC, const RECT& rcItem ) const;
	void _drawIconText( HDC hDC, const RECT& rcItem ) const;
	bool _getIconRect( const RECT& rcItem, RECT& rcIcon ) const;
	bool _getDividePara1IconRect( const RECT& rcItem, RECT& rcIcon ) const;
	bool _getDividePara2IconRect( const RECT& rcItem, RECT& rcIcon ) const;
	bool _getDividePara3IconRect( const RECT& rcItem, RECT& rcIcon ) const;
	bool _getTextRect( const RECT& rcItem, RECT& rcText ) const;
	bool _getNumberRect( const RECT& rcItem, RECT& rcNumber ) const;
	bool _getDividePara1NumberRect( const RECT& rcDividePara1, RECT& rcNumber ) const;
	bool _getDividePara2NumberRect( const RECT& rcDividePara2, RECT& rcNumber ) const;
	bool _getDividePara1Rect( const RECT& rcItem, RECT& rcDividePara1 ) const;
	bool _getDividePara2Rect( const RECT& rcItem, RECT& rcDividePara2 ) const;
	bool _getDividePara3Rect( const RECT& rcItem, RECT& rcDividePara3 ) const;
	ybctrl::YBCtrlImageStatic* _getImageStatic( unsigned int uImageId ) const;

public:
	unsigned int m_uImageIdBk;
	unsigned int m_uImageIdHover;
	unsigned int m_uImageIdDisable;
	//
	unsigned int m_uImageIdBk_Hov;
	unsigned int m_uImageIdHov_Bk;
	unsigned int m_uImageIdDis_Bk;
	unsigned int m_uImageIdBk_Dis;
	unsigned int m_uImageIdDis_Dis;
	unsigned int m_uImageIdHov_Dis;
	unsigned int m_uImageIdDis_Hov;
	//
	unsigned int m_uImageIdHov_Bk_Bk;
	unsigned int m_uImageIdBk_Bk_Hov;
	unsigned int m_uImageIdBk_Hov_Bk;
	unsigned int m_uImageIdBk_Bk_Bk;
	unsigned int m_uImageIdDis_Bk_Hov;
	unsigned int m_uImageIdDis_Bk_Bk;
	unsigned int m_uImageIdDis_Hov_Bk;
	unsigned int m_uImageIdBk_Dis_Bk;
	unsigned int m_uImageIdBk_Dis_Hov;
	unsigned int m_uImageIdHov_Dis_Bk;
	unsigned int m_uImageIdBk_Bk_Dis;
	unsigned int m_uImageIdBk_Hov_Dis;
	unsigned int m_uImageIdHov_Bk_Dis;
	unsigned int m_uImageIdDis_Dis_Hov;
	unsigned int m_uImageIdDis_Dis_Bk;
	unsigned int m_uImageIdDis_Hov_Dis;
	unsigned int m_uImageIdDis_Bk_Dis;
	unsigned int m_uImageIdBk_Dis_Dis;
	unsigned int m_uImageIdHov_Dis_Dis;
	unsigned int m_uImageIdDis_Dis_Dis;
	//
	unsigned int m_uImageIdIconNor;
	unsigned int m_uImageIdIconHor;
	unsigned int m_uImageIdIconDisable;
	unsigned int m_uFontIdNorTxt;
	unsigned int m_uFontIdDisTxt;
	unsigned int m_uFontIdHovTxt;
	unsigned int m_uFontIdIconTxt;
	unsigned int m_uFontIdNumTxt;
	tstring_type m_tstrTxt;
	tstring_type m_tstrDividePart1Txt;
	tstring_type m_tstrDividePart2Txt;
	tstring_type m_tstrDividePart3Txt;
	tstring_type m_tstrIcon;
	tstring_type m_tstrNumber;
	tstring_type m_tstrDividePart1Icon;
	tstring_type m_tstrDividePart2Icon;
	tstring_type m_tstrDividePart3Icon;
	unsigned int m_uItemHeight;
	bool m_bDrawDivideLine;

private:
	unsigned int m_uFlag;
	mutable unsigned int m_uTxtLSpace;
	unsigned int m_uTxtRSpace;
	unsigned int m_uTxtTSpace;
	unsigned int m_uTxtBSpace;
	unsigned int m_uIconRSpace;
	unsigned int m_uNumberWidth;
	unsigned int m_uNumberLSpace;
	HPEN m_hPenDivideLine;
	HPEN m_hPenNumberDivLine;
	unsigned int m_uLineRSpace;
};

#endif //__YBFSRCMENUITEM_H_