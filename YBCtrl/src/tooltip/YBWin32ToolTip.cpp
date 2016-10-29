#include "stdafx.h"
#include "tooltip/YBWin32ToolTip.h"
#include <assert.h>
#include "frame/FuncPack.h"

YBWin32Tooltip::~YBWin32Tooltip() {

}

YBWin32Tooltip* YBWin32Tooltip::getInstance() {
	static YBWin32Tooltip s_instance;
	return &s_instance;
}

YBWin32Tooltip::YBWin32Tooltip()
: m_hWnd( NULL )
, m_hWndDest( NULL ){
	_createWnd();
}

bool YBWin32Tooltip::_addTool( HWND hWndDest, const tstring_type& tstrTooltip, LPCRECT lpRectTool, UINT_PTR nIDTool ) {
	TOOLINFO ti;
	LRESULT lRes;

	if ( m_hWndDest != hWndDest ) {
		if ( m_hWndDest ) {
			::ShowWindow( m_hWnd, SW_HIDE );
			_delTool( m_hWndDest );
		}
		m_hWndDest = hWndDest;
	}

	//
	memset( &ti, 0, sizeof( TOOLINFO ) );
	ti.cbSize = 44;
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd = hWndDest;
	ti.hinst = NULL;
	ti.lpszText = (LPTSTR)tstrTooltip.c_str();
	GetClientRect( hWndDest, &ti.rect );
	lRes = ::SendMessage( m_hWnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti );
	if ( lRes == 0 ) {
		DWORD dwErr = ::GetLastError();
		int a = 0; 
	}
	::ShowWindow( m_hWnd, SW_NORMAL );
	if ( ::IsWindowVisible( m_hWnd ) ) {
		RECT rcIT;
		::GetClientRect( m_hWnd, &rcIT );
		int a = 0;
	}

	return true;
}

//bool YBWin32Tooltip::addTool( HWND hWndDest, const tstring_type& tstrTooltip, LPCRECT lpRectTool, UINT_PTR nIDTool ) {
//	assert(::IsWindow(m_hWnd));
//	assert(hWndDest != NULL);
//	// the toolrect and toolid must both be zero or both valid
//	assert((lpRectTool != NULL && nIDTool != 0) ||
//		(lpRectTool == NULL) && (nIDTool == 0));
//	TOOLINFO ti;
//	LRESULT lRes;
//
//	//
//	::ShowWindow( m_hWnd, SW_NORMAL );
//	//
//	_fillInToolInfo( ti, hWndDest, nIDTool );
//	//
//	if (lpRectTool != NULL)	{
//		memcpy_s(&ti.rect, sizeof(RECT), lpRectTool, sizeof(RECT));
//	} else {
//		::GetClientRect( hWndDest, &ti.rect );
//	}
//	//
//	ti.lpszText = (LPTSTR)tstrTooltip.c_str();
//	//
//	lRes = ::SendMessage(m_hWnd, TTM_ADDTOOL, 0, (LPARAM)&ti);
//	if ( lRes == 0 ) {
//		DWORD dwErr = ::GetLastError();
//	}
//	//
//	return ( TRUE == lRes );
//}

bool YBWin32Tooltip::_createWnd() {
	if ( m_hWnd ) {
		assert( false );
		return false;
	}
	//
	m_hWnd = ::CreateWindowEx( WS_EX_TOPMOST
		, TOOLTIPS_CLASS, 0
		, WS_POPUP | TTS_ALWAYSTIP | TTS_NOPREFIX
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, NULL // hWnd
		, NULL
		, NULL
		, NULL);
	if ( !m_hWnd ) {
		assert( false );
		return false;
	}
	//
	::SetWindowPos( m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );
	return true;
}

void YBWin32Tooltip::_fillInToolInfo(TOOLINFO& ti, HWND hWndDest, UINT_PTR nIDTool) const
{
	assert( ::IsWindow( hWndDest ) );
	//memset(&ti, 0, sizeof(AFX_OLDTOOLINFO));
	//ti.cbSize = sizeof(AFX_OLDTOOLINFO);
	memset( &ti, 0, 44 );
	ti.cbSize = 44;
	if (nIDTool == 0)
	{
		ti.hwnd = ::GetParent(hWndDest);
		//ti.hwnd = hWndDest;
		ti.uFlags = TTF_IDISHWND;
		ti.uId = (UINT_PTR)hWndDest;
	}
	else
	{
		ti.hwnd = hWndDest;
		ti.uFlags = 0;
		ti.uId = nIDTool;
	}
}
void YBWin32Tooltip::_delTool( HWND hWndDest, UINT_PTR nIDTool)
{
	assert(::IsWindow(m_hWnd));
	assert(hWndDest != NULL);

	TOOLINFO ti;
	_fillInToolInfo(ti, hWndDest, nIDTool);
	::SendMessage(m_hWnd, TTM_DELTOOL, 0, (LPARAM)&ti);
}

void YBWin32Tooltip::updateTipText( LPCTSTR lpszText, HWND hWndDest, UINT_PTR nIDTool ) {
	assert( ::IsWindow( m_hWnd ) );
	assert( ::IsWindow( hWndDest ) );
	
	//
	if ( _getToolCount() == 0 ) {
		_addTool( hWndDest, lpszText, NULL, nIDTool );
		return;
	} 
	//
	if ( m_hWndDest != hWndDest ) {
		if ( m_hWndDest ) {
			::ShowWindow( m_hWnd, SW_HIDE );
			_delTool( m_hWndDest );
			_addTool( hWndDest, lpszText, NULL, nIDTool );
		}
		m_hWndDest = hWndDest;
	} 

	//
	TOOLINFO ti;
	_fillInToolInfo(ti, hWndDest, nIDTool);
	ti.lpszText = (LPTSTR)lpszText;
	::SendMessage(m_hWnd, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
}

void YBWin32Tooltip::_activate( bool bActivate ) {
	assert( ::IsWindow( m_hWnd ) );
	//
	::SendMessage(m_hWnd, TTM_ACTIVATE, bActivate, 0L);
}

int YBWin32Tooltip::_getToolCount() {
	return (int) ::SendMessage(m_hWnd, TTM_GETTOOLCOUNT, 0, 0L);
}