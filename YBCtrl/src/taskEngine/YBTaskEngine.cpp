#include "stdafx.h"
#include "taskEngine/YBTaskEngine.h"
#include "frame/FuncPack.h"
#include "frame/MemFuncPack.h"
#include "assert.h"
//
USING_NAMESPACE_YBCTRL
//////////////////////////////////////////////////////////////////////////
#ifndef HITTEST_BIT
#define HITTEST_BIT( val, bit ) ( ( val & bit ) == bit )
#endif //HITTEST_BIT
#define YBITTE_FLAG_NONE (0x00000000)
#define YBITTE_FLAG_RUNNING (0x00000001)
#define YBITTE_FLAG_ENGINEEXITING (0x00000002)
//////////////////////////////////////////////////////////////////////////
YBITTaskBase::YBITTaskBase() 
: m_hEventTaskEngineExit( NULL )
, m_hEventCurTaskExit( NULL )
, m_uTaskId( YBITTASKID_INVALID ) {
}

YBITTaskBase::~YBITTaskBase() {
}

bool YBITTaskBase::_isNeedExitTask(unsigned int uTimeMS/* = 0*/) {
	assert( m_hEventCurTaskExit );
	assert( m_hEventTaskEngineExit );
	HANDLE aryEvent[ 2 ];
	DWORD dwWaitRet;

	aryEvent[ 0 ] = m_hEventCurTaskExit;
	aryEvent[ 1 ] = m_hEventTaskEngineExit;
    dwWaitRet = ::WaitForMultipleObjects(2, aryEvent, FALSE, uTimeMS);
	if ( dwWaitRet == WAIT_OBJECT_0 ) { // m_hEventCurTaskExit
		return true;
	} else if ( dwWaitRet == ( WAIT_OBJECT_0 + 1 ) ) { // m_hEventTaskEngineExit
		return true;
	}
	
	return false;
}

void YBITTaskBase::setEvent( HANDLE hEventTaskEngineExit, HANDLE hEventCurTaskExit ) {
	m_hEventTaskEngineExit = hEventTaskEngineExit;
	m_hEventCurTaskExit = hEventCurTaskExit;
}

void YBITTaskBase::setTaskId( unsigned int uTaskId ) {
	assert( m_uTaskId == YBITTASKID_INVALID );
	m_uTaskId = uTaskId;
}

unsigned int YBITTaskBase::getTaskId() const { 
	assert( m_uTaskId != YBITTASKID_INVALID );
	return m_uTaskId; 
}

//////////////////////////////////////////////////////////////////////////
YBITTaskEngine::YBITTaskEngine()
: m_uFlag( YBITTE_FLAG_NONE )
, m_hThreadTaskEngine( NULL )
, m_hEventAccessSafe( NULL )
, m_hEventExitTaskEngine( NULL )
, m_hEventExitCurTask( NULL )
, m_curTaskId( YBITTASKID_INVALID )
{
//	{
//#ifdef DEBUG
//		static int s_idBase = 0;
//
//		YBLOG_DEBUG( _T("YBITTaskEngine id:%d\n"), s_idBase++ );
//#endif //DEBUG
//	}

	DWORD dwThreadID;
	//
	m_hEventAccessSafe = ::CreateEvent( NULL, FALSE, TRUE, NULL );
	m_hEventExitTaskEngine = ::CreateEvent( NULL, TRUE, FALSE, NULL );
	m_hEventExitCurTask = ::CreateEvent( NULL, TRUE, FALSE, NULL );
	m_hThreadTaskEngine = ::CreateThread( NULL, 0, TaskEngineThreadProc, this, 0, &dwThreadID );
}

YBITTaskEngine::~YBITTaskEngine() {
	//
	m_uFlag |= YBITTE_FLAG_ENGINEEXITING;
	//
	_cancelAllTask();
	//
	::Sleep( 1 );
	//
	::CloseHandle( m_hEventExitTaskEngine );
	::CloseHandle( m_hEventAccessSafe );
	::CloseHandle( m_hEventExitCurTask );
	::CloseHandle( m_hThreadTaskEngine );
	//
	m_hThreadTaskEngine = NULL;
	m_hEventAccessSafe = NULL;
	m_hEventExitTaskEngine = NULL;
	m_hEventExitCurTask = NULL;
	//
	_clearAllTask();

//	{
//#ifdef DEBUG
//		static int s_idBase = 0;
//
//		YBLOG_DEBUG( _T( "~YBITTaskEngine id:%d\n" ), s_idBase++ );
//#endif //DEBUG
//	}
}

bool YBITTaskEngine::pushbackTask( YBITTaskBase* pTask, task_id_type& taskId ) {
	if ( !pTask ) {
		assert( false );
		return false;
	}
	if ( !_safeEnterFunc() ) {
		delete pTask;
		pTask = NULL;
		return false;
	}
	CMemFuncPack mfpkExitFunc( this, &YBITTaskEngine::_safeExitFunc );
	//
	taskId = _allocalTaskId();
	pTask->setEvent( m_hEventExitTaskEngine, m_hEventExitCurTask );
	pTask->setTaskId( taskId );
	m_containerTask.push_back( pTask );
	return true;
}

