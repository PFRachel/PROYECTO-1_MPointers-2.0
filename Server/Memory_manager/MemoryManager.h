#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <cstddef>  // Para size_t
#include <iostream>
#include <string>
#include <variant>
#include "MemoryBlock.h"
#include "MemoryMap.h"

// Define el tipo de dato escrito por el cliente
using ValueType = std::variant<int, float, std::string>;

class MemoryManager {
private:
    MemoryBlock memoryBlock;
    MemoryMap memoryMap;

public:
    explicit MemoryManager(size_t size);
    ~MemoryManager() = default;

    // crea una asignación de memoria dentro
    int create(size_t size, const std::string& type);

    // Asigna determinado valor de memoria al un Id determinado
    bool set(int id, const ValueType& value);

    // Obteiene el valor guardado en el Id
    ValueType get(int id);

    // Print del estado de memoria actual
    void printMemoryState();

    // memory map para las operaciones del dump
    const MemoryMap& getMemoryMap() const;
};

#endif // MEMORY_MANAGER_H
