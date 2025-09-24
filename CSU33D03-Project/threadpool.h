#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

using namespace std;

class ThreadPool {
public:
    ThreadPool(size_t num_threads = thread::hardware_concurrency());
    ~ThreadPool();

    // Enqueue task and return a future to track completion
    template<class F>
    auto enqueue(F&& f) -> future<typename result_of<F()>::type>;

private:
    vector<thread> threads_;
    queue<function<void()>> tasks_;
    mutex queue_mutex_;
    condition_variable cv_;
    bool stop_ = false;
};

template<class F>
auto ThreadPool::enqueue(F&& f) -> future<typename result_of<F()>::type> {
    using return_type = typename result_of<F()>::type;

    auto task = make_shared<packaged_task<return_type()>>(
        forward<F>(f)
    );

    future<return_type> res = task->get_future();
    {
        unique_lock<mutex> lock(queue_mutex_);
        if(stop_)
            throw runtime_error("enqueue on stopped ThreadPool");
        tasks_.emplace([task](){ (*task)(); });
    }
    cv_.notify_one();
    return res;
}

#endif // THREADPOOL_H