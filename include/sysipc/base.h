#ifndef __SYSIPC_BASE_H
#define __SYSIPC_BASE_H

#if (__cplusplus < 199711L)
#error At least C++98 is required to build SysIPC!
#endif

#if defined(_MSC_VER) && !defined(__GNUC__)
#if (_MSC_VER < 1600)
#error At least Visual Studio 2010 is required to build SysIPC!
#endif
#ifdef __SYSIPC_BUILD
#define SYSIPC_API  __declspec(dllexport)
#else
#define SYSIPC_API  __declspec(dllimport)
#endif
#elif !defined(_MSC_VER) && defined(__GNUC__)
#if (((__GNUC__ == 4) && (__GNUC_MINOR__ < 3)) || (__GNUC__ < 4))
#error At least GCC 4.3 is required to build SysIPC!
#endif
#ifdef __SYSIPC_BUILD
#define SYSIPC_API  __attribute__((visibility("default")))
#else
#define SYSIPC_API
#endif
#endif

#include <iostream>
#include <map>
#include <string>

#include <rapidjson/document.h>

namespace sysipc {

// forward declarations
class IBase;
class IHandler;
class IRouter;
class IServer;
class IClient;

// result type
// layout: 0xABBBCCCC
//  A : 4-bit severity code (0 for success or 8 for failure)
//  B : 12-bit facility code (defined below)
//  C : 16-bit status code (defined in each facility header file)
typedef unsigned int result_t;
#define SYSIPC_SEVERITY(r)                      (((r) >> 28) & 0xF)
#define SYSIPC_FACILITY(r)                      (((r) >> 16) & 0xFFF)
#define SYSIPC_STATUS                           ((r) & 0xFFFF)
#define SYSIPC_RESULT(severity,facility,status) ((((severity) & 0xF) << 28) | (((facility) & 0xFFF) << 16) | ((status) & 0xFFFF))
#define SYSIPC_SEVERITY_SUCCESS                 0x0
#define SYSIPC_SEVERITY_FAILURE                 0x8

// result facilities
#define SYSIPC_FACILITY_GENERIC     0x0000
#define SYSIPC_FACILITY_ROUTER      0x0001
#define SYSIPC_FACILITY_SERVER      0x0002
#define SYSIPC_FACILITY_CLIENT      0x0003

// generic errors
#define SYSIPC_S_OK         SYSIPC_RESULT(SYSIPC_SEVERITY_SUCCESS, SYSIPC_FACILITY_GENERIC, 0x0000)
#define SYSIPC_E_FAIL       SYSIPC_RESULT(SYSIPC_SEVERITY_FAILURE, SYSIPC_FACILITY_GENERIC, 0x0000)
#define SYSIPC_E_INTERNAL   SYSIPC_RESULT(SYSIPC_SEVERITY_FAILURE, SYSIPC_FACILITY_GENERIC, 0x0001)
#define SYSIPC_E_INVALIDARG SYSIPC_RESULT(SYSIPC_SEVERITY_FAILURE, SYSIPC_FACILITY_GENERIC, 0x0002)
#define SYSIPC_E_BADPTR     SYSIPC_RESULT(SYSIPC_SEVERITY_FAILURE, SYSIPC_FACILITY_GENERIC, 0x0003)
#define SYSIPC_E_MEMORY     SYSIPC_RESULT(SYSIPC_SEVERITY_FAILURE, SYSIPC_FACILITY_GENERIC, 0x0004)

// error checking
#define SYSIPC_SUCCEEDED(r) (SYSIPC_SEVERITY(r) == SYSIPC_SEVERITY_SUCCESS)
#define SYSIPC_FAILED(r)    (SYSIPC_SEVERITY(r) == SYSIPC_SEVERITY_FAILURE)

// base interface
class IBase {
public:
    virtual void destroy(void) = 0;
};

// class factory
class Create {
public:
    static SYSIPC_API result_t router(const std::string &name, IRouter **router);
};

};

#endif
