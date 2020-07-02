#ifndef __SYSIPC_SERVER_H
#define __SYSIPC_SERVER_H

#include "base.h"

namespace sysipc {

// handler interface
class IHandler : public IBase {
public:
    virtual void handle(const std::string &method, const std::map<std::string, rapidjson::Value *> &args, rapidjson::Value &returnValue, std::string &errorDescription) = 0;
};

// server error codes
#define SYSIPC_SERVER_E_HANDLEREXISTS   SYSIPC_RESULT(SYSIPC_SEVERITY_FAILURE, SYSIPC_FACILITY_SERVER, 0x0000)
#define SYSIPC_SERVER_E_HANDLERNOTFOUND SYSIPC_RESULT(SYSIPC_SEVERITY_FAILURE, SYSIPC_FACILITY_SERVER, 0x0001)
#define SYSIPC_SERVER_E_STOPPED         SYSIPC_RESULT(SYSIPC_SEVERITY_FAILURE, SYSIPC_FACILITY_SERVER, 0x0002)

// server interface
class IServer : public IBase {
public:
    virtual result_t handle(const std::string &method, IHandler *handler) = 0;

    virtual bool isRunning(void) = 0;
    virtual void run(void) = 0;
    virtual void stop(void) = 0;

    virtual IRouter *getRouter(void) = 0;
    virtual std::string getName(void) = 0;
    virtual std::string getAddress(void) = 0;
};

};

#endif
