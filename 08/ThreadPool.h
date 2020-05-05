#pragma once
#include <cstdlib>
#include <future>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
private:
    bool endOfWork;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;

    std::mutex mutex;
    std::condition_variable condition;

public:
    explicit ThreadPool(size_t poolSize): endOfWork(false)
    {
        for (int i = 0; i < poolSize; ++i)
        {
            threads.emplace_back(
                    [this]()
                    {
                        while (true)
                        {
                            std::unique_lock<std::mutex> lock(this->mutex);
                            this->condition.wait(lock, [this]()
                            {
                                return this->endOfWork || !this->tasks.empty();
                            });

                            if (this->endOfWork && this->tasks.empty())
                                return;

                            auto currentTask(std::move(this->tasks.front()));
                            this->tasks.pop();

                            currentTask();
                        }
                    });
        }
    }

    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(mutex);
            endOfWork = true;
        }
        condition.notify_all();

        for (auto& thread: threads)
            thread.join();
    }

    // pass arguments by value
    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))>
    {
        auto task = std::make_shared<std::packaged_task<decltype(
                func(args...))()>>(std::bind(func, args...)
        );

        {
            std::unique_lock<std::mutex> lock(mutex);

            tasks.emplace(
                [task]()
                {
                    (*task)();
                }
            );
        }

        condition.notify_one();
        return task->get_future();
    }
};