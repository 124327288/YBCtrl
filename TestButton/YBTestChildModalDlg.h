#ifndef __YBTESTCHILDMODALDLG_H__
#define __YBTESTCHILDMODALDLG_H__
#include "dialog/YBChildModalDialog.h"

class YBTestChildModalDlg : public ybctrl::YBChildModalDialog {
public:
	YBTestChildModalDlg( bool bAutoExit = false, unsigned int uExitTimeMS = 2000 );
	virtual ~YBTestChildModalDlg();

	// interface
protected:
	virtual void _createUI( HWND hWndModalDlg, HINSTANCE hInst );
	virtual unsigned int _getWndHeight() const;
	virtual unsigned int _getWndWidth() const;

private:
	void loadImage();

};


#endif //__YBTESTCHILDMODALDLG_H__