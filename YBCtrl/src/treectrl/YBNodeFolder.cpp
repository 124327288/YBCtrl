#include "stdafx.h"
#include "treectrl/YBNodeFolder.h"
#include "frame/YBCtrlFontDepository.h"
#include "frame/YBCtrlDefs.h"
#include <assert.h>
#include <tchar.h>
//
#define YBNF_FLAG_NONE (0x00000000)
#define YBNF_FLAG_FOLD (0x00000001)
//
#define YBNODEFOLDER_HEIGHT_DEFAULT (26)
#define YBNODEFOLDER_WIDTH_DEFAULT (120)
#define YBNODEFOLDER_INDENT_DEFAULT (20)
//
USING_NAMESPACE_YBCTRL
//
YBNodeFolder::YBNodeFolder()
: YBNodeBase( ENODE_TYPE_FOLDER )
, m_uFlag( YBNF_FLAG_NONE )
, m_tstrText( _T( "" ) )
, m_uFontIdTextNor( YBCTRL_FONT_ID_INVALID )
, m_uWidth( YBNODEFOLDER_WIDTH_DEFAULT )
, m_uHeight( YBNODEFOLDER_HEIGHT_DEFAULT )
, m_uIndentSpace( YBNODEFOLDER_INDENT_DEFAULT )
{

}

YBNodeFolder::~YBNodeFolder() {
	this->clearAll();
}

bool YBNodeFolder::insertSubNodeByIndex( unsigned int uIndex, YBNodeBase* pNodeNew ) {
	if ( uIndex > m_containerNodes.size() || !pNodeNew )
		return false;
	node_container_type::iterator itNode, iendNode;

	iendNode = m_containerNodes.end();
	for ( itNode = m_containerNodes.begin(); itNode != iendNode; ++itNode ) {
		if ( uIndex == 0 ) {
			m_containerNodes.insert( itNode, pNodeNew );
			return true;
		}
		--uIndex;
	}
	assert( m_containerNodes.size() == 0 );
	m_containerNodes.push_back( pNodeNew );

	return true;
}
void YBNodeFolder::enumSubNodes( pfnEnumSubNodeProcType pfnEnumSubNodeProc, void* pParam ) const {
	bool bContinue;

	bContinue = true;
	for ( auto pNode : m_containerNodes ) {
		pfnEnumSubNodeProc( pNode, pParam, &bContinue );
		if ( !bContinue )
			break;
	}
}

YBNodeBase* YBNodeFolder::getSubNode( unsigned int uNodeIdDest ) const {
	for ( auto pNode : m_containerNodes ) {
		if ( pNode->getId() == uNodeIdDest ) {
			return pNode;
		}
	}
	return NULL;
}

YBNodeBase* YBNodeFolder::getDescendantNode( unsigned int uNodeIdDest ) const {
	YBNodeFolder* pSubFolderNode = NULL;
	YBNodeBase* pNodeDest = NULL;

	for ( auto pNode : m_containerNodes ) {
		if ( pNode->getId() == uNodeIdDest ) {
			return pNode;
		}
		if ( pNode->getType() != ENODE_TYPE_FOLDER )
			continue;
		pSubFolderNode = dynamic_cast< YBNodeFolder* >( pNode );
		if ( !pSubFolderNode ) {
			assert( false );
			continue;
		}
		pNodeDest = pSubFolderNode->getDescendantNode( uNodeIdDest );
		if ( pNodeDest ) {
			return pNodeDest;
		}
	}
	return NULL;
}

bool YBNodeFolder::isFold() const {
	return ( m_uFlag & YBNF_FLAG_FOLD ) == YBNF_FLAG_FOLD;
}

void YBNodeFolder::setFold( unsigned int bFold ) {
	if ( bFold ) {
		m_uFlag |= YBNF_FLAG_FOLD;
	} else {
		m_uFlag &= ~YBNF_FLAG_FOLD;
	}
}

unsigned int YBNodeFolder::_getHeight() const {
	return m_uHeight;
}

unsigned int YBNodeFolder::_getWidth() const {
	return m_uWidth;
}

unsigned int YBNodeFolder::_getIndentSpace() const {
	return m_uIndentSpace;
}

void YBNodeFolder::_drawStatus( HDC hDC ) const {
	_drawSelf( hDC );
	if ( !isFold() ) {
		_drawSubNodes( hDC );
	}
}

void YBNodeFolder::_drawSelf( HDC hDC ) const {
	::Rectangle( hDC, m_rcInContent.left, m_rcInContent.top, m_rcInContent.right, m_rcInContent.bottom );
	_drawText( hDC );
}

void YBNodeFolder::_drawSubNodes( HDC hDC ) const {
	for ( auto pSubNode : m_containerNodes )
	{
		pSubNode->drawStatus( hDC );
	}
}

