#include "stdafx.h"
#include "YBCtrlMsgBox.h"

int YBCtrlMsgBox( const tstring_type& tstrText, const tstring_type& tstrCaption/* = _T("��ܰ��ʾ")*/, UINT uType/* = YBMB_OK|YBMB_ICONWARNING*/ ) {
	return YBPopupMessageBox::getInstance()->YBMessageBox( tstrText, tstrCaption, uType );
}