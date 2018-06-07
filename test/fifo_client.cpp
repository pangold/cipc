#include "../ipc/cipc.hpp"
#include <cstdint>
#include <vector>
#include <string>
#include <thread>
#include <iostream>

int main(int argc, char** argv)
{
    bool block = false;
    std::string mode("fifo");
    if (argc > 1) mode = std::string(argv[1]);
    if (argc > 2) block = std::atoi(argv[2]) != 0;

    cipc ipc(mode);
    std::string path = "/ipc/" + mode;
    if (!ipc.initialize(path.c_str(), block)) {
        std::cout << "failed to initialize ipc" << std::endl;
        return -1;
    }

    std::string buffer("hello, ipc, I am writing.");
    size_t size = ipc.write(buffer.data(), buffer.size());
    std::cout << "write '" << buffer << "' to the ipc" << std::endl;

    return 0;
}
