#include "stdafx.h"
#include "listbox/YBLB4SB.h"

USING_NAMESPACE_YBCTRL;

YBLB4SBViewInner::YBLB4SBViewInner()
: YBViewInner( new YBLB4SBViewConntent() ) {
}

YBLB4SBViewInner::~YBLB4SBViewInner() {

}

YBCTRL_WNDMSGMAP_BEGIN( YBLB4SBViewInner, YBCtrlBase )
YBCTRL_WNDMSGMAP_END()

//////////////////////////////////////////////////////////////////////////
YBLB4SBViewConntent::YBLB4SBViewConntent() {

}

YBLB4SBViewConntent::~YBLB4SBViewConntent() {

}

unsigned int YBLB4SBViewConntent::getWidth() const {
	return 300;
}

unsigned int YBLB4SBViewConntent::getHeight() const {
	return 200;
}

void YBLB4SBViewConntent::_drawStatus( HDC hDC, YBViewInner* pYBViewInner ) const {

}

