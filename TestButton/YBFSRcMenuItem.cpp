#include "stdafx.h"
#include "YBFSRcMenuItem.h"
#include "frame/YBCtrlImageBase.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlMisc.h"
#include "frame/YBCtrlFontDepository.h"
#include <assert.h>

#define YBCMID_FLAG_NONE (0x00000000)
#define YBCMID_FLAG_DISABLE (0x00000001)
#define YBCMID_FLAG_HOVER (0x00000002)
#define YBCMID_FLAG_CANCLICK (0x00000004)
//
#define YBCMID_FLAG_DIVIDE_TWOPARTS (0x00000008)
#define YBCMID_FLAG_DIVIDE_THREEPARTS (0x0000010)

#define YBCMID_FLAG_HOVER_INPARTONE (0x00000020)
#define YBCMID_FLAG_DISABLE_INPARTONE (0x0000040)
#define YBCMID_FLAG_CANCLICK_INPARTONE (0x00000080)
#define YBCMID_FLAG_DISABLE_INPARTTWO (0x00000100)
#define YBCMID_FLAG_HOVER_INPARTTWO (0x00000200)
#define YBCMID_FLAG_CANCLICK_INPATRTWO (0x00000400)
#define YBCMID_FLAG_DISABLE_INPARTTHREE (0x00000800)
#define YBCMID_FLAG_HOVER_INPARTTHREE (0x00001000)
#define YBCMID_FLAG_CANCLICK_INPATRTHREE (0x00002000)
//
#define COLOR_YBCMID_DIVIDELINE (RGB(255, 255, 255))
#define COLOR_YBCMID_NUMDIVLINE (RGB(141, 181, 209))
//
USING_NAMESPACE_YBCTRL


YBFSRcMenuItem::YBFSRcMenuItem()
: m_uFlag( YBCMID_FLAG_NONE )
, m_uImageIdBk( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdHover( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdIconNor( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdDisable( YBCTRL_IMAGE_ID_INVALID )
, m_uFontIdNorTxt( YBCTRL_FONT_ID_INVALID )
, m_uFontIdDisTxt( YBCTRL_FONT_ID_INVALID )
, m_uFontIdHovTxt( YBCTRL_FONT_ID_INVALID )
, m_uFontIdIconTxt( YBCTRL_FONT_ID_INVALID )
, m_uFontIdNumTxt( YBCTRL_FONT_ID_INVALID )
, m_tstrNumber( _T("") )
, m_tstrIcon( _T("点击操作") )
, m_tstrDividePart1Icon( _T("点击") )
, m_tstrDividePart2Icon( _T("点击") )
, m_tstrDividePart3Icon( _T("点击") )
, m_tstrDividePart1Txt( _T("点击操作") )
, m_tstrDividePart2Txt( _T("点击操作") )
, m_tstrDividePart3Txt( _T("点击操作") )
, m_tstrTxt( _T("") )
, m_uIconRSpace ( 4 )
, m_uTxtLSpace ( 39 )
, m_uTxtRSpace ( 80 )
, m_uTxtTSpace ( 0 )
, m_uTxtBSpace ( 0 )
, m_uItemHeight( 0 )
, m_bDrawDivideLine( false )
, m_hPenDivideLine( NULL )
, m_uLineRSpace( 5 )
, m_uNumberWidth( 33 )
, m_uNumberLSpace( 1 )
{
	m_hPenDivideLine = ::CreatePen( PS_SOLID, 2, COLOR_YBCMID_DIVIDELINE );
	m_hPenNumberDivLine = ::CreatePen( PS_SOLID, 2, COLOR_YBCMID_NUMDIVLINE );
}

YBFSRcMenuItem::~YBFSRcMenuItem() {
	if ( m_hPenDivideLine ) {
		::DeleteObject( m_hPenDivideLine );
		m_hPenDivideLine = NULL;
	}
	if ( m_hPenNumberDivLine ) {
		::DeleteObject( m_hPenNumberDivLine );
		m_hPenNumberDivLine = NULL;
	}
}

void YBFSRcMenuItem::drawStatus( HDC hDC, const RECT& rcItem ) const {
	_drawBk( hDC, rcItem );
	_drawNumber( hDC, rcItem );
	_drawIcon( hDC, rcItem );
	_drawText( hDC, rcItem );
	_drawIconText( hDC, rcItem );
	_drawNumberText( hDC, rcItem );
	_drawDivideLine( hDC, rcItem );
	_drawDividePara1Text( hDC, rcItem );
	_drawDividePara2Text( hDC, rcItem );
	_drawDividePara3Text( hDC, rcItem );
}

unsigned int YBFSRcMenuItem::getHeight() const {
	 return m_uItemHeight;
}

void YBFSRcMenuItem::_drawBk( HDC hDC, const RECT& rcItem ) const {
	YBCtrlImageStatic* pYBImage = NULL;
	if ( m_uFlag & YBCMID_FLAG_DIVIDE_TWOPARTS ) {
		if ( ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) {
			pYBImage = _getImageStatic( m_uImageIdDis_Dis );
		} else if ( ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) && ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) ) {
			pYBImage = _getImageStatic( m_uImageIdDis_Hov );
		} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) {
			pYBImage = _getImageStatic( m_uImageIdHov_Dis );
		} else if ( ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) && ( !(m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO) && ( !(m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO) ) ) ) {
			pYBImage = _getImageStatic( m_uImageIdDis_Bk );
		} else if ( (!(m_uFlag & YBCMID_FLAG_HOVER_INPARTONE)) && (!(m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) {
			pYBImage = _getImageStatic( m_uImageIdBk_Dis );
		} else if ( ( !(m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) && (!(m_uFlag & YBCMID_FLAG_HOVER_INPARTONE)) && ( !(m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO) ) && (!(m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO)) ) {
			pYBImage = _getImageStatic( m_uImageIdBk );
		} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) && ( !(m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO) ) && (!(m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO )) ) {
			pYBImage = _getImageStatic( m_uImageIdHov_Bk );
		} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) && ( !(m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) && (!(m_uFlag & YBCMID_FLAG_HOVER_INPARTONE )) ) ) {
			pYBImage = _getImageStatic( m_uImageIdBk_Hov );
		} else {
			assert( false );
		}
	} else if ( m_uFlag & YBCMID_FLAG_DIVIDE_THREEPARTS ) {


		if ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) {
			if ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) {
				if( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) {
					pYBImage = _getImageStatic( m_uImageIdDis_Dis_Dis );
				} else {
					if (  m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) {
						pYBImage = _getImageStatic( m_uImageIdDis_Dis_Hov );
					} else if ( ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) ) {
						pYBImage = _getImageStatic( m_uImageIdDis_Dis_Bk );
					}  else {
						assert( false );
						return;
					}
				}
			} else {
				if( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) {
					if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) {
						pYBImage = _getImageStatic( m_uImageIdDis_Hov_Dis );
					} else if ( ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) {
						pYBImage = _getImageStatic( m_uImageIdDis_Bk_Dis );
					}  else {
						assert( false );
						return;
					}
				} else {
					if ( ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) ) && ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) {
						pYBImage = _getImageStatic( m_uImageIdDis_Bk_Hov );
					} else if ( ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) ) && ( !( m_uFlag& YBCMID_FLAG_DISABLE_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) ) {
						pYBImage = _getImageStatic( m_uImageIdDis_Bk_Bk );
					} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) && ( !( m_uFlag& YBCMID_FLAG_DISABLE_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) ) {
						pYBImage = _getImageStatic( m_uImageIdDis_Hov_Bk );
					} else {
						assert( false );
						return;
					} 
				}
			}
		} else {
			if ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) {
				if( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) {
					if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) {
						pYBImage = _getImageStatic( m_uImageIdHov_Dis_Dis );
					} else if ( ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) {
						pYBImage = _getImageStatic( m_uImageIdBk_Dis_Dis );
					} else {
						assert( false );
						return;
					}
				} else {
					if ( ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) ) {
						pYBImage = _getImageStatic( m_uImageIdBk_Dis_Bk );
					} else if ( ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) && ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) {
						pYBImage = _getImageStatic( m_uImageIdBk_Dis_Hov );
					} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) ) {
						pYBImage = _getImageStatic( m_uImageIdHov_Dis_Bk );
					} else {
						assert( false );
						return;
					}
				}
			} else {
				if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) ) {
					pYBImage = _getImageStatic( m_uImageIdHov_Bk_Bk );
				} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) ) ) {
					pYBImage = _getImageStatic( m_uImageIdBk_Bk_Hov );
				} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) ) {
					pYBImage = _getImageStatic( m_uImageIdBk_Hov_Bk );
				} else if ( ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) ) {
					pYBImage = _getImageStatic( m_uImageIdBk_Bk_Bk );
				} else  if ( ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) {
					pYBImage = _getImageStatic( m_uImageIdBk_Bk_Dis );
				} else if ( ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) && ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) {
					pYBImage = _getImageStatic( m_uImageIdBk_Hov_Dis );
				} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) {
					pYBImage = _getImageStatic( m_uImageIdHov_Bk_Dis );
				} else {
					assert( false );
					return;
				}
			}
		}
	} else {
		if ( m_uFlag & YBCMID_FLAG_DISABLE ) {
			pYBImage = _getImageStatic( m_uImageIdDisable );
		} else {
			if ( m_uFlag & YBCMID_FLAG_HOVER ) {
				pYBImage = _getImageStatic( m_uImageIdHover );
			} else {
				pYBImage = _getImageStatic( m_uImageIdBk );
			}
		}
	}

	if( !pYBImage ){
		return;
	}
	BackGroundBitBlt( hDC, rcItem.left, rcItem.top, rcItem.right - rcItem.left, rcItem.bottom - rcItem.top, pYBImage->getMemDC(),
		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0 );

}

