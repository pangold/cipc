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

    size_t size = 0;
    std::vector<uint8_t> buffer(10240);
    while(1) {
        size = ipc.read(buffer.data(), buffer.size());
        if (size == -1) break;
        if (size > 0) {
            std::cout << std::string(buffer.begin(), buffer.begin() + size) << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}
