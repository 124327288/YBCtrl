#ifndef __YBCTRLMSGBOX_H__
#define __YBCTRLMSGBOX_H__
#include "YBPopupMessageBox.h"
#include <string>

typedef std::basic_string< TCHAR > tstring_type;
int YBCtrlMsgBox( const tstring_type& tstrText, const tstring_type& tstrCaption = _T("Œ¬‹∞Ã· æ"), UINT uType = YBMB_OK|YBMB_ICONWARNING );

#endif //__YBCTRLMSGBOX_H__