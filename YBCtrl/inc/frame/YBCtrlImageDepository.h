#ifndef __YBCTRLIMAGEDEPOSITORY_H__
#define __YBCTRLIMAGEDEPOSITORY_H__
#include "frame\YBCtrlEnvCfg.h"
#include <map>
YBCTRL_NAMESPACE_BEGIN
class YBCtrlImageBase;
//
class YBCTRL_API YBCtrlImageDepository {
public:
	typedef unsigned int image_id_type;
	typedef std::map< image_id_type, YBCtrlImageBase* > id2image_map_type;
	friend class YBImageLoader;

public:
	static YBCtrlImageDepository* getInstance();
	virtual ~YBCtrlImageDepository();
	
public:
	bool getImage( unsigned int uYBImageId, YBCtrlImageBase** ppYBImage );
	bool hasImage( unsigned int uYBImageId ) const;
	bool removeImage( unsigned int uYBImageId );

private:
	bool _insertImageById( unsigned int uYBImageId, YBCtrlImageBase* pYBImage );
	bool _insertImage( YBCtrlImageBase* pYBImage, unsigned int& uYBImageId );
	YBCtrlImageDepository(){};
	YBCtrlImageDepository( const YBCtrlImageDepository& ){};
	unsigned int _allocateId() const;
	bool _hasImage( YBCtrlImageBase* pYBImage ) const;
	bool _hasImage( unsigned int uYBImageId ) const;

private:
#pragma warning(push)
#pragma warning(disable:4251)
	id2image_map_type m_mapId2Image;
#pragma warning(pop)
};

#define YBCTRLIMAGEDEPOSITORY_INSTANCE (YBCtrlImageDepository::getInstance())
#define YBCTRLIMAGEDEPOSITORY_INSTANCE_WITHNAMESPACE (ybctrl::YBCtrlImageDepository::getInstance())

YBCTRL_NAMESPACE_END
#endif //__YBCTRLIMAGEDEPOSITORY_H__
