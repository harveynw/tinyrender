#pragma once

#include <deque>
#include <atomic>
#include <list>
#include <thread>

class BuildMeshQueue {
private:
    std::deque<std::function<void()>> q;
    std::list<std::atomic<bool>*> running;
    bool unrestricted = false; // Ignore maxThreads
    int maxThreads; // If above is false, will always have no more than maxThreads running
public:
    explicit BuildMeshQueue(int maxThreads);
    ~BuildMeshQueue();

    void update();
    void queue(std::function<void()> f);
};

extern std::unique_ptr<BuildMeshQueue> meshQueueBuilder;
