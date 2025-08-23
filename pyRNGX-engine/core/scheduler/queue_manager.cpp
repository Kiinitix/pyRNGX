// Minimal queue manager (mutex-based). Swap with a real lock-free queue later.
#include <queue>
#include <mutex>
#include <optional>

template <typename T>
class QueueManager {
public:
    void push(T item) {
        std::lock_guard<std::mutex> lg(mu_);
        q_.push(std::move(item));
    }

    std::optional<T> pop() {
        std::lock_guard<std::mutex> lg(mu_);
        if (q_.empty()) return std::nullopt;
        T v = std::move(q_.front());
        q_.pop();
        return v;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lg(mu_);
        return q_.empty();
    }
private:
    mutable std::mutex mu_;
    std::queue<T> q_;
};
