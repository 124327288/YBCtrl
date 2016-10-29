#include "stdafx.h"
#include "YBElawDropDownMenu.h"
#include "YBElawDropDownMenuItem.h"
#include "loader/YBImageLoader.h"
#include "frame/YBCtrlFontDepository.h"

YBElawDropDownMenu::YBElawDropDownMenu()
: m_menu4ElawDDMenu( 100 )
, m_uFontIdMenuTxt( YBCTRL_FONT_ID_INVALID )
, m_uImageIdMenuIcon( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdMenuItemBk( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdMenuHover( YBCTRL_IMAGE_ID_INVALID )
, m_uImageIdMenuItemDisable( YBCTRL_IMAGE_ID_INVALID ) {
	_loadFont();
	_loadImage();
	_init();
}

YBElawDropDownMenu::~YBElawDropDownMenu() {

}

void YBElawDropDownMenu::_loadFont() {
	LOGFONT lFont;
	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T( "微软雅黑" ) );
	lFont.lfHeight = 20;
	m_uFontIdMenuTxt = YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 27, 41, 62 ) );
}

void YBElawDropDownMenu::_loadImage() {
	TCHAR szModulePath[ MAX_PATH ];
	TCHAR* posTmp = NULL;
	GetModuleFileName( NULL, szModulePath, MAX_PATH );
	std::basic_string< TCHAR > tstrImageDirPath;
	std::basic_string< TCHAR > tstrImageFilePath;
	unsigned int idImage;

	posTmp = _tcsrchr( szModulePath, _T( '\\' ) );
	if ( !posTmp ){
		assert( false );
		return;
	}
	posTmp += 1;
	*posTmp = 0;

	tstrImageDirPath = szModulePath;
	tstrImageDirPath += _T( "Image\\" );
	//
	tstrImageFilePath = tstrImageDirPath + _T( "YBMenuItemBk.png" );
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdMenuItemBk = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T( "YBMenuItemHover.png" );
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdMenuHover = idImage;
	//
	tstrImageFilePath = tstrImageDirPath + _T( "YBMenuItemDisable.png" );
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	m_uImageIdMenuItemDisable = idImage;
	//
}

void YBElawDropDownMenu::_init() {
	unsigned int uItemHeight = 30;
	//
	YBElawDropDownMenuItem* pYBMenuItem1 = NULL;
	YBCtrlMenu::item_id_type uItemId = 0;
	unsigned int index = 0;
	pYBMenuItem1 = new YBElawDropDownMenuItem();
	pYBMenuItem1->m_tstrTxt = _T( "王小一" );
	pYBMenuItem1->m_uFontIdTxt = m_uFontIdMenuTxt;
	pYBMenuItem1->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem1->m_uImageIdIcon = m_uImageIdMenuIcon;
	pYBMenuItem1->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem1->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem1->m_uItemHeight = uItemHeight;
	pYBMenuItem1->setCanClickFlag( true );
	pYBMenuItem1->setDisableFlag( false );
	m_menu4ElawDDMenu.insertItem( index, pYBMenuItem1, uItemId );
	//
	YBElawDropDownMenuItem* pYBMenuItem2 = NULL;
	YBCtrlMenu::item_id_type uItemId2 = 0;
	unsigned int index1 = 1;
	pYBMenuItem2 = new YBElawDropDownMenuItem();
	pYBMenuItem2->m_tstrTxt = _T( "王小二" );
	pYBMenuItem2->m_uFontIdTxt = m_uFontIdMenuTxt;
	pYBMenuItem2->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem2->m_uImageIdIcon = m_uImageIdMenuIcon;
	pYBMenuItem2->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem2->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem2->m_uItemHeight = uItemHeight;
	pYBMenuItem2->setCanClickFlag( true );
	pYBMenuItem2->setDisableFlag( false );
	m_menu4ElawDDMenu.insertItem( index1, pYBMenuItem2, uItemId2 );
	//
	YBElawDropDownMenuItem* pYBMenuItem3 = NULL;
	YBCtrlMenu::item_id_type uItemId3 = 0;
	unsigned int index2 = 2;
	pYBMenuItem3 = new YBElawDropDownMenuItem();
	pYBMenuItem3->m_tstrTxt = _T( "王小三" );
	pYBMenuItem3->m_uFontIdTxt = m_uFontIdMenuTxt;
	pYBMenuItem3->m_uImageIdBk = m_uImageIdMenuItemBk;
	pYBMenuItem3->m_uImageIdIcon = m_uImageIdMenuIcon;
	pYBMenuItem3->m_uImageIdHover = m_uImageIdMenuHover;
	pYBMenuItem3->m_uImageIdDisable = m_uImageIdMenuItemDisable;
	pYBMenuItem3->m_uItemHeight = uItemHeight;
	pYBMenuItem3->setCanClickFlag( true );
	pYBMenuItem3->setDisableFlag( false );
	pYBMenuItem3->m_bDrawDivideLine = true;
	m_menu4ElawDDMenu.insertItem( index2, pYBMenuItem3, uItemId3 );
}