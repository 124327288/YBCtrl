#ifndef __YBLB4SB_H__
#define __YBLB4SB_H__
#include "view/YBViewInner.h"
#include "view/YBViewContentBase.h"

class YBLB4SBViewInner : public ybctrl::YBViewInner {
public:
	YBLB4SBViewInner();
	virtual ~YBLB4SBViewInner();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()


};

class YBLB4SBViewConntent : public ybctrl::YBViewContentBase {
public:
	YBLB4SBViewConntent();
	virtual ~YBLB4SBViewConntent();

public:
	virtual unsigned int getWidth() const;
	virtual unsigned int getHeight() const;
protected:
	virtual void _drawStatus( HDC hDC, ybctrl::YBViewInner* pYBViewInner ) const;


};


#endif //__YBLB4SB_H__