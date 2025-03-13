#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <cstddef>  // Para size_t
#include <cstdlib>  // Para malloc y free
#include <iostream>
#include <vector>
#include <map>

class MemoryManager {
private:
    void* memoryBlock;  // Puntero al bloque de memoria reservado
    size_t totalSize;   // Tamaño total de la memoria reservada
    std::map<int, void*> allocations;  // Mapa que guarda los bloques asignados (ID -> dirección)

public:
    explicit MemoryManager(size_t size);
    ~MemoryManager();

    void* allocate(size_t size, int id);  // Asignar memoria dentro del bloque
    void deallocate(int id);  // Liberar memoria dentro del bloque
    void printMemoryState();  // Mostrar el estado de la memoria
};

#endif // MEMORY_MANAGER_H

