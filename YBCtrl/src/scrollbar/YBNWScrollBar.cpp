#include "stdafx.h"
#include "scrollbar/YBNWScrollBar.h"
#include "frame/YBCtrlImageBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlMisc.h"
#include <assert.h>

#define YBNWSCROLLBAR_FLAG_NONE (0x00000000)
#define YBNWSCROLLBAR_FLAG_LBTNDOWN (0x00000001)
#define YBNWSCROLLBAR_TIMERID_TARROW (0x00000100)
#define YBNWSCROLLBAR_TIMERID_BARROW (0x00000101)
#define YBNWScrollBar_TIMERID_GAP (0x00000102)

USING_NAMESPACE_YBCTRL

YBNWScrollBar::YBNWScrollBar( ENUMDIRECTIONTYPE eDirection )
: m_uFlag( YBNWSCROLLBAR_FLAG_NONE )
, m_eSArrowStatus( ESARROW_STATUS_PRENORMAL )
, m_eEArrowStatus( EEARROW_STATUS_PRENORMAL )
, m_uSArrowTSpaced( 1 )
, m_uEArrowTSpaced( 1 )
, m_dDisToTopPage( 100 )
, m_dFullPage( 100 )
, m_uSteps( 2 )
, m_eDirection( eDirection )
{

}

YBNWScrollBar::~YBNWScrollBar(){

}

