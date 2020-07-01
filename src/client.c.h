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
    CClient(void);
    ~CClient(void);

    void destroy(void);

    result_t init(IRouter *router, const std::string &serverName);
    result_t call(const std::string &method, std::map<std::string, rapidjson::Value> &args, CallResult &callResult);

    IRouter *getRouter(void);
    std::string getServerName(void);
    std::string getServerAddress(void);
};

};

#endif
