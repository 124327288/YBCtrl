#include "stdafx.h"
#include "treectrl/YBTreeCtrl.h"
#include "treectrl/YBNodeRoot.h"
#include "treectrl/YBTCViewContent.h"
#include "scrollwnd/YBScrollWnd.h"
#include "frame/YBTSHelper.h"
#include <assert.h>

#define YBTREECTRL_NODE_ID_BASE (0)

USING_NAMESPACE_YBCTRL

YBTreeCtrl::YBTreeCtrl( unsigned int uScrollBarThick, unsigned int uStepMove )
: m_pNodeRoot( NULL )
, m_pScrollWnd( NULL )
, m_pTSHelper( new YBTSHelper() )
{
	_init( uScrollBarThick, uStepMove );
}

YBTreeCtrl::~YBTreeCtrl() {
	_unInit();
}

void YBTreeCtrl::_init( unsigned int uScrollBarThick, unsigned int uStepMove ) {
	m_pNodeRoot = new YBNodeRoot();
	m_pScrollWnd = new YBScrollWnd( new YBTCView( new YBTCViewContent( m_pNodeRoot ) ), uScrollBarThick, uStepMove );
}

void YBTreeCtrl::_unInit() {
	//
	if ( m_pScrollWnd )
		delete m_pScrollWnd;
	m_pScrollWnd = NULL;
	//
	if ( m_pNodeRoot )
		delete m_pNodeRoot;
	m_pNodeRoot = NULL;
}

bool YBTreeCtrl::insertNode( YBNodeFolder* pNodeParent, unsigned int uSubNodeIndex, YBNodeBase* pNodeNew, bool bCalcPos ) {
	if ( !pNodeNew ) {
		assert( false );
		return false;
	}
	if ( !pNodeParent ) {
		pNodeParent = m_pNodeRoot;
	}
	assert( pNodeParent );
	//
	if ( !pNodeParent->insertSubNodeByIndex( uSubNodeIndex, pNodeNew ) ) {
		assert( false );
		return false;
	}
	//
	if ( bCalcPos ) {
		m_pNodeRoot->calcAllSubNodePos();
	}

	return true;
}

void YBTreeCtrl::calcAllNodesPos() {
	if ( !m_pNodeRoot )
		return;
	m_pNodeRoot->calcAllSubNodePos();
}

HWND YBTreeCtrl::create( int x, int y, int nWidth, int nHeight, HWND hWndParent, WORD wCtrlID, HINSTANCE hInstance ) {
	return m_pScrollWnd->create( x, y, nWidth, nHeight, hWndParent, wCtrlID, hInstance );
}

void YBTreeCtrl::updateStatus() {
	return m_pScrollWnd->updateStatus();
}

void YBTreeCtrl::clearAll() {
	m_pNodeRoot->clearAll();
	m_pScrollWnd->updateStatus();
}





