#ifndef __YBCTRLENVCFG_H__
#define __YBCTRLENVCFG_H__

#define YBCTRL_NAMESPACE_BEGIN namespace ybctrl {
#define YBCTRL_NAMESPACE_END };
#define USING_NAMESPACE_YBCTRL using namespace ybctrl;

#ifdef YBCTRL_STATICLIB
	#define YBCTRL_API
#else 
	#ifdef YBCTRL_EXPORTS
	#define YBCTRL_API __declspec(dllexport)
	#else
	#define YBCTRL_API __declspec(dllimport)
	#endif //YBCTRL_EXPORTS
#endif //_LIB

#define EXTERN_C_START extern "C" {
#define EXTERN_C_END }

#endif //__YBCTRLENVCFG_H__