#ifndef __SYSIPC_FILE_DISPATCH_H
#define __SYSIPC_FILE_DISPATCH_H

#include <sysipc/base.h>

namespace sysipc {

class FileDispatch {
private:
    std::string name, lockName;

public:
    SYSIPC_API FileDispatch(const std::string &name);

    SYSIPC_API bool exists(void);
    SYSIPC_API void remove(void);

    SYSIPC_API bool isLocked(void);
    SYSIPC_API result_t lock(void);
    SYSIPC_API void unlock(void);

    SYSIPC_API result_t send(const std::string &buffer, bool lock = false);
    SYSIPC_API result_t receive(std::string &buffer, bool lock = false);
};

};

#endif
