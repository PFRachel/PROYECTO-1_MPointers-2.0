//
// Created by Rachel on 09/03/2025.
//

#include "MemoryBlock.h"
#include <cstring>  // For memset

MemoryBlock::MemoryBlock(size_t size) : totalSize(size), nextAvailableOffset(0) {
    memoryBlock = malloc(size); //Se asigna el size de MB en bytes al memory block
    if (!memoryBlock) {
        std::cerr << "No se pudo asignar el bloque de memoria" << std::endl;
        exit(1);
    }
    memset(memoryBlock, 0, size);  // Initializa la memoria en 0
    std::cout << "Bloque de memoria asignado con exito: " << size << " bytes" << std::endl;
}

MemoryBlock::~MemoryBlock() {
    free(memoryBlock);  // Liberar el bloque de memoria
    std::cout << "Bloque de memoria liberado." << std::endl;
}

void* MemoryBlock::allocate(size_t size) {
    if (nextAvailableOffset + size > totalSize) {
        std::cerr << "No hay suficiente espacio en el memory block" << std::endl;
        return nullptr;
    }

    // Calcula las direcciones de memoria en el memory block
    void* blockAddress = static_cast<char*>(memoryBlock) + nextAvailableOffset;

    // actualiza la posición de la siguiente memoria disponible
    nextAvailableOffset += size;

    return blockAddress;
}
