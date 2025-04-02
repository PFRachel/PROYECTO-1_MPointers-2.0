#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#include <cstddef>  // Para size_t
#include <string>
#include <map>

// Estructura del memory map
struct MemoryMapEntry {
    int id;                 // Identificador unico para el memory block
    size_t size;            // Tamanno de la memoria asignada
    std::string type;       // Tipo de dato almacenado
    void* blockPointer;     // Ptr al memory block
    bool isAllocated;       // Indicativo de que si se ha asignado memoria

    MemoryMapEntry(int _id, size_t _size, const std::string& _type)
        : id(_id), size(_size), type(_type), blockPointer(nullptr), isAllocated(false) {}
};

// Clase de MemoryMap para manejar las olaciones
class MemoryMap {
private:
    std::map<int, MemoryMapEntry> entries;
    int nextId = 1;

public:
    MemoryMap() = default;
    ~MemoryMap() = default;

    // Annade un nuevo entry a la memoria sin asignarla
    int addEntry(size_t size, const std::string& type);

    // Establece el puntero de bloque para una entrada
    bool setBlockPointer(int id, void* blockPointer);

    // Obtiene el Id de entrada
    MemoryMapEntry* getEntry(int id);

    // Remueve Id (por implementar)
    bool removeEntry(int id);

    // Obtiene todas las entradas
    const std::map<int, MemoryMapEntry>& getAllEntries() const;
};

#endif // MEMORY_MAP_H