void YBFSRcMenuItem::_drawIcon( HDC hDC, const RECT& rcItem ) const {
	RECT rcIcon;
	RECT rcDividePart1Icon;
	RECT rcDividePart2Icon;
	RECT rcDividePart3Icon;
	YBCtrlImageStatic* pYBImage = NULL;
	YBCtrlImageStatic* pYBIconPart1 = NULL;
	YBCtrlImageStatic* pYBIconPart2 = NULL;
	YBCtrlImageStatic* pYBIconPart3 = NULL;

	if ( m_uFlag & YBCMID_FLAG_DIVIDE_TWOPARTS ) {
		if ( ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) {
			pYBIconPart1 = _getImageStatic( m_uImageIdIconDisable );
			pYBIconPart2 = _getImageStatic( m_uImageIdIconDisable );
		} else if ( ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) && ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) ) {
			pYBIconPart1 = _getImageStatic( m_uImageIdIconDisable );
			pYBIconPart2 = _getImageStatic( m_uImageIdIconHor );
		} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) {
			pYBIconPart1 = _getImageStatic( m_uImageIdIconHor );
			pYBIconPart2 = _getImageStatic( m_uImageIdIconDisable );
		} else if ( ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) && ( !(m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO) && ( !(m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO) ) ) ) {
			pYBIconPart1 = _getImageStatic( m_uImageIdIconDisable );
			pYBIconPart2 = _getImageStatic( m_uImageIdIconNor );
		} else if ( (!(m_uFlag & YBCMID_FLAG_HOVER_INPARTONE)) && (!(m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) {
			pYBIconPart1 = _getImageStatic( m_uImageIdIconNor );
			pYBIconPart2 = _getImageStatic( m_uImageIdIconDisable );
		} else if ( ( !(m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE )  ) && (!(m_uFlag & YBCMID_FLAG_HOVER_INPARTONE)) && ( !(m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO) ) && (!(m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO)) ) {
			pYBIconPart1 = _getImageStatic( m_uImageIdIconNor );
			pYBIconPart2 = _getImageStatic( m_uImageIdIconNor );
		} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) && ( !(m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO) ) && (!(m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO )) ) {
			pYBIconPart1 = _getImageStatic( m_uImageIdIconHor );
			pYBIconPart2 = _getImageStatic( m_uImageIdIconNor );
		} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) && ( !(m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) && (!(m_uFlag & YBCMID_FLAG_HOVER_INPARTONE )) ) ) {
			pYBIconPart1 = _getImageStatic( m_uImageIdIconNor );
			pYBIconPart2 = _getImageStatic( m_uImageIdIconHor );
		} else {
			assert( false );
		}
		//
		if ( !pYBIconPart1 || !pYBIconPart2 ) {
			return;
		}
		//
		Gdiplus::Graphics graphics( hDC );
		if ( ( _getDividePara1IconRect( rcItem, rcDividePart1Icon ) ) && ( _getDividePara2IconRect( rcItem, rcDividePart2Icon ) ) ) {
			// Icon1
			GdipBitBlt( &graphics, pYBIconPart1->getImage(), rcDividePart1Icon.left, rcDividePart1Icon.top, ( rcDividePart1Icon.right - rcDividePart1Icon.left ),
				( rcDividePart1Icon.bottom - rcDividePart1Icon.top ), 0, 0, pYBIconPart1->getWidth(), pYBIconPart1->getHeight() );
			// Icon2
			GdipBitBlt( &graphics, pYBIconPart2->getImage(), rcDividePart2Icon.left, rcDividePart2Icon.top, ( rcDividePart2Icon.right - rcDividePart2Icon.left ),
				( rcDividePart2Icon.bottom - rcDividePart2Icon.top ), 0, 0, pYBIconPart2->getWidth(), pYBIconPart2->getHeight() );
		}
	} else if ( m_uFlag & YBCMID_FLAG_DIVIDE_THREEPARTS ) {
		if ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) {
			if ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) {
				if( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) {
					pYBIconPart1 = _getImageStatic( m_uImageIdIconDisable );
					pYBIconPart2 = _getImageStatic( m_uImageIdIconDisable );
					pYBIconPart3 = _getImageStatic( m_uImageIdIconDisable );
				} else {
					if (  m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) {
						pYBIconPart1 = _getImageStatic( m_uImageIdIconDisable );
						pYBIconPart2 = _getImageStatic( m_uImageIdIconDisable );
						pYBIconPart3 = _getImageStatic( m_uImageIdIconHor );
					} else if ( ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) ) {
						pYBIconPart1 = _getImageStatic( m_uImageIdIconDisable );
						pYBIconPart2 = _getImageStatic( m_uImageIdIconDisable );
						pYBIconPart3 = _getImageStatic( m_uImageIdIconNor );
					}  else {
						assert( false );
						return;
					}
				}
			} else {
				if( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) {
					if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) {
						pYBIconPart1 = _getImageStatic( m_uImageIdIconDisable );
						pYBIconPart2 = _getImageStatic( m_uImageIdIconHor );
						pYBIconPart3 = _getImageStatic( m_uImageIdIconDisable );
					} else if ( ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) {
						pYBIconPart1 = _getImageStatic( m_uImageIdIconDisable );
						pYBIconPart2 = _getImageStatic( m_uImageIdIconNor );
						pYBIconPart3 = _getImageStatic( m_uImageIdIconDisable );
					}  else {
						assert( false );
						return;
					}
				} else {
					if ( ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) ) && ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) {
						pYBIconPart1 = _getImageStatic( m_uImageIdIconDisable );
						pYBIconPart2 = _getImageStatic( m_uImageIdIconNor );
						pYBIconPart3 = _getImageStatic( m_uImageIdIconHor );
					} else if ( ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) ) && ( !( m_uFlag& YBCMID_FLAG_DISABLE_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) ) {
						pYBIconPart1 = _getImageStatic( m_uImageIdIconDisable );
						pYBIconPart2 = _getImageStatic( m_uImageIdIconNor );
						pYBIconPart3 = _getImageStatic( m_uImageIdIconNor );
					} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) && ( !( m_uFlag& YBCMID_FLAG_DISABLE_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) ) {
						pYBIconPart1 = _getImageStatic( m_uImageIdIconDisable );
						pYBIconPart2 = _getImageStatic( m_uImageIdIconHor );
						pYBIconPart3 = _getImageStatic( m_uImageIdIconNor );
					} else {
						assert( false );
						return;
					} 
				}
			}
		} else {
			if ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) {
				if( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) {
					if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) {
						pYBIconPart1 = _getImageStatic( m_uImageIdIconHor );
						pYBIconPart2 = _getImageStatic( m_uImageIdIconDisable );
						pYBIconPart3 = _getImageStatic( m_uImageIdIconDisable );
					} else if ( ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) {
						pYBIconPart1 = _getImageStatic( m_uImageIdIconNor );
						pYBIconPart2 = _getImageStatic( m_uImageIdIconDisable );
						pYBIconPart3 = _getImageStatic( m_uImageIdIconDisable );
					} else {
						assert( false );
						return;
					}
				} else {
					if ( ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) ) {
						pYBIconPart1 = _getImageStatic( m_uImageIdIconNor );
						pYBIconPart2 = _getImageStatic( m_uImageIdIconDisable );
						pYBIconPart3 = _getImageStatic( m_uImageIdIconNor );
					} else if ( ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) && ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) {
						pYBIconPart1 = _getImageStatic( m_uImageIdIconNor );
						pYBIconPart2 = _getImageStatic( m_uImageIdIconDisable );
						pYBIconPart3 = _getImageStatic( m_uImageIdIconHor );
					} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) ) {
						pYBIconPart1 = _getImageStatic( m_uImageIdIconHor );
						pYBIconPart2 = _getImageStatic( m_uImageIdIconDisable );
						pYBIconPart3 = _getImageStatic( m_uImageIdIconNor );
					} else {
						assert( false );
						return;
					}
				}
			} else {
				if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) ) {
					pYBIconPart1 = _getImageStatic( m_uImageIdIconHor );
					pYBIconPart2 = _getImageStatic( m_uImageIdIconNor );
					pYBIconPart3 = _getImageStatic( m_uImageIdIconNor );
				} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) ) ) {
					pYBIconPart1 = _getImageStatic( m_uImageIdIconNor );
					pYBIconPart2 = _getImageStatic( m_uImageIdIconNor );
					pYBIconPart3 = _getImageStatic( m_uImageIdIconHor );
				} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) ) {
					pYBIconPart1 = _getImageStatic( m_uImageIdIconNor );
					pYBIconPart2 = _getImageStatic( m_uImageIdIconHor );
					pYBIconPart3 = _getImageStatic( m_uImageIdIconNor );
				} else if ( ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) ) {
					pYBIconPart1 = _getImageStatic( m_uImageIdIconNor );
					pYBIconPart2 = _getImageStatic( m_uImageIdIconNor );
					pYBIconPart3 = _getImageStatic( m_uImageIdIconNor );
				} else  if ( ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) {
					pYBIconPart1 = _getImageStatic( m_uImageIdIconNor );
					pYBIconPart2 = _getImageStatic( m_uImageIdIconNor );
					pYBIconPart3 = _getImageStatic( m_uImageIdIconDisable );
				} else if ( ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) ) && ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) {
					pYBIconPart1 = _getImageStatic( m_uImageIdIconNor );
					pYBIconPart2 = _getImageStatic( m_uImageIdIconHor );
					pYBIconPart3 = _getImageStatic( m_uImageIdIconDisable );
				} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) && ( !( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) ) && ( !( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) ) && ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) ) {
					pYBIconPart1 = _getImageStatic( m_uImageIdIconHor );
					pYBIconPart2 = _getImageStatic( m_uImageIdIconNor );
					pYBIconPart3 = _getImageStatic( m_uImageIdIconDisable );
				} else {
					assert( false );
					return;
				}
			}
		}
		//
		if ( !pYBIconPart1 || !pYBIconPart2 || !pYBIconPart3 ) {
			return;
		}
		//
		Gdiplus::Graphics graphics( hDC );
		if ( ( _getDividePara1IconRect( rcItem, rcDividePart1Icon ) ) && ( _getDividePara2IconRect( rcItem, rcDividePart2Icon ) ) && ( _getDividePara3IconRect( rcItem, rcDividePart3Icon ) ) ) {
			// Icon1
			GdipBitBlt( &graphics, pYBIconPart1->getImage(), rcDividePart1Icon.left, rcDividePart1Icon.top, ( rcDividePart1Icon.right - rcDividePart1Icon.left ),
				( rcDividePart1Icon.bottom - rcDividePart1Icon.top ), 0, 0, pYBIconPart1->getWidth(), pYBIconPart1->getHeight() );
			// Icon2
			GdipBitBlt( &graphics, pYBIconPart2->getImage(), rcDividePart2Icon.left, rcDividePart2Icon.top, ( rcDividePart2Icon.right - rcDividePart2Icon.left ),
				( rcDividePart2Icon.bottom - rcDividePart2Icon.top ), 0, 0, pYBIconPart2->getWidth(), pYBIconPart2->getHeight() );
			// Icon3
			GdipBitBlt( &graphics, pYBIconPart3->getImage(), rcDividePart3Icon.left, rcDividePart3Icon.top, ( rcDividePart3Icon.right - rcDividePart3Icon.left ),
				( rcDividePart3Icon.bottom - rcDividePart3Icon.top ), 0, 0, pYBIconPart3->getWidth(), pYBIconPart3->getHeight() );
		}
	} else {
		if ( m_uFlag & YBCMID_FLAG_DISABLE ) {
			pYBImage = _getImageStatic( m_uImageIdIconDisable );
		} else {
			if ( m_uFlag & YBCMID_FLAG_HOVER ) {
				pYBImage = _getImageStatic( m_uImageIdIconHor );
			} else {
				pYBImage = _getImageStatic( m_uImageIdIconNor );
			}
		}
		if ( !pYBImage ) {
			return;
		}
		//
		Gdiplus::Graphics graphics( hDC );
		if ( _getIconRect( rcItem, rcIcon ) ) {
			GdipBitBlt( &graphics, pYBImage->getImage(), rcIcon.left, rcIcon.top, ( rcIcon.right - rcIcon.left ), ( rcIcon.bottom - rcIcon.top )
				, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
		}
	}
}

