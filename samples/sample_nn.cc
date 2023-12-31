#include "runtime/net.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("usage: ./bin/sample_nn "
               "{param} "
               "{bin} \n");
        return -1;
    }

    auto net = std::make_shared<nn::Net>();
    net->Init(argv[1], argv[2]);

    return 0;
}