void YBITTaskEngine::cancelAllTask() {
	if ( !_safeEnterFunc() ) {
		return;
	}
	CMemFuncPack mfpkSafeExit( this, &YBITTaskEngine::_safeExitFunc );
	task_container_type::iterator itTask, iendTask;
	YBITTaskBase* pTaskObj = NULL;

	// cancel all task in queue.
	iendTask = m_containerTask.end();
	for ( itTask = m_containerTask.begin(); itTask!=iendTask; ++itTask ) {
		pTaskObj = *itTask;
		if ( pTaskObj ) {
			delete pTaskObj;
			pTaskObj = NULL;
		}
	}
	m_containerTask.clear();

	// cancel the current task
	::SetEvent( m_hEventExitCurTask );
	m_curTaskId = YBITTASKID_INVALID;
}

void YBITTaskEngine::_cancelAllTask() {
	::SetEvent( m_hEventExitTaskEngine );
	::WaitForSingleObject( m_hThreadTaskEngine, INFINITE );
}

bool YBITTaskEngine::_isRunning() {
	return HITTEST_BIT( m_uFlag, YBITTE_FLAG_RUNNING );
}

bool YBITTaskEngine::getCurTaskId( task_id_type& idTask ) {
	if ( !_safeEnterFunc() ) {
		return false;
	}
	CMemFuncPack mfpkExitFunc( this, &YBITTaskEngine::_safeExitFunc );

	idTask = m_curTaskId;
	return (idTask!=YBITTASKID_INVALID);
}

bool YBITTaskEngine::cancelTask( task_id_type idTask ) {
	if ( !_safeEnterFunc() ) {
		return false;
	}
	CMemFuncPack mfpkExitFunc( this, &YBITTaskEngine::_safeExitFunc );
	task_container_type::iterator itTask, iendTask;
	YBITTaskBase* pTask = NULL;

	if ( m_containerTask.size() > 0 ) {
		iendTask = m_containerTask.end();
		for ( itTask = m_containerTask.begin(); itTask!=iendTask; ++itTask ) {
			if ( idTask == (*itTask)->getTaskId() ) {
				m_containerTask.erase( itTask );
				return true;
			}
		}
	}

	if ( idTask == m_curTaskId ) {
		// if task is running, set exit current task event.
		::SetEvent( m_hEventExitCurTask );
		m_curTaskId = YBITTASKID_INVALID;
	}

	return true;
}

void YBITTaskEngine::_clearAllTask() {
	task_container_type::iterator itTask, iendTask;
	YBITTaskBase* pTask = NULL;

	iendTask = m_containerTask.end();
	for ( itTask = m_containerTask.begin(); itTask!=iendTask; ++itTask ) {
		pTask = *itTask;
		delete pTask;
		pTask = NULL;
	}
	m_containerTask.clear();
}

YBITTaskBase* YBITTaskEngine::_popupTask() {
	if ( !_safeEnterFunc() ) {
		return false;
	}
	CMemFuncPack mfpkExitFunc( this, &YBITTaskEngine::_safeExitFunc );
	CFuncPack fpkResetExitCurTaskEvent( ::ResetEvent, m_hEventExitCurTask );
	YBITTaskBase* pTask1st = NULL;

	//
	if ( m_containerTask.size() == 0 ) {
		m_curTaskId = YBITTASKID_INVALID;
		return NULL;
	}
	pTask1st = m_containerTask[ 0 ];
	m_containerTask.erase( m_containerTask.begin() );
	// save current task id
	m_curTaskId = pTask1st->getTaskId();

	return pTask1st;
}

bool YBITTaskEngine::_safeEnterFunc() {
	if ( HITTEST_BIT( m_uFlag, YBITTE_FLAG_ENGINEEXITING ) ) {
		return false;
	}
	DWORD dwWaitRet = 0;
	HANDLE aryEvent[ 2 ];

	aryEvent[ 0 ] = m_hEventExitTaskEngine;
	aryEvent[ 1 ] = m_hEventAccessSafe;
	dwWaitRet = ::WaitForMultipleObjects( 2, aryEvent, FALSE, INFINITE );
	if ( dwWaitRet == WAIT_OBJECT_0 ) {
		return false; // m_hEventExitTaskEngine
	} else if ( dwWaitRet == WAIT_OBJECT_0 + 1 ) {
		return true; // m_hEventAccessSafe
	}

	assert( false );
	return false;
}

void YBITTaskEngine::_safeExitFunc() {
	::SetEvent( m_hEventAccessSafe );
}

DWORD WINAPI YBITTaskEngine::TaskEngineThreadProc( LPVOID lpParameter ) {
	if ( !lpParameter )
		return 1;
	YBITTaskEngine* pTaskEngine = NULL;
	YBITTaskBase* pTaskObj = NULL;

	pTaskEngine = (YBITTaskEngine*)lpParameter;
	pTaskEngine->m_uFlag |= YBITTE_FLAG_RUNNING;
	while ( 1 ) {
		pTaskObj = pTaskEngine->_popupTask();
		CFuncPack fpkDelTask( gfnDelObj< YBITTaskBase >, pTaskObj );
		if ( pTaskObj ) {
			pTaskObj->Run();
		}

		// check if need exit thread.
		if ( WAIT_OBJECT_0 == ::WaitForSingleObject( pTaskEngine->m_hEventExitTaskEngine, 0 ) ) {
			break; // exit thread.
		}

		::Sleep( 1 ); // reduce cpu occupy.
	}

	pTaskEngine->m_uFlag &= ~YBITTE_FLAG_RUNNING;
	return 0;
}

YBITTaskEngine::task_id_type YBITTaskEngine::_allocalTaskId() {
	static task_id_type s_idBase = YBITTASKID_BEGIN;
	return ++s_idBase;
}
