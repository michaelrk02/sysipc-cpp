#include "arith_handler.h"

ArithHandler::ArithHandler(void) {
}

void ArithHandler::destroy(void) {
    delete this;
}

void ArithHandler::handle(const std::string &method, const std::map<std::string, rapidjson::Value *> &args, rapidjson::Value &returnValue, std::string &errorDescription) {
    std::map<std::string, rapidjson::Value *>::const_iterator lhsIt = args.find("lhs");
    std::map<std::string, rapidjson::Value *>::const_iterator rhsIt = args.find("rhs");
    if ((lhsIt != args.end()) && (rhsIt != args.end())) {
        rapidjson::Value &lhs = *lhsIt->second;
        rapidjson::Value &rhs = *rhsIt->second;
        if (lhs.IsNumber() && rhs.IsNumber()) {
            double a = lhs.GetDouble();
            double b = rhs.GetDouble();
            double result = 0.0;
            if (method == "add") {
                this->add(a, b, result, errorDescription);
            } else if (method == "sub") {
                this->sub(a, b, result, errorDescription);
            } else if (method == "mul") {
                this->mul(a, b, result, errorDescription);
            } else if (method == "div") {
                this->div(a, b, result, errorDescription);
            }
            returnValue.SetDouble(result);
        } else {
            errorDescription = "expecting numerical arguments";
        }
    } else {
        errorDescription = "expecting 'lhs' and 'rhs' arguments";
    }
}

void ArithHandler::add(double a, double b, double &result, std::string &error) {
    result = a + b;
}

void ArithHandler::sub(double a, double b, double &result, std::string &error) {
    result = a - b;
}

void ArithHandler::mul(double a, double b, double &result, std::string &error) {
    result = a * b;
}

void ArithHandler::div(double a, double b, double &result, std::string &error) {
    if (b != 0.0) {
        result = a / b;
    } else {
        error = "attempted to divide by zero";
    }
}

