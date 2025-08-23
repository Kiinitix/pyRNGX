// Simple worker pool consuming std::function<void()> tasks.
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

class WorkerPool {
public:
    explicit WorkerPool(size_t n) : stop_(false) {
        for (size_t i = 0; i < n; ++i) {
            workers_.emplace_back([this] {
                for (;;) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lk(mu_);
                        cv_.wait(lk, [this] { return stop_ || !q_.empty(); });
                        if (stop_ && q_.empty()) return;
                        task = std::move(q_.front()); q_.pop();
                    }
                    task();
                }
            });
        }
    }

    void submit(std::function<void()> f) {
        {
            std::lock_guard<std::mutex> lg(mu_);
            q_.push(std::move(f));
        }
        cv_.notify_one();
    }

    ~WorkerPool() {
        {
            std::lock_guard<std::mutex> lg(mu_);
            stop_ = true;
        }
        cv_.notify_all();
        for (auto& w : workers_) if (w.joinable()) w.join();
    }

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> q_;
    std::mutex mu_;
    std::condition_variable cv_;
    bool stop_;
};
