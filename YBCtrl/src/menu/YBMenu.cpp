#include "stdafx.h"
#include "menu/YBMenu.h"
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlMisc.h"
#include "frame/YBCtrlImageStatic.h"
#include "frame/YBCtrlImageBase.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlFontDepository.h"
#include <WindowsX.h>
#include "gdiplus.h"
#include <assert.h>
#include <map>

USING_NAMESPACE_YBCTRL

YBMenu::YBMenu()
: m_uBkImageId(YBCTRL_IMAGE_ID_INVALID)
{

}

YBMenu::~YBMenu(){
	_clearAllMenuItem();
}

YBCTRL_WNDMSGMAP_BEGIN( YBMenu, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_KILLFOCUS, _onWM_KILLFOCUS );
	YBCTRL_WNDMSGMAP_END()

	void YBMenu::_onWM_PAINT( YBCtrlWndMsg* pYBCtrlWndMsg ){
		YBCTRLMSG_ENTER_CHECK( WM_PAINT );
		RECT rcClient;
		PAINTSTRUCT ps;
		HDC hPaintDC;

		::GetClientRect( m_hWnd, &rcClient );
		YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
		_drawBk( memDC.getMemDC() );
		_drawText( memDC.getMemDC() );

		hPaintDC = ::BeginPaint( m_hWnd, &ps );
		::BitBlt( hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
		::EndPaint( m_hWnd, &ps );

}


void YBMenu::_onWM_KILLFOCUS( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_KILLFOCUS );

	::SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
}

void YBMenu::_onWM_LBUTTONDOWN( YBCtrlWndMsg* pYBCtrlWndMsg ){
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
	unsigned int uCount;
	RECT rcItemRect;
	RECT rcClient;
	POINT ptMouse;
	HDC hDC;
	YBMenuItem* pYBMenuItem = NULL;
	//
	::GetClientRect( m_hWnd, &rcClient );
	YBCtrlMemDC memDC( rcClient.right - rcClient.left, rcClient.bottom - rcClient.top );
	_drawBk( memDC.getMemDC() );
	_drawText( memDC.getMemDC() );

	hDC = ::GetDC( m_hWnd );
	::BitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY );
	::ReleaseDC( m_hWnd, hDC );
	//
	ptMouse.x = GET_X_LPARAM( pYBCtrlWndMsg->m_lParam );
	ptMouse.y = GET_Y_LPARAM( pYBCtrlWndMsg->m_lParam );
	uCount = m_containerMenuItem.size();
	for( unsigned int uIndex = 0; uIndex < uCount; ++uIndex ){
		pYBMenuItem = m_containerMenuItem[ uIndex ];
		if( !pYBMenuItem )
			continue;
		_calcItemRect( uIndex, rcItemRect );
		if( PtInRect( &rcItemRect, ptMouse ) ){
			::SendMessage( m_hWnd, WM_COMMAND, (WPARAM)pYBMenuItem->m_wordCtrlId, (LPARAM)m_hWnd );
		}
	}
}


HWND YBMenu::Create(  int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance ){
	HWND hWnd;
	DWORD dwExStyle;
	DWORD dwStyle;

	dwExStyle = WS_EX_TOOLWINDOW|WS_EX_TOPMOST;
	dwStyle = WS_VISIBLE|WS_POPUPWINDOW;

	for( std::map< HWND, YBMenu* >::iterator iter = g_mapHWNDMenu.begin(); iter != g_mapHWNDMenu.end(); iter++ )
	{
		if( iter->first )
		{
			::SendMessage( iter->first, WM_SYSCOMMAND, SC_CLOSE, 0 );
		}
	}
	hWnd = _createHWND( dwExStyle, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, NULL );
	if( !hWnd ){
		assert(false);
	}
	g_mapHWNDMenu.insert( std::map< HWND, YBMenu* >::value_type( hWnd, this ) );

	return hWnd;
}

