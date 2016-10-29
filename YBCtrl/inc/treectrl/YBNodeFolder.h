#ifndef __YBNODEFOLDER_H__
#define __YBNODEFOLDER_H__
#include "treectrl/YBNodeBase.h"
#include <list>

YBCTRL_NAMESPACE_BEGIN

class YBNodeLeaf;
//
class YBCTRL_API YBNodeFolder : public YBNodeBase {
public:
	typedef std::basic_string< TCHAR > tstring_type;
	typedef std::list< YBNodeBase* > node_container_type;
	typedef void( *pfnEnumSubNodeProcType )( const YBNodeBase* pNode, void* pParam, bool* pbContinue );
public:
	YBNodeFolder();
	virtual ~YBNodeFolder();

	// interface of YBNodeBase
protected:
	virtual void _calcPosition( unsigned int uXPosStart, unsigned int uYPosStart );
	virtual void _onWndMsg( HWND hWnd, unsigned int uMessage, WPARAM wParam, LPARAM lParam, PARAMONWNDMSG* pParamOnWndMsg );

protected:
	void _drawText( HDC hDC ) const;

private:
	virtual unsigned int _getHeight() const;
	virtual unsigned int _getWidth() const;
	virtual void _drawStatus( HDC hDC ) const;
	virtual void _drawSelf( HDC hDC ) const;
	// interface
private:
	virtual unsigned int _getIndentSpace() const;

private:
	void _drawSubNodes( HDC hDC ) const;
	void _calcSelfPos( unsigned int uXPosStart, unsigned int uYPosStart );
	void _calcSubNodesPos( unsigned int uXPosStart, unsigned int uYPosStart );
	void _onEvtFolderNodeClick( HWND hWnd, PARAMONWNDMSG* pParamOnWndMsg );

public:
	bool insertSubNodeByIndex( unsigned int uIndex, YBNodeBase* pNodeNew );
	void enumSubNodes( pfnEnumSubNodeProcType pfnEnumSubNodeProc, void* pParam ) const;
	YBNodeBase* getSubNode( unsigned int uNodeIdDest ) const;
	YBNodeBase* getDescendantNode( unsigned int uNodeIdDest ) const;
	inline bool isFold() const;
	unsigned int getAllSubNodeHeight() const;
	unsigned int getWidthIncludeAllSubNode() const;
	bool HitTest( const POINT& ptInContent, node_id_type& uNodeId );
	bool MoveTest( const POINT& ptInContent, node_id_type& uNodeId );
	void setFold( unsigned int bFold );
	void clearAll();

protected:
	unsigned int m_uFlag;
	unsigned int m_uWidth;
	unsigned int m_uHeight;
	unsigned int m_uIndentSpace;

public:
	unsigned int m_uFontIdTextNor;
#pragma warning(push)
#pragma warning(disable:4251)
	tstring_type m_tstrText;
	node_container_type m_containerNodes;
#pragma warning(pop)

};

YBCTRL_NAMESPACE_END

#endif //__YBNODEFOLDER_H__