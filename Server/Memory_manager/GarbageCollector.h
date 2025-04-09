//
// Created by Rachel on 09/03/2025.
//

#ifndef GARBAGECOLLECTOR_H
#define GARBAGECOLLECTOR_H

#include "MemoryBlock.h"
#include <atomic>
#include <chrono>
#include <thread>

#include "MemoryManager.h"
#include "MemoryMap.h"

class GarbageCollector {
    private:
    MemoryManager& memoryManager;
    std::thread gcThread;
    std::atomic<bool> running;

    public:
    void collectGarbage();
    explicit GarbageCollector(MemoryManager& mm);
    ~GarbageCollector();
    void start();
    void stop();

};
#endif //GARBAGECOLLECTOR_H
