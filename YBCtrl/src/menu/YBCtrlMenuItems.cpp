#include "stdafx.h"
#include "menu/YBCtrlMenuItems.h"
#include "frame/YBCtrlImageBase.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlMisc.h"
#include "frame/YBCtrlFontDepository.h"
#include "gdiplus.h"
#include <assert.h>

#define YBCMID_FLAG_NONE (0x00000000)
#define YBCMID_FLAG_DISABLE (0x00000001)
#define YBCMID_FLAG_HOVER (0x00000002)
#define YBCMID_FLAG_CANCLICK (0x00000004)
//
#define COLOR_YBCMID_DIVIDELINE (RGB(128, 128, 128))
//
USING_NAMESPACE_YBCTRL


YBCtrlMIDefault::YBCtrlMIDefault()
: m_uFlag( YBCMID_FLAG_NONE )
, m_uImageIdBk( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdHover( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdIcon( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdDisable( YBCTRL_IMAGE_ID_INVALID )
, m_uFontIdTxt( YBCTRL_FONT_ID_INVALID )
, m_uIconLSpace ( 4 )
, m_uTxtLSpace ( 30 )
, m_uTxtRSpace ( 10 )
, m_uTxtTSpace ( 2 )
, m_uTxtBSpace ( 2 )
, m_uItemHeight( 0 )
, m_bDrawDivideLine( false )
, m_hPenDivideLine( NULL )
, m_uLineLSpace( 20 )
, m_uLineRSpace( 10 )
{
	m_hPenDivideLine = ::CreatePen( PS_SOLID, 2, COLOR_YBCMID_DIVIDELINE );
}

YBCtrlMIDefault::~YBCtrlMIDefault() {
	if ( m_hPenDivideLine ) {
		::DeleteObject( m_hPenDivideLine );
		m_hPenDivideLine = NULL;
	}
}

void YBCtrlMIDefault::drawStatus( HDC hDC, const RECT& rcItem ) const {
	_drawBk( hDC, rcItem );
	_drawIcon( hDC, rcItem );
	_drawText( hDC, rcItem );
	_drawDivideLine( hDC, rcItem );
	_drawSelf( hDC, rcItem );
}

unsigned int YBCtrlMIDefault::getHeight() const {
	 return m_uItemHeight;
}

void YBCtrlMIDefault::_drawBk( HDC hDC, const RECT& rcItem ) const {
	YBCtrlImageStatic* pYBImage = NULL;

	if ( m_uFlag & YBCMID_FLAG_DISABLE ) {
		pYBImage = _getImageStatic( m_uImageIdDisable );
	} else {
		if( m_uFlag & YBCMID_FLAG_HOVER ){
			pYBImage = _getImageStatic( m_uImageIdHover );
		}else {
			pYBImage = _getImageStatic( m_uImageIdBk );
		}
	}
	if( !pYBImage ){
		return;
	}
	BackGroundBitBlt( hDC, rcItem.left, rcItem.top, rcItem.right - rcItem.left, rcItem.bottom - rcItem.top, pYBImage->getMemDC(),
		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0 );

}

void YBCtrlMIDefault::_drawIcon( HDC hDC, const RECT& rcItem ) const {
	RECT rcIcon;
	YBCtrlImageStatic* pYBImage = NULL;
	
	pYBImage = _getImageStatic( m_uImageIdIcon );
	if ( !pYBImage ) {
		return;
	}

	Gdiplus::Graphics graphics( hDC );
	if ( _getIconRect( rcItem, rcIcon ) ) {
		GdipBitBlt( &graphics, pYBImage->getImage(), rcIcon.left, rcIcon.top, ( rcIcon.right - rcIcon.left ), ( rcIcon.bottom - rcIcon.top )
			, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
	}
}

void YBCtrlMIDefault::_drawDivideLine( HDC hDC, const RECT& rcItem ) const {
	if ( m_bDrawDivideLine ) {
		HPEN hPenOld = NULL;	
		hPenOld = (HPEN)::SelectObject( hDC, m_hPenDivideLine );
		//
		if ( ( m_uLineLSpace > unsigned int( rcItem.right - rcItem.left ))
			|| ( m_uLineRSpace ) > unsigned int( rcItem.right - rcItem.left )
			|| ( ( m_uLineRSpace + m_uLineLSpace ) > unsigned int( rcItem.right - rcItem.left ) ) ) {
				assert( false );
				return;
		}
		::MoveToEx( hDC, m_uLineLSpace, rcItem.bottom, NULL );
		::LineTo( hDC, rcItem.right - m_uLineRSpace, rcItem.bottom );
		::SelectObject( hDC, hPenOld );
	}
	return;
}

bool YBCtrlMIDefault::_getIconRect( const RECT& rcItem, RECT& rcIcon ) const {
	YBCtrlImageStatic* pYBImage = NULL;

	pYBImage = _getImageStatic( m_uImageIdIcon );
	if ( !pYBImage ) {
		return false;
	}
	if( ( pYBImage->getHeight() > unsigned int( rcItem.bottom - rcItem.top ) ) || ( pYBImage->getWidth() > unsigned int( rcItem.right - rcItem.left ))){
		assert(false);
		return false;
	}
	rcIcon.left = rcItem.left + m_uIconLSpace;
	rcIcon.right = rcIcon.left + pYBImage->getWidth();
	rcIcon.top = rcItem.top + ( rcItem.bottom - rcItem.top - pYBImage->getHeight() )/2;
	rcIcon.bottom = rcIcon.top + pYBImage->getHeight();

	return ( ( rcIcon.right > rcIcon.left ) && (rcIcon.bottom > rcIcon.top ) );
}

void YBCtrlMIDefault::_drawText( HDC hDC, const RECT& rcItem ) const {
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcText;

	if( !_getTextRect( rcItem, rcText ) )
		return;

	/*if ( m_uFlag & YBCMID_FLAG_DISABLE ) {
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdDisableTxt );
	} else {
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdTxt );
	}*/
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdTxt );
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		if ( m_uFlag & YBCMID_FLAG_DISABLE ) {
			clrTxtOld = ::SetTextColor( hDC, ::GetSysColor(COLOR_GRAYTEXT));
		} else {
			clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
		}
		::DrawText( hDC, m_tstrTxt.c_str(), m_tstrTxt.length(), &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

bool YBCtrlMIDefault::_getTextRect( const RECT& rcItem, RECT& rcText ) const {
// 	YBCtrlImageStatic* pYBImage = NULL;
// 
// 	pYBImage = _getImageStatic( m_uImageIdIcon );
// 	if( !pYBImage )
// 		return false;
// 	if( m_uTxtLSpace < ( m_uIconLSpace + pYBImage->getWidth() ) ){
// 		m_uTxtLSpace = m_uIconLSpace + pYBImage->getWidth();
// 	}

	rcText.left = rcItem.left + m_uTxtLSpace;
	rcText.right = rcItem.right - m_uTxtRSpace;
	rcText.top = rcItem.top + m_uTxtTSpace;
	rcText.bottom = rcItem.bottom - m_uTxtBSpace;

	return ( ( rcText.right > rcText.left ) && ( rcText.bottom > rcText.top ) );
}

YBCtrlImageStatic* YBCtrlMIDefault::_getImageStatic( unsigned int uImageId ) const{
	YBCtrlImageBase* pYBImageBase = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pYBImageBase ) )
		return NULL;
	if( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;

}

bool YBCtrlMIDefault::canClick(){
	return ( ( m_uFlag&YBCMID_FLAG_CANCLICK ) == YBCMID_FLAG_CANCLICK );
}

void YBCtrlMIDefault::setMouseHoverFlag( bool bHover ){
	if ( bHover ) {
		m_uFlag |= YBCMID_FLAG_HOVER;
	} else {
		m_uFlag &= ~YBCMID_FLAG_HOVER;
	}
}

void YBCtrlMIDefault::setCanClickFlag( bool bCanClick ) {
	if ( bCanClick ) {
		m_uFlag |= YBCMID_FLAG_CANCLICK;
	} else {
		m_uFlag &= ~YBCMID_FLAG_CANCLICK;
	}
}

bool YBCtrlMIDefault::isDisable() {
	return ( ( m_uFlag & YBCMID_FLAG_DISABLE ) == YBCMID_FLAG_DISABLE );
}

void YBCtrlMIDefault::setDisableFlag( bool bDisable ) {
	if ( bDisable ) {
		m_uFlag |= YBCMID_FLAG_DISABLE;
	} else {
		m_uFlag &= ~YBCMID_FLAG_DISABLE;
	}
}

bool YBCtrlMIDefault::isDivide2Part() {
	return false;
}
bool YBCtrlMIDefault::isDivide3Part() {
	return false;
}
bool YBCtrlMIDefault::isHover() const {
	return ( ( m_uFlag & YBCMID_FLAG_HOVER ) == YBCMID_FLAG_HOVER );
}
void YBCtrlMIDefault::setMouseHoverInPart1Flag( bool bMouseHoverInPart1 ){}
void YBCtrlMIDefault::setMouseHoverInPart2Flag( bool bMouseHoverInPart2 ){}
void YBCtrlMIDefault::setDisableInPart1Flag( bool bDisableInPart1 ){}
void YBCtrlMIDefault::setDisableInPart2Flag( bool bDisableInPart2 ){}
void YBCtrlMIDefault::setNormalInPart1Flag( bool bMouseNormalInPart1 ){}
void YBCtrlMIDefault::setNormalInPart2Flag( bool bMouseNormalInPart2 ){}
void YBCtrlMIDefault::setMouseHoverInPart3Flag( bool bMouseHoverInPart3 ){}
bool YBCtrlMIDefault::CanClickFlagInPart1() {return true;}
bool YBCtrlMIDefault::CanClickFlagInPart2() {return true;}
bool YBCtrlMIDefault::CanClickFlagInPart3() {return true;}
bool YBCtrlMIDefault::isDividePara1Disable() {return true;}
bool YBCtrlMIDefault::isDividePara2Disable() {return true;}
bool YBCtrlMIDefault::isDividePara3Disable() {return true;}
bool YBCtrlMIDefault::isDivide2PartCenter() {return true;}