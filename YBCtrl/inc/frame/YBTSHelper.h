#ifndef __YBTSHELPER_H__
#define __YBTSHELPER_H__
#include "frame/YBCtrlEnvCfg.h"

YBCTRL_NAMESPACE_BEGIN
//
class YBTSHelper {
public:
    YBTSHelper();
    virtual ~YBTSHelper();

public:
    bool safeEnterFunc() const;
    void safeExitFunc() const;
    void cancelAllAccess() const;
    HANDLE getExitEvent() const { return m_hTSEventExit; }

private:
    HANDLE m_hTSEventAccessSafe;
    HANDLE m_hTSEventExit;
    mutable DWORD m_dwTSFlag;
};
YBCTRL_NAMESPACE_END

#endif //__YBTSHELPER_H__





















