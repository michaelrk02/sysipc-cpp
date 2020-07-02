#include <ctime>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "router.c.h"

#include "client.c.h"

namespace sysipc {

CClient::CClient(void) {
}

CClient::~CClient(void) {
    delete this->serverDispatch;
    delete this->request;
    delete this->response;
}

result_t CClient::init(IRouter *router, const std::string &serverName) {
    result_t result = SYSIPC_S_OK;

    if ((router == NULL) || (serverName.size() == 0)) {
        return SYSIPC_E_INVALIDARG;
    }

    this->router = router;
    this->serverName = serverName;

    this->serverDispatch = new FileDispatch(this->getServerAddress());
    this->request = new FileDispatch(this->getServerAddress() + ".request");
    this->response = new FileDispatch(this->getServerAddress() + ".response");

    return result;
}

void CClient::destroy(void) {
    delete this;
}

result_t CClient::call(const std::string &method, std::map<std::string, rapidjson::Value> &args, CallResult &callResult) {
    result_t result = SYSIPC_S_OK;

    result = this->serverDispatch->lock();
    if (SYSIPC_SUCCEEDED(result)) {
        rapidjson::Document reqDoc(rapidjson::kObjectType);

        rapidjson::Value argsObj(rapidjson::kObjectType);
        for (std::map<std::string, rapidjson::Value>::iterator it = args.begin(); it != args.end(); ++it) {
            argsObj.AddMember(rapidjson::StringRef(it->first.c_str()), it->second, reqDoc.GetAllocator());
        }

        // it is guaranteed to be unique although it is not random
        unsigned long long callId = (unsigned long long)std::time(NULL);
        callId += (unsigned long long)std::clock();
        rapidjson::Value callIdValue;
        callIdValue.SetUint64(callId);

        reqDoc.AddMember("call_id", callIdValue, reqDoc.GetAllocator());
        reqDoc.AddMember("method", rapidjson::StringRef(method.c_str()), reqDoc.GetAllocator());
        reqDoc.AddMember("args", argsObj, reqDoc.GetAllocator());

        rapidjson::StringBuffer reqBuf;
        rapidjson::Writer<rapidjson::StringBuffer> reqWriter(reqBuf);
        reqDoc.Accept(reqWriter);

        result = this->request->send(reqBuf.GetString(), true);
        if (SYSIPC_SUCCEEDED(result)) {
            while (this->serverDispatch->isLocked() && !this->response->exists()) {
            }
            result = this->serverDispatch->isLocked() ? SYSIPC_S_OK : SYSIPC_CLIENT_E_SERVERUNLOCKED;
            if (SYSIPC_SUCCEEDED(result)) {
                result = this->response->lock();
                if (SYSIPC_SUCCEEDED(result)) {
                    std::string resBuf;
                    result = this->response->receive(resBuf);
                    if (SYSIPC_SUCCEEDED(result)) {
                        rapidjson::Document resDoc;
                        resDoc.Parse(resBuf.c_str());

                        rapidjson::Value &resCallIdValue = resDoc["call_id"];
                        unsigned long long resCallId = resCallIdValue.GetUint64();
                        result = (callId == resCallId) ? SYSIPC_S_OK : SYSIPC_CLIENT_E_IDMISMATCH;
                        if (SYSIPC_SUCCEEDED(result)) {
                            this->response->remove();

                            callResult.returnValue = resDoc["return"];
                            callResult.errorDescription = resDoc["error"].GetString();

                            result = (callResult.errorDescription.size() == 0) ? SYSIPC_S_OK : SYSIPC_CLIENT_E_REMOTE;
                        }
                    }
                    this->response->unlock();
                }
            }
        }

        this->serverDispatch->unlock();
    }

    return result;
}

IRouter *CClient::getRouter(void) {
    return this->router;
}

std::string CClient::getServerName(void) {
    return this->serverName;
}

std::string CClient::getServerAddress(void) {
    return this->router->getName() + "/" + this->serverName;
}

};
