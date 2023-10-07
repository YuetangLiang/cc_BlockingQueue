#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

template<typename T>
class BlockingQueue
{
public:
    BlockingQueue(size_t cap)
    {
        capacity_ = cap;
        size_ = 0;
    }

    bool Empty() const
    {
        std::lock_guard<std::mutex> lock(m_);
        return queue_.empty();
    }

    void Push(T const& data)
    {
        {
        std::lock_guard<std::mutex> lock(m_);

        if(!abort) {

            if(size_ >= capacity_){
                printf("drop\n");
                //queue_.pop();
                //size_--;
                return;
            }

            queue_.push(data);
            size_++;
        }

        }
        cv_.notify_one();
    }

    int shouldwait() {
        return queue_.empty() && !abort;
    }

    int Pop(T &value)
    {
        std::unique_lock<std::mutex> lock(m_);
        while (shouldwait()) {
            cv_.wait(lock, [this]{return !shouldwait(); });
        }

        if(queue_.empty()) {
            return 1;
        }

        value = queue_.front();
        queue_.pop();
        size_--;
        return 0;
    }

    int Pop(T &value, int timeout_ms)
    {
        std::unique_lock<std::mutex> lock(m_);
        while (shouldwait()) {
            if(cv_.wait_for(lock,
                         std::chrono::milliseconds(timeout_ms)) == std::cv_status::timeout) {
                //printf("timeout %d ms\n", timeout_ms);
                return 2;
            }
        }

        if(queue_.empty()) {
            return 1;
        }

        value = queue_.front();
        queue_.pop();
        size_--;
        return 0;
    }

    int Front(T& value)
    {
        std::unique_lock<std::mutex> lock(m_);
        if(queue_.empty()) {
            return 1;
        }
        value = queue_.front();
        return 0;
    }

    void Abort()
    {
        abort = true;
        cv_.notify_one();
    }


private:
    std::queue<T> queue_;
    mutable std::mutex m_;
    mutable std::atomic<bool> abort{false};
    std::condition_variable cv_;

    size_t capacity_;
    size_t size_;
};

#endif

