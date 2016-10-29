#ifndef __TASKENGINE_H__
#define __TASKENGINE_H__
#include "frame/YBCtrlEnvCfg.h"
#include <deque>
//
YBCTRL_NAMESPACE_BEGIN
//////////////////////////////////////////////////////////////////////////
#define YBITTASKID_INVALID (0xFFFFFFFF)
#define YBITTASKID_BEGIN (0x00000000)
//
class YBITTaskEngine;
//
class YBCTRL_API YBITTaskBase
{
	friend class YBITTaskEngine;
public:
	YBITTaskBase();
	virtual ~YBITTaskBase();

public:
	virtual void Run() = 0;

public:
	unsigned int getTaskId() const;

protected:
    bool _isNeedExitTask(unsigned int uTimeMS = 0);

private:
	void setEvent( HANDLE hEventTaskEngineExit, HANDLE hEventCurTaskExit );
	void setTaskId( unsigned int uTaskId );

protected:
	HANDLE m_hEventTaskEngineExit;
	HANDLE m_hEventCurTaskExit;
private:
	unsigned int m_uTaskId;
};

class YBCTRL_API YBITTaskEngine {
public:
	typedef unsigned int task_id_type;
	typedef std::deque< YBITTaskBase* > task_container_type;
public:
	YBITTaskEngine();
	virtual ~YBITTaskEngine();

public:
	bool pushbackTask( YBITTaskBase* pTask, task_id_type& taskId );
	bool cancelTask( task_id_type idTask );
	void cancelAllTask();
	bool getCurTaskId( task_id_type& idTask );

private:
	YBITTaskBase* _popupTask();
	bool _safeEnterFunc();
	void _safeExitFunc();
	static DWORD WINAPI TaskEngineThreadProc( LPVOID lpParameter );
	task_id_type _allocalTaskId();
	void _cancelAllTask();
	bool _isRunning();
	void _clearAllTask();
	
private:
	task_id_type m_curTaskId; // 
	unsigned int m_uFlag;
	HANDLE m_hThreadTaskEngine;
	HANDLE m_hEventAccessSafe;
	HANDLE m_hEventExitTaskEngine;
	HANDLE m_hEventExitCurTask;
	
#pragma warning(push)
#pragma warning(disable:4251)
	task_container_type m_containerTask;
#pragma warning(pop)
};

YBCTRL_NAMESPACE_END

#endif //__TASKENGINE_H__