#include <iostream>
#include <queue>
#include <thread>
#include <string>
#include <functional>
#include <vector>
#include <condition_variable>

template <typename T>
class ThreadSafeQueue{
    private:

    std::queue<T> queue_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool stop_ = false;

   public:
    void push(T val) {
        std::lock_guard<std::mutex> lock(mtx_);
        queue_.push(val);
        cv_.notify_one();
    }

    bool pop(T& val) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this] { return !queue_.empty() || stop_; });

        if (queue_.empty() && stop_) {
            return false; 
        }

        val = queue_.front();
        queue_.pop();
        return true;
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            stop_ = true;
        }
        cv_.notify_all();
    }


};

using Task = std::function<void()>;

class ThreadPool{
    private:
    std::vector<std::thread> workers_;
    ThreadSafeQueue<Task> tasks_;
    bool stop_{false};

    void worker_loop();
    
    public:
    explicit ThreadPool(size_t threads);
    ~ThreadPool();

    void enqueue(Task task);
};