#ifndef __IPC_CPIPE_HPP__
#define __IPC_CPIPE_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

namespace ipc {

class cpipe : public ipc::session {
public:
    cpipe() 
        : _fd()
        , _result(::pipe(_fd))
        , _wfd(&_fd[1]) // the second one(index = 1) is for writing
        , _rfd(&_fd[0]) // the first one(index = 0) is for reading
    {
    }
    ~cpipe()
    {
        destroy();
    }
    bool initialize(const char* path, bool block) 
    { 
        return _result != -1; 
    };
    void destroy() 
    {
        close(_fd[0]);
        close(_fd[1]);
    };
    int read(void* data, size_t size) 
    { 
        if (_result == -1) return -1;
        return ::read(*_rfd, data, size);
    };
    int write(const void* data, size_t size) 
    {
        if (_result == -1) return -1;
        return ::write(*_wfd, data, size);
    };
    void option(int type, void* buf) 
    {
    };
private:
    int _fd[2];
    int _result;
    int* _wfd;
    int* _rfd;
};

} // namespace ipc

#endif
