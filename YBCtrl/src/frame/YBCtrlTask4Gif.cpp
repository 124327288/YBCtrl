#include "stdafx.h"
#include "frame/YBCtrlTask4Gif.h"
#include "gdiplus.h"
#include "GdiplusImaging.h"
#include "frame/YBCtrlMemDC.h"
#include <tchar.h>
#include <assert.h>

using namespace Gdiplus;
YBCtrlTask4Gif::YBCtrlTask4Gif(  Gdiplus::Image* pImage )
: m_pImage( pImage )
, m_uFrameCount ( 0 )
, m_uFramePos( 0 )
, m_pPropertyItem( NULL )
, m_pMemDC( NULL ) {
	_init();
}

YBCtrlTask4Gif::~YBCtrlTask4Gif() {
	if ( m_pPropertyItem ) {
		delete [] m_pPropertyItem;
		m_pPropertyItem = NULL;
	}
}

void YBCtrlTask4Gif::_init() {
	unsigned int uDimensinCount = 0;
	GUID* pGuid = NULL;
	TCHAR szGuid[ 64 ] = { 0 };

	uDimensinCount = m_pImage->GetFrameDimensionsCount();
	pGuid = new GUID[ uDimensinCount ];
	if ( !pGuid ) {
		assert( false );
		return;
	}
	m_pImage->GetFrameDimensionsList( pGuid, uDimensinCount );
	StringFromGUID2( pGuid[0], szGuid, 64 );
	m_uFrameCount = m_pImage->GetFrameCount( &pGuid[0] );
	if ( m_uFrameCount == 1 )
		return;
	//
	if ( pGuid ) {
		delete [] pGuid;
		pGuid = NULL;
	}
}

void YBCtrlTask4Gif::Run() {
	unsigned int uWidth = 0;
	unsigned int uHeight = 0;
	unsigned int uPropertyItemSize = 0;
	unsigned int uFrameDalayTime = 0;
	GUID guid = FrameDimensionTime;
	Gdiplus::RectF rcDest;
	TCHAR szMsg[30] = _T("WWWW");
	::OutputDebugString( szMsg );
	uWidth = m_pImage->GetWidth();
	uHeight = m_pImage->GetHeight();
	uPropertyItemSize = m_pImage->GetPropertyItemSize(PropertyTagFrameDelay);
	m_pPropertyItem = new PropertyItem[uPropertyItemSize];
	m_pImage->GetPropertyItem(PropertyTagFrameDelay, uPropertyItemSize, m_pPropertyItem);
	//
	while( 1 ) {
		m_pImage->SelectActiveFrame( &guid, m_uFramePos );
		uFrameDalayTime = ((unsigned int*)m_pPropertyItem[0].value)[ m_uFramePos ]*10;
		if ( uFrameDalayTime == 0 ) {
			uFrameDalayTime = 100;
		}
		++m_uFramePos;
		if ( m_uFramePos == m_uFrameCount ) {
			m_uFramePos = 0;
		}
		//
		m_pMemDC = new YBCtrlMemDC( uWidth, uHeight );
		Gdiplus::Graphics graphics(m_pMemDC->getMemDC());
		rcDest.X = 0;
		rcDest.Y = 0;
		rcDest.Width = (Gdiplus::REAL)uWidth;
		rcDest.Height = (Gdiplus::REAL)uHeight;
		graphics.DrawImage(m_pImage, rcDest, (Gdiplus::REAL)0, (Gdiplus::REAL)0, (Gdiplus::REAL)uWidth, (Gdiplus::REAL)uHeight, Gdiplus::UnitPixel);
		Sleep( 100 );
	}
}