bool YBFSRcMenuItem::_getDividePara1Rect( const RECT& rcItem, RECT& rcDividePara1 ) const{
	rcDividePara1.left = rcItem.left + m_uTxtLSpace;
	//
	if ( ( m_uFlag & YBCMID_FLAG_DIVIDE_TWOPARTS ) == YBCMID_FLAG_DIVIDE_TWOPARTS ) {
		rcDividePara1.right = ( rcItem.right - rcItem.left )/2;
	} else if ( ( m_uFlag & YBCMID_FLAG_DIVIDE_THREEPARTS ) == YBCMID_FLAG_DIVIDE_THREEPARTS ) {
		rcDividePara1.right = rcItem.left + 106;
	} else {
		return false;
	}
	rcDividePara1.top = rcItem.top;
	rcDividePara1.bottom = rcItem.bottom;

	return ( ( rcDividePara1.right > rcDividePara1.left ) && ( rcDividePara1.bottom > rcDividePara1.top ) );
}

bool YBFSRcMenuItem::_getDividePara2Rect( const RECT& rcItem, RECT& rcDividePara2 ) const{
	if ( ( m_uFlag & YBCMID_FLAG_DIVIDE_TWOPARTS ) == YBCMID_FLAG_DIVIDE_TWOPARTS ) {
		rcDividePara2.left = ( rcItem.right - rcItem.left )/2 + m_uTxtLSpace - m_uNumberWidth;
		rcDividePara2.right = rcItem.right;
	} else if ( ( m_uFlag & YBCMID_FLAG_DIVIDE_THREEPARTS ) == YBCMID_FLAG_DIVIDE_THREEPARTS ) {
		rcDividePara2.left = rcItem.left + 111;
		rcDividePara2.right = rcItem.left + 179;
	} else {
		return false;
	}
	rcDividePara2.top = rcItem.top;
	rcDividePara2.bottom = rcItem.bottom;

	return ( ( rcDividePara2.right > rcDividePara2.left ) && ( rcDividePara2.bottom > rcDividePara2.top ) );
}

