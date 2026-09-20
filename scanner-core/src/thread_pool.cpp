#include "thread_pool.h"

ThreadPool::ThreadPool(size_t threads){
    for(size_t i = 0; i < threads; ++i){
        workers_.emplace_back(&ThreadPool::worker_loop, this);
    }
}

void ThreadPool::enqueue(Task task){
    tasks_.push(task);
}

void ThreadPool::worker_loop(){
    while(true){
        Task task;
        if(tasks_.pop(task)){
            task();
        } else {
            break;
        }
    }
}

ThreadPool::~ThreadPool() {
    tasks_.stop(); 
    for (std::thread &worker : workers_) {
        if (worker.joinable()) {
            worker.join(); 
        }
    }
}