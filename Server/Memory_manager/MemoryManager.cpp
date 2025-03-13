//
// Created by Rachel on 09/03/2025.
//
#include "MemoryManager.h"
#include <cstring>  // Para memset

MemoryManager::MemoryManager(size_t size) : totalSize(size) {
    memoryBlock = malloc(size);  // Reservamos el bloque de memoria en el heap
    if (!memoryBlock) {
        std::cerr << "Error: No se pudo reservar memoria." << std::endl;
        exit(1);
    }
    memset(memoryBlock, 0, size);  // Inicializamos la memoria en 0
    std::cout << "Memoria reservada correctamente: " << size << " bytes" << std::endl;
}

MemoryManager::~MemoryManager() {
    free(memoryBlock);  // Liberamos la memoria al destruir el objeto
    std::cout << "Memoria liberada." << std::endl;
}

void* MemoryManager::allocate(size_t size, int id) {
    if (allocations.find(id) != allocations.end()) {
        std::cerr << "Error: ID ya está en uso." << std::endl;
        return nullptr;
    }

    if (size > totalSize) {
        std::cerr << "Error: No hay suficiente memoria." << std::endl;
        return nullptr;
    }

    allocations[id] = memoryBlock;  // Asignamos el bloque al ID
    std::cout << "Memoria asignada a ID " << id << std::endl;
    return memoryBlock;
}

void MemoryManager::deallocate(int id) {
    allocations.erase(id);
    std::cerr << "Error: ID no encontrado." << std::endl;

    }



void MemoryManager::printMemoryState() {
    std::cout << "Estado actual de la memoria:\n";
    for (const auto& pair : allocations) {
        std::cout << "ID " << pair.first << " -> Dirección: " << pair.second << std::endl;
    }
}

