#include "MemoryMap.h"

int MemoryMap::addEntry(size_t size, const std::string& type, void* blockPointer) {
    int id = nextId++;
    entries.emplace(id, MemoryMapEntry(id, size, type, blockPointer));
    return id;
}

MemoryMapEntry* MemoryMap::getEntry(int id) {
    auto it = entries.find(id);
    if (it != entries.end()) {
        return &(it->second);
    }
    return nullptr;
}

bool MemoryMap::removeEntry(int id) {
    return entries.erase(id) > 0;
}

const std::map<int, MemoryMapEntry>& MemoryMap::getAllEntries() const {
    return entries;
}

