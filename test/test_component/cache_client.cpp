#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "DSM.h"
#include "Tree.h"
#include <chrono>
#define SERVER_IP "10.26.57.55"
#define SERVER_PORT 18080

void initTree(Tree *tree){
  //  the time of insert
  auto start = std::chrono::high_resolution_clock::now();
  for(uint64_t i = 1; i < 10240; ++i){
    tree->insert(i, i * 2);
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start; // 计算执行时间
  std::cout << "Task execution time: " << duration.count() << " seconds" << std::endl;
  // average insert time
  std::cout << "average insert time: " << duration.count() / 10240 << " seconds" << std::endl;
}
void send_command(int sock, const std::string& command) {
    if (send(sock, command.c_str(), command.length(), 0) < 0) {
        std::cerr << "Send failed: " << strerror(errno) << std::endl;
        return;
    }
    std::cout << "Command sent: " << command << std::endl;

    char buffer[1024] = {0};
    int valread = read(sock, buffer, 1024);
    if (valread < 0) {
        std::cerr << "Read failed: " << strerror(errno) << std::endl;
    } else {
        std::cout << "Server response: " << buffer << std::endl;
    }
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;


    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error: " << strerror(errno) << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported: " << strerror(errno) << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed: " << strerror(errno) << std::endl;
        return -1;
    }
    DSMConfig config;
    // set node number
    config.machineNR = 2;
    DSM *dsm = DSM::getInstance(config);

    dsm->registerThread();

    auto tree = new Tree(dsm);

    Value v;
    std::cout << "get node id" << std::endl;
    if (dsm->getMyNodeID() != 0) {
    }
    // initTree(tree);
    std::cout << "-----------------insert<1,1>-----------------" << std::endl;
    tree->insert(1, 1);
    send_command(sock, "read");
    tree->insert(1, 2);
    send_command(sock, "read");
    while(true)
    ;

    send_command(sock, "read");
    send_command(sock, "insert");

    close(sock);
    return 0;
}