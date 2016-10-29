#include "stdafx.h"
#include "frame\YBCtrlMisc.h"
#include "frame\YBCtrlImageGif.h"
#include "frame\YBCtrlImageStatic.h"
#include "frame\YBCtrlDefs.h"
#include "gdiplus.h"
#include <assert.h>
USING_NAMESPACE_YBCTRL
#pragma warning(push)
#pragma warning(disable:4244)
bool ybctrl::GdipStretchBlt( Gdiplus::Graphics* pGraphics, Gdiplus::Image* pImage
	, unsigned int uXDest, unsigned int uYDest, unsigned int uWidthDest, unsigned int uHeightDest
	, unsigned int uXSrc, unsigned int uYSrc, unsigned int uWidthSrc, unsigned int uHeightSrc ) {
	if ( !pGraphics || !pImage )
		return false;
	Gdiplus::RectF rcDest;
	
	rcDest.X = uXDest;
	rcDest.Y = uYDest;
	rcDest.Width = uWidthDest;
	rcDest.Height = uHeightDest;

	return ( Gdiplus::Ok == pGraphics->DrawImage( pImage, rcDest, (Gdiplus::REAL)uXSrc, (Gdiplus::REAL)uYSrc, (Gdiplus::REAL)uWidthSrc, (Gdiplus::REAL)uHeightSrc, Gdiplus::UnitPixel, NULL, NULL, NULL ) );
}

bool ybctrl::GdipBitBlt( Gdiplus::Graphics* pGraphics, Gdiplus::Image* pImage
	, unsigned int uXDest, unsigned int uYDest, unsigned int uWidthDest, unsigned int uHeightDest
	, unsigned int uXSrc, unsigned int uYSrc, unsigned int uWidthSrc, unsigned int uHeightSrc ) {
	if ( !pGraphics || !pImage )
		return false;
	Gdiplus::RectF rcDest;
	Gdiplus::RectF rcSrc;
	
	rcDest.X = uXDest;
	rcDest.Y = uYDest;
	rcDest.Width = uWidthDest;
	rcDest.Height = uHeightDest;
	rcSrc.X = uXSrc;
	rcSrc.Y = uYSrc;
	rcSrc.Width = uWidthSrc;
	rcSrc.Height = uHeightSrc;
	return ( Gdiplus::Ok == pGraphics->DrawImage( pImage, rcDest, uXSrc, uYSrc, uWidthSrc, uHeightSrc, Gdiplus::UnitPixel, NULL ) );
}
#pragma warning(pop)

bool ybctrl::YBImage2GPImage( YBCtrlImageBase* pYBImage, Gdiplus::Image** ppGPImage ) {
	if ( !pYBImage || !ppGPImage )
		return NULL;

	switch ( pYBImage->getType() ) {
	case YBCTRL_IMAGE_TYPE_GIF:
		{
			YBCtrlImageGif* pYBImageGif = NULL;
	
			pYBImageGif = (YBCtrlImageGif*)pYBImage;
			*ppGPImage = pYBImageGif->getImage();
			return true;
		}
		break;
	case YBCTRL_IMAGE_TYPE_STATIC:
		{
			YBCtrlImageStatic* pYBImageStatic = NULL;

			pYBImageStatic = ( YBCtrlImageStatic* )pYBImage;
			*ppGPImage = pYBImageStatic->getImage();
			return true;
		}
		break;
	default:
		{
			assert( false );
			return false;
		}
		break;
	}
	assert( false );
	return false;
}

void ybctrl::BackGroundBitBlt( HDC hDest, unsigned int xDest, unsigned int yDest, unsigned int uWidthDest, unsigned int uHeightDest
	, HDC hMemSrc, unsigned int xSrc, unsigned int ySrc, unsigned int uWidthSrc, unsigned int uHeightSrc
	, unsigned int uLFixed, unsigned int uTFixed, unsigned int uRFixed, unsigned int uBFixed ) {
	RECT rcInner;
	
	rcInner.left = xDest + uLFixed;
	rcInner.top = yDest + uTFixed;
	rcInner.right = xDest + uWidthDest - uRFixed;
	rcInner.bottom = yDest + uHeightDest - uBFixed;
	// left-top
	::BitBlt( hDest, xDest, yDest, uLFixed, uTFixed, hMemSrc, xSrc, ySrc, SRCCOPY );
	// left-bottom
	::BitBlt( hDest, xDest, rcInner.bottom, uLFixed, uBFixed, hMemSrc, xSrc, ySrc + uHeightSrc - uBFixed, SRCCOPY );
	// right-top
	::BitBlt( hDest, rcInner.right, yDest, uRFixed, uTFixed, hMemSrc, xSrc + uWidthSrc - uRFixed, ySrc, SRCCOPY );
	// right-bottom
	::BitBlt( hDest, rcInner.right, rcInner.bottom, uRFixed, uBFixed, hMemSrc, xSrc + uWidthSrc - uRFixed, ySrc + uHeightSrc - uBFixed, SRCCOPY );
	// top
	if ( rcInner.right > rcInner.left 
	&& uWidthSrc > ( uLFixed + uRFixed ) ) {
		::StretchBlt( hDest, rcInner.left, yDest, ( rcInner.right - rcInner.left ), uTFixed
			, hMemSrc, xSrc + uLFixed, ySrc, ( uWidthSrc - uLFixed - uRFixed ), uTFixed
			, SRCCOPY );
	}
	// left
	if ( rcInner.bottom > rcInner.top 
	&& uHeightSrc > ( uTFixed + uBFixed )	) {
		::StretchBlt( hDest, xDest, rcInner.top, uLFixed, ( rcInner.bottom - rcInner.top )
			, hMemSrc, xSrc, ySrc + uTFixed, uLFixed, ( uHeightSrc - uTFixed - uBFixed )
			, SRCCOPY );
	}
	// bottom
	if ( rcInner.right > rcInner.left 
	&& uWidthSrc > ( uLFixed + uRFixed ) ) {
		::StretchBlt( hDest, rcInner.left, rcInner.bottom, ( rcInner.right - rcInner.left ), uBFixed
			, hMemSrc, xSrc + uLFixed, ySrc + uHeightSrc - uBFixed, ( uWidthSrc - uLFixed - uRFixed ), uBFixed
			, SRCCOPY );
	}
	// right 
	if ( rcInner.bottom > rcInner.top 
	&& uHeightSrc > ( uTFixed + uBFixed	) ) {
		::StretchBlt( hDest, rcInner.right, rcInner.top, uRFixed, ( rcInner.bottom - rcInner.top )
			, hMemSrc, xSrc + uWidthSrc - uRFixed, ySrc + uTFixed, uRFixed, ( uHeightSrc - uTFixed - uBFixed )
			, SRCCOPY );
	}
	// center
	if ( rcInner.right > rcInner.left 
	&& rcInner.bottom > rcInner.top 
	&& uWidthSrc > ( uLFixed + uRFixed )
	&& uHeightSrc > ( uTFixed + uBFixed ) ) {
		::StretchBlt( hDest, rcInner.left, rcInner.top, ( rcInner.right - rcInner.left ), ( rcInner.bottom - rcInner.top )
			, hMemSrc, xSrc + uLFixed, ySrc + uTFixed, ( uWidthSrc - uLFixed - uRFixed ), ( uHeightSrc - uTFixed - uBFixed )
			, SRCCOPY );
	}

}