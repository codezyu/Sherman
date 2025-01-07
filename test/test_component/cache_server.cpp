#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "DSM.h"
#include "Tree.h"
#include <chrono>
#define PORT 18080

typedef int Key1;
typedef int Value1;

int main() {
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // 创建套接字
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "socket failed" << std::endl;
        return -1;
    }

    // 绑定套接字
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("0.0.0.0");
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "bind failed" << std::endl;
        return -1;
    }

    // 监听连接
    if (listen(server_fd, 3) < 0) {
        std::cerr << "listen failed" << std::endl;
        return -1;
    }
    DSMConfig config;
    // set node number
    config.machineNR = 2;
    DSM *dsm = DSM::getInstance(config);

    dsm->registerThread();

    auto tree = new Tree(dsm);
    tree->insert(1, 10);
    Value v;
    std::cout << "get node id" << std::endl;
    if (dsm->getMyNodeID() != 0) {
    }
    // 阻塞接受连接
    int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
        std::cerr << "accept failed" << std::endl;
    }
    // 处理客户端连接
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, 1024);
        if (valread <= 0) {
            std::cerr << "read failed or connection closed" << std::endl;
            break;
        }
        buffer[valread] = '\0';
        std::cout << "Received: " << buffer << std::endl;

        // 处理指令
        if (strncmp(buffer, "insert", 6) == 0) {
            Key1 k = 1;
            Value1 v = 1;
            std::cout << "Inserting key: " << k << " with value: " << v << std::endl;
            // 插入数据的逻辑
            const char* response = "Insert operation successful\n";
            send(new_socket, response, strlen(response), 0);
        } else if (strncmp(buffer, "read", 4) == 0) {
            Key1 k = 1;
            Value v = 2; // 假设读取到的值
            std::cout << "Reading key: " << k << " with value: " << v << std::endl;
            // 读取数据的逻辑
            bool result = tree->search(k, v);
            std::cout << "Key found: " << result << std::endl;
            std::cout << "Value: " << v << std::endl;
            const char* response = (result == false ? "Key not found\n" : "Key found\n");
            send(new_socket,response, strlen(response), 0);
        } else {
            std::cerr << "Unknown command" << std::endl;
            const char* response = "Unknown command\n";
            send(new_socket, response, strlen(response), 0);
        }
    }



    // 关闭服务器套接字
    close(server_fd);

    return 0;
}