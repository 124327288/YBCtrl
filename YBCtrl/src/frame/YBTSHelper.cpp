#include "stdafx.h"
#include "frame/YBTSHelper.h"
#include <assert.h>

USING_NAMESPACE_YBCTRL
//
#define HITTEST_BIT( val, bit ) ( ( val & bit ) == bit )
#define YBTSHELPER_FLAG_NONE (0x00000000)
#define YBTSHELPER_FLAG_EXIT (0x00000001)
//
YBTSHelper::YBTSHelper()
: m_hTSEventAccessSafe( NULL )
, m_hTSEventExit( NULL )
, m_dwTSFlag( YBTSHELPER_FLAG_NONE ) {
    m_hTSEventAccessSafe = ::CreateEvent( NULL, FALSE, TRUE, NULL );
    m_hTSEventExit = ::CreateEvent( NULL, TRUE, FALSE, NULL );
}

YBTSHelper::~YBTSHelper() {
    cancelAllAccess();
    ::Sleep( 1 );
    ::CloseHandle( m_hTSEventAccessSafe );
    ::CloseHandle( m_hTSEventExit );
    m_hTSEventAccessSafe = NULL;
    m_hTSEventExit = NULL;
}

bool YBTSHelper::safeEnterFunc() const {
    DWORD dwWaitRet = 0;
    HANDLE aryEvent[ 2 ];

    if ( HITTEST_BIT( m_dwTSFlag, YBTSHELPER_FLAG_EXIT ) ) {
        return false;
    }
    aryEvent[ 0 ] = m_hTSEventExit;
    aryEvent[ 1 ] = m_hTSEventAccessSafe;
    dwWaitRet = ::WaitForMultipleObjects( 2, aryEvent, FALSE, INFINITE );
    if ( dwWaitRet == WAIT_OBJECT_0 ) {
        return false; // m_hTSEventExit
    } else if ( dwWaitRet == WAIT_OBJECT_0 + 1 ) {
        return true; // m_hTSEventAccessSafe
    }

    assert( false );
    return false;
}

void YBTSHelper::safeExitFunc() const {
    assert( m_hTSEventAccessSafe );
    ::SetEvent( m_hTSEventAccessSafe );
}

void YBTSHelper::cancelAllAccess() const {
    m_dwTSFlag |= YBTSHELPER_FLAG_EXIT;
    ::SetEvent( m_hTSEventExit );
}
