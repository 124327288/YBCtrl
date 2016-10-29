#include "stdafx.h"
#include "YBTopMostBar.h"
#include "frame\YBCtrlImageDepository.h"
#include "frame\YBCtrlDefs.h"
#include "loader\YBImageLoader.h"
#include "frame\YBCtrlMisc.h"
#include "frame\YBCtrlImageStatic.h"
#include <assert.h>

YBTopMostBar::YBTopMostBar() 
: m_uImageIdBK( YBCTRL_IMAGE_ID_INVALID ) {

}

YBTopMostBar::~YBTopMostBar() {
}

YBCTRL_WNDMSGMAP_BEGIN_WITHNAMESPACE( YBTopMostBar, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_KEYDOWN, _onWM_KEYDOWN )
	YBCTRL_ONWNDMSG( WM_CLOSE, _onWM_CLOSE )
YBCTRL_WNDMSGMAP_END()

bool YBTopMostBar::setBkImage( const tstring_type& tstrBkImagePath ) {
	if ( m_uImageIdBK != YBCTRL_IMAGE_ID_INVALID )
		return false;
	if( !ybctrl::YBImageLoader::getInstance()->loadImage( tstrBkImagePath, m_uImageIdBK ) ) {
		assert( false );
		return false;
	}

	return true;
}

HWND YBTopMostBar::create( int x, int y, int nWidth, int nHeight, HWND hWndParent, HINSTANCE hInstance ) {
	HWND hWnd;
	DWORD dwExStyle;
	DWORD dwStyle;

//	dwExStyle = WS_EX_TOPMOST|WS_EX_TOOLWINDOW;  
//	dwStyle = WS_POPUP|WS_VISIBLE; 
	dwExStyle = 0;
	dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS;
	hWnd = _createHWND( dwExStyle, dwStyle, x, y, nWidth, nHeight, hWndParent, (HMENU)0, hInstance, NULL );
	return hWnd;
}

#ifndef KEYDOWN_FLAG
#define KEYDOWN_FLAG (0x8000)
#endif //KEYDOWN_FLAG

void YBTopMostBar::_onWM_KEYDOWN( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	if ( !pYBCtrlWndMsg || ( pYBCtrlWndMsg->m_uMsg != WM_KEYDOWN ) ) 
		return;

	switch ( pYBCtrlWndMsg->m_wParam ) {
	case 'A':
		{
			int a = 0;
		}
		break;
	default:
		break;
	}
}

void YBTopMostBar::_onWM_PAINT( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	if ( !pYBCtrlWndMsg )
		return;
	HWND hWnd;
	HDC hPaintDC;
	HBRUSH hBrushBK;
	PAINTSTRUCT ps;
	RECT rcClient;

	hWnd = pYBCtrlWndMsg->m_hWnd;
	::GetClientRect( hWnd, &rcClient );
	hPaintDC = ::BeginPaint( hWnd, &ps );

	if ( m_uImageIdBK == YBCTRL_IMAGE_ID_INVALID ) {
		hBrushBK = ::CreateSolidBrush( RGB( 0, 29, 73 ) );
		::FillRect( hPaintDC, &rcClient, hBrushBK );
		::DeleteObject( hBrushBK );
	} else {
		ybctrl::YBCtrlImageBase* pYBImage = NULL;

		if ( !YBCTRLIMAGEDEPOSITORY_INSTANCE_WITHNAMESPACE->getImage( m_uImageIdBK, &pYBImage ) ) {
			assert( false );
		} else {
			ybctrl::YBCtrlImageStatic* pYBImageStatic = NULL;
			if ( pYBImage->getType() == YBCTRL_IMAGE_TYPE_STATIC ) {
				pYBImageStatic = (ybctrl::YBCtrlImageStatic*)pYBImage;
				Gdiplus::Image* pGPImage = NULL;

				pGPImage = pYBImageStatic->getImage();
				::StretchBlt( hPaintDC, 0, 0, (rcClient.right - rcClient.left), ( rcClient.bottom - rcClient.top )
					, pYBImageStatic->getMemDC(), 0, 0, pGPImage->GetWidth(), pGPImage->GetHeight(), SRCCOPY );

				//Gdiplus::Graphics graphics( hPaintDC );
				//ybctrl::GdipStretchBlt( &graphics, pYBImageStatic->getImage(), 0, 0, ( rcClient.right - rcClient.left ), ( rcClient.bottom - rcClient.top )
				//	, 0, 0, pYBImageStatic->getImage()->GetWidth(), pYBImageStatic->getImage()->GetHeight() );

			}

		}
	}
	::EndPaint( hWnd, &ps );
}

void YBTopMostBar::_onWM_CLOSE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {

	pYBCtrlWndMsg->m_dwFlag |= YBCTRLWNDMSG_FLAG_EATMSG;
}
