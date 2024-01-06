#pragma once
#include <vector>

#include <thread>
#include <queue>
#include <functional>
#include <condition_variable>

class ThreadPool {

public:

    const unsigned int numberOfThreads;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> functionsToEvaluate;
    std::mutex mutex;
    bool ready = false;
    std::condition_variable conditionVariable;

public:

    ThreadPool(int numberOfThreads = 12) : numberOfThreads(numberOfThreads) {
        for (int i = 0; i < numberOfThreads; ++i) {
            std::thread thread(&ThreadPool::putTask, this);
            threads.push_back(std::move(thread));
        }
    };

    void addTask(std::function<void()>& task) {

        {
            std::unique_lock<std::mutex> lock(mutex);
            functionsToEvaluate.push(std::move(task));
        }
        conditionVariable.notify_one();
    }

    void putTask() {
        std::function<void()> task;
        while (!ready)
        {
            {
                std::unique_lock<std::mutex> lock(mutex);
                conditionVariable.wait(lock, [&] { return ready || !functionsToEvaluate.empty(); });

                if (!functionsToEvaluate.empty()) {
                    task = std::move(functionsToEvaluate.front());
                    functionsToEvaluate.pop();
                }
            }
            task();
        }
    }

    void stop() {
        {
            std::unique_lock<std::mutex> lock(mutex);
            ready = true;
        }
        conditionVariable.notify_all();
        for (auto& thread : threads) {
            if (thread.joinable())
                thread.join();
        }
        //threads.clear();
    }
};
