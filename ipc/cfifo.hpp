#ifndef __IPC_CFIFO_HPP__
#define __IPC_CFIFO_HPP__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <cassert>

namespace ipc {

class cfifo : public ipc::session {
public:
    cfifo() 
        : _fd(0)
    {
    }
    ~cfifo() 
    {
        destroy();
    }
    bool initialize(const char* path, bool block) 
    { 
        if ((mkfifo(path, O_CREAT | O_EXCL) == -1) && (errno != EEXIST)) {
            printf("mkfifo error.\n");
            return false; 
        }
        if ((_fd = open(path, O_RDWR | (block ? 0 : O_NONBLOCK), 0)) == -1) {
            printf("open error.\n");
            return false;
        }
        return true;
    };
    void destroy() 
    {
        close(_fd);
    };
    int read(void* data, size_t size) 
    { 
        int result = -1;
        assert(_fd != 0);
        if ((result = ::read(_fd, data, size)) == -1) {
            // non-blocking only,
            // no data yet.
            if (errno == EAGAIN) {
                return 0;
            }
            return -1;
        }
        return result;
    };
    int write(const void* data, size_t size) 
    {
        int result = -1;
        assert(_fd != 0);
        if ((result = ::write(_fd, data, size)) == -1) {
            // non-blocking only,
            // not enough space for writing. 
            if (errno == EAGAIN) {
                return 0;
            }
            return -1;
        }
        return result;
    };
    void option(int type, void* buf) 
    { 
    };
private:
    int _fd;
};

} // namespace ipc

#endif
