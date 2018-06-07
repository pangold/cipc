#include "../ipc/cipc.hpp"
#include "../ipc/cfork.hpp"
#include <string>
#include <iostream>
#include <cstdint>
#include <vector>

int main(int argc, char** argv)
{
    cipc ipc("pipe");
    if (!ipc.initialize(NULL, false)) {
        std::cout << "failed to initialize ipc module" << std::endl;
        return -1;
    }

    cfork fork;
    if (fork.invalid()) {
        std::cout << "failed to fork process" << std::endl;
        return -1;
    }

    size_t size;
    std::vector<uint8_t> buffer(10, 50);
    if (argc == 2) {
        std::vector<uint8_t> temp(argv[1], argv[1] + strlen(argv[1]));
        std::swap(temp, buffer);
    }
    if (fork.parent()) {
        size = ipc.read(buffer.data(), buffer.size());
        std::cout << "sended: " << std::string(buffer.data(), buffer.data() + size) << std::endl;
    } else {
        size = ipc.write(buffer.data(), buffer.size());
        std::cout << "received: " << std::string(buffer.data(), buffer.data() + size) << std::endl;
    }

    return 0;
}