void YBFSRcMenuItem::_drawNumber( HDC hDC, const RECT& rcItem ) const {
	HBRUSH hBrushNumber = NULL;
	RECT rcNumber;
	//RECT rcDividePara1;
	//RECT rcDividePara2;
	//RECT rcDividePara1Number;
	//RECT rcDividePara2Number;

	//if ( m_uFlag & YBCMID_FLAG_DIVIDE_TWOPARTS ) {
	//	if ( _getDividePara1Rect( rcItem, rcDividePara1 ) && ( _getDividePara2Rect( rcItem, rcDividePara2 ) ) ) {
	//		if ( _getDividePara1NumberRect( rcDividePara1, rcDividePara1Number ) && _getDividePara2NumberRect( rcDividePara2, rcDividePara2Number ) ) {
	//			hBrushNumber = ::CreateSolidBrush( RGB( 158, 200, 228 ) );
	//			::FillRect( hDC, &rcDividePara1Number, hBrushNumber );
	//			::FillRect( hDC, &rcDividePara2Number, hBrushNumber );
	//			::DeleteObject( hBrushNumber );
	//		}
	//	}
	//} else {
	if ( !_getNumberRect( rcItem, rcNumber ) )
		return;
	hBrushNumber = ::CreateSolidBrush( RGB( 158, 200, 228 ) );
	::FillRect( hDC, &rcNumber, hBrushNumber );
	::DeleteObject( hBrushNumber );
	//}
}

