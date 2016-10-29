#include "stdafx.h"
#include "frame\YBCtrlBase.h"
#include "tchar.h"
#include <assert.h>
USING_NAMESPACE_YBCTRL
//
bool YBCtrlBase::s_bRegWndClass = false;
YBCtrlBase::hwnd2ybctrlbase_map_type YBCtrlBase::s_mapHWND2Ctrl;

//
YBCtrlBase::YBCtrlBase()
: m_flag4YBCtrlBase( YBCTRLBASE_FLAG_NONE ) 
, m_hWnd( NULL ) {
	_init();
}

YBCtrlBase::~YBCtrlBase() {
	_uninit();
}

extern HINSTANCE g_hInstance4YBCtrls;

void YBCtrlBase::_init() {
	_regWndClass();
}

void YBCtrlBase::_uninit() {
	if ( m_hWnd ) {
		_unBind2HWND();
		m_hWnd = NULL;
	}
}

void YBCtrlBase::_regWndClass() {
	if ( s_bRegWndClass )
		return;
	WNDCLASS wndCls;

	memset( &wndCls, 0, sizeof( WNDCLASS ) );
	wndCls.cbClsExtra = 0;
	wndCls.cbWndExtra = 0;
	wndCls.style = CS_DBLCLKS;
	wndCls.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wndCls.hCursor = ::LoadCursor( NULL, IDC_ARROW );
	wndCls.lpfnWndProc = WndProc;
	wndCls.hInstance = NULL;
	wndCls.lpszClassName = YBCTLS_CLASSNAME;
	::RegisterClass( &wndCls );
	//
	s_bRegWndClass = true;
}

LRESULT YBCtrlBase::WndProc(   
	__in  HWND hwnd,
	__in  UINT uMsg,
	__in  WPARAM wParam,
	__in  LPARAM lParam
	) {
		YBCtrlBase* pYBCtrlHit = NULL;
		YBCtrlWndMsg ybctrlWndMsg;
		
		// dispatch message.
		pYBCtrlHit = HWND2Ctrl( hwnd );
		if ( pYBCtrlHit ) {
			ybctrlWndMsg.m_hWnd = hwnd;
			ybctrlWndMsg.m_uMsg = uMsg;
			ybctrlWndMsg.m_wParam = wParam;
			ybctrlWndMsg.m_lParam = lParam;
			ybctrlWndMsg.m_dwFlag = YBCTRLWNDMSG_FLAG_NONE;
			ybctrlWndMsg.m_lRes = 0;
			pYBCtrlHit->onWndMsg( &ybctrlWndMsg );

			if ( ybctrlWndMsg.m_dwFlag & YBCTRLWNDMSG_FLAG_EATMSG ) {
				if ( ybctrlWndMsg.m_dwFlag & YBCTRLWNDMSG_FLAG_RESULT_VALID ) {
					return ybctrlWndMsg.m_lRes;
				} else {
					return 0;
				}
				assert( false );
				return 0;
			}
		}

		return ::DefWindowProc( hwnd, uMsg, wParam, lParam );
}

void YBCtrlBase::onWndMsg( YBCtrlWndMsg* pYBCtrlWndMsg ) {
	if ( !pYBCtrlWndMsg )
		return;

	switch ( pYBCtrlWndMsg->m_uMsg ) {
	case WM_DESTROY:
		{
			_uninit();
		}
		break;
	default:
		break;
	}
}

void YBCtrlBase::_bind2HWND( HWND hWnd ) {
	if ( _isBind2HWND() ) {
		assert( false );
		return;
	}
	s_mapHWND2Ctrl[ hWnd ] = this;
	m_hWnd = hWnd;
}

bool YBCtrlBase::_isBind2HWND() const {
	hwnd2ybctrlbase_map_type::iterator itHWND2YBCtrlBase, iendHWND2YBCtrlBase;

	iendHWND2YBCtrlBase = s_mapHWND2Ctrl.end();
	for ( itHWND2YBCtrlBase = s_mapHWND2Ctrl.begin(); itHWND2YBCtrlBase!=iendHWND2YBCtrlBase; ++itHWND2YBCtrlBase ) {
		if ( itHWND2YBCtrlBase->second == this )
			return true;
	}

	return false;
}

void YBCtrlBase::_unBind2HWND() {
	if ( s_mapHWND2Ctrl.size() == 0 ) {
		return;
	}
	hwnd2ybctrlbase_map_type::iterator itHWND2YBCtrlBase, iendHWND2YBCtrlBase;

	iendHWND2YBCtrlBase = s_mapHWND2Ctrl.end();
	for ( itHWND2YBCtrlBase = s_mapHWND2Ctrl.begin(); itHWND2YBCtrlBase!=iendHWND2YBCtrlBase; ++itHWND2YBCtrlBase ) {
		if ( itHWND2YBCtrlBase->second == this ) {
			s_mapHWND2Ctrl.erase( itHWND2YBCtrlBase );
			return;
		}
	}

	assert( false );
}

YBCtrlBase* YBCtrlBase::HWND2Ctrl( HWND hWnd ) {
	hwnd2ybctrlbase_map_type::iterator itHWND2YBCtrlBase;

	itHWND2YBCtrlBase = s_mapHWND2Ctrl.find( hWnd );

	return ( itHWND2YBCtrlBase == s_mapHWND2Ctrl.end() ) ? NULL : itHWND2YBCtrlBase->second;
}

HWND YBCtrlBase::_createHWND( DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, void* lpParam ) {

	HWND hWndDest;

	hWndDest = ::CreateWindowEx( dwExStyle, YBCTLS_CLASSNAME, _T(""), dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam );
	if ( !hWndDest ) {
		assert( false );
		return NULL;
	}

	m_hWnd = hWndDest;
	_bind2HWND( m_hWnd );
	return m_hWnd;
}