#include "stdafx.h"
#include "treectrl/YBTCViewContent.h"
#include "treectrl/YBNodeRoot.h"
#include "treectrl/YBNodeFolder.h"
#include "treectrl/YBNodeLeaf.h"
#include "frame/YBCtrlMemDC.h"
#include "scrollwnd/YBScrollWnd.h"
#include <assert.h>
//
USING_NAMESPACE_YBCTRL
//
YBTCView::YBTCView( YBTCViewContent* pTCVC )
: YBViewInner( pTCVC ) {
	
}

YBTCView::~YBTCView() {

}

void YBTCView::onWndMsg( YBCtrlWndMsg* pYBCtrlWndMsg ) {
		if ( !pYBCtrlWndMsg ) {
				return;
		}
		switch ( pYBCtrlWndMsg->m_uMsg ) {
		case WM_NULL: {} break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
 		case WM_MOUSEMOVE:
 		case WM_MOUSELEAVE:
		{
			YBTCViewContent* pView4TCContent = NULL;
			YBNodeRoot* pNodeRoot = NULL;
			YBNodeBase* pNodeDest = NULL;
			YBNodeBase::node_id_type uNodeId;
			YBNodeBase::PARAMONWNDMSG paramOnWndMsg;
			TRACKMOUSEEVENT trackMouseEvt;
			RECT rcClient;
			POINT ptCurPos;
			POINT ptInV;
			POINT ptInC;

			::GetCursorPos( &ptCurPos );
			ptInV = ptCurPos;
			::ScreenToClient( m_hWnd, &ptInV );
			//
			if ( !PointView2Content( ptInV, ptInC ) )
				return;
			pView4TCContent = ( YBTCViewContent* )getViewContentObj();
			pNodeRoot = pView4TCContent->m_pNodeRoot;
			if ( !pNodeRoot )
				return;
			// not In Client.
			::GetClientRect( m_hWnd, &rcClient );
			if ( !::PtInRect( &rcClient, ptInV ) ) {
				trackMouseEvt.cbSize = sizeof( TRACKMOUSEEVENT );
				trackMouseEvt.dwFlags = TME_LEAVE|TME_CANCEL;
				trackMouseEvt.hwndTrack = pYBCtrlWndMsg->m_hWnd;
				trackMouseEvt.dwHoverTime = HOVER_DEFAULT;
				::TrackMouseEvent( &trackMouseEvt );
				for ( auto pNode : pNodeRoot->m_containerNodes ) {
					pNode->onWndMsg( pYBCtrlWndMsg->m_hWnd, WM_MOUSELEAVE, 0, 0, &paramOnWndMsg );
					return;
				}
			}
			//
			if ( ::PtInRect( &rcClient, ptInV ) ) {
				trackMouseEvt.cbSize = sizeof( TRACKMOUSEEVENT );
				trackMouseEvt.dwFlags = TME_LEAVE;
				trackMouseEvt.hwndTrack = pYBCtrlWndMsg->m_hWnd;
				trackMouseEvt.dwHoverTime = HOVER_DEFAULT;
				::TrackMouseEvent( &trackMouseEvt );
			}
			// 
			if ( !pNodeRoot->MoveTest( ptInC, uNodeId ) ) {
				for ( auto pNode : pNodeRoot->m_containerNodes ) {
					pNode->onWndMsg( pYBCtrlWndMsg->m_hWnd, WM_MOUSELEAVE, 0, 0, &paramOnWndMsg );
					return;
				}
			}
			pNodeDest = pNodeRoot->getDescendantNode( uNodeId );
			if ( !pNodeDest )
				return;
			pNodeDest->onWndMsg( pYBCtrlWndMsg->m_hWnd, pYBCtrlWndMsg->m_uMsg, pYBCtrlWndMsg->m_wParam, pYBCtrlWndMsg->m_lParam, &paramOnWndMsg );
			if ( paramOnWndMsg.m_bNeedReCalcPos ) {
				pNodeRoot->calcAllSubNodePos();
				//
				::InvalidateRect( m_hWnd, NULL, TRUE );
				if ( m_pScrollWnd )
					m_pScrollWnd->updateStatus();
			}
		}
		default:
			break;
		}
		if ( pYBCtrlWndMsg->m_dwFlag & YBCTRLWNDMSG_FLAG_EATMSG ) {
				return;
		}
		YBViewInner::onWndMsg( pYBCtrlWndMsg );
}

////////////////////////////////////////////////////////////////////////////
YBTCViewContent::YBTCViewContent( YBNodeRoot* pNodeRoot )
: m_pNodeRoot( pNodeRoot )
, m_pMemDC( new YBCtrlMemDC( 100, 100 ) )
{

}

YBTCViewContent::~YBTCViewContent() {
	if ( m_pMemDC ) {
		delete m_pMemDC;
		m_pMemDC = NULL;
	}
}

unsigned int YBTCViewContent::getWidth() const {
	return m_pNodeRoot->getWidthIncludeAllSubNode();
}
	
unsigned int YBTCViewContent::getHeight() const {
	return m_pNodeRoot->getAllSubNodeHeight();
}

void YBTCViewContent::_drawStatus( HDC hDC, YBViewInner* pYBViewInner ) const {
	RECT rcInContent;
	RECT rcInView;

	::SetRect( &rcInContent, 0, 0, getWidth(), getHeight() );

	if ( !pYBViewInner->getRectContent2View( rcInContent, rcInView ) )
		return;
	//
	m_pMemDC->resize( getWidth(), getHeight() );
	//
	_drawBk( m_pMemDC->getMemDC(), rcInContent );
	m_pNodeRoot->drawStatus( m_pMemDC->getMemDC() );
	//
	::BitBlt( hDC, rcInView.left, rcInView.top, ( rcInView.right - rcInView.left ), ( rcInView.bottom - rcInView.top ),
		m_pMemDC->getMemDC(), rcInContent.left, rcInContent.top, SRCCOPY );
}

void YBTCViewContent::_drawBk( HDC hDC, const RECT& rcInContent ) const {
	HBRUSH hBrushBK;

	hBrushBK = ::CreateSolidBrush( RGB( 255, 255, 255 ) );
	::FillRect( hDC, &rcInContent, hBrushBK );
	::DeleteObject( hBrushBK );
}