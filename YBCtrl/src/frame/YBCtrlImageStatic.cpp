#include "stdafx.h"
#include "frame\YBCtrlImageStatic.h"
#include "frame\YBCtrlMemDC.h"
#include "frame\FuncPack.h"
#include "gdiplus.h"
#include <assert.h>
#include <atlconv.h>

USING_NAMESPACE_YBCTRL
#define YBCTRLIMAGESTATIC_FLAG_NONE (0x00000000)
#define YBCTRLIMAGESTATIC_FLAG_READY (0x0000001)

YBCtrlImageStatic::YBCtrlImageStatic( const tstring_type& tstrImageFilePath )
: m_pImage( NULL )
, m_pMemDC( NULL )
, m_uWidth( 0 )
, m_uHeight( 0 ) {	
	_init( tstrImageFilePath );
}

YBCtrlImageStatic::YBCtrlImageStatic( HMODULE hInst, UINT nResourceID, LPCTSTR lpszResourceType )
: m_pImage( NULL )
, m_pMemDC( NULL )
, m_uWidth( 0 )
, m_uHeight( 0 ) {
	_init( hInst, nResourceID, lpszResourceType );
}

YBCtrlImageStatic::~YBCtrlImageStatic() {
	_unInit();
}

YBCtrlImageStatic::YBCtrlImageStatic( const void* pBuffer, size_t size ) {
	_init( pBuffer, size );
}

Gdiplus::Image* YBCtrlImageStatic::_getImageFromMemory( const void* buf, size_t size ) {
	IStream *pIStream = NULL;
	HGLOBAL hGlobal = ::GlobalAlloc( GMEM_MOVEABLE, size );
	if( hGlobal == NULL ) {
		return NULL;
	}
	CFuncPack fpkFreeBuf( ::GlobalFree, hGlobal );
	/* copy the buf content to the HGLOBAL */
	void *pBufDest = ::GlobalLock( hGlobal );
	CFuncPack fpkUnlockBuf( ::GlobalUnlock, pBufDest );
	if( pBufDest == NULL) {
		return NULL;
	}
	memcpy( pBufDest, buf, size );
	/* get the IStream from the global object */
	if( ::CreateStreamOnHGlobal( hGlobal, TRUE, &pIStream ) != S_OK ) {
		return NULL;
	}
	/* create the image from the stream */
	Gdiplus::Image *pImage = Gdiplus::Image::FromStream( pIStream );

	pIStream->Release();
	/* i suppose when the reference count for stream is 0, it will 
	GlobalFree automatically. The Image maintain the object also.*/	
	return pImage;
}

Gdiplus::Image* YBCtrlImageStatic::_getImageFromResource( HMODULE hInst, UINT nResourceID, LPCTSTR lpszResourceType ) {
	DWORD dwRsrcSize = 0;
	HRSRC hRsrc = NULL;
	LPVOID pRsrcBuf = NULL;
	HRESULT hr = S_FALSE;
	HGLOBAL hRsrcBuf = NULL;
	HGLOBAL hRsrcData = NULL;
	LPCVOID pRsrcData = NULL;
	IStream* pIStream = NULL;
	Gdiplus::Image* pImage;
	
	if ( lpszResourceType == RT_BITMAP ) {
		assert( false );
		return NULL;
	}
	//hInstance = AfxGetResourceHandle();

	hRsrc = ::FindResource( hInst, MAKEINTRESOURCE( nResourceID ), lpszResourceType );
	if ( !hRsrc )
		return NULL;
	dwRsrcSize = ::SizeofResource( hInst, hRsrc );
	if ( !dwRsrcSize )
		return NULL;
	hRsrcBuf = ::GlobalAlloc( GMEM_FIXED, dwRsrcSize );
	if ( NULL == hRsrcBuf )
		return NULL;
	CFuncPack fpkFreeBuf( ::GlobalFree, hRsrcBuf );
	hRsrcData = ::LoadResource( hInst, hRsrc );
	if ( NULL == hRsrcData )
		return NULL;
	pRsrcData = ::LockResource( hRsrcData );
	if ( NULL == pRsrcData )
		return NULL;
	
	pRsrcBuf = ::GlobalLock( hRsrcBuf );
	CFuncPack fpkUnlockBuf( ::GlobalUnlock, hRsrcBuf );
	if ( pRsrcBuf ) {
		::CopyMemory( pRsrcBuf, pRsrcData, dwRsrcSize );
		if ( ::CreateStreamOnHGlobal( hRsrcBuf, FALSE, &pIStream ) == S_OK ) {
			pImage = Gdiplus::Image::FromStream( pIStream );
			pIStream->Release();
			pIStream = NULL;
			if ( !pImage )
				return NULL;
		}
	}
	return (Gdiplus::Image*)pImage;
}