void YBFSRcMenuItem::_drawDivideLine( HDC hDC, const RECT& rcItem ) const {
	if ( m_bDrawDivideLine ) {
		HPEN hPenOld = NULL;
		HPEN hPenNumOld = NULL;
		hPenOld = (HPEN)::SelectObject( hDC, m_hPenDivideLine );
		if ( m_uNumberLSpace > unsigned int( rcItem.right - rcItem.left )
			|| ( m_uNumberWidth > unsigned int( rcItem.right - rcItem.left )
			|| ( ( m_uNumberWidth + m_uNumberLSpace ) > unsigned int( rcItem.right - rcItem.left ) )
			|| ( ( m_uNumberLSpace + m_uNumberWidth + m_uLineRSpace > unsigned int( rcItem.right - rcItem.left ) ) )) ) {
				assert( false );
				return;
		}
		::MoveToEx( hDC, m_uNumberLSpace + m_uNumberWidth - 1, rcItem.bottom, NULL );
		::LineTo( hDC, rcItem.right - m_uLineRSpace - 1, rcItem.bottom );
		// 同一个Item内分割线
		if ( m_uFlag & YBCMID_FLAG_DIVIDE_TWOPARTS ) {
			::MoveToEx( hDC, ( rcItem.right - rcItem.left )/2, rcItem.top, NULL );
			::LineTo( hDC, ( rcItem.right - rcItem.left )/2, rcItem.bottom );
		}
		if ( m_uFlag & YBCMID_FLAG_DIVIDE_THREEPARTS ) {
			::MoveToEx( hDC, 107, rcItem.top, NULL );
			::LineTo( hDC, 107, rcItem.bottom );
			//
			::MoveToEx( hDC, 180, rcItem.top, NULL );
			::LineTo( hDC, 180, rcItem.bottom );
		}
		::SelectObject( hDC, hPenOld );
		//
		hPenNumOld = (HPEN)::SelectObject( hDC, m_hPenNumberDivLine );
		::MoveToEx( hDC, rcItem.left - 1, rcItem.bottom, NULL );
		::LineTo( hDC, m_uNumberLSpace + m_uNumberWidth - 1, rcItem.bottom );
		::SelectObject( hDC, hPenNumOld );
	}
	return;
}


bool YBFSRcMenuItem::_getNumberRect( const RECT& rcItem, RECT& rcNumber ) const {
	if ( ( m_uNumberLSpace + m_uNumberWidth ) > unsigned int( rcItem.right - rcItem.left ) ) {
		assert( false );
		return false;
	}
	rcNumber.left = rcItem.left + m_uNumberLSpace;
	rcNumber.right = rcNumber.left + m_uNumberWidth;
	rcNumber.top = rcItem.top;
	rcNumber.bottom = rcItem.bottom;

	return ( ( rcNumber.right > rcNumber.left ) && ( rcNumber.bottom > rcNumber.top ) );
}

bool YBFSRcMenuItem::_getDividePara1NumberRect( const RECT& rcDividePara1, RECT& rcNumber ) const {
	if ( ( m_uNumberLSpace + m_uNumberWidth ) > unsigned int( rcDividePara1.right - rcDividePara1.left ) ) {
		assert( false );
		return false;
	}
	rcNumber.left = rcDividePara1.left + m_uNumberLSpace;
	rcNumber.right = rcNumber.left + m_uNumberWidth;
	rcNumber.top = rcDividePara1.top;
	rcNumber.bottom = rcDividePara1.bottom;

	return ( ( rcNumber.right > rcNumber.left ) && ( rcNumber.bottom > rcNumber.top ) );
}

bool YBFSRcMenuItem::_getDividePara2NumberRect( const RECT& rcDividePara2, RECT& rcNumber ) const {
	if ( ( m_uNumberLSpace + m_uNumberWidth ) > unsigned int( rcDividePara2.right - rcDividePara2.left ) ) {
		assert( false );
		return false;
	}
	rcNumber.left = rcDividePara2.left + m_uNumberLSpace;
	rcNumber.right = rcNumber.left + m_uNumberWidth;
	rcNumber.top = rcDividePara2.top;
	rcNumber.bottom = rcDividePara2.bottom;

	return ( ( rcNumber.right > rcNumber.left ) && ( rcNumber.bottom > rcNumber.top ) );
}

bool YBFSRcMenuItem::_getDividePara1IconRect( const RECT& rcItem, RECT& rcIcon ) const {
	YBCtrlImageStatic* pYBImage = NULL;

	//if ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) {
	//	pYBImage = _getImageStatic( m_uImageIdIconDisable );
	//} else if ( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) {
	//	pYBImage = _getImageStatic( m_uImageIdIconHor );
	//} else {
	//	pYBImage = _getImageStatic( m_uImageIdIconNor );
	//}
	pYBImage = _getImageStatic( m_uImageIdIconNor );

	if ( !pYBImage ) {
		return false;
	}

	if( ( pYBImage->getHeight() > unsigned int( rcItem.bottom - rcItem.top ) )
		|| ( pYBImage->getWidth() > unsigned int( rcItem.right - rcItem.left ) )
		|| ( ( pYBImage->getWidth() + m_uIconRSpace ) > unsigned int( rcItem.right - rcItem.left ) ) ){
			assert(false);
			return false;
	}
	
	if ( m_uFlag & YBCMID_FLAG_DIVIDE_TWOPARTS ) {
		rcIcon.right =  (rcItem.right - rcItem.left)/2 - m_uIconRSpace;
		rcIcon.left = rcIcon.right - 40;
	} else if ( m_uFlag & YBCMID_FLAG_DIVIDE_THREEPARTS ) {
		rcIcon.right =  rcItem.left + 104;
		rcIcon.left = rcIcon.right - 37;
	} else {
		assert( false );
		return false;
	}
	rcIcon.top = rcItem.top + ( rcItem.bottom - rcItem.top - pYBImage->getHeight() )/2;
	rcIcon.bottom = rcIcon.top + pYBImage->getHeight();

	return ( ( rcIcon.right > rcIcon.left ) && (rcIcon.bottom > rcIcon.top ) );
}

bool YBFSRcMenuItem::_getDividePara2IconRect( const RECT& rcItem, RECT& rcIcon ) const {
	YBCtrlImageStatic* pYBImage = NULL;

	//if ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) {
	//	pYBImage = _getImageStatic( m_uImageIdIconDisable );
	//} else if ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) {
	//	pYBImage = _getImageStatic( m_uImageIdIconHor );
	//} else {
	//	pYBImage = _getImageStatic( m_uImageIdIconNor );
	//}
	pYBImage = _getImageStatic( m_uImageIdIconNor );

	if ( !pYBImage ) {
		return false;
	}

	if( ( pYBImage->getHeight() > unsigned int( rcItem.bottom - rcItem.top ) )
		|| ( pYBImage->getWidth() > unsigned int( rcItem.right - rcItem.left ) )
		|| ( ( pYBImage->getWidth() + m_uIconRSpace ) > unsigned int( rcItem.right - rcItem.left ) ) ){
			assert(false);
			return false;
	}

	if ( m_uFlag & YBCMID_FLAG_DIVIDE_TWOPARTS ) {
		rcIcon.right =  rcItem.right - m_uIconRSpace;
		rcIcon.left = rcIcon.right - 40;
	} else if ( m_uFlag & YBCMID_FLAG_DIVIDE_THREEPARTS ) {
		rcIcon.right =  177;
		rcIcon.left = rcIcon.right - 37;
	} else {
		assert( false );
		return false;
	}
	rcIcon.top = rcItem.top + ( rcItem.bottom - rcItem.top - pYBImage->getHeight() )/2;
	rcIcon.bottom = rcIcon.top + pYBImage->getHeight();

	return ( ( rcIcon.right > rcIcon.left ) && (rcIcon.bottom > rcIcon.top ) );
}

