#ifndef __YBCTRLIMAGEBASE_H__
#define __YBCTRLIMAGEBASE_H__
#include "frame\YBCtrlEnvCfg.h"
YBCTRL_NAMESPACE_BEGIN

class YBCTRL_API YBCtrlImageBase {
public:
	YBCtrlImageBase(){};
	virtual ~YBCtrlImageBase(){};

public:
	virtual unsigned int getType() = 0;
	virtual unsigned int getWidth() const = 0;
	virtual unsigned int getHeight() const = 0;

};

YBCTRL_NAMESPACE_END
#endif //__YBCTRLIMAGEBASE_H__