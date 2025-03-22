#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#include <cstddef>  // For size_t
#include <string>
#include <map>

// Estructura del memory map
struct MemoryMapEntry {
    int id;                 // Identificador unico para el memory block
    size_t size;            // Tamanno de la memoria asignada
    std::string type;       // Tipo de dato almacenado
    void* blockPointer;     // Ptr al memory block

    MemoryMapEntry(int _id, size_t _size, const std::string& _type, void* _ptr)
        : id(_id), size(_size), type(_type), blockPointer(_ptr) {}
};

// Memory Map class to manage memory allocations
class MemoryMap {
private:
    std::map<int, MemoryMapEntry> entries;
    int nextId = 1;

public:
    MemoryMap() = default;
    ~MemoryMap() = default;

    // Add a new entry to the memory map
    int addEntry(size_t size, const std::string& type, void* blockPointer);

    // Obtiene el Id de entrada
    MemoryMapEntry* getEntry(int id);

    // Remueve Id (por implementar)
    bool removeEntry(int id);

    // Obtiene todas las entradas
    const std::map<int, MemoryMapEntry>& getAllEntries() const;
};

#endif // MEMORY_MAP_H