bool YBFSRcMenuItem::_getDividePara3IconRect( const RECT& rcItem, RECT& rcIcon ) const {
	YBCtrlImageStatic* pYBImage = NULL;

	pYBImage = _getImageStatic( m_uImageIdIconNor );
	if ( !pYBImage ) {
		return false;
	}
	rcIcon.right =  rcItem.right - m_uIconRSpace;
	rcIcon.left = rcIcon.right - 40;
	rcIcon.top = rcItem.top + ( rcItem.bottom - rcItem.top - pYBImage->getHeight() )/2;
	rcIcon.bottom = rcIcon.top + pYBImage->getHeight();

	return ( ( rcIcon.right > rcIcon.left ) && (rcIcon.bottom > rcIcon.top ) );
}

bool YBFSRcMenuItem::_getIconRect( const RECT& rcItem, RECT& rcIcon ) const {
	YBCtrlImageStatic* pYBImage = NULL;

	if ( m_uFlag & YBCMID_FLAG_DISABLE ) {
		pYBImage = _getImageStatic( m_uImageIdIconDisable );
	} else if ( m_uFlag & YBCMID_FLAG_HOVER ) {
		pYBImage = _getImageStatic( m_uImageIdIconHor );
	} else {
		pYBImage = _getImageStatic( m_uImageIdIconNor );
	}

	if ( !pYBImage ) {
		return false;
	}
	if( ( pYBImage->getHeight() > unsigned int( rcItem.bottom - rcItem.top ) )
		|| ( pYBImage->getWidth() > unsigned int( rcItem.right - rcItem.left ) )
		|| ( ( pYBImage->getWidth() + m_uIconRSpace ) > unsigned int( rcItem.right - rcItem.left ) ) ){
			assert(false);
			return false;
	}
	rcIcon.right = rcItem.right - m_uIconRSpace;
	rcIcon.left = rcIcon.right - pYBImage->getWidth();
	rcIcon.top = rcItem.top + ( rcItem.bottom - rcItem.top - pYBImage->getHeight() )/2;
	rcIcon.bottom = rcIcon.top + pYBImage->getHeight();

	return ( ( rcIcon.right > rcIcon.left ) && (rcIcon.bottom > rcIcon.top ) );
}