YBCtrlImageStatic* YBMenu::_getBkImageStatic( unsigned int uImageId ) {
	YBCtrlImageBase* pYBImageBase = NULL;
	YBCtrlImageStatic* pImageStatic = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pYBImageBase ) )
		return NULL;

	if( !pYBImageBase || ( pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

void YBMenu::SetBkImage( unsigned int uBkImageId ){
	m_uBkImageId = uBkImageId;

}

void YBMenu::_drawBk( HDC hDC ){
	if( !getHWND() )
		return;

	RECT rcClient;
	::GetClientRect( m_hWnd, &rcClient );
	
	YBCtrlImageStatic* pYBImage = NULL;
	pYBImage = _getBkImageStatic( m_uBkImageId );
	//
	if ( !pYBImage ) {
		return;
	}
	//
	BackGroundBitBlt( hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pYBImage->getMemDC(),
		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 2, 2, 2, 2 );
}

BOOL YBMenu::InsertMenuItem( YBMenuItem* pYBMenuItem ){
	if( !pYBMenuItem )
		return FALSE;
	m_containerMenuItem.push_back( pYBMenuItem );
	return TRUE;
}

void YBMenu::_clearAllMenuItem(){
	mi_container_type::iterator itMI, iendMI;
	YBMenuItem* pYBMI = NULL;

	iendMI = m_containerMenuItem.end();
	for( itMI = m_containerMenuItem.begin(); itMI != iendMI; ++itMI ){
		pYBMI = *itMI;
		if( pYBMI ){
	//		delete pYBMI;
			pYBMI = NULL;
		}
	}
}

BOOL YBMenu::_calcItemRect( unsigned int uIndex, RECT& rcItemRect ){
	unsigned int uHeight;
	unsigned int uItemHeight;
	RECT rcClient;

	::GetClientRect( m_hWnd, &rcClient );
	uHeight = rcClient.bottom - rcClient.top;
	if( m_containerMenuItem.size() != 0 ){
		uItemHeight = uHeight/m_containerMenuItem.size();
		rcItemRect.right = rcClient.right;
		rcItemRect.left = rcClient.left;
		rcItemRect.top = uIndex*uItemHeight;
		rcItemRect.bottom = rcItemRect.top + uItemHeight;
	}
	return TRUE;

}

void YBMenu::_drawText( HDC hDC ){
	unsigned int uItemHeight;
	unsigned int uHeight;
	unsigned int uItemCount;
	RECT rcClient;
	RECT rcDrawText;
	YBMenuItem* pYBMI = NULL;
	HFONT hFontOld;
	int iBkModeOld;
	COLORREF clrTxtOld;
	const YBCtrlFont* pYBCtrlFont = NULL;

	::GetClientRect( m_hWnd, &rcClient );
	uItemCount = m_containerMenuItem.size();
	uHeight = rcClient.bottom - rcClient.top;
	if( uItemCount > 0 ){
		uItemHeight = uHeight/uItemCount;
		for( unsigned int uIndex = 0; uIndex < uItemCount; ++uIndex ){
			pYBMI = m_containerMenuItem[ uIndex ];
			if( !pYBMI ){
				continue;
			}
			if( _calcItemRect( uIndex, rcDrawText )
				&&( rcDrawText.right - rcDrawText.left )
				&&( rcDrawText.bottom - rcDrawText.top )
				&&( pYBMI->m_tstrText.length() > 0 )){
					pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontId );
					if ( pYBCtrlFont ) {
						hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
						iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
						clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );
						::DrawText( hDC, pYBMI->m_tstrText.c_str(), pYBMI->m_tstrText.length(), &rcDrawText, DT_LEFT );
						::SetTextColor( hDC, clrTxtOld );
						::SetBkMode( hDC, iBkModeOld );
						::SelectObject( hDC, hFontOld );
					}
			}

		}
	}

}

void YBMenu::SetFontId( unsigned int uFontId ){
	m_uFontId = uFontId;
}