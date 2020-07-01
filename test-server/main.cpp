#include "arith_handler.h"
#include <iostream>
#include <sysipc/sysipc.h>

int main(int argc, char **argv) {
    sysipc::result_t result = SYSIPC_S_OK;

    sysipc::IRouter *router;
    result = sysipc::Create::router(".", &router);
    if (SYSIPC_SUCCEEDED(result)) {
        sysipc::IServer *server;
        result = router->createServer("arith", std::clog, &server);
        if (SYSIPC_SUCCEEDED(result)) {
            ArithHandler *handler = new ArithHandler;
            server->handle("add", handler);
            server->handle("sub", handler);
            server->handle("mul", handler);
            server->handle("div", handler);
            server->run();
            delete handler;
            server->destroy();
        }
        router->destroy();
    }

    if (SYSIPC_FAILED(result)) {
        std::cerr.setf(std::ios::hex);
        std::cerr << "SysIPC error: 0x" << result << std::endl;
        std::cerr.unsetf(std::ios::hex);
    }

    return 0;
}

