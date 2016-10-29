#include "stdafx.h"
#include "frame\YBCtrlFontDepository.h"
#include "frame\YBCtrlDefs.h"
#include <assert.h>
USING_NAMESPACE_YBCTRL

YBCtrlFontDepository::YBCtrlFontDepository() {

}

YBCtrlFontDepository* YBCtrlFontDepository::getInstance() {
	static YBCtrlFontDepository s_instance;
	return &s_instance;
}

YBCtrlFontDepository::~YBCtrlFontDepository() {
	_clearAll();
}

unsigned int YBCtrlFontDepository::insertLOGFONT( const LOGFONT& lFont, COLORREF clrTxt ) {
	HFONT hFont = NULL;
	unsigned int uFontId;
	YBCtrlFont* pYBCtrlFont = NULL;

	hFont = ::CreateFontIndirect( &lFont );
	if ( !hFont ) {
		return YBCTRL_FONT_ID_INVALID;
	}
	uFontId = _allocateId();
	//
	pYBCtrlFont = new YBCtrlFont();
	pYBCtrlFont->m_hFont = hFont;
	pYBCtrlFont->m_logFont = lFont;
	pYBCtrlFont->m_clrTxt = clrTxt;
	m_mapId2YBFont[ uFontId ] = pYBCtrlFont;
	return uFontId;
}

unsigned int YBCtrlFontDepository::_allocateId() const {
	static unsigned int s_idBase = YBCTRL_FONT_ID_BASE;
	return ++s_idBase;
}

void YBCtrlFontDepository::_clearAll() {
	id2ybfont_map_type::iterator itId2YBFont, iendId2YBFont;
	YBCtrlFont* pYBCtrlFont = NULL;

	iendId2YBFont = m_mapId2YBFont.end();
	for ( itId2YBFont = m_mapId2YBFont.begin(); itId2YBFont!=iendId2YBFont; ++itId2YBFont ) {
		pYBCtrlFont = itId2YBFont->second;
		DeleteObject( pYBCtrlFont->m_hFont );
		pYBCtrlFont->m_hFont = NULL;
		//
		delete pYBCtrlFont;
		pYBCtrlFont = NULL;
	}
	m_mapId2YBFont.clear();
}

const YBCtrlFont* YBCtrlFontDepository::getYBFont( unsigned int uYBFontId ) const {
	if ( m_mapId2YBFont.size() == 0 ) {
		assert( false );
		return NULL;
	}
	id2ybfont_map_type::const_iterator citId2YBFont;
	YBCtrlFont* pYBCtrlFont = NULL;

	citId2YBFont = m_mapId2YBFont.find( uYBFontId );

	return ( citId2YBFont != m_mapId2YBFont.end() ) ? citId2YBFont->second : NULL;
}

bool YBCtrlFontDepository::hasYBFont( unsigned int uYBFontId ) const {
	id2ybfont_map_type::const_iterator citId2YBFont;

	citId2YBFont = m_mapId2YBFont.find( uYBFontId );

	return citId2YBFont!=m_mapId2YBFont.end();
}

