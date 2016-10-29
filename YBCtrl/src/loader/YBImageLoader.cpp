#include "stdafx.h"
#include "loader\YBImageLoader.h"
#include "frame\YBCtrlImageDepository.h"
#include "frame\YBCtrlImageGif.h"
#include "frame\YBCtrlImageStatic.h"
#include "io.h"
#include "tchar.h"
USING_NAMESPACE_YBCTRL

YBImageLoader* YBImageLoader::getInstance() {
	static YBImageLoader s_instance;
	return &s_instance;
}

YBImageLoader::~YBImageLoader() {

}

bool YBImageLoader::loadImage( const tstring_type& tstrImageFilePath, unsigned int& imageId ) {
	if ( _taccess_s( tstrImageFilePath.c_str(), 0 ) != 0 )
		return false;
	ybctrl::YBCtrlImageBase* pYBCtrlImage = NULL;

	if ( _isGif( tstrImageFilePath ) ) {
		pYBCtrlImage = new YBCtrlImageGif( tstrImageFilePath );
		if ( !YBCTRLIMAGEDEPOSITORY_INSTANCE->_insertImage( pYBCtrlImage, imageId ) ) {
			delete pYBCtrlImage;
			pYBCtrlImage = NULL;
		}
	} else {
		pYBCtrlImage = new YBCtrlImageStatic( tstrImageFilePath );
		if ( !YBCTRLIMAGEDEPOSITORY_INSTANCE->_insertImage( pYBCtrlImage, imageId ) ) {
			delete pYBCtrlImage;
			pYBCtrlImage = NULL;
		}
	}

	return true;
}

bool YBImageLoader::loadImage( HMODULE hInst, UINT nResourceID, LPCTSTR lpszResourceType, unsigned int& imageId ) {
	ybctrl::YBCtrlImageBase* pYBCtrlImage = NULL;
	
	pYBCtrlImage = new YBCtrlImageStatic( hInst, nResourceID, lpszResourceType );
	if ( !YBCTRLIMAGEDEPOSITORY_INSTANCE->_insertImage( pYBCtrlImage, imageId ) ) {
		delete pYBCtrlImage;
		pYBCtrlImage = NULL;
	}

	return true;
}

bool YBImageLoader::loadImage( const void* pBuffer, size_t size, unsigned int& imageId ) {
		ybctrl::YBCtrlImageBase* pYBCtrlImage = NULL;
	
	pYBCtrlImage = new YBCtrlImageStatic( pBuffer, size );
	if ( !YBCTRLIMAGEDEPOSITORY_INSTANCE->_insertImage( pYBCtrlImage, imageId ) ) {
		delete pYBCtrlImage;
		pYBCtrlImage = NULL;
	}

	return true;
}

void YBImageLoader::insertImageById( unsigned int uImageId, const tstring_type& tstrImageFilePath ) {
	if ( _taccess_s( tstrImageFilePath.c_str(), 0 ) != 0 ) {
		return;
	}
	ybctrl::YBCtrlImageBase* pYBCtrlImage = NULL;

	if ( _isGif( tstrImageFilePath ) ) {
		pYBCtrlImage = new YBCtrlImageGif( tstrImageFilePath );
		if ( !YBCTRLIMAGEDEPOSITORY_INSTANCE->_insertImageById( uImageId, pYBCtrlImage ) ) {
			delete pYBCtrlImage;
			pYBCtrlImage = NULL;
		}
	} else {
		pYBCtrlImage = new YBCtrlImageStatic( tstrImageFilePath );
		if ( !YBCTRLIMAGEDEPOSITORY_INSTANCE->_insertImageById( uImageId, pYBCtrlImage ) ) {
			delete pYBCtrlImage;
			pYBCtrlImage = NULL;
		}
	}

}

bool YBImageLoader::_isGif( const tstring_type& tstrGifFilePath ) {
	
	return ( NULL != _tcsstr( tstrGifFilePath.c_str(), _T(".gif") ) );
}