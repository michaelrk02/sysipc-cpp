#include "arith_handler.h"
#include <csignal>
#include <iostream>
#include <sysipc/sysipc.h>

sysipc::IServer *server;

void stopServer(int signal) {
    server->stop();
}

int main(int argc, char **argv) {
    sysipc::result_t result = SYSIPC_S_OK;

    sysipc::IRouter *router;
    result = sysipc::Create::router(".", &router);
    if (SYSIPC_SUCCEEDED(result)) {
        result = router->createServer("arith", std::clog, &server);
        if (SYSIPC_SUCCEEDED(result)) {
            ArithHandler *handler = new ArithHandler;
            server->handle("add", handler);
            server->handle("sub", handler);
            server->handle("mul", handler);
            server->handle("div", handler);

            std::signal(SIGINT, stopServer);
            server->run();

            delete handler;
            server->destroy();
        }
        router->destroy();
    }

    if (SYSIPC_FAILED(result)) {
        std::ios::fmtflags flags = std::cerr.flags();
        std::cerr << "SysIPC error: " << std::showbase << std::hex << result << std::endl;
        std::cerr.flags(flags);
    }

    return 0;
}

