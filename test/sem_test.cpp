#include "../ipc/csem.hpp"
#include <string>
#include <iostream>
#include <cstdint>
#include <vector>

int main(int argc, char** argv)
{
    ipc::csem sem;
    if (!sem.initialize("/ipc/sem", 'a', 3)) {
        std::cout << "failed to initialize sem" << std::endl;
        return -1;
    }
    sem.p();
    for (int i = 0; i < 3; i++) {
        sem.p();
        sem.v();
    }
    std::cout << "sem.value = " << sem.get() << std::endl;
    
    return 0;
}
