#ifndef __YBIMAGELOADER_H__
#define __YBIMAGELOADER_H__
#include "frame\YBCtrlEnvCfg.h"
#include "frame\YBCtrlDefs.h"
#include <string>
#include "tchar.h"
YBCTRL_NAMESPACE_BEGIN

class YBCTRL_API YBImageLoader {
public:
	typedef std::basic_string< TCHAR > tstring_type;

public:
	static YBImageLoader* getInstance();
	bool loadImage( const tstring_type& tstrImageFilePath, unsigned int& imageId );
	bool loadImage( const void* pBuffer, size_t size, unsigned int& imageId );
	bool loadImage( HMODULE hInst, UINT nResourceID, LPCTSTR lpszResourceType, unsigned int& imageId );
	void insertImageById( unsigned int uImageId, const tstring_type& tstrImageFilePath );
	
public:
	virtual ~YBImageLoader();
protected:
	YBImageLoader() {}
	YBImageLoader( const YBImageLoader& ) {}

protected:
	bool _isGif( const tstring_type& tstrGifFilePath );

};

YBCTRL_NAMESPACE_END
#endif //__YBIMAGELOADER_H__