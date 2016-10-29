#ifndef __YBCTRLFONTDEPOSITORY_H__
#define __YBCTRLFONTDEPOSITORY_H__
#include "frame\YBCtrlEnvCfg.h"
#include <map>
YBCTRL_NAMESPACE_BEGIN

struct YBCtrlFont {
	LOGFONT m_logFont;
	HFONT m_hFont;
	COLORREF m_clrTxt;
};
//
class YBCTRL_API YBCtrlFontDepository {
public:
	typedef unsigned int font_id_type;
	typedef std::map< font_id_type, YBCtrlFont* > id2ybfont_map_type;

private:
	YBCtrlFontDepository();
public:
	static YBCtrlFontDepository* getInstance();
	virtual ~YBCtrlFontDepository();

public:
	unsigned int insertLOGFONT( const LOGFONT& lFont, COLORREF clrTxt );
	const YBCtrlFont* getYBFont( unsigned int uYBFontId ) const;
	bool hasYBFont( unsigned int uYBFontId ) const;

private:
	unsigned int _allocateId() const;
	void _clearAll();

private:
#pragma warning(push)
#pragma warning(disable:4251)
	id2ybfont_map_type m_mapId2YBFont;
#pragma warning(pop)
};

YBCTRL_NAMESPACE_END
#endif //__YBCTRLFONTDEPOSITORY_H__