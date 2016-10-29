#ifndef __YBNODEBASE_H__
#define __YBNODEBASE_H__
#include "frame/YBCtrlEnvCfg.h"

YBCTRL_NAMESPACE_BEGIN

class YBCTRL_API YBNodeBase {
	friend class YBNodeFolder;
public:
	typedef unsigned int node_id_type;
	typedef enum _ENUMNODETYPE {
		ENODE_TYPE_UNKNOWN = -1,
		ENODE_TYPE_LEAF,
		ENODE_TYPE_FOLDER
	} ENUMNODETYPE;
	struct PARAMONWNDMSG {
		PARAMONWNDMSG(){ m_bNeedReCalcPos = false; };
		~PARAMONWNDMSG(){};
		bool m_bNeedReCalcPos;
	};
public:
	YBNodeBase( ENUMNODETYPE eNodeType = ENODE_TYPE_UNKNOWN );
	virtual ~YBNodeBase();

	// interface.
protected:
	virtual void _calcPosition( unsigned int uXPosStart, unsigned int uYPosStart ) = 0;
private:
	virtual unsigned int _getHeight() const = 0;
	virtual unsigned int _getWidth() const = 0;
	virtual void _drawStatus( HDC hDC ) const = 0;
	virtual void _onWndMsg( HWND hWnd, unsigned int uMessage, WPARAM wParam, LPARAM lParam, PARAMONWNDMSG* pParamOnWndMsg ) = 0;

public:
	node_id_type getId() const { return m_uNodeId; };
	ENUMNODETYPE getType() const { return m_eNodeType; }
	inline unsigned int getHeight() const { return _getHeight(); }
	inline unsigned int getWidth() const { return _getWidth(); }
	void drawStatus( HDC hDC ) const;
	void onWndMsg( HWND hWnd, unsigned int uMessage, WPARAM wParam, LPARAM lParam, PARAMONWNDMSG* pParamOnWndMsg );

protected:
	node_id_type _allcoateId() const;

protected:
	node_id_type m_uNodeId;
	ENUMNODETYPE m_eNodeType;
	RECT m_rcInContent;
};

YBCTRL_NAMESPACE_END
#endif //__YBNODEBASE_H__