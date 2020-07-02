#ifndef __SYSIPC_FILE_DISPATCH_H
#define __SYSIPC_FILE_DISPATCH_H

#include <sysipc/base.h>

namespace sysipc {

class SYSIPC_API FileDispatch {
private:
    std::string name, lockName;

public:
    FileDispatch(const std::string &name);

    bool exists(void);
    void remove(void);

    bool isLocked(void);
    result_t lock(void);
    void unlock(void);

    result_t send(const std::string &buffer, bool lock = false);
    result_t receive(std::string &buffer, bool lock = false);
};

};

#endif
