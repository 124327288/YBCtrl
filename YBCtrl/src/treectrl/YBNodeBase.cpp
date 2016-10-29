#include "stdafx.h"
#include "treectrl/YBNodeBase.h"

#define YBNODE_ID_BASE (0)
//
USING_NAMESPACE_YBCTRL

YBNodeBase::YBNodeBase( ENUMNODETYPE eNodeType )
: m_uNodeId( _allcoateId() )
, m_eNodeType( eNodeType ) {
	::memset( &m_rcInContent, 0, sizeof(RECT) );
}

YBNodeBase::~YBNodeBase() {

}

YBNodeBase::node_id_type YBNodeBase::_allcoateId() const {
	static node_id_type s_idBase = YBNODE_ID_BASE;
	return s_idBase++;
}

void YBNodeBase::drawStatus( HDC hDC ) const {
	_drawStatus( hDC );
}

void YBNodeBase::onWndMsg( HWND hWnd, unsigned int uMessage, WPARAM wParam, LPARAM lParam, PARAMONWNDMSG* pParamOnWndMsg ) {
	_onWndMsg( hWnd, uMessage, wParam, lParam, pParamOnWndMsg );
}