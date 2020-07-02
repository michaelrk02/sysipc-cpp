#ifndef __SYSIPC_ROUTER_C_H
#define __SYSIPC_ROUTER_C_H

#include <sysipc/router.h>

namespace sysipc {

class SYSIPC_API CRouter : public IRouter {
private:
    std::string name;

public:
    CRouter(void);

    result_t init(const std::string &name);

    void destroy(void);

    result_t createServer(const std::string &name, IServer **server);
    result_t createServer(const std::string &name, std::ostream &logger, IServer **server);
    result_t createClient(const std::string &name, IClient **client);

    std::string getName(void);
};

};

#endif
