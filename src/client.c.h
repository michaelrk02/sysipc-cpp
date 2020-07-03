#ifndef __SYSIPC_CLIENT_C_H
#define __SYSIPC_CLIENT_C_H

#include "file_dispatch.h"

#include <sysipc/client.h>

namespace sysipc {

class CClient : public IClient {
private:
    IRouter *router;
    std::string serverName;

    FileDispatch *serverDispatch;
    FileDispatch *request;
    FileDispatch *response;

public:
    SYSIPC_API CClient(void);
    SYSIPC_API ~CClient(void);

    SYSIPC_API void destroy(void);

    SYSIPC_API result_t init(IRouter *router, const std::string &serverName);
    SYSIPC_API result_t call(const std::string &method, const std::map<std::string, rapidjson::Value *> &args, CallResult &callResult);

    SYSIPC_API IRouter *getRouter(void);
    SYSIPC_API std::string getServerName(void);
    SYSIPC_API std::string getServerAddress(void);
};

};

#endif
