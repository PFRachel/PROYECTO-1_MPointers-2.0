//
// Created by Rachel on 09/03/2025.
//
#include "MemoryManager.h"
#include <cstring>

MemoryManager::MemoryManager(size_t memsize) : memoryBlock(memsize) {
    std::cout << "Memory Manager inicializado con " << memsize << " bytes" << std::endl;
}

int MemoryManager::create(size_t size, const std::string& type) {
    // Asigna memoria en memory block
    int id = memoryMap.addEntry(size, type);

    std::cout << "Memory entry de tipo: '" << type << "' creado con ID " << id
              << ", size: " << size << " bytes (La memoria se asignara al bloque con un set)" << std::endl;
    return id;
}

bool MemoryManager::set(int id, const ValueType& value) {
    MemoryMapEntry* entry = memoryMap.getEntry(id);
    if (!entry) {
        std::cerr << "Error: Id " << id << "no encontrado " << std::endl;
        return false;
    }

    // Asigna la memoria si es la primera vez que se establece un valor
    if (!entry->isAllocated) {
        void* blockAddress = memoryBlock.allocate(entry->size);
        if (!blockAddress) {
            std::cerr << "Error: No se pudo asignar memoria en el bloque." << std::endl;
            return false;
        }

        // Actualizar la entrada con la dirección de memoria asignada
        memoryMap.setBlockPointer(id, blockAddress);
        std::cout << "Memoria asignada con id: " << id << " en la direccion " << blockAddress << std::endl;
    }

    // Handle different value types based on the entry type
    if (entry->type == "int") {
        if (std::holds_alternative<int>(value)) {
            *static_cast<int*>(entry->blockPointer) = std::get<int>(value);
            std::cout << "Int value " << std::get<int>(value) << " almacenado en el Id " << id << std::endl;
            return true;
        } else {
            std::cerr << "Error: la entrada debe ser un entero" << std::endl;
            return false;
        }
    } else if (entry->type == "float") {
        if (std::holds_alternative<float>(value)) {
            *static_cast<float*>(entry->blockPointer) = std::get<float>(value);
            std::cout << "Float value " << std::get<float>(value) << " almacenado en el Id " << id << std::endl;
            return true;
        } else {
            std::cerr << "Error: la entrada debe ser un flotante" << std::endl;
            return false;
        }
    } else if (entry->type == "string") {
        if (std::holds_alternative<std::string>(value)) {
            const std::string& str = std::get<std::string>(value);
            // Comprueba si el string cabe en la memoria asignada
            if (str.length() >= entry->size) {
                std::cerr << "Error: El string supera la capacidad de memoria asignada"
                           << std::endl;
                return false;
            }
            // Copia el string incluyendo el terminador nulo
            std::strncpy(static_cast<char*>(entry->blockPointer), str.c_str(), entry->size);

            static_cast<char*>(entry->blockPointer)[entry->size - 1] = '\0';
            std::cout << "String value \"" << str << "\" almacenado en el Id " << id << std::endl;
            return true;
        } else {
            std::cerr << "Error de tipo" << std::endl;
            return false;
        }
    } else {
        std::cerr << "Error: Solo se aceptan Enteros, Flotantes y Strings" << std::endl;
        return false;
    }
}

ValueType MemoryManager::get(int id) {
    MemoryMapEntry* entry = memoryMap.getEntry(id);
    if (!entry) {
        std::cerr << "Error: ID " << id << " no encontrado" << std::endl;
        return 0; // Default return value
    }

    // Revisa si la memoria ha sido asignada
    if (!entry->isAllocated) {
        std::cerr << "Error: La memoria para el ID: " << id << " no ha sido asignada aun." << std::endl;
        return 0; // Default retorna value
    }

    if (entry->type == "int") {
        int value = *static_cast<int*>(entry->blockPointer);
        std::cout << "Int value " << value << " del Id " << id << std::endl;
        return value;
    } else if (entry->type == "float") {
        float value = *static_cast<float*>(entry->blockPointer);
        std::cout << "Float value " << value << " del Id " << id << std::endl;
        return value;
    } else if (entry->type == "string") {
        std::string value = static_cast<char*>(entry->blockPointer);
        std::cout << "String value \"" << value << "\" del Id " << id << std::endl;
        return value;
    } else {
        std::cerr << "Solo se aceptan Enteros, Flotantes y Strings " << entry->type << std::endl;
        return 0; // Default return value
    }
}

void MemoryManager::printMemoryState() {
    std::cout << "Current memory state:\n";
    for (const auto& pair : memoryMap.getAllEntries()) {
        const auto& entry = pair.second;
        std::cout << "ID " << entry.id
                  << " -> Type: " << entry.type
                  << ", Size: " << entry.size;

        if (entry.isAllocated) {
            std::cout << ", Address: " << entry.blockPointer;

            // Print the value based on the type
            if (entry.type == "int") {
                std::cout << ", Value: " << *static_cast<int*>(entry.blockPointer);
            } else if (entry.type == "float") {
                std::cout << ", Value: " << *static_cast<float*>(entry.blockPointer);
            } else if (entry.type == "string") {
                std::cout << ", Value: \"" << static_cast<char*>(entry.blockPointer) << "\"";
            }
        } else {
            std::cout << " (Memoria sin asignar)";
        }

        std::cout << std::endl;
    }
}

const MemoryMap& MemoryManager::getMemoryMap() const {
    return memoryMap;
}

