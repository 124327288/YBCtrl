#ifndef __YBCTRLIMAGESTATIC_H__
#define __YBCTRLIMAGESTATIC_H__
#include "frame\YBCtrlImageBase.h"
#include "frame\YBCtrlDefs.h"
#include <string>
#include "tchar.h"
namespace Gdiplus {
	class Image;
}
YBCTRL_NAMESPACE_BEGIN
//
class YBCtrlMemDC;
//
class YBCTRL_API YBCtrlImageStatic : public YBCtrlImageBase{
public:
	typedef std::basic_string< TCHAR > tstring_type;
public:
	YBCtrlImageStatic( const tstring_type& tstrImageFilePath );
	YBCtrlImageStatic( HMODULE hInst, UINT nResourceID, LPCTSTR lpszResourceType );
	YBCtrlImageStatic( const void* pBuffer, size_t size );
	virtual ~YBCtrlImageStatic();

public:
	virtual unsigned int getType() { return YBCTRL_IMAGE_TYPE_STATIC; }
	virtual unsigned int getWidth() const { return m_uWidth; }
	virtual unsigned int getHeight() const { return m_uHeight; }

public:
	Gdiplus::Image* getImage() const { return m_pImage; }
	Gdiplus::Image* getImage() { return m_pImage; }
	HDC getMemDC() const;

private:
	void _init( const tstring_type& tstrImageFilePath );
	void _init( HMODULE hInst, UINT nResourceID, LPCTSTR lpszResourceType );
	void _init( const void* pBuffer, size_t size );
	void _unInit();
	Gdiplus::Image* _getImageFromResource( HMODULE hInst, UINT nResourceID, LPCTSTR lpszResourceType );
	Gdiplus::Image* _getImageFromMemory( const void* buf, size_t size );
private:
	unsigned int m_uFlag;
	Gdiplus::Image* m_pImage;
	YBCtrlMemDC* m_pMemDC;
	unsigned int m_uWidth;
	unsigned int m_uHeight;
};

YBCTRL_NAMESPACE_END
#endif //__YBCTRLIMAGESTATIC_H__