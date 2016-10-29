#include "stdafx.h"
#include "YBCornerNotifyCenter.h"
#include <assert.h>

static void gfnCNDDestoryProc4YBCNC( YBCornerNotifyDlg* pCNDlg, void* pParam );

//////////////////////////////////////////////////////////////////////////
YBCNMsg::YBCNMsg()
: m_tstrTypeName(_T(""))
, m_tstrContentText(_T(""))
, m_uClickEvtFlag( YBCNMSG_CLICKEVTFLAG_NONE )
, m_tstrClickEvtParam( _T("") )
, m_uExitTimeMS( 2000 )
, m_pfnWndLBtnDownProc( NULL )
, m_tstrTime(_T(""))
, m_nType( 1 )
{

}

YBCNMsg::~YBCNMsg(){

}

//////////////////////////////////////////////////////////////////////////

YBCornerNotifyCenter::YBCornerNotifyCenter()
: m_nIndexMax( -1 )
, m_uWidthCNDlg( 205 )
, m_uHeightCNDlg( 183 )
, m_uBottomSpace( 26 )
, m_uRightSpace( 18 ) {

}

YBCornerNotifyCenter::~YBCornerNotifyCenter(){
	_clearAll();
}

//int YBCornerNotifyCenter::onUpdateEvent(int iEventId, void* param) {
//	switch ( iEventId ) {
//	case PNI_USER_LOGIN_IN:
//		{
//			int a = 0;
//		}
//		break;
//	case PNI_USER_LOGIN_OUT:
//		{
//			int a = 0;
//		}
//		break;
//	default:
//		break;
//	}
//
//	return TRUE;
//}

void YBCornerNotifyCenter::pushback( const YBCNMsg* pYBCNMsg ){
	if( !pYBCNMsg ){
		assert(false);
		return;
	}
	YBCornerNotifyDlg* pCNDlg = NULL;
	unsigned int uIndexCornerNotifyDlg;
	RECT rcCND;

	//
	uIndexCornerNotifyDlg = _allocalNDIndex();
	//
	pCNDlg = new YBCornerNotifyDlg( pYBCNMsg->m_nType, pYBCNMsg->m_tstrTime, uIndexCornerNotifyDlg, pYBCNMsg->m_tstrTypeName, pYBCNMsg->m_tstrContentText, gfnCNDDestoryProc4YBCNC, this, pYBCNMsg->m_pfnWndLBtnDownProc, pYBCNMsg->m_tstrClickEvtParam, pYBCNMsg->m_uExitTimeMS );
	//
	if( !_calcNotityDlgWndRect( uIndexCornerNotifyDlg, rcCND ) ) {
		delete pCNDlg;
		pCNDlg = NULL;
		return;
	}
	pCNDlg->create( rcCND.left, rcCND.top, (rcCND.right - rcCND.left), (rcCND.bottom - rcCND.top), NULL, NULL );
	::ShowWindow( pCNDlg->getHWND(), SW_NORMAL );
	//
	m_containerCornerNotifyDlg.push_back( pCNDlg );
	m_nIndexMax = uIndexCornerNotifyDlg;
}

void YBCornerNotifyCenter::eraseCNDlg( YBCornerNotifyDlg* pCNDlg ) {
	if ( !pCNDlg ) {
		assert( false );
		return;
	}
	cndlg_container_type::iterator itCNDlg, iendCNDlg;
	int nIndexMax;
	YBCornerNotifyDlg* pCNDlgTmp = NULL;

	iendCNDlg = m_containerCornerNotifyDlg.end();
	for ( itCNDlg = m_containerCornerNotifyDlg.begin(); itCNDlg!=iendCNDlg; ++itCNDlg ) {
		pCNDlgTmp = *itCNDlg;
		if ( pCNDlgTmp == pCNDlg ) {
			delete pCNDlgTmp;
			m_containerCornerNotifyDlg.erase( itCNDlg );
			break;
		}
	}
	// find the m_nIndexMax
	nIndexMax = -1;
	iendCNDlg = m_containerCornerNotifyDlg.end();
	for ( itCNDlg = m_containerCornerNotifyDlg.begin(); itCNDlg!=iendCNDlg; ++itCNDlg ) {
		pCNDlgTmp = (*itCNDlg);
		nIndexMax = max( nIndexMax, pCNDlgTmp->getCNDlgIndex() );
	}
	m_nIndexMax = nIndexMax;

}

void YBCornerNotifyCenter::_clearAll(){
	cndlg_container_type::iterator itCN, iendCN;
	YBCornerNotifyDlg* pYBCNDlg = NULL;

	iendCN = m_containerCornerNotifyDlg.end();
	for ( itCN = m_containerCornerNotifyDlg.begin(); itCN != iendCN; ++itCN ) {
		pYBCNDlg = (*itCN);
		delete pYBCNDlg;
		pYBCNDlg = NULL;
	}
	m_containerCornerNotifyDlg.clear();
}

bool YBCornerNotifyCenter::_calcNotityDlgWndRect( unsigned int uIndex, RECT& rcSWlocation ) {
	if ( m_uHeightCNDlg == 0
		|| m_uWidthCNDlg == 0 ) {
			assert( false );
			return false;
	}
	RECT rcWorkArea;
	int nWndCountVMax;
	int nWndCountHMax;
	int nRow;
	int nLine;

	if ( !::SystemParametersInfo( SPI_GETWORKAREA, NULL, &rcWorkArea, NULL ) )
		return false;
	if ( rcWorkArea.top > (int)( rcWorkArea.bottom - m_uBottomSpace ) ) {
		rcWorkArea.bottom = rcWorkArea.top + m_uHeightCNDlg;
	} else {
		rcWorkArea.bottom -= m_uBottomSpace;
	}
	if ( rcWorkArea.left > (int)( rcWorkArea.right - m_uRightSpace ) ) {
		rcWorkArea.right = rcWorkArea.left + m_uRightSpace;
	} else {
		rcWorkArea.right -= m_uRightSpace;
	}
	
	nWndCountVMax  = ( rcWorkArea.bottom - rcWorkArea.top )/m_uHeightCNDlg;
	nWndCountHMax = ( rcWorkArea.right - rcWorkArea.left )/m_uWidthCNDlg;
	if ( nWndCountVMax == 0 || nWndCountHMax == 0 ) {
		::SetRect( &rcSWlocation, 0, 0, (rcWorkArea.right - rcWorkArea.left), ( rcWorkArea.bottom - rcWorkArea.top ) );
		return true;
	}
	//
	if ( nWndCountVMax == 0 ) {
		assert( false );
		return false;
	}
	nRow = ( uIndex / nWndCountVMax ) % nWndCountHMax;
	nLine = uIndex % nWndCountVMax;
	rcSWlocation.left = rcWorkArea.right - (nRow+1)*m_uWidthCNDlg;
	rcSWlocation.top = rcWorkArea.bottom - (nLine+1)*m_uHeightCNDlg;
	rcSWlocation.right = rcSWlocation.left + m_uWidthCNDlg;
	rcSWlocation.bottom = rcSWlocation.top + m_uHeightCNDlg;
	return true;
}

unsigned int YBCornerNotifyCenter::_allocalNDIndex() const {
	return ++m_nIndexMax;
}

void gfnCNDDestoryProc4YBCNC( YBCornerNotifyDlg* pCNDlg, void* pParam ) {
	YBCornerNotifyCenter* pCNCenter = (YBCornerNotifyCenter*)pParam;

	pCNCenter->eraseCNDlg( pCNDlg );
}
