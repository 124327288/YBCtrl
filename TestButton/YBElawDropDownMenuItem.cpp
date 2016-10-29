#include "stdafx.h"
#include "YBElawDropDownMenuItem.h"
#include "frame/YBCtrlMisc.h"

#define YBCMID_FLAG_DRAWICON (0x00000008)
USING_NAMESPACE_YBCTRL

YBElawDropDownMenuItem::YBElawDropDownMenuItem()
: m_uImageIdCheckIcon( YBCTRL_IMAGE_ID_INVALID )
{

}

YBElawDropDownMenuItem::~YBElawDropDownMenuItem() {

}

void YBElawDropDownMenuItem::_drawSelf( HDC hDC, const RECT& rcItem ) {
	if ( ( m_uFlag & YBCMID_FLAG_DRAWICON ) != YBCMID_FLAG_DRAWICON )
		return;
	//
	RECT rcCheckIcon;
	YBCtrlImageStatic* pYBImage = NULL;

	pYBImage = _getImageStatic( m_uImageIdCheckIcon );
	if ( !pYBImage ) {
		return;
	}

	Gdiplus::Graphics graphics( hDC );
	if ( _getCheckIconRect( rcItem, rcCheckIcon ) ) {
		GdipBitBlt( &graphics, pYBImage->getImage(), rcCheckIcon.left, rcCheckIcon.top, ( rcCheckIcon.right - rcCheckIcon.left ), ( rcCheckIcon.bottom - rcCheckIcon.top )
			, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
	}
}

bool YBElawDropDownMenuItem::_getCheckIconRect( const RECT& rcItem, RECT& rcCheckIcon ) const {
	YBCtrlImageStatic* pYBImage = NULL;

	pYBImage = _getImageStatic( m_uImageIdCheckIcon );
	if ( !pYBImage )
		return false;
	rcCheckIcon.right = rcItem.right - 8;
	rcCheckIcon.left = rcCheckIcon.right - pYBImage->getWidth();
	rcCheckIcon.top = rcItem.top + ( rcItem.bottom - rcItem.top - pYBImage->getHeight() )/2;
	rcCheckIcon.bottom = rcCheckIcon.top + pYBImage->getHeight();

	return ( ( rcCheckIcon.right > rcCheckIcon.left ) && ( rcCheckIcon.bottom > rcCheckIcon.top ) );
}

bool YBElawDropDownMenuItem::isDrawCheckIcon() {
	return ( ( m_uFlag & YBCMID_FLAG_DRAWICON ) == YBCMID_FLAG_DRAWICON );
}

void YBElawDropDownMenuItem::setDrawCheckIcon( bool bDrawCheckIcon ) {
	if ( bDrawCheckIcon ) {
		m_uFlag |= YBCMID_FLAG_DRAWICON;
	} else {
		m_uFlag &= ~YBCMID_FLAG_DRAWICON;
	}
}