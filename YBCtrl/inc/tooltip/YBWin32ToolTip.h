#ifndef __YBWIN32TOOLTIP_H__
#define __YBWIN32TOOLTIP_H__
#include <string>
#include "tchar.h"
#include <CommCtrl.h>
//
class YBWin32Tooltip {
public:
	typedef std::basic_string< TCHAR > tstring_type;
public:
	~YBWin32Tooltip();
	static YBWin32Tooltip* getInstance();

public:
	void updateTipText( LPCTSTR lpszText, HWND hWndDest, UINT_PTR nIDTool );

private:
	YBWin32Tooltip();
	YBWin32Tooltip( const YBWin32Tooltip& );
	bool _createWnd();
	void _fillInToolInfo(TOOLINFO& ti, HWND hWndDest, UINT_PTR nIDTool) const;
	void _delTool( HWND hWndDest, UINT_PTR nIDTool = 0 );
	bool _addTool( HWND hWndDest, const tstring_type& tstrTooltip, LPCRECT lpRectTool = NULL, UINT_PTR nIDTool = 0 );
	void _activate( bool bActivate );
	int _getToolCount();

private:
	HWND m_hWnd;
	HWND m_hWndDest;
};


#endif //__YBWIN32TOOLTIP_H__