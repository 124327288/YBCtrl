#include "stdafx.h"
#include "YBFileTPDlgMgr4IT.h"
//
#define YBFTPDM4IT_FLAG_NONE (0x00000000)
//#define YBFTPDM4IT_FLAG_DOWNLOAD_VISIBLE (0x00000001)
//#define YBFTPDM4IT_FLAG_UPLOAD_VISIBLE (0x00000002)
//
YBFileTPDlgMgr4IT::YBFileTPDlgMgr4IT()
: m_dlgFileTPDlg4Download( YBFileTPDlg::EFILETP_DOWNLOAD )
, m_dlgFileTPDlg4Upload( YBFileTPDlg::EFILETP_UPLOAD )
, m_uFlag( YBFTPDM4IT_FLAG_NONE )
{
	_init();
}

YBFileTPDlgMgr4IT::~YBFileTPDlgMgr4IT() {
	_unInit();
}

YBFileTPDlgMgr4IT* YBFileTPDlgMgr4IT::getInstance() {
	static YBFileTPDlgMgr4IT s_instance;
	return &s_instance;
}

void YBFileTPDlgMgr4IT::_init() {
	m_dlgFileTPDlg4Download.create( 0,0,100,30,NULL,NULL );
	m_dlgFileTPDlg4Upload.create( 0,0,100,300,NULL,NULL );
}

void YBFileTPDlgMgr4IT::_unInit() {
	
}

void YBFileTPDlgMgr4IT::setStatus( YBFileTPDlg::EFILETPTYPE eFileTPType, tstring_type tstrTitle, __int64 i64CurPos, __int64 i64MaxPos ) {
	YBFileTPDlg* pFileTPDlgDest = NULL;

	pFileTPDlgDest = _getFileTPDlg( eFileTPType );
	if ( !pFileTPDlgDest )
		return;
	//
	pFileTPDlgDest->setPos( i64CurPos, i64MaxPos );
	pFileTPDlgDest->setTitleText( tstrTitle );
	//
	_adjustTPDlgPos();
	//
	pFileTPDlgDest->updateStatus();
}

void YBFileTPDlgMgr4IT::onEvtFinish( YBFileTPDlg::EFILETPTYPE eFileTPType, bool bSuccess ) {
	YBFileTPDlg* pFileTPDlgDest = NULL;

	pFileTPDlgDest = _getFileTPDlg( eFileTPType );
	if ( !pFileTPDlgDest ) {
		return;
	}
	pFileTPDlgDest->onFinished( bSuccess );
}

YBFileTPDlg* YBFileTPDlgMgr4IT::_getFileTPDlg( YBFileTPDlg::EFILETPTYPE eFileTPType ) {
	switch ( eFileTPType ) {
	case YBFileTPDlg::EFILETP_DOWNLOAD:
		{
			return &m_dlgFileTPDlg4Download;
		}
		break;
	case YBFileTPDlg::EFILETP_UPLOAD:
		{
			return &m_dlgFileTPDlg4Upload;
		}
		break;
	default:
		assert( false );
		break;
	}

	return NULL;
} 

void YBFileTPDlgMgr4IT::_adjustTPDlgPos() {
	bool bDlgDLVisible = false;
	bool bDlgULVisible = false;

	if ( ::IsWindow( m_dlgFileTPDlg4Download.getHWND() ) ) {
		bDlgDLVisible = ( TRUE == ::IsWindowVisible( m_dlgFileTPDlg4Download.getHWND() ) );
	}
	if ( ::IsWindow( m_dlgFileTPDlg4Upload.getHWND() ) ) {
		bDlgULVisible = ( TRUE == ::IsWindowVisible( m_dlgFileTPDlg4Upload.getHWND() ) );
	}
	//
	_movewindow( bDlgDLVisible, bDlgULVisible );

}

void YBFileTPDlgMgr4IT::_movewindow( bool bDlgDLVisible, bool bDlgULVisible ) {
	RECT rcWorkArea;
	int nBottomSpace = 25;
	unsigned int uDlgHeight = 32;
	unsigned int uDlgWidth = 735;
	unsigned int uDlgTSpace;
	unsigned int uDlgLSpace;
	unsigned int u2DlgWidth = 500;
	unsigned int u2DlgHeight = 32;
	unsigned int u2DlgLSpace;
	unsigned int u2DlgTSpace;
	//
	if ( !::SystemParametersInfo( SPI_GETWORKAREA, NULL, &rcWorkArea, NULL ) )
		return;
	uDlgTSpace = rcWorkArea.bottom - nBottomSpace - uDlgHeight;
	uDlgLSpace = rcWorkArea.left + ( rcWorkArea.right - rcWorkArea.left - uDlgWidth )/2;
	u2DlgTSpace = uDlgTSpace;
	u2DlgLSpace = ( ( rcWorkArea.right - rcWorkArea.left )/2 - u2DlgWidth )/2;

	if ( bDlgDLVisible && bDlgULVisible ) {
		::MoveWindow( m_dlgFileTPDlg4Upload.getHWND(), u2DlgLSpace, u2DlgTSpace, u2DlgWidth, u2DlgHeight, TRUE );
		::MoveWindow( m_dlgFileTPDlg4Download.getHWND(), rcWorkArea.right/2 + u2DlgLSpace, u2DlgTSpace, u2DlgWidth, u2DlgHeight, TRUE );
	} else if ( ( bDlgDLVisible ) && ( !bDlgULVisible ) ) {
		::MoveWindow( m_dlgFileTPDlg4Download.getHWND(), uDlgLSpace, uDlgTSpace, uDlgWidth, uDlgHeight, TRUE );
	} else if ( ( !bDlgDLVisible ) && ( bDlgULVisible ) ) {
		::MoveWindow( m_dlgFileTPDlg4Upload.getHWND(), uDlgLSpace, uDlgTSpace, uDlgWidth, uDlgHeight, TRUE );
	} else {
		return;
	}
}