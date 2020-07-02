#ifndef __SYSIPC_CLIENT_H
#define __SYSIPC_CLIENT_H

#include "base.h"

namespace sysipc {

// client call result
struct CallResult {
    rapidjson::Value returnValue;
    std::string errorDescription;
};

// client error codes
#define SYSIPC_CLIENT_E_REMOTE          SYSIPC_RESULT(SYSIPC_SEVERITY_FAILURE, SYSIPC_FACILITY_CLIENT, 0x0000)
#define SYSIPC_CLIENT_E_SERVERUNLOCKED  SYSIPC_RESULT(SYSIPC_SEVERITY_FAILURE, SYSIPC_FACILITY_CLIENT, 0x0001)
#define SYSIPC_CLIENT_E_IDMISMATCH      SYSIPC_RESULT(SYSIPC_SEVERITY_FAILURE, SYSIPC_FACILITY_CLIENT, 0x0002)

// client interface
class IClient : public IBase {
public:
    virtual result_t call(const std::string &method, const std::map<std::string, rapidjson::Value *> &args, CallResult &callResult) = 0;

    virtual IRouter *getRouter(void) = 0;
    virtual std::string getServerName(void) = 0;
    virtual std::string getServerAddress(void) = 0;
};

};

#endif