void YBNodeFolder::_drawText( HDC hDC ) const {
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

unsigned int YBNodeFolder::getAllSubNodeHeight() const {
	if ( isFold() )
		return 0;
	unsigned int uTotalHeight;
	YBNodeFolder* pSubNodeFolder = NULL;

	uTotalHeight = 0;
	for ( auto pNode : m_containerNodes ) {
		uTotalHeight += pNode->getHeight();
		if ( pNode->getType() != YBNodeBase::ENODE_TYPE_FOLDER )
			continue;
		pSubNodeFolder = dynamic_cast< YBNodeFolder* >( pNode );
		if ( !pSubNodeFolder ) {
			assert( false );
			continue;
		}
		uTotalHeight += pSubNodeFolder->getAllSubNodeHeight();
	}

	return uTotalHeight;
}

unsigned int YBNodeFolder::getWidthIncludeAllSubNode() const {
	unsigned int uNodeWidth;
	unsigned int uIndentWidth;
	YBNodeFolder* pSubNodeFolder = NULL;
	unsigned int uMaxWidth;
	
	uNodeWidth = _getWidth();
	if ( isFold() )
		return uNodeWidth;
	uIndentWidth = _getIndentSpace();
	uMaxWidth = uNodeWidth;
	for ( auto pSubNode : m_containerNodes ) {
		if ( YBNodeBase::ENODE_TYPE_FOLDER != pSubNode->getType() ) {
			uMaxWidth = max( uIndentWidth + pSubNode->getWidth(), uMaxWidth );
			continue;
		} 
		pSubNodeFolder = dynamic_cast< YBNodeFolder* >( pSubNode );
		if ( !pSubNodeFolder ) {
			assert( false );
			continue;
		}
		uMaxWidth = max( uIndentWidth + pSubNodeFolder->getWidthIncludeAllSubNode(), uMaxWidth );
	}

	return uMaxWidth;
}

void YBNodeFolder::_calcPosition( unsigned int uXPosStart, unsigned int uYPosStart ) {
	_calcSelfPos( uXPosStart, uYPosStart );
	_calcSubNodesPos( uXPosStart + _getIndentSpace(), uYPosStart + _getHeight() );
}

void YBNodeFolder::_calcSelfPos( unsigned int uXPosStart, unsigned int uYPosStart ) {
	::SetRect( &m_rcInContent, uXPosStart, uYPosStart, uXPosStart + _getWidth(), uYPosStart + _getHeight() );
}

void YBNodeFolder::_calcSubNodesPos( unsigned int uXPosStart, unsigned int uYPosStart ) {
	if ( isFold() )
		return;
	YBNodeFolder* pSubNodeFolder = NULL;

	for ( auto pSubNode : m_containerNodes ) {
		//
		pSubNode->_calcPosition( uXPosStart, uYPosStart );
		//
		uYPosStart += pSubNode->getHeight();
		if ( pSubNode->getType() == YBNodeBase::ENODE_TYPE_FOLDER ) {
			pSubNodeFolder = dynamic_cast< YBNodeFolder* >( pSubNode );
			if ( !pSubNodeFolder )
				continue;
			uYPosStart += pSubNodeFolder->getAllSubNodeHeight();
		}
	}

}

bool YBNodeFolder::MoveTest( const POINT& ptInContent, node_id_type& uNodeId ) {
	return HitTest( ptInContent, uNodeId );
}

bool YBNodeFolder::HitTest( const POINT& ptInContent, node_id_type& uNodeId ) {
	YBNodeFolder* pNodeFolder = NULL;
	for ( auto pNode : m_containerNodes ) {
		if ( ::PtInRect( &( pNode->m_rcInContent ), ptInContent ) ) {
			uNodeId = pNode->getId();
			return true;
		}
		if ( pNode->getType() != YBNodeBase::ENODE_TYPE_FOLDER ) {
			if ( ::PtInRect( &( pNode->m_rcInContent ), ptInContent ) ) {
				uNodeId = pNode->getId();
				return true;
			}
			continue;
		} 
		pNodeFolder = dynamic_cast< YBNodeFolder* > ( pNode );
		if ( !pNodeFolder ) {
			assert( false );
			continue;
		}
		if ( pNodeFolder->isFold() )
			continue;
		if ( pNodeFolder->HitTest( ptInContent, uNodeId ) ) {
			return true;
		}
	}
	return false;
}

void YBNodeFolder::_onEvtFolderNodeClick( HWND hWnd, PARAMONWNDMSG* pParamOnWndMsg ) {
	if ( isFold() ) {
		setFold( false );
	} else {
		setFold( true );
	}
	//
	pParamOnWndMsg->m_bNeedReCalcPos = true;
}

void YBNodeFolder::_onWndMsg( HWND hWnd, unsigned int uMessage, WPARAM wParam, LPARAM lParam, PARAMONWNDMSG* pParamOnWndMsg ) {
	switch ( uMessage ) {
		case WM_LBUTTONDOWN:
		{
			_onEvtFolderNodeClick( hWnd, pParamOnWndMsg );
		}
		break;
		case WM_LBUTTONDBLCLK:
		{
			_onEvtFolderNodeClick( hWnd, pParamOnWndMsg );
		}
		break;
	default:
		break;
	}
}

void YBNodeFolder::clearAll() {
	for ( auto pNode : m_containerNodes ) {
		if ( pNode ) {
			delete pNode;
			pNode = NULL;
		}
	}
	m_containerNodes.clear();
}