void YBFSRcMenuItem::_drawNumberText( HDC hDC, const RECT& rcItem ) const {
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcText;

	if( !_getNumberRect( rcItem, rcText ) )
		return;

	//if ( ( m_uFlag & YBCMID_FLAG_DISABLE ) == YBCMID_FLAG_DISABLE ) {
	//	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdDisTxt );
	//} else if ( ( m_uFlag & YBCMID_FLAG_HOVER ) == YBCMID_FLAG_HOVER ) {
	//	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdHovTxt );
	//} else {
	//	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdNorTxt );
	//}
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdNumTxt );
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
		::DrawText( hDC, m_tstrNumber.c_str(), m_tstrNumber.length(), &rcText, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

void YBFSRcMenuItem::_drawIconText( HDC hDC, const RECT& rcItem ) const {
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcText;
	RECT rcPart1Text;
	RECT rcPart2Text;
	RECT rcPart3Text;

	if ( m_uFlag & YBCMID_FLAG_DIVIDE_TWOPARTS ) {
		if ( ( ! _getDividePara1IconRect( rcItem, rcPart1Text ) || ( !_getDividePara2IconRect( rcItem, rcPart2Text ) ) ) ) 
			return;
		pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdIconTxt );
		if( pYBCtrlFont ){
			hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
			iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
			clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
			::DrawText( hDC, m_tstrDividePart1Icon.c_str(), m_tstrDividePart1Icon.length(), &rcPart1Text, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
			::DrawText( hDC, m_tstrDividePart2Icon.c_str(), m_tstrDividePart2Icon.length(), &rcPart2Text, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
			::SetTextColor( hDC, clrTxtOld );
			::SetBkMode( hDC, iBkModeOld );
			::SelectObject( hDC, hFontOld );
		}
	} else if ( m_uFlag & YBCMID_FLAG_DIVIDE_THREEPARTS ) {
		if ( ( ! _getDividePara1IconRect( rcItem, rcPart1Text ) || ( !_getDividePara2IconRect( rcItem, rcPart2Text ) ) || ( !_getDividePara3IconRect( rcItem, rcPart3Text ) ) ) ) 
			return;
		pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdIconTxt );
		if( pYBCtrlFont ){
			hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
			iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
			clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
			::DrawText( hDC, m_tstrDividePart1Icon.c_str(), m_tstrDividePart1Icon.length(), &rcPart1Text, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
			::DrawText( hDC, m_tstrDividePart2Icon.c_str(), m_tstrDividePart2Icon.length(), &rcPart2Text, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
			::DrawText( hDC, m_tstrDividePart3Icon.c_str(), m_tstrDividePart3Icon.length(), &rcPart3Text, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
			::SetTextColor( hDC, clrTxtOld );
			::SetBkMode( hDC, iBkModeOld );
			::SelectObject( hDC, hFontOld );
		}
	} else {
		if( !_getIconRect( rcItem, rcText ) )
			return;
		pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdIconTxt );
		if( pYBCtrlFont ){
			hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
			iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
			clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
			::DrawText( hDC, m_tstrIcon.c_str(), m_tstrIcon.length(), &rcText, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
			::SetTextColor( hDC, clrTxtOld );
			::SetBkMode( hDC, iBkModeOld );
			::SelectObject( hDC, hFontOld );
		}
	}
}

void YBFSRcMenuItem::_drawDividePara1Text( HDC hDC, const RECT& rcItem ) const {
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcText;
	
	if ( !_getDividePara1Rect( rcItem, rcText ) )
		return;
	
	if ( ( m_uFlag & YBCMID_FLAG_DIVIDE_TWOPARTS ) == YBCMID_FLAG_DIVIDE_TWOPARTS ) {
		if ( ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) == YBCMID_FLAG_DISABLE_INPARTONE ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdDisTxt );
		} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) == YBCMID_FLAG_HOVER_INPARTONE ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdHovTxt );
		} else {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdNorTxt );
		}
	} else if ( ( m_uFlag & YBCMID_FLAG_DIVIDE_THREEPARTS ) == YBCMID_FLAG_DIVIDE_THREEPARTS ) {
		if ( ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) == YBCMID_FLAG_DISABLE_INPARTONE ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdDisTxt );
		} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTONE ) == YBCMID_FLAG_HOVER_INPARTONE ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdHovTxt );
		} else {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdNorTxt );
		}
	}
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
		::DrawText( hDC, m_tstrDividePart1Txt.c_str(), m_tstrDividePart1Txt.length(), &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

void YBFSRcMenuItem::_drawDividePara2Text( HDC hDC, const RECT& rcItem ) const {
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcText;

	if ( !_getDividePara2Rect( rcItem, rcText ) )
		return;
	if ( ( m_uFlag & YBCMID_FLAG_DIVIDE_TWOPARTS ) == YBCMID_FLAG_DIVIDE_TWOPARTS ) {
		if ( ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) == YBCMID_FLAG_DISABLE_INPARTTWO ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdDisTxt );
		} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) == YBCMID_FLAG_HOVER_INPARTTWO ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdHovTxt );
		} else {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdNorTxt );
		}
	} else if ( ( m_uFlag & YBCMID_FLAG_DIVIDE_THREEPARTS ) == YBCMID_FLAG_DIVIDE_THREEPARTS ) {
		if ( ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) == YBCMID_FLAG_DISABLE_INPARTTWO ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdDisTxt );
		} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTWO ) == YBCMID_FLAG_HOVER_INPARTTWO ) {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdHovTxt );
		} else {
			pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdNorTxt );
		}
	} else {
		return;
	}
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
		::DrawText( hDC, m_tstrDividePart2Txt.c_str(), m_tstrDividePart2Txt.length(), &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

bool YBFSRcMenuItem::_getDividePara3Rect( const RECT& rcItem, RECT& rcDividePara3 ) const {
	if ( ( m_uFlag & YBCMID_FLAG_DIVIDE_THREEPARTS ) != YBCMID_FLAG_DIVIDE_THREEPARTS ) {
		return false;
	}
	rcDividePara3.left = rcItem.left + 182;
	rcDividePara3.right = rcItem.right;
	rcDividePara3.top = rcItem.top;
	rcDividePara3.bottom = rcItem.bottom;

	return ( ( rcDividePara3.right > rcDividePara3.left ) && ( rcDividePara3.bottom > rcDividePara3.top ) );
}

void YBFSRcMenuItem::_drawDividePara3Text( HDC hDC, const RECT& rcItem ) const {
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcText;

	if ( !_getDividePara3Rect( rcItem, rcText ) )
		return;
	if ( ( m_uFlag & YBCMID_FLAG_DIVIDE_THREEPARTS ) != YBCMID_FLAG_DIVIDE_THREEPARTS )
		return;

	if ( ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) == YBCMID_FLAG_DISABLE_INPARTTHREE ) {
		pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdDisTxt );
	} else if ( ( m_uFlag & YBCMID_FLAG_HOVER_INPARTTHREE ) == YBCMID_FLAG_HOVER_INPARTTHREE ) {
		pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdHovTxt );
	} else {
		pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdNorTxt );
	}
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
		::DrawText( hDC, m_tstrDividePart3Txt.c_str(), m_tstrDividePart3Txt.length(), &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

void YBFSRcMenuItem::_drawText( HDC hDC, const RECT& rcItem ) const {
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcText;

	if ( ( m_uFlag & YBCMID_FLAG_DIVIDE_TWOPARTS ) == YBCMID_FLAG_DIVIDE_TWOPARTS ) {
		return;
	}

	if ( ( m_uFlag & YBCMID_FLAG_DIVIDE_THREEPARTS ) == YBCMID_FLAG_DIVIDE_THREEPARTS ) {
		return;
	}

	if( !_getTextRect( rcItem, rcText ) )
		return;

	if ( ( m_uFlag & YBCMID_FLAG_DISABLE ) == YBCMID_FLAG_DISABLE ) {
		pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdDisTxt );
	} else if ( ( m_uFlag & YBCMID_FLAG_HOVER ) == YBCMID_FLAG_HOVER ) {
		pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdHovTxt );
	} else {
		pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdNorTxt );
	}
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
		::DrawText( hDC, m_tstrTxt.c_str(), m_tstrTxt.length(), &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

bool YBFSRcMenuItem::_getTextRect( const RECT& rcItem, RECT& rcText ) const {
	YBCtrlImageStatic* pYBImage = NULL;

	if ( m_uFlag & YBCMID_FLAG_DISABLE ) {
		pYBImage = _getImageStatic( m_uImageIdIconDisable );
	} else if ( m_uFlag & YBCMID_FLAG_HOVER ) {
		pYBImage = _getImageStatic( m_uImageIdIconHor );
	} else {
		pYBImage = _getImageStatic( m_uImageIdIconNor );
	}
	if( !pYBImage )
		return false;
	if( m_uTxtLSpace < ( m_uNumberLSpace + m_uNumberWidth ) ){
		m_uTxtLSpace = m_uNumberLSpace + m_uNumberWidth;
	}

	rcText.left = rcItem.left + m_uTxtLSpace;
	rcText.right = rcItem.right - m_uTxtRSpace;
	rcText.top = rcItem.top + m_uTxtTSpace;
	rcText.bottom = rcItem.bottom - m_uTxtBSpace;

	return ( ( rcText.right > rcText.left ) && ( rcText.bottom > rcText.top ) );
}

YBCtrlImageStatic* YBFSRcMenuItem::_getImageStatic( unsigned int uImageId ) const{
	YBCtrlImageBase* pYBImageBase = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pYBImageBase ) )
		return NULL;
	if( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;

}

bool YBFSRcMenuItem::canClick(){
	return ( ( m_uFlag & YBCMID_FLAG_CANCLICK ) == YBCMID_FLAG_CANCLICK );
}

void YBFSRcMenuItem::setMouseHoverFlag( bool bHover ){
	if ( bHover ) {
		m_uFlag |= YBCMID_FLAG_HOVER;
	} else {
		m_uFlag &= ~YBCMID_FLAG_HOVER;
	}
}

