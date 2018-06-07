#ifndef __CFORK_HPP__
#define __CFORK_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

class cfork {
public:
    cfork() 
        : _pid(fork()) 
    {
    }
    bool invalid() const 
    {
        return _pid == -1;
    }
    bool sub() const 
    {
        return _pid == 0;
    }
    bool parent() const 
    {
        return !sub() && !invalid();
    }
private:
    pid_t _pid;
};

#endif
