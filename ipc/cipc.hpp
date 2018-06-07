#ifndef __CIPC_HPP__
#define __CIPC_HPP__

#include "utils/factory.hpp"
#include "session.h"
#include "cpipe.hpp"
#include "cfifo.hpp"
#include "cmsg.hpp"
#include "cshm.hpp"

class cipc : public ipc::session {
public:
    cipc(const std::string& mode) 
        : _mode(mode)
    { 
        allocate(); 
    }
    ~cipc() 
    { 
        free(); 
    }
    bool initialize(const char* path, bool block)
    {
        return _session->initialize(path, block);
    }
    void destroy() 
    {
        _session->destroy();
    }
    int read(void* data, size_t size) 
    {
        return _session->read(data, size);
    }
    int write(const void* data, size_t size) 
    {
        return _session->write(data, size);
    }
    void option(int type, void* buf) 
    {
        _session->option(type, buf);
    }
private:
    void allocate() 
    {
        _factory.add<ipc::cpipe>("pipe");
        _factory.add<ipc::cfifo>("fifo");
        _factory.add<ipc::cmsg<1024>>("msg");
        _factory.add<ipc::cshm>("shm");
        _session = _factory.instance(_mode);
    }
    void free()
    {
        _factory.clear();
    }
private:
    std::string _mode;
    ipc::utils::factory<ipc::session> _factory;
    ipc::session* _session;

};

#endif 
