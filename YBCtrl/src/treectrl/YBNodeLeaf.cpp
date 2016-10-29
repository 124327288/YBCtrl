#include "stdafx.h"
#include "treectrl/YBNodeLeaf.h"
#include "frame/YBCtrlDefs.h"
#include "frame/YBCtrlFontDepository.h"
#include "treectrl/YBNodeFolder.h"
//
#define YBNODELEAF_HEIGHT_DEFAULT (26)
#define YBNODELEAF_WIDTH_DEFAULT (120)
//
USING_NAMESPACE_YBCTRL

YBNodeLeaf::YBNodeLeaf()
: YBNodeBase( ENODE_TYPE_LEAF )
, m_tstrText( _T( "" ) )
, m_uFontIdTextNor( YBCTRL_FONT_ID_INVALID )
, m_uWidth( YBNODELEAF_WIDTH_DEFAULT )
, m_uHeight( YBNODELEAF_HEIGHT_DEFAULT )
{

}

YBNodeLeaf::~YBNodeLeaf() {

}

inline unsigned int YBNodeLeaf::_getHeight() const {
	return m_uHeight;
}

unsigned int YBNodeLeaf::_getWidth() const {
	return m_uWidth;
}

void YBNodeLeaf::_drawStatus( HDC hDC ) const {
	_drawSelf( hDC );
}

void YBNodeLeaf::_drawSelf( HDC hDC ) const {
	Rectangle( hDC, m_rcInContent.left, m_rcInContent.top, m_rcInContent.right, m_rcInContent.bottom );
	_drawText( hDC );
}

void YBNodeLeaf::_calcPosition( unsigned int uXPosStart, unsigned int uYPosStart ) {
	::SetRect( &m_rcInContent, uXPosStart, uYPosStart, uXPosStart + _getWidth(), uYPosStart + _getHeight() );
}

void YBNodeLeaf::_onWndMsg( HWND hWnd, unsigned int uMessage, WPARAM wParam, LPARAM lParam, PARAMONWNDMSG* pParamOnWndMsg ) {
	switch ( uMessage ) {
		case WM_LBUTTONDBLCLK:
		{
			_onEvtLeafNodeDBClk( hWnd, pParamOnWndMsg );
		}
		break;
	default:
		break;
	}
}

void YBNodeLeaf::_onEvtLeafNodeDBClk( HWND hWnd, PARAMONWNDMSG* pParamOnWndMsg ) {
	WPARAM wParam;
	LPARAM lParam;
	WORD wCtrlId;
	HWND hWndScrollWnd = NULL;
	YBNodeBase::node_id_type nodeId;

	nodeId = getId();
	hWndScrollWnd = ::GetParent( hWnd );
	wCtrlId = ::GetDlgCtrlID( hWndScrollWnd );
	wParam = MAKELONG( wCtrlId, YBTREECTRL_CMDEVT_LEAFDBLCLK );
	lParam = nodeId;
	::SendMessage( ::GetParent( hWndScrollWnd ), WM_COMMAND, wParam, lParam );
}

void YBNodeLeaf::_drawText( HDC hDC ) const {
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcNode;

	::SetRect( &rcNode, m_rcInContent.left, m_rcInContent.top, m_rcInContent.right, m_rcInContent.bottom );
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdTextNor );
	if ( pYBCtrlFont ){
		hFontOld = ( HFONT )::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
		//
		::DrawText( hDC, m_tstrText.c_str(), m_tstrText.length(), &rcNode, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS );
		//
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}
