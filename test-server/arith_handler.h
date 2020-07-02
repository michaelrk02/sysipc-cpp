#ifndef __ARITH_HANDLER_H
#define __ARITH_HANDLER_H

#include <sysipc/sysipc.h>

class ArithHandler : public sysipc::IHandler {
public:
    ArithHandler(void);

    void destroy(void);

    void handle(const std::string &method, const std::map<std::string, rapidjson::Value *> &args, rapidjson::Value &returnValue, std::string &errorDescription);

private:
    void add(double a, double b, double &result, std::string &error);
    void sub(double a, double b, double &result, std::string &error);
    void mul(double a, double b, double &result, std::string &error);
    void div(double a, double b, double &result, std::string &error);
};

#endif
