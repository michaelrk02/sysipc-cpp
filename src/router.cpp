#include "client.c.h"
#include "server.c.h"

#include "router.c.h"

namespace sysipc {

result_t Create::router(const std::string &name, IRouter **router) {
    result_t result = SYSIPC_S_OK;

    if (router == NULL) {
        return SYSIPC_E_INVALIDARG;
    }

    CRouter *routerObj = new CRouter;
    result = routerObj->init(name);
    if (SYSIPC_SUCCEEDED(result)) {
        *router = routerObj;
    }

    return result;
}

CRouter::CRouter(void) {
}

result_t CRouter::init(const std::string &name) {
    result_t result = SYSIPC_S_OK;

    if (name.size() == 0) {
        return SYSIPC_E_INVALIDARG;
    }

    this->name = name;

    return result;
}

void CRouter::destroy(void) {
    delete this;
}

result_t CRouter::createServer(const std::string &name, IServer **server) {
    result_t result = SYSIPC_S_OK;

    if (server == NULL) {
        return SYSIPC_E_INVALIDARG;
    }

    CServer *serverObj = new CServer;
    result = serverObj->init(this, name);
    if (SYSIPC_SUCCEEDED(result)) {
        *server = serverObj;
    }

    return result;
}

result_t CRouter::createServer(const std::string &name, std::ostream &logger, IServer **server) {
    result_t result = SYSIPC_S_OK;

    if (server == NULL) {
        return SYSIPC_E_INVALIDARG;
    }

    CServer *serverObj = new CServer;
    result = serverObj->init(this, name, logger);
    if (SYSIPC_SUCCEEDED(result)) {
        *server = serverObj;
    }

    return result;
}

result_t CRouter::createClient(const std::string &name, IClient **client) {
    result_t result = SYSIPC_S_OK;

    if (client == NULL) {
        return SYSIPC_E_INVALIDARG;
    }

    CClient *clientObj = new CClient;
    result = clientObj->init(this, name);
    if (SYSIPC_SUCCEEDED(result)) {
        *client = clientObj;
    }

    return result;
}

std::string CRouter::getName(void) {
    return this->name;
}

};
