#ifndef __IPC_SESSION_H__
#define __IPC_SESSION_H__

#include <stdlib.h>

namespace ipc {

enum option_type {
    IPC_OP_MSG_SET,
    IPC_OP_MSG_GET,
    IPC_OP_MSG_RESET,
};

class session {
public:
    virtual ~session() { }
    virtual bool initialize(const char* path, bool block) = 0;
    virtual void destroy() = 0;
    virtual int read(void* data, size_t size) = 0;
    virtual int write(const void* data, size_t size) = 0;
    virtual void option(int type, void* buf) = 0;
};

} // namespace ipc

#endif
