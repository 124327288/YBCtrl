#ifndef __YBNODEROOT_H__
#define __YBNODEROOT_H__
#include "frame/YBCtrlEnvCfg.h"
#include "treectrl/YBNodeFolder.h"

YBCTRL_NAMESPACE_BEGIN

class YBCTRL_API YBNodeRoot : public YBNodeFolder {
public:
	YBNodeRoot();
	virtual ~YBNodeRoot();
	
	// interface of YBNodeBase
private:
	virtual unsigned int _getHeight() const;
	virtual unsigned int _getWidth() const;
	// interface of YBNodeFolder
private:
	virtual unsigned int _getIndentSpace() const;

public:
	void calcAllSubNodePos();

};

YBCTRL_NAMESPACE_END

#endif //__YBNODEROOT_H__