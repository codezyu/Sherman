#include "DSM.h"
#include "Tree.h"
#include <chrono>
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
void insertTree(Tree *tree){
 //  the time of insert
  auto start = std::chrono::high_resolution_clock::now();
  for(uint64_t i = 1; i < 10240; ++i){
    tree->insert(1, i * 2);
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start; // 计算执行时间
  std::cout << "Task execution time: " << duration.count() << " seconds" << std::endl;
  // average insert time
  std::cout << "average insert time: " << duration.count() / 10240 << " seconds" << std::endl;
}

int main() {

  DSMConfig config;
  // set node number
  config.machineNR = 1;
  DSM *dsm = DSM::getInstance(config);

  dsm->registerThread();

  auto tree = new Tree(dsm);

  Value v;
  std::cout << "get node id" << std::endl;
  if (dsm->getMyNodeID() != 0) {
    // for master node
    while (true)
      ;
  }
    initTree(tree);
    std::cout << "-----------------insert-----------------" << std::endl;
    insertTree(tree);
    std::cout << "-----------------end-----------------" << std::endl;
    while(true)
    ;
}