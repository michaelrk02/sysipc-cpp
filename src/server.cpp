#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "router.c.h"

#include "server.c.h"

namespace sysipc {

CServer::CServer(void) {
    this->logger = NULL;
}

CServer::~CServer(void) {
    delete this->dispatch;
    delete this->request;
    delete this->response;
}

result_t CServer::init(IRouter *router, const std::string &name) {
    result_t result = SYSIPC_S_OK;

    if ((router == NULL) || (name.size() == 0)) {
        return SYSIPC_E_INVALIDARG;
    }

    this->router = router;
    this->name = name;

    this->dispatch = new FileDispatch(this->getAddress());
    this->request = new FileDispatch(this->getAddress() + ".request");
    this->response = new FileDispatch(this->getAddress() + ".response");

    return result;
}

result_t CServer::init(IRouter *router, const std::string &name, std::ostream &logger) {
    result_t result = this->init(router, name);
    if (SYSIPC_SUCCEEDED(result)) {
        this->logger = &logger;
    }
    return result;
}

void CServer::destroy(void) {
    delete this;
}

result_t CServer::handle(const std::string &method, IHandler *handler) {
    result_t result = SYSIPC_S_OK;

    if (handler == NULL) {
        return SYSIPC_E_INVALIDARG;
    }

    std::map<std::string, IHandler *>::iterator prevHandler = this->handlers.find(method);
    result = (prevHandler == this->handlers.end()) ? SYSIPC_S_OK : SYSIPC_SERVER_E_HANDLEREXISTS;
    if (SYSIPC_SUCCEEDED(result)) {
        this->handlers[method] = handler;
    }
    
    return result;
}

void CServer::run(void) {
    this->dispatch->unlock();
    this->request->remove();
    this->request->unlock();
    this->response->remove();
    this->response->unlock();
    while (true) {
        result_t result = this->intercept();
        if (SYSIPC_FAILED(result)) {
            this->logError(result);
        }
    }
}

IRouter *CServer::getRouter(void) {
    return this->router;
}

std::string CServer::getName(void) {
    return this->name;
}

std::string CServer::getAddress(void) {
    return this->router->getName() + "/" + this->name;
}

result_t CServer::intercept(void) {
    result_t result;

    while (!this->request->exists()) {
    }

    std::string reqBuf;
    result = this->request->lock();
    if (SYSIPC_SUCCEEDED(result)) {
        result = this->request->receive(reqBuf);
        if (SYSIPC_SUCCEEDED(result)) {
            rapidjson::Document reqDoc;
            reqDoc.Parse(reqBuf.c_str());

            unsigned long long callId = reqDoc["call_id"].GetUint64();
            std::string method = reqDoc["method"].GetString();
            rapidjson::Value &args = reqDoc["args"];

            std::map<std::string, rapidjson::Value> argsMap;
            for (rapidjson::Value::MemberIterator it = args.MemberBegin(); it != args.MemberEnd(); ++it) {
                argsMap[it->name.GetString()] = it->value;
            }

            std::map<std::string, IHandler *>::iterator handlerIt = this->handlers.find(method);
            result = (handlerIt != this->handlers.end()) ? SYSIPC_S_OK : SYSIPC_SERVER_E_HANDLERNOTFOUND;
            if (SYSIPC_SUCCEEDED(result)) {
                IHandler *handler = handlerIt->second;
                result = (handler != NULL) ? SYSIPC_S_OK : SYSIPC_E_BADPTR;
                if (SYSIPC_SUCCEEDED(result)) {
                    rapidjson::Value returnValue;
                    std::string errorDescription;
                    handler->handle(method, argsMap, returnValue, errorDescription);

                    rapidjson::Document resDoc(rapidjson::kObjectType);
                    rapidjson::Value callIdValue;
                    callIdValue.SetUint64(callId);
                    resDoc.AddMember("call_id", callIdValue, resDoc.GetAllocator());
                    resDoc.AddMember("return", returnValue, resDoc.GetAllocator());
                    resDoc.AddMember("error", rapidjson::StringRef(errorDescription.c_str()), resDoc.GetAllocator());

                    rapidjson::StringBuffer resBuf;
                    rapidjson::Writer<rapidjson::StringBuffer> resWriter(resBuf);
                    resDoc.Accept(resWriter);

                    result = this->response->send(resBuf.GetString(), true);
                }
            }
        }
        this->request->remove();
        this->request->unlock();
    }

    return result;
}

void CServer::logError(result_t result) {
    if (this->logger != NULL) {
        std::ostream &logger = *this->logger;
        logger.setf(std::ios::hex);
        logger << "SysIPC [" << this->getAddress() << "] error: 0x" << result << std::endl;
        logger.unsetf(std::ios::hex);
    }
}

};