void YBCtrlImageStatic::_init( const void* pBuffer, size_t size ) {
	Gdiplus::RectF rcDest;

	m_pImage = _getImageFromMemory( pBuffer, size );
	if(m_pImage) {
		//
		m_uWidth = m_pImage->GetWidth();
		m_uHeight = m_pImage->GetHeight();
		assert( (m_uWidth != 0) && (m_uHeight != 0) );
		//
		m_pMemDC = new YBCtrlMemDC( m_pImage->GetWidth(), m_pImage->GetHeight() );
		Gdiplus::Graphics graphics( m_pMemDC->getMemDC() );
		rcDest.X = 0;
		rcDest.Y = 0;
		rcDest.Width = (Gdiplus::REAL)m_uWidth;
		rcDest.Height = (Gdiplus::REAL)m_uHeight;
		graphics.DrawImage( m_pImage, rcDest, (Gdiplus::REAL)0, (Gdiplus::REAL)0, (Gdiplus::REAL)m_pImage->GetWidth(), (Gdiplus::REAL)m_pImage->GetHeight(), Gdiplus::UnitPixel );
		//
		m_uFlag |= YBCTRLIMAGESTATIC_FLAG_READY;
	}
	else {
		assert( false );
		m_uFlag &= ~YBCTRLIMAGESTATIC_FLAG_READY;
	}
}

void YBCtrlImageStatic::_init( HMODULE hInst, UINT nResourceID, LPCTSTR lpszResourceType ) {
	Gdiplus::RectF rcDest;

	m_pImage = _getImageFromResource( hInst, nResourceID, lpszResourceType );
	if ( m_pImage ) {
		//
		m_uWidth = m_pImage->GetWidth();
		m_uHeight = m_pImage->GetHeight();
		assert( ( m_uWidth != 0 ) && ( m_uHeight != 0 ) );
		//
		m_pMemDC = new YBCtrlMemDC( m_pImage->GetWidth(), m_pImage->GetHeight() );
		Gdiplus::Graphics graphics( m_pMemDC->getMemDC() );
		rcDest.X = 0;
		rcDest.Y = 0;
		rcDest.Width = (Gdiplus::REAL)m_uWidth;
		rcDest.Height = (Gdiplus::REAL)m_uHeight;
		graphics.DrawImage( m_pImage, rcDest, (Gdiplus::REAL)0, (Gdiplus::REAL)0, (Gdiplus::REAL)m_pImage->GetWidth(), (Gdiplus::REAL)m_pImage->GetHeight(), Gdiplus::UnitPixel );
		//
		m_uFlag |= YBCTRLIMAGESTATIC_FLAG_READY;
	} else {
		assert( false );
		m_uFlag &= ~YBCTRLIMAGESTATIC_FLAG_READY;
	}
}

void YBCtrlImageStatic::_init( const tstring_type& tstrImageFilePath ) {
	Gdiplus::RectF rcDest;
	USES_CONVERSION;

#ifdef _UNICODE
	m_pImage = new Gdiplus::Image( tstrImageFilePath.c_str() );
#else 
	m_pImage = new Gdiplus::Image( T2W(tstrImageFilePath.c_str()) );
#endif //_UNICODE
	if ( m_pImage ) {
		//
		m_uWidth = m_pImage->GetWidth();
		m_uHeight = m_pImage->GetHeight();
		assert( ( m_uWidth != 0 ) && ( m_uHeight != 0 ) );
		//
		m_pMemDC = new YBCtrlMemDC( m_pImage->GetWidth(), m_pImage->GetHeight() );
		Gdiplus::Graphics graphics( m_pMemDC->getMemDC() );
		rcDest.X = 0;
		rcDest.Y = 0;
		rcDest.Width = (Gdiplus::REAL)m_uWidth;
		rcDest.Height = (Gdiplus::REAL)m_uHeight;
		graphics.DrawImage( m_pImage, rcDest, (Gdiplus::REAL)0, (Gdiplus::REAL)0, (Gdiplus::REAL)m_pImage->GetWidth(), (Gdiplus::REAL)m_pImage->GetHeight(), Gdiplus::UnitPixel );
		//
		m_uFlag |= YBCTRLIMAGESTATIC_FLAG_READY;
	} else {
		assert( false );
		m_uFlag &= ~YBCTRLIMAGESTATIC_FLAG_READY;
	}
}

void YBCtrlImageStatic::_unInit() {
	if ( m_pMemDC ) {
		delete m_pMemDC;
		m_pMemDC = NULL;
	}

	if ( m_pImage )
		delete m_pImage;
	m_pImage = NULL;
	m_uFlag &= ~YBCTRLIMAGESTATIC_FLAG_READY;
}

HDC YBCtrlImageStatic::getMemDC() const { 
	if ( !( m_uFlag & YBCTRLIMAGESTATIC_FLAG_READY ) ) {
		assert( false );
		return NULL;
	}
	return m_pMemDC->getMemDC(); 
}




