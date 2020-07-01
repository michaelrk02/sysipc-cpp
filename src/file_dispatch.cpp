#include <cstdio>
#include <fstream>
#include <stdexcept>

#include "file_dispatch.h"

namespace sysipc {

FileDispatch::FileDispatch(const std::string &name) {
    this->name = name;
    this->lockName = name + ".lock";
}

bool FileDispatch::exists(void) {
    std::ifstream f(this->name.c_str());
    return f.is_open();
}

void FileDispatch::remove(void) {
    std::remove(this->name.c_str());
}

bool FileDispatch::isLocked(void) {
    std::ifstream f(this->lockName.c_str());
    return f.is_open();
}

result_t FileDispatch::lock(void) {
    while (this->isLocked()) {
    }
    std::ofstream f(this->lockName.c_str());
    return f.is_open() ? SYSIPC_S_OK : SYSIPC_E_INTERNAL;
}

void FileDispatch::unlock(void) {
    std::remove(this->lockName.c_str());
}

result_t FileDispatch::send(const std::string &buffer, bool lock) {
    result_t result = SYSIPC_S_OK;
    if (lock) {
        result = this->lock();
    }
    if (SYSIPC_SUCCEEDED(result)) {
        std::ofstream f(this->name.c_str());
        result = f.is_open() ? SYSIPC_S_OK : SYSIPC_E_INTERNAL;
        if (SYSIPC_SUCCEEDED(result)) {
            f.write(buffer.data(), buffer.size());
            f.flush();
        }
        if (lock) {
            this->unlock();
        }
    }
    return result;
}

result_t FileDispatch::receive(std::string &buffer, bool lock) {
    result_t result = SYSIPC_S_OK;
    if (lock) {
        result = this->lock();
    }
    if (SYSIPC_SUCCEEDED(result)) {
        std::ifstream f(this->name.c_str());
        result = f.is_open() ? SYSIPC_S_OK : SYSIPC_E_INTERNAL;
        if (SYSIPC_SUCCEEDED(result)) {
            f.seekg(0, std::ios::end);
            size_t size = f.tellg();
            f.seekg(0, std::ios::beg);

            char *buf = new char[size];
            result = (buf != NULL) ? SYSIPC_S_OK : SYSIPC_E_MEMORY;
            if (SYSIPC_SUCCEEDED(result)) {
                f.read(buf, size);
                buffer = (const char *)buf;
                delete[] buf;
            }
        }
        if (lock) {
            this->unlock();
        }
    }
    return result;
}

};
