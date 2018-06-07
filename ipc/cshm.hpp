#ifndef __IPC_SHM_HPP__
#define __IPC_SHM_HPP__

#include "csem.hpp"
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>

namespace ipc {

class cshm : public session {
public:
    cshm() 
        : _sem(0)
        , _shm_id(0)
        , _shm_size(1024)
        , _shm_flag(IPC_CREAT | 0604)
    {
    }
    ~cshm()
    {
        destroy();
    }
    bool initialize(const char* path, bool block) 
    { 
        if (!_sem.initialize(path, block)) {
            printf("failed to initialize sem\n");
            return false;
        }
        if ((_shm_id = shmget(_sem.key(), _shm_size, _shm_flag)) == -1) {
            show_error();
            printf("failed to create shm\n");
            return false;
        }
        if ((_shm = (char*)shmat(_shm_id, 0, 0)) == 0) {
            printf("failed to map shm\n");
            return false;
        }
        return true;
    };
    void destroy() 
    {
        shmdt(_shm);
    };
    int read(void* data, size_t size)
    { 
        // assert(_shm_size > size);
        if (_shm_id == -1) return -1;
        if (_sem.v() != -1) { 
            // non-blocking will return -1
            size_t actual = std::min(size, _shm_size);
            memcpy((char*)data, _shm, actual);
            return actual; 
        }
        return 0;
    };
    int write(const void* data, size_t size) 
    { 
        // assert(_shm_size > size);
        if (_shm_id == -1) return -1;
        size_t actual = std::min(size, _shm_size);
        memcpy(_shm, data, actual);
        _sem.p();
        return actual; 
    };
    void option(int type, void* buf) 
    {
    };
private:
    void show_error()
    {
        switch (errno) {
        case EINVAL:
            printf("invalid size\n");
            break;
        case EEXIST:
            printf("shared memory had been exist\n");
            break;
        case EIDRM:
            printf("related shared memory had been removed\n");
            break;
        case ENOSPC:
            printf("exceeding the maximum count\n");
            break;
        case ENOENT:
            printf("key related shared memory is not exist, beware shmflg should be IPC_CREAT\n");
            break;
        case EACCES:
            printf("no permission\n");
            break;
        case ENOMEM:
            printf("not enough memory\n");
            break;
        }
    }
private:
    csem _sem;
    int _shm_id;
    char* _shm;
    size_t _shm_size;
    int _shm_flag;
};

} // namespace ipc

#endif
