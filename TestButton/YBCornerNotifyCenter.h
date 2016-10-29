#ifndef __YBCORNERNOTIFYCENTER_H__
#define __YBCORNERNOTIFYCENTER_H__
#include <deque>
#include "YBCornerNotifyDlg.h"
//#include "ModuleManager.h"

//////////////////////////////////////////////////////////////////////////
// m_uClickEvtFlag
#define YBCNMSG_CLICKEVTFLAG_NONE (0x00000000)
#define YBCNMSG_CLICKEVTFLAG_OPENURL (0x00000001)
// YBCNMsg
struct YBCNMsg{
	YBCNMsg();
	~YBCNMsg();
	std::basic_string< TCHAR > m_tstrTypeName;
	std::basic_string< TCHAR > m_tstrContentText;
	std::basic_string< TCHAR > m_tstrTime;
	unsigned int m_uExitTimeMS; 
	unsigned int m_uClickEvtFlag;
	int m_nType;
	std::basic_string< TCHAR > m_tstrClickEvtParam;
	pfnWndLBtnDownProcType m_pfnWndLBtnDownProc;
};
  
class YBCornerNotifyCenter{
public:
	typedef std::deque< YBCornerNotifyDlg* > cndlg_container_type;

public:
	YBCornerNotifyCenter();
	virtual ~YBCornerNotifyCenter();

public:
	// 事件通知
	//virtual int onUpdateEvent(int iEventId, void* param);

public:
	void pushback( const YBCNMsg* pYBCNMsg );
	void eraseCNDlg( YBCornerNotifyDlg* pCNDlg );

private:
	void _clearAll();
	bool _calcNotityDlgWndRect( unsigned int uIndex, RECT& rcSWlocation );
	unsigned int _getMaxIndex() const { return m_nIndexMax; };
	unsigned int _allocalNDIndex() const;

private:
	cndlg_container_type m_containerCornerNotifyDlg;
	mutable int m_nIndexMax;
	unsigned int m_uWidthCNDlg;
	unsigned int m_uHeightCNDlg;
	unsigned int m_uBottomSpace; //
	unsigned int m_uRightSpace; //
};

#endif // __YBCORNERNOTIFYCENTER_H__