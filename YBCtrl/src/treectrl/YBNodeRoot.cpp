#include "stdafx.h"
#include "treectrl/YBNodeRoot.h"
#include <assert.h>
//
USING_NAMESPACE_YBCTRL

YBNodeRoot::YBNodeRoot() {

}

YBNodeRoot::~YBNodeRoot() {

}

unsigned int YBNodeRoot::_getHeight() const {
	return 0;
}

unsigned int YBNodeRoot::_getWidth() const {
	return 0;
}

unsigned int YBNodeRoot::_getIndentSpace() const {
	return 0;
}

void YBNodeRoot::calcAllSubNodePos() {
	_calcPosition( 0, 0 );
}