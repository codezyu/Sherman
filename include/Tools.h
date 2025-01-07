enum  MyOperationType{
    RDMA = 1 ,
    Atomic = 1 << 1 ,
    RDMA_WRITE = 1 << 2,
    RDMA_READ = 1 << 3

};
class Counter{
    // may not need atomic operation
    public:
        long long count;
        long long atomic_count;
        long long write_count;
        long long read_count;
        void increment(int type = MyOperationType::RDMA){
            count++;
            if(type & MyOperationType::Atomic){
                atomic_count++;
            }
            else if(type & MyOperationType::RDMA_WRITE){
                write_count++;
            }
            else if(type & MyOperationType::RDMA_READ){
                read_count++;
            }

        }
        long long get_count(){
            return count;
        }
        Counter(){
            reset();
        }
        void reset(){
            count = 0;
            atomic_count = 0;
            write_count = 0;
            read_count = 0;
        }
};