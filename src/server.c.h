#ifndef __SYSIPC_SERVER_C_H
#define __SYSIPC_SERVER_C_H

#include "file_dispatch.h"
#include "mutex.h"

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

    bool running;
    mutex_t mtx;

public:
    SYSIPC_API CServer(void);
    SYSIPC_API ~CServer(void);

    SYSIPC_API result_t init(IRouter *router, const std::string &name);
    SYSIPC_API result_t init(IRouter *router, const std::string &name, std::ostream &logger);

    SYSIPC_API void destroy(void);

    SYSIPC_API result_t handle(const std::string &method, IHandler *handler);

    SYSIPC_API bool isRunning(void);
    SYSIPC_API void run(void);
    SYSIPC_API void stop(void);

    SYSIPC_API IRouter *getRouter(void);
    SYSIPC_API std::string getName(void);
    SYSIPC_API std::string getAddress(void);

private:
    SYSIPC_API result_t intercept(void);
    SYSIPC_API void logError(result_t result);
};

};

#endif
