#ifndef __IPC_CMSG_HPP__
#define __IPC_CMSG_HPP__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <algorithm>

namespace ipc {

template <int MaxSize>
class cmsg : public ipc::session {
    struct msgmbuf {
        long type;          // this data type must be long
        char text[MaxSize]; // whatever
    };
public:
    cmsg() 
        : _key(0)
        , _msg_id(0) 
        , _msg_buf()
        , _msg_flag(IPC_CREAT /* | IPC_EXCL */ | 0666)
        , _msg_send_flag(0)
        , _msg_recv_flag(MSG_NOERROR)
    {
        _msg_buf.type = 10;
    }
    ~cmsg()
    {
        // do not destroy when destructe.
        // destroy();
    }
    bool initialize(const char* path, bool block) 
    {
        if ((_key = ftok(path, 'a')) == -1) {
            printf("failed to generate key\n");
            return false;
        }
        if ((_msg_id = msgget(_key, _msg_flag)) == -1) {
            printf("failed to create message queue\n");
            return false;
        }
        if (!block) {
            _msg_send_flag |= IPC_NOWAIT;
            _msg_recv_flag |= IPC_NOWAIT;
        }
        return true; 
    };
    void destroy()
    {
        if (msgctl(_msg_id, IPC_RMID, NULL) == -1) {
            printf("failed to remove message queue\n");
            return;
        }
    };
    int read(void* data, size_t size) 
    { 
        int result = msgrcv(_msg_id, &_msg_buf, sizeof(_msg_buf.text), _msg_buf.type, _msg_recv_flag);
        if (result != -1) {
            memcpy((char*)data, _msg_buf.text, std::min(result, static_cast<int>(size)));
            return result;
        }
        //
        if (errno == ENOMSG) {
            printf("no data yet.\n");
            return 0;
        }
        else if (errno == EINTR) {
            printf("interupt signal.\n");
            return 0;
        }
        else if (errno == EIDRM) {
            printf("message queue had been removed.\n");
            return -1;
        } 
        printf("failed to receive message\n");
        return -1;
    };
    int write(const void* data, size_t size) 
    { 
        int result = -1;
        memcpy(_msg_buf.text, data, size);
        if ((result = msgsnd(_msg_id, &_msg_buf, size, _msg_send_flag)) == -1) {
            show_error();
            printf("failed to send message\n");
            return -1;
        }
        return result; 
    };
    void option(int type, void* buf) 
    {
        switch (type) {
        case IPC_OP_MSG_GET:
            msgctl(_msg_id, IPC_STAT, (struct msqid_ds*)buf);
            break;
        case IPC_OP_MSG_SET:
            msgctl(_msg_id, IPC_SET, (struct msqid_ds*)buf);
            break;
        case IPC_OP_MSG_RESET:
            msgctl(_msg_id, IPC_RMID, NULL);
            break;
        }
    };
private:
    void show_error()
    {
        switch (errno) {
        case EINTR:
            printf("interupt signal.\n");
            break;
        case EIDRM:
            printf("message queue had been removed.\n");
            break;
        }
    }
private:
    key_t _key;
    int _msg_id;
    struct msgmbuf _msg_buf;
    int _msg_flag;
    int _msg_send_flag;
    int _msg_recv_flag;
};

} // namespace ipc

#endif
