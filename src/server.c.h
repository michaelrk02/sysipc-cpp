#ifndef __SYSIPC_SERVER_C_H
#define __SYSIPC_SERVER_C_H

#include "file_dispatch.h"

#include <sysipc/server.h>

namespace sysipc {

class CServer : public IServer {
private:
    std::ostream *logger;

    IRouter *router;
    std::string name;

    FileDispatch *dispatch;
    FileDispatch *request;
    FileDispatch *response;

    std::map<std::string, IHandler *> handlers;

public:
    CServer(void);
    ~CServer(void);

    result_t init(IRouter *router, const std::string &name);
    result_t init(IRouter *router, const std::string &name, std::ostream &logger);

    void destroy(void);

    result_t handle(const std::string &method, IHandler *handler);
    void run(void);

    IRouter *getRouter(void);
    std::string getName(void);
    std::string getAddress(void);

private:
    result_t intercept(void);
    void logError(result_t result);
};

};

#endif
