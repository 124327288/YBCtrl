#include "stdafx.h"
#include "YBListBox4IMEDlg.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlFontDepository.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlMisc.h"
#include "frame/YBCtrlImageStatic.h"
#include <assert.h>
//
USING_NAMESPACE_YBCTRL

#define COLOR_YBLB4IMEDLG_ALL_ITEMBK_DEFAULT (RGB(213, 240, 254))
#define COLOR_YBLB4IMEDLG_ALL_CHECKBOXBK_DEFAULT (RGB(170, 215, 238))
#define COLOR_YBLB4IMEDLG_SELECTED_ITEMBK_DEFAULT (RGB(254, 237, 181))
#define COLOR_YBLB4IMEDLG_ITEMBK_DEFAULT (RGB(255,255,255))
#define COLOR_YBLB4IMEDLG_VIEWBK_DEFAULT (RGB(122,122,122))

#define YBIMEDLG_INDEX_INVALID (0xFFFFFFFF)
//
#define YBIMEDLG_FLAG_NONE (0x00000000)
#define YBIMEDLG_FLAG_INHOVER (0x00000001)
//
#define YBIMEDLG_WIDTH_NUMBER (30)
#define YBIMEDLG_LSPACE_BTN (159)
//////////////////////////////////////////////////////////////////////////
YBListBoxIMEItem::YBListBoxIMEItem()
: m_tstrIMEName( _T("") )
, m_uImageIdIMEBtnUse( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdIMEBtnInstall( YBCTRL_IMAGE_ID_INVALID )
, m_uItemId( YBCTRL_MENU_ID_INVALID )
, m_pBtnRadio( new YBRadioBtn() )
{
	// radio 资源
	m_pBtnRadio->setRadioBtnLSpace( 0 );
	//m_pBtnRadio->setImage( YBRadioBtn::ENORADIOBTN_STATUS_NORMAL, YBOS_Common::m_pResourceLoader->m_uImageIdNRBNormal );
	//m_pBtnRadio->setImage( YBRadioBtn::ENORADIOBTN_STATUS_HOVER, YBOS_Common::m_pResourceLoader->m_uImageIdNRBHover );
	//m_pBtnRadio->setImage( YBRadioBtn::ENORADIOBTN_STATUS_PRESS, YBOS_Common::m_pResourceLoader->m_uImageIdNRBPress );
	//m_pBtnRadio->setImage( YBRadioBtn::ERADIOBTN_STATUS_NORMAL, YBOS_Common::m_pResourceLoader->m_uImageIdRBNormal );
	//m_pBtnRadio->setImage( YBRadioBtn::ERADIOBTN_STATUS_HOVER, YBOS_Common::m_pResourceLoader->m_uImageIdRBHover );
	//m_pBtnRadio->setImage( YBRadioBtn::ERADIOBTN_STATUS_PRESS, YBOS_Common::m_pResourceLoader->m_uImageIdRBPress );
}

YBListBoxIMEItem::~YBListBoxIMEItem() {
	if ( m_pBtnRadio ) {
		if ( ::IsWindow( m_pBtnRadio->getHWND() ) ) {
			::DestroyWindow( m_pBtnRadio->getHWND() );
		}
		delete m_pBtnRadio;
		m_pBtnRadio = NULL;
	}
}


unsigned int YBListBoxIMEItem::getItemId() const{
	assert( m_uItemId != YBCTRL_MENU_ID_INVALID );
	return m_uItemId;
}

void YBListBoxIMEItem::_setItemId( unsigned int uItemId ){
	assert( m_uItemId == YBCTRL_MENU_ID_INVALID );
	m_uItemId = uItemId;
}

//////////////////////////////////////////////////////////////////////////
#pragma warning( push )
#pragma warning (disable:4355 )
YBView4IMEDlgInner::YBView4IMEDlgInner( unsigned int uContentWidth, unsigned int uItemHeight )
: ybctrl::YBViewInner( new YBView4IMEContent( uContentWidth, uItemHeight, this ) ) {
	setBkClr( COLOR_YBLB4IMEDLG_VIEWBK_DEFAULT );
	setContentXPosLT( 0 );
	setContentYPosLT( 0 );
}
#pragma warning( pop )

YBView4IMEDlgInner::~YBView4IMEDlgInner() {

}

YBCTRL_WNDMSGMAP_BEGIN( YBView4IMEDlgInner, YBViewInner )
	YBCTRL_ONWNDMSG( WM_COMMAND, _onWM_COMMAND )
	YBCTRL_ONWNDMSG( WM_LBUTTONDOWN, _onWM_LBUTTONDOWN )
	YBCTRL_ONWNDMSG( WM_MOUSEMOVE, _onWM_MOUSEMOVE )
	YBCTRL_ONWNDMSG( WM_MOUSELEAVE, _onWM_MOUSELEAVE )
	YBCTRL_WNDMSGMAP_END()

void YBView4IMEDlgInner::_onWM_COMMAND(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_COMMAND );

}

