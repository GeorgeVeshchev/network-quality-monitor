#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>


template <typename T>
class ThreadSafeQueue{
    private:
    std::queue<T> queue_;
    std::mutex mtx_;
    std::condition_variable cv_;

    public:
    void push(T val){
        std::lock_guard<std::mutex> lock(mtx_);
        queue_.push(val);
        cv_.notify_one();
    }

    bool pop(T& val){
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this]{
            return !queue_.empty();
        });
        val = queue_.front();
        queue_.pop();
        return true;
    }
};