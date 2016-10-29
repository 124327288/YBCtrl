#include "stdafx.h"
#include "YBTestBar.h"

YBTestBar::YBTestBar() {

}

YBTestBar::~YBTestBar() {

}

YBCTRL_WNDMSGMAP_BEGIN_WITHNAMESPACE( YBTestBar, YBTopMostBar )
YBCTRL_WNDMSGMAP_END()

extern unsigned int g_imageIdBtnNormal;
extern unsigned int g_imageIdBtnHover;
extern unsigned int g_imageIdBtnPress;
extern unsigned int g_imageIdBtnDisable;
extern unsigned int g_imageIdBtnXNormal;
extern unsigned int g_imageIdBtnXHover;
extern unsigned int g_imageIdBtnXPress;

HWND YBTestBar::Create( int x, int y, int nWidth, int nHeight, HINSTANCE hInstance ) {
	HWND hWnd;

	hWnd = YBTopMostBar::create( x, y, nWidth, nHeight, NULL, hInstance );

	m_btnxELawyer.Create( 0, WS_CHILD|WS_VISIBLE, 5, 1, 100, 20, hWnd, (HMENU)321, hInstance );
	m_btnxELawyer.setFixedSpace( 5, 5, 5, 5 );
	m_btnxELawyer.setImage( ybctrl::YBButtonX::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
	m_btnxELawyer.setImage( ybctrl::YBButtonX::EBTN_STATUS_HOVER, g_imageIdBtnHover );
	m_btnxELawyer.setImage( ybctrl::YBButtonX::EBTN_STATUS_PRESS, g_imageIdBtnPress );
	m_btnxELawyer.setImage( ybctrl::YBButtonX::EBTNX_STATUS_NORMAL, g_imageIdBtnXNormal );
	m_btnxELawyer.setImage( ybctrl::YBButtonX::EBTNX_STATUS_HOVER, g_imageIdBtnXHover );
	m_btnxELawyer.setImage( ybctrl::YBButtonX::EBTNX_STATUS_PRESS, g_imageIdBtnXPress );
	m_btnxELawyer.setIcon(  LoadIcon(NULL, IDI_ERROR), 16, 16 );


	m_btnxBaiDu.Create( 0, WS_CHILD|WS_VISIBLE, 105, 1, 100, 20, hWnd, (HMENU)322, hInstance );
	m_btnxBaiDu.setFixedSpace( 5, 5, 5, 5 );
	m_btnxBaiDu.setImage( ybctrl::YBButtonX::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
	m_btnxBaiDu.setImage( ybctrl::YBButtonX::EBTN_STATUS_HOVER, g_imageIdBtnHover );
	m_btnxBaiDu.setImage( ybctrl::YBButtonX::EBTN_STATUS_PRESS, g_imageIdBtnPress );
	m_btnxBaiDu.setImage( ybctrl::YBButtonX::EBTNX_STATUS_NORMAL, g_imageIdBtnXNormal );
	m_btnxBaiDu.setImage( ybctrl::YBButtonX::EBTNX_STATUS_HOVER, g_imageIdBtnXHover );
	m_btnxBaiDu.setImage( ybctrl::YBButtonX::EBTNX_STATUS_PRESS, g_imageIdBtnXPress );
	m_btnxBaiDu.enableBtnX( false );
	

	return hWnd;
}










