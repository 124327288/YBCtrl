#include "stdafx.h"
#include "frame\YBCtrlImageGif.h"
#include "gdiplus.h"
#include "taskEngine/YBTaskEngine.h"
#include "frame/YBCtrlMemDC.h"
#include <assert.h>
#include <atlconv.h>

USING_NAMESPACE_YBCTRL
//
YBITTaskEngine* YBCtrlImageGif::m_pEngine4DrawGif = new YBITTaskEngine();

YBCtrlImageGif::YBCtrlImageGif( const tstring_type& tstrGifFilePath )
: m_pImage ( NULL )
, m_pTask4Gif( NULL )
, m_uWidth( 0 )
, m_uHeight( 0 ) {
	_init( tstrGifFilePath );
}

YBCtrlImageGif::~YBCtrlImageGif() {
	_uninit();
}

void YBCtrlImageGif::_init( const tstring_type& tstrGifFilePath ) {
	Gdiplus::RectF rcDest;
	YBITTaskEngine::task_id_type idTask;
	USES_CONVERSION;

#ifdef _UNICODE
	m_pImage = new Gdiplus::Image( tstrGifFilePath.c_str() );
#else
	m_pImage = new Gdiplus::Image( T2W( tstrGifFilePath.c_str() ) );
#endif //_UNICODE
	if ( m_pImage ) {
		m_uWidth = m_pImage->GetWidth();
		m_uHeight = m_pImage->GetHeight();
		assert( ( m_uWidth != 0 ) && ( m_uHeight != 0 ) );
		//
		m_pTask4Gif = new YBCtrlTask4Gif( m_pImage );
		m_pEngine4DrawGif->pushbackTask( m_pTask4Gif, idTask );
	}
}

void YBCtrlImageGif::_uninit() {
	if ( m_pImage ) {
		delete m_pImage;
		m_pImage = NULL;
	}
}