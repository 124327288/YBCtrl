#ifndef __YBTREECTRL_H__
#define __YBTREECTRL_H__
#include "frame/YBCtrlEnvCfg.h"
#include <list>
#include <deque>
#include "treectrl/YBNodeBase.h"
//
YBCTRL_NAMESPACE_BEGIN
//
class YBNodeRoot;
class YBScrollWnd;
class YBNodeFolder;
class YBTSHelper;
//
class YBCTRL_API YBTreeCtrl {
public:
	typedef std::deque< unsigned int > ctrlid_container_type;
public:
	YBTreeCtrl( unsigned int uScrollBarThick = 20, unsigned int uStepMove = 20 );
	virtual ~YBTreeCtrl();

public:
	HWND create( int x, int y, int nWidth, int nHeight, HWND hWndParent, WORD wCtrlID, HINSTANCE hInstance );
	bool insertNode( YBNodeFolder* pNodeParent, unsigned int uSubNodeIndex, YBNodeBase* pNodeNew, bool bCalcPos = true );
	YBScrollWnd* getScrollWnd() { return m_pScrollWnd; }
	YBNodeRoot* getNodeRoot() { return m_pNodeRoot; }
	void calcAllNodesPos();
	void updateStatus();
	void clearAll();
		  
private:
	void _init( unsigned int uScrollBarThick, unsigned int uStepMove );
	void _unInit();

protected:
	YBTSHelper* m_pTSHelper;
	YBNodeRoot* m_pNodeRoot;
	YBScrollWnd* m_pScrollWnd;
};

YBCTRL_NAMESPACE_END
#endif //__YBTREECTRL_H__