void YBView4IMEDlgInner::_onWM_LBUTTONDOWN(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_LBUTTONDOWN );
	POINT ptInV;
	POINT ptInC;
	unsigned int uItemIndex;
	RECT rcHit;
	YBView4IMEContent* pView4WSAWMenuContent = NULL;
	HWND hWndScrollWnd = NULL;

	hWndScrollWnd = ::GetParent( m_hWnd );
	ptInV.x = (__int16)LOWORD( pYBCtrlWndMsg->m_lParam );
	ptInV.y = (__int16)HIWORD( pYBCtrlWndMsg->m_lParam );

	if ( !PointView2Content( ptInV, ptInC ) )
		return;
	pView4WSAWMenuContent = (YBView4IMEContent*)getViewContentObj();
	if ( pView4WSAWMenuContent->moveHitTest( ptInC, uItemIndex ) ) {
		if ( !pView4WSAWMenuContent->getHitRect( uItemIndex, rcHit ) )
			return;
		if ( ::PtInRect( &rcHit, ptInC ) ) {
			::SendMessage( ::GetParent( hWndScrollWnd ), WM_COMMAND, (WPARAM)GetDlgCtrlID( hWndScrollWnd ), (LPARAM)uItemIndex );
			::InvalidateRect( m_hWnd, NULL ,FALSE );
		}
	}
}

void YBView4IMEDlgInner::_onWM_MOUSEMOVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSEMOVE );
	POINT ptInV;
	POINT ptInC;
	unsigned int uItemIndexMove;
	unsigned int uItemIndexMoveIn;
	TRACKMOUSEEVENT trackMouseEvt;
	YBView4IMEContent* pView4WSAWMenuContent = NULL;

	ptInV.x = LOWORD( pYBCtrlWndMsg->m_lParam );
	ptInV.y = HIWORD( pYBCtrlWndMsg->m_lParam );
	if ( !PointView2Content( ptInV, ptInC ) )
		return;
	//
	trackMouseEvt.cbSize = sizeof( TRACKMOUSEEVENT );
	trackMouseEvt.dwFlags = TME_LEAVE;
	trackMouseEvt.hwndTrack = pYBCtrlWndMsg->m_hWnd;
	trackMouseEvt.dwHoverTime = HOVER_DEFAULT;
	//
	::TrackMouseEvent ( &trackMouseEvt );
	pView4WSAWMenuContent = (YBView4IMEContent*)getViewContentObj();
	if ( pView4WSAWMenuContent ) {
		if ( pView4WSAWMenuContent->moveTest( ptInC, uItemIndexMove ) ) {
			pView4WSAWMenuContent->setSelItem( uItemIndexMove );		
		} else {
			pView4WSAWMenuContent->setItemInvalid( YBIMEDLG_INDEX_INVALID );
		}
		//
		if ( pView4WSAWMenuContent->moveHitTest( ptInC, uItemIndexMoveIn ) ) {
			pView4WSAWMenuContent->setSelInItem( uItemIndexMoveIn );
		} else {
			pView4WSAWMenuContent->setInItemInvalid( YBIMEDLG_INDEX_INVALID );
		}
	}
}

void YBView4IMEDlgInner::_onWM_MOUSELEAVE( ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg ) {
	YBCTRLMSG_ENTER_CHECK( WM_MOUSELEAVE );

	YBView4IMEContent* pView4WSAWMenuContent = NULL;
	pView4WSAWMenuContent = (YBView4IMEContent*)getViewContentObj();

	for ( unsigned int index = 0; index < pView4WSAWMenuContent->getItemCount(); ++index ) {
		pView4WSAWMenuContent->setItemInvalid( YBIMEDLG_INDEX_INVALID );
		pView4WSAWMenuContent->setInItemInvalid( YBIMEDLG_INDEX_INVALID );
	}
	::InvalidateRect( m_hWnd, NULL, FALSE );
}

