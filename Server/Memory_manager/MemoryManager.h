#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <cstddef>  // Para size_t
#include <iostream>
#include <string>
#include <variant>
#include "MemoryBlock.h"
#include "MemoryMap.h"
#include <mutex>
#include <map>

#include <unordered_map>

// Define el tipo de dato escrito por el cliente
using ValueType = std::variant<int, float, std::string>;

class MemoryManager {
private:
    MemoryBlock memoryBlock;  // Memory block guardas los valores
    MemoryMap memoryMap;      // Memory map para seguir los punteros
    //std::mutex memoryMutex;// mutable para poder usarlo en métodos const
    mutable std::mutex memoryMutex;
public:
    explicit MemoryManager(size_t size);
    ~MemoryManager() = default;
    //-------------
    // Métodos de sincronización
    void lockMemory();
    void unlockMemory();
    // Acceso al memory map
    MemoryMap& getMemoryMap();
    const MemoryMap& getMemoryMap() const;
    //==========OPERADORES=====================\

    // crea una asignación de memoria dentro
    int create(size_t size, const std::string& type);

    // Asigna determinado valor de memoria al un Id determinado
    bool set(int id, const ValueType& value);

    // Obteiene el valor guardado en el Id
    ValueType get(int id);

    // incrementa conteo
    void IncreaseRefCount(int id);
    //Decrementa conteo
    void DecreaseRefCount(int id);
    //manejo de referencias
    int getReferenceCount(int id) const;
    // liberar espacio
    bool Free(int id);
    void removeEntry(int id);


    //----------------------------------
    // Print del estado de memoria actual
    void printMemoryState();
    // memory map para las operaciones del dump
    const std::map<int, MemoryMapEntry>& getAllEntries() const;
};

#endif // MEMORY_MANAGER_H
