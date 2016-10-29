#include "stdafx.h"
#include "frame\YBCtrlImageDepository.h"
#include "frame\YBCtrlImageBase.h"
#include "frame\YBCtrlDefs.h"
#include <assert.h>
USING_NAMESPACE_YBCTRL
//
YBCtrlImageDepository::~YBCtrlImageDepository() {

}

YBCtrlImageDepository* YBCtrlImageDepository::getInstance() {
	static YBCtrlImageDepository s_instance;
	return &s_instance;
}

//unsigned int YBCtrlImageDepository::_allocateId() const {
//	static unsigned int s_idBase = YBCTRL_IMAGE_ID_BASE;
//	return ++s_idBase;
//}

unsigned int YBCtrlImageDepository::_allocateId() const {
	static unsigned int s_idBase = YBCTRL_IMAGE_ID_BASE;
	id2image_map_type::const_reverse_iterator critBegin;

	if ( m_mapId2Image.size() == 0 ) {
		return ( YBCTRL_IMAGE_ID_BASE + 1 );
	}
	critBegin = m_mapId2Image.crbegin();
	return ( (critBegin->first) + 1 );
}

bool YBCtrlImageDepository::_insertImageById( unsigned int uYBImageId, YBCtrlImageBase* pYBImage ) {
	if ( !pYBImage ) {
		assert( false );
		return false;
	}
	id2image_map_type::iterator itId2Image, iendId2Image;

	if ( m_mapId2Image.size() != 0 ) {
		iendId2Image = m_mapId2Image.end();
		itId2Image = m_mapId2Image.find( uYBImageId );
		if ( itId2Image != iendId2Image ) { // uYBImageId existed.
			assert( false );
			return false; 
		}
	}

	m_mapId2Image[ uYBImageId ] = pYBImage;
	return true;
}

bool YBCtrlImageDepository::_insertImage( YBCtrlImageBase* pYBImage, unsigned int& uYBImageId ) {
	if ( !pYBImage || _hasImage( pYBImage ) ) {
		assert( false );
		return false;
	}
	unsigned int uImageIdNew = _allocateId();
	assert( !_hasImage( uImageIdNew ) );
	m_mapId2Image[ uImageIdNew ] = pYBImage;
	uYBImageId = uImageIdNew;

	return true;
}

bool YBCtrlImageDepository::_hasImage( YBCtrlImageBase* pYBImage ) const {
	id2image_map_type::const_iterator citId2Image, ciendId2Image;
	
	ciendId2Image = m_mapId2Image.end();
	for ( citId2Image = m_mapId2Image.begin(); citId2Image!=ciendId2Image; ++citId2Image ) {
		if ( pYBImage == citId2Image->second ) {
			return true;
		}
	}

	return false;
}

bool YBCtrlImageDepository::_hasImage( unsigned int uYBImageId ) const {
	id2image_map_type::const_iterator citId2Image;

	citId2Image = m_mapId2Image.find( uYBImageId );
	return ( citId2Image != m_mapId2Image.end() );
}

bool YBCtrlImageDepository::getImage( unsigned int uYBImageId, YBCtrlImageBase** ppYBImage ) {
	if ( !ppYBImage ) {
		assert( false );
		return false;
	}
	id2image_map_type::iterator itId2Image;

	itId2Image = m_mapId2Image.find( uYBImageId );
	if ( itId2Image == m_mapId2Image.end() )
		return false;
	
	*ppYBImage = itId2Image->second;
	return true;
}

bool YBCtrlImageDepository::hasImage( unsigned int uYBImageId ) const {
	return _hasImage( uYBImageId );
}

bool YBCtrlImageDepository::removeImage( unsigned int uYBImageId ) {
	if(!_hasImage( uYBImageId )) {
		//assert( false );
		return false;
	}
	id2image_map_type::iterator itId2Image;

	itId2Image = m_mapId2Image.find( uYBImageId );
	if(itId2Image == m_mapId2Image.end())
		return false;
	YBCtrlImageBase* pYBImage = NULL;
	pYBImage = itId2Image->second;
	if(pYBImage) {
		delete pYBImage;
		pYBImage = NULL;
	}
	m_mapId2Image.erase( itId2Image );
	
	return true;
}
