#ifndef __YBVIEWCONTENTBASE_H__
#define __YBVIEWCONTENTBASE_H__
#include "frame/YBCtrlEnvCfg.h"
YBCTRL_NAMESPACE_BEGIN
//
class YBCTRL_API YBViewContentBase {
	friend class YBViewInner;
public:
	YBViewContentBase() {};
	virtual ~YBViewContentBase() {};
	
public:
	virtual unsigned int getWidth() const = 0;
	virtual unsigned int getHeight() const = 0;
protected:
	virtual void _drawStatus( HDC hDC, YBViewInner* pYBViewInner ) const = 0;

};

YBCTRL_NAMESPACE_END
#endif //__YBVIEWCONTENTBASE_H__