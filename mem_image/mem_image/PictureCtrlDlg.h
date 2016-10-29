#ifndef __PICTURECTRLDLG_H__
#define __PICTURECTRLDLG_H__
#include "frame/YBCtrlBase.h"
#include "frame/YBCtrlImageStatic.h"
#include "PictureCtrl.h"
#include "Base64Coder.h"
#include "FtpAndHttpDownload.h"

class PictureCtrlDlg : public ybctrl::YBCtrlBase {
public:
	typedef std::basic_string< TCHAR > tstring_type;
public:
	PictureCtrlDlg( const tstring_type& tstrAppPath );
	~PictureCtrlDlg();

public:
	YBCTRL_WNDMSGMAP_DECLARE_WITHNAMESPACE()

public:
	HWND create( int uWidth, int uHeight );
	//解码图片数据并显示.
	void decodeImageData( string tstrData );

private:
	void _onWM_PAINT(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg);
	void _onWM_ERASEBKGND(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg);
	void _onWM_NCHITTEST(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg);

private:
	void _drawBk( HDC hDC );
	ybctrl::YBCtrlImageStatic* _getImageStatic(unsigned int uImageId);

private:
	void _loadImage();


private:
	unsigned int m_uImageIdBk;
	tstring_type m_tstrAppPath;
	//
	PictureCtrl m_dlg4PicCtrl;
	Base64Coder m_base64Coder;
	FtpAndHttpDownload m_download;
};

#endif //__PICTURECTRLDLG_H__