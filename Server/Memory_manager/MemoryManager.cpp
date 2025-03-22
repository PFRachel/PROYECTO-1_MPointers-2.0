//
// Created by Rachel on 09/03/2025.
//
#include "MemoryManager.h"
#include <cstring>  // For memset and strcpy

MemoryManager::MemoryManager(size_t memsize) : memoryBlock(memsize) {
    std::cout << "Memory Manager inicializado con " << memsize << " bytes" << std::endl;
}

int MemoryManager::create(size_t size, const std::string& type) {
    // Asigna memoria en memory block
    void* blockAddress = memoryBlock.allocate(size);
    if (!blockAddress) {
        std::cerr << "Error: No se pudo asignar la memoria" << std::endl;
        return -1;
    }

    // Entrada al memory map
    int id = memoryMap.addEntry(size, type, blockAddress);

    std::cout << "Memoria de tipo '" << type << "' asignada al Id " << id
              << ", de Tamanno: " << size << "en bytes." << std::endl;
    return id;
}

bool MemoryManager::set(int id, const ValueType& value) {
    MemoryMapEntry* entry = memoryMap.getEntry(id);
    if (!entry) {
        std::cerr << "Error: Id " << id << "no encontrado " << std::endl;
        return false;
    }

    // Handle different value types based on the entry type
    if (entry->type == "int") {
        if (std::holds_alternative<int>(value)) {
            *static_cast<int*>(entry->blockPointer) = std::get<int>(value);
            std::cout << "Int value " << std::get<int>(value) << " almacenado en el Id " << id << std::endl;
            return true;
        } else {
            std::cerr << "Error de Tipo" << std::endl;
            return false;
        }
    } else if (entry->type == "float") {
        if (std::holds_alternative<float>(value)) {
            *static_cast<float*>(entry->blockPointer) = std::get<float>(value);
            std::cout << "Float value " << std::get<float>(value) << " almacenado en el Id " << id << std::endl;
            return true;
        } else if (std::holds_alternative<int>(value)) {
            // Allow implicit conversion from int to float
            *static_cast<float*>(entry->blockPointer) = static_cast<float>(std::get<int>(value));
            std::cout << "Float value " << std::get<int>(value) << " almacenado en el Id " << id << std::endl;
            return true;
        } else {
            std::cerr << "Error de tipo" << std::endl;
            return false;
        }
    } else if (entry->type == "string") {
        if (std::holds_alternative<std::string>(value)) {
            const std::string& str = std::get<std::string>(value);
            if (str.length() + 1 > entry->size) {
                std::cerr << "Error: El string excede el tamanno de memoria asignado" << std::endl;
                return false;
            }
            strcpy(static_cast<char*>(entry->blockPointer), str.c_str());
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
        std::cerr << "Todavia no se ha creado un dato con este Id "<< std::endl;
        return 0;
    }

    // Obtiene el valor segun el tipo de dato almacenado
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
        std::cerr << "Solo se aceptan Enteros, Flotantes y Strings " << std::endl;
        return 0; // Default return value
    }
}

void MemoryManager::printMemoryState() {
    std::cout << "Estado de memoria actual:\n";
    for (const auto& pair : memoryMap.getAllEntries()) {
        const auto& entry = pair.second;
        std::cout << "ID " << entry.id
                  << " -> Type: " << entry.type
                  << ", Size: " << entry.size
                  << ", Address: " << entry.blockPointer;

        // Print the value based on the type
        if (entry.type == "int") {
            std::cout << ", Value: " << *static_cast<int*>(entry.blockPointer);
        } else if (entry.type == "float") {
            std::cout << ", Value: " << *static_cast<float*>(entry.blockPointer);
        } else if (entry.type == "string") {
            std::cout << ", Value: \"" << static_cast<char*>(entry.blockPointer) << "\"";
        }

        std::cout << std::endl;
    }
}

const MemoryMap& MemoryManager::getMemoryMap() const {
    return memoryMap;
}