YBCtrlImageStatic* YBNWScrollBar::_getImageStatic( unsigned int uImageId ){
	YBCtrlImageBase* pImageBase = NULL;
	YBCtrlImageStatic* pImageStatic = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pImageBase ) )
		return NULL;
	if( !pImageBase || ( pImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pImageBase;
}

void YBNWScrollBar::drawBk( HDC hDC ){
	
	YBCtrlImageStatic* pYBImageBk = NULL;
	pYBImageBk = _getImageStatic( m_uImageIdBk );
	if ( !pYBImageBk ) {
		return;
	}
	//
	BackGroundBitBlt( hDC, m_rcBk.left, m_rcBk.top, m_rcBk.right - m_rcBk.left, m_rcBk.bottom - m_rcBk.top, pYBImageBk->getMemDC(),
		0, 0, pYBImageBk->getWidth(), pYBImageBk->getHeight(), 1, 1, 1, 1 );

}

void YBNWScrollBar::drawSArrow( HDC hDC ){
	unsigned int uImageIdArrow;
	unsigned int uOriginL;
	unsigned int uBkWidth;
	YBCtrlImageStatic* pYBImage = NULL;

	if( !_getImageIdSArrow( m_eSArrowStatus, uImageIdArrow ) ){
		return;
	}
	pYBImage = _getImageStatic( uImageIdArrow );
	if ( !pYBImage ) {
		return;
	}

	m_uArrowWidth = pYBImage->getWidth();
	m_uArrowHeight = pYBImage->getHeight();
	uBkWidth = m_rcBk.right - m_rcBk.left;
	uOriginL = ( uBkWidth - m_uArrowWidth )/2;
	// Arrow Rect
	m_rcSArrow.left = uOriginL + m_rcBk.left;
	m_rcSArrow.top = m_uSArrowTSpaced + m_rcBk.top;
	m_rcSArrow.right = m_rcSArrow.left + m_uArrowWidth;
	m_rcSArrow.bottom = m_rcSArrow.top + m_uArrowHeight;
	//
	BackGroundBitBlt( hDC, m_rcSArrow.left, m_rcSArrow.top, m_uArrowWidth, m_uArrowHeight, pYBImage->getMemDC(),
		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0 );
}

void YBNWScrollBar::drawEArrow( HDC hDC ){
	unsigned int uImageIdArrow;
	unsigned int uOriginL;
	unsigned int uBkWidth;
	YBCtrlImageStatic* pYBImage = NULL;

	if( !_getImageIdEArrow( m_eEArrowStatus, uImageIdArrow ) ){
		return;
	}
	pYBImage = _getImageStatic( uImageIdArrow );
	if ( !pYBImage ) {
		return;
	}
	//
	m_uArrowWidth = pYBImage->getWidth();
	m_uArrowHeight = pYBImage->getHeight();
	uBkWidth = m_rcBk.right - m_rcBk.left;
	uOriginL = ( uBkWidth - m_uArrowWidth )/2;
	// Arrow Rect
	m_rcEArrow.left = m_rcBk.left + uOriginL;
	m_rcEArrow.right = m_rcEArrow.left + m_uArrowWidth;
	m_rcEArrow.bottom = m_rcBk.bottom - m_uEArrowTSpaced;
	m_rcEArrow.top = m_rcEArrow.bottom - m_uArrowHeight;
	//
	BackGroundBitBlt( hDC, m_rcEArrow.left, m_rcEArrow.top, m_uArrowWidth, m_uArrowHeight, pYBImage->getMemDC(),
		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0 );
	
}

bool YBNWScrollBar::_getImageIdSArrow( ENUMTARROWSTATUS eArrowStatus, unsigned int& uImageId ){
	switch( eArrowStatus )
	{
	case ESARROW_STATUS_NORMAL:
		{
			uImageId = m_uImageIdSArrNor;
		}
		break;
	case ESARROW_STATUS_HOVER:
		{
			uImageId = m_uImageIdSArrHov;
		}
		break;
	case ESARROW_STATUS_PRESS:
		{
			uImageId = m_uImageIdSArrPre;
		}
		break;
	case ESARROW_STATUS_PRENORMAL:
		{
			uImageId = m_uImageIdSArrPreNor;
		}
		break;
	default:
		{
			assert(false);
			return false;
		}
	}
	return true;
}

void YBNWScrollBar::setImageIdTArrow( ENUMTARROWSTATUS eArrowStatus, unsigned int uImageId ){
	switch( eArrowStatus ){
	case ESARROW_STATUS_NORMAL:
		{
			m_uImageIdSArrNor = uImageId;
		}
		break;
	case ESARROW_STATUS_HOVER:
		{
			m_uImageIdSArrHov = uImageId;
		}
		break;
	case ESARROW_STATUS_PRESS:
		{
			m_uImageIdSArrPre = uImageId;
		}
		break;
	case ESARROW_STATUS_PRENORMAL:
		{
			m_uImageIdSArrPreNor = uImageId;
		}
		break;
	default:
		break;
	}
}

void YBNWScrollBar::onPAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	HDC hDC;
	RECT rcClient;
	PAINTSTRUCT ps;

	::GetClientRect( pYBCtrlWndMsg->m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	drawStatus( memDC.getMemDC() );

	hDC = ::BeginPaint( pYBCtrlWndMsg->m_hWnd, &ps );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::EndPaint( pYBCtrlWndMsg->m_hWnd, &ps );
}

void YBNWScrollBar::onMOUSEWHEEL( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){

}

void YBNWScrollBar::onTIMER( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	switch( pYBCtrlWndMsg->m_wParam ){
	case YBNWSCROLLBAR_TIMERID_TARROW:
		{
			m_dDisToTopPage -= m_uSteps;
			if( m_dDisToTopPage < 0  ){
				m_dDisToTopPage = 0;
			}
		}
		break;
	case YBNWSCROLLBAR_TIMERID_BARROW:
		{
			m_dDisToTopPage += m_uSteps;
		}
		break;
	default:
		break;
	}

}

void YBNWScrollBar::onLBUTTONDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	POINT pt;
	HDC hDC;
	RECT rcClient;
	WPARAM wParam;
	LPARAM lParam;

	::GetClientRect( pYBCtrlWndMsg->m_hWnd, &rcClient );
	pt.x = (__int16)LOWORD( pYBCtrlWndMsg->m_lParam );
	pt.y = HIWORD( pYBCtrlWndMsg->m_lParam );

	if( ::PtInRect( &m_rcBk, pt ) ){
		
		if( ::PtInRect( &m_rcBk, pt ) ){
			if( ::PtInRect( &m_rcSArrow, pt ) ){
				m_uFlag |= YBNWSCROLLBAR_FLAG_LBTNDOWN;
				m_eSArrowStatus = ESARROW_STATUS_PRESS;

				wParam = MAKELONG( m_SArrowCtrlId, YBBTNX_CMDEVT_BTNXDOWN );
				lParam = (LPARAM)_getHWND();
				// set a timer when LButton press always
				//::SetTimer( pYBCtrlWndMsg->m_hWnd, YBNWSCROLLBAR_TIMERID_TARROW, 100, NULL );
				m_dDisToTopPage -= m_uSteps;
				if( m_dDisToTopPage < 0 ){
					m_dDisToTopPage = 0;
				}

				if( ::PtInRect( &m_rcSArrow, pt ) && m_uFlag ){
					::SendMessage( m_hWnd, WM_COMMAND, wParam, lParam ); // add press top Arrow button event;
				}
			}else if( ::PtInRect( &m_rcEArrow, pt ) ){

				m_uFlag |= YBNWSCROLLBAR_FLAG_LBTNDOWN;
				m_eEArrowStatus = EEARROW_STATUS_PRESS;

				wParam = MAKELONG( m_EArrowCtrlId, YBBTNX_CMDEVT_BTNXDOWN );
				lParam = (LPARAM)_getHWND();
				// set a timer when LButton press always
				//::SetTimer( pYBCtrlWndMsg->m_hWnd, YBNWSCROLLBAR_TIMERID_BARROW, 100, NULL );
				m_dDisToTopPage += m_uSteps;
				if( m_dDisToTopPage > m_dFullPage ){
					m_dDisToTopPage = m_dFullPage;
				}

				if( ::PtInRect( &m_rcEArrow, pt ) && m_uFlag ){
					::SendMessage( m_hWnd, WM_COMMAND, wParam, lParam ); // add press bottom Arrow button event;
				}

			}else if( ::PtInRect( &m_rcFT, pt ) ){
				m_uFlag |= YBNWSCROLLBAR_FLAG_LBTNDOWN;
				m_eFtStatus = EFT_STATUS_PRESS;
			}else{
				if( pt.y < m_rcFT.top ){
					m_dDisToTopPage -= m_uSteps;
					if( m_dDisToTopPage < 0 ){
						m_dDisToTopPage = 0;
					}
				}else{
					m_dDisToTopPage += m_uSteps;
					if( m_dDisToTopPage > m_dFullPage ){
						m_dDisToTopPage = m_dFullPage;
					}
				}
			}
		}
	}

	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	drawStatus( memDC.getMemDC() );

	hDC = ::GetDC( pYBCtrlWndMsg->m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( pYBCtrlWndMsg->m_hWnd, hDC );
}

void YBNWScrollBar::onLBUTTONUP( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	POINT ptMouse;
	HDC hDC;
	RECT rcClient;

	::GetClientRect( pYBCtrlWndMsg->m_hWnd, &rcClient );
	ptMouse.x = (__int16)LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = (__int16)HIWORD( pYBCtrlWndMsg->m_lParam );

	m_uFlag &= ~YBNWSCROLLBAR_FLAG_LBTNDOWN;

	if( ::PtInRect( &m_rcSArrow, ptMouse ) ){
		
		m_eSArrowStatus = ESARROW_STATUS_HOVER;
	}
	if( ::PtInRect( &m_rcEArrow, ptMouse ) ){

		m_eEArrowStatus = EEARROW_STATUS_HOVER;
	}
	if( ::PtInRect( &m_rcFT, ptMouse ) ){

		m_eFtStatus = EFT_STATUS_HOVER;
	}

	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	drawStatus( memDC.getMemDC() );

	hDC = ::GetDC( pYBCtrlWndMsg->m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( pYBCtrlWndMsg->m_hWnd, hDC );
	
}

void YBNWScrollBar::onMOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ){
	POINT ptMouse;
	HDC hDC;
	RECT rcClient;
	
	::GetClientRect( pYBCtrlWndMsg->m_hWnd, &rcClient );
	ptMouse.x = (__int16)LOWORD( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = (__int16)HIWORD( pYBCtrlWndMsg->m_lParam );

	if( ::PtInRect( &m_rcBk, ptMouse ) ){
		if( ::PtInRect( &m_rcSArrow, ptMouse ) ){
			if( m_uFlag & YBNWSCROLLBAR_FLAG_LBTNDOWN ){
				m_eSArrowStatus = ESARROW_STATUS_PRESS;
			}else{
				m_uFlag &= ~YBNWSCROLLBAR_FLAG_LBTNDOWN;
				m_eSArrowStatus = ESARROW_STATUS_HOVER;
			}
		}else if( ::PtInRect( &m_rcEArrow, ptMouse ) ){
			if( m_uFlag & YBNWSCROLLBAR_FLAG_LBTNDOWN ){
				m_eEArrowStatus = EEARROW_STATUS_PRESS;
			}else{
				m_uFlag &= ~YBNWSCROLLBAR_FLAG_LBTNDOWN;
				m_eEArrowStatus = EEARROW_STATUS_HOVER;
			}
		}else if( ::PtInRect( &m_rcFT, ptMouse ) ){
			if( m_uFlag & YBNWSCROLLBAR_FLAG_LBTNDOWN ){
				m_eFtStatus = EFT_STATUS_PRESS;
				m_dDisToTopPage = _calcDisToSPage( ptMouse.y - m_rcSArrow.bottom  );
			}else{
				m_uFlag &= ~YBNWSCROLLBAR_FLAG_LBTNDOWN;
				m_eFtStatus = EFT_STATUS_HOVER;
			}
		}else{
			m_uFlag &= ~YBNWSCROLLBAR_FLAG_LBTNDOWN;
			m_eEArrowStatus = EEARROW_STATUS_NORMAL;
			m_eSArrowStatus = ESARROW_STATUS_NORMAL;
			m_eFtStatus = EFT_STATUS_NORMAL;
		}
	}else{
		m_uFlag &= ~YBNWSCROLLBAR_FLAG_LBTNDOWN;
		m_eSArrowStatus = ESARROW_STATUS_PRENORMAL;
		m_eEArrowStatus = EEARROW_STATUS_PRENORMAL;
		m_eFtStatus = EFT_STATUS_NORMAL;
	}

	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	drawStatus( memDC.getMemDC() );

	hDC = ::GetDC( pYBCtrlWndMsg->m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( pYBCtrlWndMsg->m_hWnd, hDC );
}

void YBNWScrollBar::setRectBk( RECT& rcBk ){
	m_rcBk = rcBk;
}

void YBNWScrollBar::setImageIdBk( unsigned int uImageId ){
	m_uImageIdBk = uImageId;
}

void YBNWScrollBar::drawStatus( HDC hDC ){
	drawBk( hDC );
	drawSArrow( hDC );
	drawEArrow( hDC );
	drawFT( hDC );
}

void YBNWScrollBar::setHWND( HWND hWnd ){
	m_hWnd = hWnd;
}

void YBNWScrollBar::setCtrlID( WORD SArrowWord, WORD EArrowWord ){
	m_SArrowCtrlId = SArrowWord;
	m_EArrowCtrlId = EArrowWord;
}

void YBNWScrollBar::getCtrlId( WORD& SArrowWord, WORD& EArrowWord ) const{
	SArrowWord = m_SArrowCtrlId;
	EArrowWord = m_EArrowCtrlId;
}

bool YBNWScrollBar::_getImageIdEArrow( ENUMBARROWSTATUS eArrowStatus, unsigned int& uImageId ){
	switch( eArrowStatus )
	{
	case EEARROW_STATUS_NORMAL:
		{
			uImageId = m_uImageIdEArrNor;
		}
		break;
	case EEARROW_STATUS_HOVER:
		{
			uImageId = m_uImageIdEArrHov;
		}
		break;
	case EEARROW_STATUS_PRESS:
		{
			uImageId = m_uImageIdEArrPre;
		}
		break;
	case EEARROW_STATUS_PRENORMAL:
		{
			uImageId = m_uImageIdEArrPreNor;
		}
		break;
	default:
		{
			assert(false);
			return false;
		}
	}
	return true;
}

void YBNWScrollBar::setImageIdBArrow( ENUMBARROWSTATUS eArrowStatus, unsigned int uImageId ){
	switch( eArrowStatus ){
	case EEARROW_STATUS_NORMAL:
		{
			m_uImageIdEArrNor = uImageId;
		}
		break;
	case EEARROW_STATUS_HOVER:
		{
			m_uImageIdEArrHov = uImageId;
		}
		break;
	case EEARROW_STATUS_PRESS:
		{
			m_uImageIdEArrPre = uImageId;
		}
		break;
	case ESARROW_STATUS_PRENORMAL:
		{
			m_uImageIdEArrPreNor = uImageId;
		}
		break;
	default:
		break;
	}
}

void YBNWScrollBar::drawFT( HDC hDC ){
	unsigned int uImageIdFT;
	YBCtrlImageStatic* pYBImage = NULL;
	double dDisToSArrow;

	if( !_getImageIdFT( m_eFtStatus, uImageIdFT ) ){
		return;
	}
	pYBImage = _getImageStatic( uImageIdFT );
	if ( !pYBImage ) {
		return;
	}
	//
	dDisToSArrow = _calcDisToSArrow( m_rcEArrow.top - m_rcSArrow.bottom - m_uFTHeight );
	m_rcFT.left = m_rcBk.left;
	m_rcFT.right = m_rcBk.right;
	m_rcFT.top = m_rcSArrow.bottom + (LONG)dDisToSArrow + m_uFTHeight/2;
	m_rcFT.bottom = m_rcFT.top + m_uFTHeight;

	BackGroundBitBlt( hDC, m_rcFT.left, m_rcFT.top, m_rcFT.right - m_rcFT.left, m_rcFT.bottom - m_rcFT.top, pYBImage->getMemDC(),
		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0 );

}

void YBNWScrollBar::setImageIdFT( ENUMTHUMBSTATUS eFtStatus, unsigned int uImageId ){
	switch( eFtStatus ){
	case EFT_STATUS_NORMAL:
		{
			m_uImageIdFtNor = uImageId;
		}
		break;
	case EFT_STATUS_HOVER:
		{
			m_uImageIdFtHov = uImageId;
		}
		break;
	case EFT_STATUS_PRESS:
		{
			m_uImageIdFtPre = uImageId;
		}
		break;
	default:
		break;
	}
}

bool YBNWScrollBar::_getImageIdFT( ENUMTHUMBSTATUS eFtStatus, unsigned int& uImageId ){
	switch( eFtStatus ){
	case EFT_STATUS_NORMAL:
		{
			uImageId = m_uImageIdFtNor;
		}
		break;
	case EFT_STATUS_HOVER:
		{
			uImageId = m_uImageIdFtHov;
		}
		break;
	case EFT_STATUS_PRESS:
		{
			uImageId = m_uImageIdFtPre;
		}
		break;
	default:
		{
			assert(false);
			return false;
		}
	}
	return true;
}

void YBNWScrollBar::setFTCurHeight( unsigned int nHeight ){
	m_uFTHeight = nHeight;
}

void YBNWScrollBar::setWidth( double dDisToTopPage, double dFullPage ){
	if( dFullPage < dDisToTopPage ){
		assert(false);
	}
	m_dDisToTopPage = dDisToTopPage;
	m_dFullPage = dFullPage;
}

double YBNWScrollBar::_calcDisToSArrow( double dFullScrollBar ){
	if( m_dFullPage < m_dDisToTopPage ){
		assert(false)	;
	}
	return ( m_dDisToTopPage/m_dFullPage ) * dFullScrollBar;
	
}

void YBNWScrollBar::setLimitBarWidth( unsigned int uMaxBar, unsigned int uMinBar ){
	m_uMinBar = uMinBar;
	m_uMaxBar = uMaxBar;
}

double YBNWScrollBar::_calcDisToSPage( double dDisToSArrow ){
	if( dDisToSArrow > ( m_rcEArrow.top - m_rcSArrow.bottom ) ){
		assert(false);
	}
	return dDisToSArrow * m_dFullPage/( m_rcEArrow.top - m_rcSArrow.bottom );
}