//////////////////////////////////////////////////////////////////////////

YBView4IMEContent::YBView4IMEContent( unsigned int uContentWidth, unsigned int uItemHeight, ybctrl::YBViewInner* pViewInner )
	: m_pViewInner( pViewInner )
	, m_uWidth( uContentWidth )
	, m_uItemHeight( uItemHeight )
	, m_pMemDC( new ybctrl::YBCtrlMemDC( m_uWidth, YBView4IMEContent::getHeight() ) )
	, m_hBrushItem( NULL )
	, m_hBrushItemSel( NULL )
	, m_hBrushItemBK( NULL )
	, m_uSelItemIndex( YBIMEDLG_INDEX_INVALID )
	, m_uSelInItemIndex( YBIMEDLG_INDEX_INVALID )
	, m_tstrIcon( _T("点击使用") )
	, m_uFontIdIMEItem( YBCTRL_FONT_ID_INVALID )
	, m_uLSpaceIMENumber( 35 )
{
	_loadFont();
	m_hBrushItem = ::CreateSolidBrush( COLOR_YBLB4IMEDLG_ALL_ITEMBK_DEFAULT );
	m_hBrushItemSel = ::CreateSolidBrush( COLOR_YBLB4IMEDLG_SELECTED_ITEMBK_DEFAULT );
	m_hBrushItemBK = ::CreateSolidBrush( COLOR_YBLB4IMEDLG_ITEMBK_DEFAULT );
	m_hBrushItemChkBk = ::CreateSolidBrush( COLOR_YBLB4IMEDLG_ALL_CHECKBOXBK_DEFAULT );
}

YBView4IMEContent::~YBView4IMEContent() {
	if ( m_hBrushItemSel ) {
		::DeleteObject( m_hBrushItemSel );
		m_hBrushItemSel = NULL;
	}

	if ( m_hBrushItemBK ) {
		::DeleteObject( m_hBrushItemBK );
		m_hBrushItemBK = NULL;
	}

	if ( m_hBrushItem ) {
		::DeleteObject( m_hBrushItem );
		m_hBrushItem = NULL;
	}

	if ( m_hBrushItemChkBk ) {
		::DeleteObject( m_hBrushItemChkBk );
		m_hBrushItemChkBk = NULL;
	}

	if ( m_pMemDC ) {
		delete m_pMemDC;
		m_pMemDC = NULL;
	}
}

void YBView4IMEContent::clearAllItem() {
	//
	_clearAllItem();
	//
	m_uSelItemIndex = YBIMEDLG_INDEX_INVALID;
}

void YBView4IMEContent::_clearAllItem() {
	listboxitem_container_type containerListBoxItemsNeedDel;
	listboxitem_container_type::iterator itLBItem, iendLBItem;
	YBListBoxIMEItem* pLBItem = NULL;

	containerListBoxItemsNeedDel = m_containerListBoxItems;
	m_containerListBoxItems.clear();
	//
	iendLBItem = containerListBoxItemsNeedDel.end();
	for ( itLBItem = containerListBoxItemsNeedDel.begin(); itLBItem!=iendLBItem; ++itLBItem ) {
		pLBItem = (*itLBItem);
		delete pLBItem;
		pLBItem = NULL;
	}
}

unsigned int YBView4IMEContent::getWidth() const {
	return m_uWidth;
}

unsigned int YBView4IMEContent::getHeight() const {
	return m_uItemHeight * m_containerListBoxItems.size();
}

void YBView4IMEContent::_drawStatus( HDC hDC, ybctrl::YBViewInner* pYBViewInner ) const {
	if ( !pYBViewInner )
		return;
	RECT rcDestInContent;
	RECT rcDestInView;

	if ( !pYBViewInner->getContentUpdateRect( rcDestInContent, rcDestInView ) )
		return;
	//
	m_pMemDC->resize( getWidth(), getHeight() );
	_drawBK( m_pMemDC->getMemDC(), rcDestInContent, pYBViewInner );
	_drawAllItem( m_pMemDC->getMemDC(), rcDestInContent, pYBViewInner );

	::BitBlt( hDC, rcDestInView.left, rcDestInView.top, (rcDestInView.right - rcDestInView.left), (rcDestInView.bottom - rcDestInView.top),
		m_pMemDC->getMemDC(), rcDestInContent.left, rcDestInContent.top, SRCCOPY );

}

