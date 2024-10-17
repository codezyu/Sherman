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
}
void testRangeSearch(Tree *tree){


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
    while (true)
      ;
  }
  initTree(tree);
  std::cout << "-----------------end-----------------" << std::endl;
    while(true)
    ;
}