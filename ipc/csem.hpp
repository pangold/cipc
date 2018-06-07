#ifndef __IPC_CSEM_HPP__
#define __IPC_CSEM_HPP__

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

namespace ipc {

class csem {
    typedef int sem_t;
public:
    csem(unsigned short num = 0)
        : _key(0)
        , _sem_id(0)
        , _sem_num(num)
        , _nsems(1) // must be 1 or more.
        , _sem_flag(IPC_CREAT | 0666)
    {
    }
    ~csem()
    {
        // destroy();
    }
    const int& key() const
    {
        return _key;
    }
    bool invalid() const 
    {
        return _sem_id == -1;
    }
    bool initialize(const char* path, bool block = false, int proj = 'a', int value = 0)
    {
        if ((_key = ftok(path, proj)) == -1) {
            printf("failed to create key\n");
            return false;
        }
        if ((_sem_id = semget(_key, _nsems, _sem_flag)) == -1) {
            show_error();
            printf("failed to create sem\n");
            return false;
        }
        union semun sem = { value };
        semctl(_sem_id, _sem_num, SETVAL, sem);
        _block = block;
        return true;
    }
    void destroy()
    {
        if (invalid()) return;
        union semun sem = { 0 };
        semctl(_sem_id, _sem_num, IPC_RMID, sem);
    }
    int p()
    {
        if (invalid()) return -1;
        short flag = _block ? 0 : IPC_NOWAIT;
        struct sembuf ops = { _sem_num, +1, flag };
        return semop(_sem_id, &ops, 1);
    }
    int v()
    {
        if (invalid()) return -1;
        short flag = _block ? 0 : IPC_NOWAIT;
        struct sembuf ops = { _sem_num, -1, flag };
        return semop(_sem_id, &ops, 1);
    }
    void set(int value)
    {
        if (invalid()) return;
        union semun sem;
        sem.val = value;
        semctl(_sem_id, _sem_num, SETVAL, sem);
    }
    int get()
    {
        if (invalid()) return -1;
        union semun sem;
        return semctl(_sem_id, _sem_num, GETVAL, sem);
    }
private:
    void show_error()
    {
        switch (errno) {
        case EACCES:
            printf("no permission\n");
            break;
        case EEXIST:
            printf("already been exist\n");
            break;
        case EINVAL:
            printf("invalid nsems\n");
            break;
        case ENOENT:
            printf("not exist\n");
            break;
        case ENOMEM:
            printf("not enough memory\n");
            break;
        case ENOSPC:
            printf("exceeding the limitation of system\n");
            break;
        }
    }
private:
    key_t _key;
    sem_t _sem_id;
    unsigned short _sem_num;
    int _nsems;
    int _sem_flag;
    bool _block;
};

} // namespace ipc

#endif
