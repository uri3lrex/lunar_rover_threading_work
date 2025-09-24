#include "threadpool.h"
#include <iostream>
#include <stdexcept>

using namespace std;

ThreadPool::ThreadPool(size_t num_threads) : stop_(false) {
    for(size_t i = 0; i < num_threads; ++i) {
        threads_.emplace_back([this] {
            while(true) {
                function<void()> task;
                {
                    unique_lock<mutex> lock(queue_mutex_);
                    cv_.wait(lock, [this] {
                        return !tasks_.empty() || stop_;
                    });
                    if(stop_ && tasks_.empty())
                        return;
                    task = move(tasks_.front());
                    tasks_.pop();
                }
                try {
                    task();
                } catch(const exception& e) {
                    cerr << "ThreadPool task failed: " << e.what() << endl;
                }
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        unique_lock<mutex> lock(queue_mutex_);
        stop_ = true;
    }
    cv_.notify_all();
    for(thread &worker: threads_) {
        if(worker.joinable())
            worker.join();
    }
}