void YBFSRcMenuItem::setCanClickFlag( bool bCanClick ) {
	if ( bCanClick ) {
		m_uFlag |= YBCMID_FLAG_CANCLICK;
	} else {
		m_uFlag &= ~YBCMID_FLAG_CANCLICK;
	}
}

bool YBFSRcMenuItem::isDisable() {
	return ( ( m_uFlag & YBCMID_FLAG_DISABLE ) == YBCMID_FLAG_DISABLE );
}

bool YBFSRcMenuItem::isDivide2Part() {
	return ( ( m_uFlag & YBCMID_FLAG_DIVIDE_TWOPARTS ) == YBCMID_FLAG_DIVIDE_TWOPARTS );
}

bool YBFSRcMenuItem::isDivide3Part() {
	return ( ( m_uFlag & YBCMID_FLAG_DIVIDE_THREEPARTS ) == YBCMID_FLAG_DIVIDE_THREEPARTS );
}

void YBFSRcMenuItem::setDisableFlag( bool bDisable ) {
	if ( bDisable ) {
		m_uFlag |= YBCMID_FLAG_DISABLE;
	} else {
		m_uFlag &= ~YBCMID_FLAG_DISABLE;
	}
}

// DIVIDE
void YBFSRcMenuItem::setDividePart2Flag( bool bDivide ) {
	if ( bDivide ) {
		m_uFlag |= YBCMID_FLAG_DIVIDE_TWOPARTS;
	} else {
		m_uFlag &= ~YBCMID_FLAG_DIVIDE_TWOPARTS;
	}
}

void YBFSRcMenuItem::setDividePart3Flag( bool bDivide ) {
	if ( bDivide ) {
		m_uFlag |= YBCMID_FLAG_DIVIDE_THREEPARTS;
	} else {
		m_uFlag &= ~YBCMID_FLAG_DIVIDE_THREEPARTS;
	}
}

// HOVER
void YBFSRcMenuItem::setMouseHoverInPart1Flag( bool bMouseHoverInPart1 ) {
	if ( bMouseHoverInPart1 ) {
		m_uFlag |= YBCMID_FLAG_HOVER_INPARTONE;
	} else {
		m_uFlag &= ~YBCMID_FLAG_HOVER_INPARTONE;
	}
}

void YBFSRcMenuItem::setMouseHoverInPart2Flag( bool bMouseHoverInPart2 ) {
	if ( bMouseHoverInPart2 ) {
		m_uFlag |= YBCMID_FLAG_HOVER_INPARTTWO;
	} else {
		m_uFlag &= ~YBCMID_FLAG_HOVER_INPARTTWO;
	}
}

void YBFSRcMenuItem::setMouseHoverInPart3Flag( bool bMouseHoverInPart3 ) {
	if ( bMouseHoverInPart3 ) {
		m_uFlag |= YBCMID_FLAG_HOVER_INPARTTHREE;
	} else {
		m_uFlag &= ~YBCMID_FLAG_HOVER_INPARTTHREE;
	}
}
// DISABLE
void YBFSRcMenuItem::setDisableInPart1Flag( bool bDisableInPart1 ) {
	if ( bDisableInPart1 ) {
		m_uFlag |= YBCMID_FLAG_DISABLE_INPARTONE;
	} else {
		m_uFlag &= ~YBCMID_FLAG_DISABLE_INPARTONE;
	}
}

void YBFSRcMenuItem::setDisableInPart2Flag( bool bDisableInPart2 ) {
	if ( bDisableInPart2 ) {
		m_uFlag |= YBCMID_FLAG_DISABLE_INPARTTWO;
	} else {
		m_uFlag &= ~YBCMID_FLAG_DISABLE_INPARTTWO;
	}
}

void YBFSRcMenuItem::setDisableInPart3Flag( bool bDisableInPart3 ) {
	if ( bDisableInPart3 ) {
		m_uFlag |= YBCMID_FLAG_DISABLE_INPARTTHREE;
	} else {
		m_uFlag &= ~YBCMID_FLAG_DISABLE_INPARTTHREE;
	}
}

// CLICK
bool YBFSRcMenuItem::CanClickFlagInPart1() {
	return ( ( m_uFlag & YBCMID_FLAG_CANCLICK_INPARTONE ) == YBCMID_FLAG_CANCLICK_INPARTONE );
}

bool YBFSRcMenuItem::CanClickFlagInPart2() {
	return ( ( m_uFlag & YBCMID_FLAG_CANCLICK_INPATRTWO ) == YBCMID_FLAG_CANCLICK_INPATRTWO );
}

bool YBFSRcMenuItem::CanClickFlagInPart3() {
	return ( ( m_uFlag & YBCMID_FLAG_CANCLICK_INPATRTHREE ) == YBCMID_FLAG_CANCLICK_INPATRTHREE );
}
// Disable
bool YBFSRcMenuItem::isDividePara1Disable() {
	return ( ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTONE ) == YBCMID_FLAG_DISABLE_INPARTONE );
}

bool YBFSRcMenuItem::isDividePara2Disable() {
	return ( ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTWO ) == YBCMID_FLAG_DISABLE_INPARTTWO );
}

bool YBFSRcMenuItem::isDividePara3Disable() {
	return ( ( m_uFlag & YBCMID_FLAG_DISABLE_INPARTTHREE ) == YBCMID_FLAG_DISABLE_INPARTTHREE );
}

void YBFSRcMenuItem::setCanClickInPart1Flag( bool bCanClickInPart1 ) {
	if ( bCanClickInPart1 ) {
		m_uFlag |= YBCMID_FLAG_CANCLICK_INPARTONE;
	} else {
		m_uFlag &= ~YBCMID_FLAG_CANCLICK_INPARTONE;
	}
}

void YBFSRcMenuItem::setCanClickInPart2Flag( bool bCanClickInPart2 ) {
	if ( bCanClickInPart2 ) {
		m_uFlag |= YBCMID_FLAG_CANCLICK_INPATRTWO;
	} else {
		m_uFlag &= ~YBCMID_FLAG_CANCLICK_INPATRTWO;
	}
}

void YBFSRcMenuItem::setCanClickInPart3Flag( bool bCanClickInPart3 ) {
	if ( bCanClickInPart3 ) {
		m_uFlag |= YBCMID_FLAG_CANCLICK_INPATRTHREE;
	} else {
		m_uFlag &= ~YBCMID_FLAG_CANCLICK_INPATRTHREE;
	}
}

bool YBFSRcMenuItem::isDivide2PartCenter() {
	return true;
}