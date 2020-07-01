#ifndef __SYSIPC_ROUTER_H
#define __SYSIPC_ROUTER_H

#include "base.h"

namespace sysipc {

// router interface
class IRouter : public IBase {
public:
    virtual result_t createServer(const std::string &name, IServer **server) = 0;
    virtual result_t createServer(const std::string &name, std::ostream &logger, IServer **server) = 0;
    virtual result_t createClient(const std::string &name, IClient **client) = 0;

    virtual std::string getName(void) = 0;
};

};

#endif
