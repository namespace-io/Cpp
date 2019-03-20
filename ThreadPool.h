#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <future>
#include <functional>
#include <atomic>
#include <stdexcept>

#define THREADPOOL_MAXSIZE 10
class ThreadPool
{
    using Task = std::function<void()>;
private:
    
    size_t size_;
    std::atomic<bool> stop_;
    std::condition_variable cv_;
    std::vector<std::thread> thread_pool_;
    std::queue<Task> task_queue_;
    std::mutex queue_lock_;
    // std::vector<>
public:
    template<typename F, typename ...Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))>{
        if(stop_) throw std::runtime_error("ThreadPool is stopped.");
        using RetType = decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<RetType()>>
                    (std::bind(std::forward<F>(f), std::forward<Args>(args)...)); 
                    // https://en.cppreference.com/w/cpp/thread/packaged_task 
                     
        auto res = task->get_future(); //获取返回值
        {
            std::unique_lock<std::mutex> lock{queue_lock_};
            task_queue_.emplace([task](){ (*task)(); });
        }
        cv_.notify_one(); // 任务入队，唤醒一个线程
        return res;
    };

    ThreadPool(size_t size):size_(size), stop_(false){
        for(size_t i = 0; i < size_; ++i){
            thread_pool_.emplace_back([this]{ // 
                while(true){
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock{this->queue_lock_};
                        this->cv_.wait(lock, [this]{ return this->stop_ || !this->task_queue_.empty(); });
                        // std::condition_variable works only with std::unique_lock<std::mutex>
                        if(this->stop_ && this->task_queue_.empty()) return; // 如果线程池stop了直接返回
                        task = std::move(this->task_queue_.front()); // 返回右值引用
                        this->task_queue_.pop(); 
                    }
                    task();
                }
            });
        }
    };
    ~ThreadPool(){
        stop_ = true;
        cv_.notify_all();
        for(std::thread &thread : thread_pool_)
            thread.join();
    };
};