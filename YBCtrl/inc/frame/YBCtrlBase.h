#ifndef __YBCTRLBASE_H__
#define __YBCTRLBASE_H__
#include "YBCtrlEnvCfg.h"
#include <map>
YBCTRL_NAMESPACE_BEGIN
//
#define YBCTRL_WNDMSGMAP_DECLARE() \
public: \
	virtual void onWndMsg( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

#define YBCTRL_WNDMSGMAP_BEGIN( theClass, baseClass ) \
void theClass::onWndMsg( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) { \
	typedef theClass ThisClass; \
	typedef baseClass TheBaseClass; \
	if ( !pYBCtrlWndMsg ) {\
		return; \
	} \
	switch ( pYBCtrlWndMsg->m_uMsg ) { \
	case WM_NULL: {} break; 
 
//
#define YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE() \
public: \
	virtual void onWndMsg( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg );

#define YBCTRL_WNDMSGMAP_BEGIN_WITHNAMESPACE( theClass, baseClass ) \
void theClass::onWndMsg( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) { \
	typedef theClass ThisClass; \
	typedef baseClass TheBaseClass; \
	if ( !pYBCtrlWndMsg ) {\
		return; \
	} \
	switch ( pYBCtrlWndMsg->m_uMsg ) { \
	case WM_NULL: {} break; 

#define YBCTRL_WNDMSGMAP_END() \
	default: break; \
	} \
	if ( pYBCtrlWndMsg->m_dwFlag & YBCTRLWNDMSG_FLAG_EATMSG ) { \
		return; \
	} \
	TheBaseClass::onWndMsg( pYBCtrlWndMsg ); \
}

#define YBCTRL_ONWNDMSG( uMsg, memfxn ) \
	case uMsg: \
    { \
		memfxn( pYBCtrlWndMsg );\
	} \
	break;

#define YBCTRLMSG_ENTER_CHECK( msg ) \
	if ( !pYBCtrlWndMsg || ( pYBCtrlWndMsg->m_uMsg != msg ) ) { \
		assert( false ); \
		return; \
	}

//
#define YBCTLS_CLASSNAME _T("YBCtls")
//
#define YBCTRLBASE_FLAG_NONE (0x00000000)
#define YBCTRLBASE_FLAG_BIND2HWND (0x00000001)
//
#define YBCTRLWNDMSG_FLAG_NONE (0x00000000)
#define YBCTRLWNDMSG_FLAG_EATMSG (0x00000001)
#define YBCTRLWNDMSG_FLAG_RESULT_VALID (0x00000002)
//
struct YBCTRL_API YBCtrlWndMsg {
	HWND m_hWnd;
	UINT m_uMsg;
	WPARAM m_wParam;
	LPARAM m_lParam;
	LRESULT m_lRes;
	DWORD m_dwFlag;
};
//
class YBCTRL_API YBCtrlBase {
public:
	typedef __int32 flag_type;
	typedef std::map< HWND, YBCtrlBase* > hwnd2ybctrlbase_map_type;
public:
	YBCtrlBase();
	virtual ~YBCtrlBase();

public:
	virtual void onWndMsg( YBCtrlWndMsg* pYBCtrlWndMsg );

public:
	HWND getHWND() const { return m_hWnd; }

protected:
	HWND _createHWND( DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, void* lpParam );
	void _bind2HWND( HWND hWnd );
	bool _isBind2HWND() const;
	void _unBind2HWND();

private:
	static LRESULT CALLBACK WndProc( __in  HWND hwnd, __in  UINT uMsg, __in  WPARAM wParam, __in  LPARAM lParam );
	static YBCtrlBase* HWND2Ctrl( HWND hWnd ) ;

private:
	static bool s_bRegWndClass;
#pragma warning(push)
#pragma warning(disable:4251)
	static hwnd2ybctrlbase_map_type s_mapHWND2Ctrl;
#pragma warning(pop)

private:
	void _init();
	void _uninit();
	void _regWndClass();

private:
	flag_type m_flag4YBCtrlBase;
protected:
	HWND m_hWnd;
};

YBCTRL_NAMESPACE_END
#endif //__YBCTRLBASE_H__
