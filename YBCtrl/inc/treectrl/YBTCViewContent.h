#ifndef __YBTCViewContent_H__
#define __YBTCViewContent_H__
#include "frame/YBCtrlEnvCfg.h"
#include "view/YBViewContentBase.h"
#include "view/YBViewInner.h"
//
YBCTRL_NAMESPACE_BEGIN
//
class YBCtrlMemDC;
class YBNodeRoot;
class YBScrollWnd;
//
class YBTCViewContent;
//
class YBTCView : public YBViewInner {
public:
	YBTCView( YBTCViewContent* pTCVC );
	virtual ~YBTCView();
	
public:
	YBCTRL_WNDMSGMAP_DECLARE();

};
//
class YBTCViewContent : public YBViewContentBase {
	friend class YBTCView;
public:
	YBTCViewContent( YBNodeRoot* pNodeRoot );
	virtual ~YBTCViewContent();

public:
	virtual unsigned int getWidth() const;
	virtual unsigned int getHeight() const;
protected:
	virtual void _drawStatus( HDC hDC, YBViewInner* pYBViewInner ) const;

private:
	void _drawBk( HDC hDC, const RECT& rcInContent ) const;

protected:
	YBNodeRoot* m_pNodeRoot;
	YBCtrlMemDC* m_pMemDC;
};

YBCTRL_NAMESPACE_END
#endif //__YBTCViewContent_H__