void YBView4IMEContent::_drawBK( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner ) const{
	HBRUSH hBrushBK = NULL;
	RECT rcCInC;

	::SetRect( &rcCInC, 0, 0, getWidth(), getHeight() );
	::FillRect( hDC, &rcCInC, m_hBrushItemBK );
}

void YBView4IMEContent::_drawAllItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner ) const {
	if ( m_containerListBoxItems.size() == 0 )
		return;
	unsigned int uItemCount;

	uItemCount = m_containerListBoxItems.size();
	for ( unsigned int uItemIndex = 0; uItemIndex<uItemCount; ++uItemIndex ) {
		_drawItem( hDC, rcNeedShowInContent, pYBViewInner, uItemIndex );
	}
}

bool YBView4IMEContent::_getItemBtnRadioRect( const RECT& rcItemInC, RECT& rcBtnRadioInV ) const {
	RECT rcItemInV;

	if ( !m_pViewInner->getRectContent2View( rcItemInC, rcItemInV ) ) {
		return false;
	}
	rcBtnRadioInV.left = rcItemInV.left + 8;
	rcBtnRadioInV.right = rcBtnRadioInV.left + 19;
	rcBtnRadioInV.top = ( rcItemInV.top + rcItemInV.bottom - 19 )/2;
	rcBtnRadioInV.bottom = rcBtnRadioInV.top + 19;
	return true;
}

void YBView4IMEContent::_drawItem( HDC hDC, const RECT& rcNeedShowInContent, ybctrl::YBViewInner* pYBViewInner, unsigned int uItemIndex ) const {
	assert( uItemIndex < m_containerListBoxItems.size() );
	RECT rcItem;
	RECT rcChkBk;
	RECT rcBtnChkInV;
	RECT rcDest;
	YBListBoxIMEItem* pItemMenu = NULL;

	if ( !_getItemRect( uItemIndex, rcItem ) ) {
		return;
	}

	//
	pItemMenu = m_containerListBoxItems[ uItemIndex ];
	if ( !::IntersectRect( &rcDest, &rcItem, &rcNeedShowInContent ) ) {
		::ShowWindow( pItemMenu->m_pBtnRadio->getHWND(), SW_HIDE );
		return; // do not intersect
	} 

	if ( !_getItemChkBk( uItemIndex, rcChkBk ) )
		return;
	// draw item bk
	rcItem.bottom -= 1;
	if ( m_uSelItemIndex == uItemIndex ) {
		::FillRect( hDC, &rcItem, m_hBrushItemSel );
	} else {
		::FillRect( hDC, &rcItem, m_hBrushItem );
	}
	// draw item checkbk
	::FillRect( hDC, &rcChkBk, m_hBrushItemChkBk );

	// _drawIMENameTxt
	_drawIMENameTxt( hDC, rcItem, uItemIndex );
	// _drawHitBtn
	_drawHitBtn( hDC, uItemIndex );
	// drawNumber 123456789
	_drawIMENumber( hDC, rcItem, uItemIndex );
	// _drawHitBtnTxt “点击使用”
	_drawHitBtnTxt( hDC, rcItem, uItemIndex );
	//
	_drawIcon( hDC, rcItem, uItemIndex );

	// move radio button.
	if ( !_getItemBtnRadioRect( rcItem, rcBtnChkInV ) )
		return;
	::MoveWindow( pItemMenu->m_pBtnRadio->getHWND(), rcBtnChkInV.left, rcBtnChkInV.top, 
		( rcBtnChkInV.right - rcBtnChkInV.left ), ( rcBtnChkInV.bottom - rcBtnChkInV.top ), TRUE );
	::ShowWindow( pItemMenu->m_pBtnRadio->getHWND(), SW_NORMAL );
}

