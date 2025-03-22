//
// Created by Rachel on 09/03/2025.
//

#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

#include <cstddef>
#include <cstdlib>
#include <iostream>

class MemoryBlock {
private:
    void* memoryBlock;      // Puntero al memory block asignado
    size_t totalSize;       // Tamanno total del memoryblock
    size_t nextAvailableOffset = 0;  // Offset para la siguiente asignacion

public:
    explicit MemoryBlock(size_t size);
    ~MemoryBlock();

    // Asigna un espacio de memoria determinada y retorna el puntero que la señala
    void* allocate(size_t size);

    // Get the total size of the memory block
    size_t getTotalSize() const { return totalSize; }

    // Get the available size in the memory block
    size_t getAvailableSize() const { return totalSize - nextAvailableOffset; }

    // Get the base address of the memory block
    void* getBaseAddress() const { return memoryBlock; }
};

#endif // MEMORY_BLOCK_H




