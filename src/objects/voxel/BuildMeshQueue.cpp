#include "BuildMeshQueue.hpp"

std::unique_ptr<BuildMeshQueue> meshQueueBuilder = std::make_unique<BuildMeshQueue>(8);

BuildMeshQueue::BuildMeshQueue(int maxThreads): maxThreads(maxThreads) {
    if(maxThreads <= 0)
        unrestricted = true;
}

BuildMeshQueue::~BuildMeshQueue() = default;

void BuildMeshQueue::update() {
    // Clear tracking of finished threads
    auto it = running.begin();
    while(it != running.end()) {
        bool finished = (*it)->load();
        if(finished) {
            delete (*it);
            running.erase(it++);
        } else 
            it++;
    }


    // Start new threads if able to
    int allowedNew = maxThreads - running.size();
    //printf("BuildMeshQueue: queued %i, running %i, can start %i\n", (int) q.size(), (int) running.size(), allowedNew);
    while(!q.empty() && (unrestricted || allowedNew > 0)) {
        // Create tracker
        auto *tracker = new std::atomic<bool>();
        running.push_back(tracker);

        // Start thread
        auto f = q.front();
        q.pop_front();
        std::thread t = std::thread([tracker, f]() {
            f(); // Execute
            tracker->store(true); // Mark finished
        });
        t.detach();
        allowedNew--;
    }
}

void BuildMeshQueue::queue(std::function<void()> f) {
    q.push_back(f);
}
