#include <iostream>
#include <map>
#include <string>
#include <sysipc/sysipc.h>

int main(int argc, char **argv) {
    sysipc::result_t result = SYSIPC_S_OK;

    sysipc::IRouter *router;
    result = sysipc::Create::router(".", &router);
    if (SYSIPC_SUCCEEDED(result)) {
        sysipc::IClient *client;
        result = router->createClient("arith", &client);
        if (SYSIPC_SUCCEEDED(result)) {
            while (true) {
                std::string op;
                double a, b;

                std::cout << "Enter op, a, b: ";
                std::cin >> op >> a >> b;

                rapidjson::Value lhs;
                lhs.SetDouble(a);

                rapidjson::Value rhs;
                rhs.SetDouble(b);

                std::map<std::string, rapidjson::Value> args;
                args["lhs"] = lhs;
                args["rhs"] = rhs;

                sysipc::CallResult opResult;
                result = client->call(op, args, opResult);
                if (SYSIPC_SUCCEEDED(result)) {
                    std::cout << "Result: " << opResult.returnValue.GetDouble() << std::endl;
                }

                if (SYSIPC_FAILED(result)) {
                    std::ios::fmtflags flags = std::cerr.flags();
                    std::cerr << "SysIPC error: " << std::showbase << std::hex << result << std::endl;
                    std::cerr.flags(flags);

                    if (result == SYSIPC_CLIENT_E_REMOTE) {
                        std::cerr << "remote: " << opResult.errorDescription << std::endl;
                    }
                }
            }
            client->destroy();
        }
        router->destroy();
    }

    return 0;
}