void YBView4IMEContent::_drawHitBtn( HDC hDC, unsigned int uItemIndex ) const {
	ybctrl::YBCtrlImageStatic* pYBImage = NULL;
	YBListBoxIMEItem* pListBoxItem = NULL;
	RECT rcHit;

	pListBoxItem = m_containerListBoxItems[uItemIndex];

	if ( m_uSelInItemIndex == uItemIndex ) {
		pYBImage = _getImageStatic( pListBoxItem->m_uImageIdIMEBtnInstall );
	} else{
		pYBImage = _getImageStatic( pListBoxItem->m_uImageIdIMEBtnUse );
	}

	if ( !getHitRect( uItemIndex, rcHit ) ) {
		assert( false );
		return;
	}

	Gdiplus::Graphics graphics( hDC );
	GdipBitBlt( &graphics, pYBImage->getImage(), rcHit.left, rcHit.top, rcHit.right - rcHit.left, rcHit.bottom - rcHit.top
		, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
}

ybctrl::YBCtrlImageStatic* YBView4IMEContent::_getImageStatic( unsigned int uImageId ) const{
	YBCtrlImageBase* pImageBase = NULL;
	YBCtrlImageStatic* pImageStatic = NULL;

	if( !YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage( uImageId, &pImageBase ) )
		return NULL;
	if( !pImageBase || ( pImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC ) )
		return NULL;

	return (YBCtrlImageStatic*)pImageBase;
}

void YBView4IMEContent::setWidth( unsigned int uWidthNew ) {
	if ( m_uWidth == uWidthNew )
		return;
	m_uWidth = uWidthNew;
}

void YBView4IMEContent::_drawIMENameTxt( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const{
	HFONT hFontOld = NULL;
	const YBListBoxIMEItem* pListBoxItem = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcItemText;

	if ( !_getIMENameRect( uItemIndex, rcItemText ) )
		return;
	pListBoxItem = m_containerListBoxItems[ uItemIndex ];
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdIMEItem );
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

		::DrawText( hDC, pListBoxItem->m_tstrIMEName.c_str(), pListBoxItem->m_tstrIMEName.length(), &rcItemText, DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}

}

void YBView4IMEContent::_drawHitBtnTxt( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const {
	HFONT hFontOld = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcIcon;

	getHitRect( uItemIndex, rcIcon );
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdIMEItemBtnTxt );
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

		::DrawText( hDC, m_tstrIcon.c_str(), m_tstrIcon.length(), &rcIcon, DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

void YBView4IMEContent::_drawIcon( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const {
	ybctrl::YBCtrlImageStatic* pYBImage = NULL;
	YBListBoxIMEItem* pListBoxItem = NULL;
	RECT rcIcon;

	pListBoxItem = m_containerListBoxItems[uItemIndex];
	
	pYBImage = _getImageStatic( pListBoxItem->m_uImageIdIMEIcon );
	//
	if ( !pYBImage )
		return;

	if ( !_getIconRect( uItemIndex, rcIcon ) ) {
		assert( false );
		return;
	}

	Gdiplus::Graphics graphics( hDC );
	GdipBitBlt( &graphics, pYBImage->getImage(), rcIcon.left, rcIcon.top, rcIcon.right - rcIcon.left, rcIcon.bottom - rcIcon.top
		, 0, 0, pYBImage->getWidth(), pYBImage->getHeight() );
}

void YBView4IMEContent::_drawIMENumber( HDC hDC, RECT& rcItem, unsigned int uItemIndex ) const {
	HFONT hFontOld = NULL;
	const YBListBoxIMEItem* pListBoxItem = NULL;
	const YBCtrlFont* pYBCtrlFont = NULL;
	COLORREF clrTxtOld;
	int iBkModeOld;
	RECT rcNumber;
	TCHAR szMsgBoxNumber[10];

	_stprintf_s( szMsgBoxNumber, _T("%d"), uItemIndex + 1 );
	if ( !_getNumberRect( uItemIndex, rcNumber ) )
		return;
	pListBoxItem = m_containerListBoxItems[ uItemIndex ];
	pYBCtrlFont = YBCtrlFontDepository::getInstance()->getYBFont( m_uFontIdIMEItem );
	if( pYBCtrlFont ){
		hFontOld = (HFONT)::SelectObject( hDC, pYBCtrlFont->m_hFont );
		iBkModeOld = ::SetBkMode( hDC, TRANSPARENT );
		clrTxtOld = ::SetTextColor( hDC, pYBCtrlFont->m_clrTxt );

		::DrawText( hDC, szMsgBoxNumber, _tcslen(szMsgBoxNumber), &rcNumber, DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS );
		::SetTextColor( hDC, clrTxtOld );
		::SetBkMode( hDC, iBkModeOld );
		::SelectObject( hDC, hFontOld );
	}
}

bool YBView4IMEContent::_getIconRect( unsigned int uIndex, RECT& rcIcon ) const {
	if ( uIndex >= m_containerListBoxItems.size() )
		return false;
	RECT rcClient;

	YBListBoxIMEItem* pYBListBoxItem = NULL;
	YBCtrlImageStatic* pYBImage = NULL;
	
	pYBListBoxItem = m_containerListBoxItems[ uIndex ];
	if ( !pYBListBoxItem )
		return false;
	pYBImage = _getImageStatic( pYBListBoxItem->m_uImageIdIMEIcon );
	if ( !pYBImage || ( pYBImage->getHeight() > m_uItemHeight ) || ( pYBImage->getWidth() > m_uWidth ) )
		return false;

	::GetClientRect( m_pViewInner->getHWND(), &rcClient );
	rcIcon.left = 63;
	rcIcon.right = rcIcon.left + pYBImage->getWidth();
	rcIcon.top = uIndex*m_uItemHeight + ( m_uItemHeight - pYBImage->getHeight() )/2 ;
	rcIcon.bottom = rcIcon.top + pYBImage->getHeight();

	return true;
}

bool YBView4IMEContent::_getItemRect( unsigned int uIndex, RECT& rcItem ) const {
	if ( uIndex >= m_containerListBoxItems.size() )
		return false;
	rcItem.left = 0;
	rcItem.right = getWidth();
	rcItem.top = uIndex*m_uItemHeight;
	rcItem.bottom = ( uIndex + 1 )*m_uItemHeight;
	return true;
}

bool YBView4IMEContent::_getItemChkBk( unsigned int uIndex, RECT& rcChkBk ) const {
	if ( uIndex >= m_containerListBoxItems.size() )
		return false;
	rcChkBk.left = 0;
	rcChkBk.right = 34;
	rcChkBk.top = uIndex*m_uItemHeight;
	rcChkBk.bottom = ( uIndex + 1 )*m_uItemHeight - 1;
	return true;
}

bool YBView4IMEContent::_getNumberRect( unsigned int uIndex, RECT& rcNumber ) const {
	if ( uIndex >= m_containerListBoxItems.size() )
		return false;
	RECT rcClient;

	::GetClientRect( m_pViewInner->getHWND(), &rcClient );
	rcNumber.left = m_uLSpaceIMENumber;
	rcNumber.right = rcNumber.left + YBIMEDLG_WIDTH_NUMBER;
	rcNumber.top = uIndex*m_uItemHeight + 7;
	rcNumber.bottom = ( uIndex + 1 )*m_uItemHeight - 2;

	return true;
}
bool YBView4IMEContent::_getIMENameRect( unsigned int uIndex, RECT& rcContent ) const {
	if ( uIndex >= m_containerListBoxItems.size() )
		return false;
	RECT rcClient;

	::GetClientRect( m_pViewInner->getHWND(), &rcClient );
	rcContent.left = rcClient.left + 84;
	rcContent.right = rcContent.left + 100;
	rcContent.top = uIndex*m_uItemHeight + 7;
	rcContent.bottom = ( uIndex + 1 )*m_uItemHeight - 2;

	return true;
}

bool YBView4IMEContent::getHitRect( unsigned int uIndex, RECT& rcHit ) const {
	RECT rcClient;

	YBCtrlImageStatic* pYBImage = NULL;
	YBListBoxIMEItem* pListBoxItem = NULL;

	pListBoxItem = m_containerListBoxItems[ uIndex ];
	if ( m_uSelItemIndex == uIndex ) {
		pYBImage = _getImageStatic( pListBoxItem->m_uImageIdIMEBtnInstall );
	} else {
		pYBImage = _getImageStatic( pListBoxItem->m_uImageIdIMEBtnUse );
	}
	if ( !pYBImage || ( pYBImage->getHeight() > m_uItemHeight ) || ( pYBImage->getWidth() > m_uWidth ) )
		return false;

	::GetClientRect( m_pViewInner->getHWND(), &rcClient );
	if ( uIndex >= m_containerListBoxItems.size() )
		return false;
	rcHit.left = rcClient.left + YBIMEDLG_LSPACE_BTN;
	rcHit.right = rcHit.left + pYBImage->getWidth();
	rcHit.top = uIndex*m_uItemHeight + ( m_uItemHeight - pYBImage->getHeight() )/2;
	rcHit.bottom = rcHit.top + pYBImage->getHeight();

	return true;
}

bool YBView4IMEContent::insertItem( unsigned int uIndexItem, YBListBoxIMEItem* pListBoxItem, item_id_type& itemId ) {
	if (!m_pViewInner ||
		!pListBoxItem || 
		!::IsWindow( m_pViewInner->getHWND() ) ||
		!pListBoxItem->m_pBtnRadio ||
		uIndexItem > m_containerListBoxItems.size() ) {
			assert( false );
			return false;
	}
	//
	if ( !pListBoxItem->m_pBtnRadio->Create( 0, WS_CHILD, 0, 0, 0, 0, m_pViewInner->getHWND(), (HMENU)pListBoxItem->m_wBtnRadioCtrlId, NULL ) ) {
		assert( false );
		return false;
	}
	//
	itemId = _allocalItemId();
	pListBoxItem->_setItemId( itemId );
	//
	m_containerListBoxItems.insert( m_containerListBoxItems.begin() + uIndexItem, pListBoxItem );

	return true;
}

bool YBView4IMEContent::push_front( YBListBoxIMEItem* pListBoxItem, item_id_type& itemId ) {
	if (!m_pViewInner ||
		!pListBoxItem || 
		!::IsWindow( m_pViewInner->getHWND() ) ){ 
			assert( false );
			return false;
	}

	if ( !insertItem( 0, pListBoxItem, itemId ) ) {
		assert( false );
		return false;
	}

	return true;
}

void YBView4IMEContent::_loadFont(){
	LOGFONT lFont;

	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 18;

	m_uFontIdIMEItem = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 8, 43, 77 ) );
	//
	memset( &lFont, 0, sizeof(LOGFONT) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 18;

	m_uFontIdIMEItemBtnTxt = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );

}

bool YBView4IMEContent::moveTest( const POINT& ptInContent, unsigned int& uIndexItem ) {
	int nItemCount = m_containerListBoxItems.size();
	RECT rcItem;

	for ( int i = 0; i!=nItemCount; ++i ) {
		if ( !_getItemRect( i, rcItem ) )
			return false;
		if ( ::PtInRect( &rcItem, ptInContent ) ) {
			uIndexItem = i;
			return true;
		}
	}

	return false;
}

bool YBView4IMEContent::moveHitTest( const POINT& ptInContent, unsigned int& uIndexItem ) {
	int nItemCount = m_containerListBoxItems.size();
	RECT rcHit;

	for ( int i = 0; i!=nItemCount; ++i ) {
		if ( !getHitRect( i, rcHit ) )
			return false;
		if ( ::PtInRect( &rcHit, ptInContent ) ) {
			uIndexItem = i;
			return true;
		}
	}

	return false;
}

bool YBView4IMEContent::setSelItem( unsigned int uIndexItem ) {
	if ( uIndexItem >= m_containerListBoxItems.size() ) {
		assert( false );
		return false;
	}
	//
	m_uSelItemIndex = uIndexItem;
	//
	if ( ::IsWindow( m_pViewInner->getHWND() ) ) {
		::InvalidateRect( m_pViewInner->getHWND(), NULL, FALSE );
	}
	return true;
}

void YBView4IMEContent::setItemInvalid( unsigned int uIndexItem ) {
	m_uSelItemIndex = uIndexItem;
}

bool YBView4IMEContent::setSelInItem( unsigned int uIndexItem ) {
	if ( uIndexItem >= m_containerListBoxItems.size() ) {
		assert( false );
		return false;
	}
	//
	m_uSelInItemIndex = uIndexItem;
	//
	if ( ::IsWindow( m_pViewInner->getHWND() ) ) {
		::InvalidateRect( m_pViewInner->getHWND(), NULL, FALSE );
	}
	return true;
}

void YBView4IMEContent::setInItemInvalid( unsigned int uIndexItem ) {
	m_uSelInItemIndex = uIndexItem;
}

void YBView4IMEContent::deleteMenuItemByIndex( unsigned int uIndex ) {
	YBListBoxIMEItem* pLB4WSAWMenuItem = NULL;
	YBListBoxIMEItem* pLB4WSAWMenuItemTmp = NULL;

	pLB4WSAWMenuItem = m_containerListBoxItems[ uIndex ];
	if( !pLB4WSAWMenuItem ) {
		assert( false );
		return;
	}

	listboxitem_container_type::iterator itLB4WSAWItem, iendLB4WSAWItem;
	iendLB4WSAWItem = m_containerListBoxItems.end();
	for ( itLB4WSAWItem = m_containerListBoxItems.begin(); itLB4WSAWItem != iendLB4WSAWItem; ++itLB4WSAWItem ) {
		pLB4WSAWMenuItemTmp = *itLB4WSAWItem;
		if ( pLB4WSAWMenuItemTmp == pLB4WSAWMenuItem ) {
			delete pLB4WSAWMenuItem;
			m_containerListBoxItems.erase( itLB4WSAWItem );
			break;
		}
	}
}

void YBView4IMEContent::deleteMenuItemById( unsigned int uItemId ) {
	if ( !_hasItemId( uItemId ) ) {
		assert( false );
		return;
	}
	YBListBoxIMEItem* pLB4WSAWMenuItem = NULL;
	listboxitem_container_type::iterator itLB4WSAWItem, iendLB4WSAWItem;

	iendLB4WSAWItem = m_containerListBoxItems.end();
	for ( itLB4WSAWItem = m_containerListBoxItems.begin(); itLB4WSAWItem != iendLB4WSAWItem; ++itLB4WSAWItem ) {
		pLB4WSAWMenuItem = *itLB4WSAWItem;
		if ( pLB4WSAWMenuItem->getItemId() == uItemId ) {
			delete pLB4WSAWMenuItem;
			m_containerListBoxItems.erase( itLB4WSAWItem );
			break;
		}
	}

}

YBListBoxIMEItem* YBView4IMEContent::getItemById( item_id_type itemId ) {
	unsigned int uItemIndex;
	YBListBoxIMEItem* pLB4WSAWMenuItem = NULL;

	itemId2Index( itemId, uItemIndex );
	pLB4WSAWMenuItem = m_containerListBoxItems[ uItemIndex ];

	return pLB4WSAWMenuItem;
}

YBView4IMEContent::item_id_type YBView4IMEContent::_allocalItemId() const {
	static item_id_type s_idBase = YBCTRL_MENU_ID_BASE;
	return ++s_idBase;
}

bool YBView4IMEContent::itemId2Index( item_id_type idItem, unsigned int& uIndex ) const {
	if ( m_containerListBoxItems.size() == 0 ) {
		return false;
	}

	listboxitem_container_type::const_iterator citItem, ciendItem;

	ciendItem = m_containerListBoxItems.end();
	for ( citItem = m_containerListBoxItems.begin(); citItem != ciendItem; ++citItem ) {
		if ( (*citItem)->getItemId() == idItem ) {
			uIndex = citItem - m_containerListBoxItems.begin();
			return true;
		}
	}

	return false;
}

bool YBView4IMEContent::index2ItemId( unsigned int uIndex, item_id_type& idItem ) const {
	if ( m_containerListBoxItems.size() == 0 ) {
		return false;
	}
	YBListBoxIMEItem* pLB4WSAWMenuItem = NULL;

	pLB4WSAWMenuItem = m_containerListBoxItems[ uIndex ];
	idItem = pLB4WSAWMenuItem->getItemId();

	return true;
}

bool YBView4IMEContent::hasItemId( unsigned int uItemId ) const {
	return _hasItemId( uItemId );
}

bool YBView4IMEContent::_hasItemId( unsigned int uItemId ) const {
	listboxitem_container_type::const_iterator citItem, ciendItem;

	ciendItem = m_containerListBoxItems.end();
	for ( citItem = m_containerListBoxItems.begin(); citItem != ciendItem; ++citItem ) {
		if ( (*citItem)->getItemId() == uItemId ) {
			return true;
		}
	}

	return false;
}