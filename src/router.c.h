#ifndef __SYSIPC_ROUTER_C_H
#define __SYSIPC_ROUTER_C_H

#include <sysipc/router.h>

namespace sysipc {

class CRouter : public IRouter {
private:
    std::string name;

public:
    SYSIPC_API CRouter(void);

    SYSIPC_API result_t init(const std::string &name);

    SYSIPC_API void destroy(void);

    SYSIPC_API result_t createServer(const std::string &name, IServer **server);
    SYSIPC_API result_t createServer(const std::string &name, std::ostream &logger, IServer **server);
    SYSIPC_API result_t createClient(const std::string &name, IClient **client);

    SYSIPC_API std::string getName(void);
};

};

#endif
