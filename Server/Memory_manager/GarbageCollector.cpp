//
// Created by Rachel on 09/03/2025.
//

#include "GarbageCollector.h"
#include "../Memory_manager/MemoryManager.h"
#include <vector>
#include <mutex>
#include <memory>
#include <thread>
#include <chrono>
#include <iostream>

GarbageCollector::GarbageCollector(MemoryManager& mm) : memoryManager(mm), running(false) {}
GarbageCollector::~GarbageCollector() {
    stop();
}
void GarbageCollector::start() {
    running = true;
    gcThread = std::thread(&GarbageCollector::collectGarbage, this);
}
void GarbageCollector::stop() {
    running = false;
    if (gcThread.joinable()) {
        gcThread.join();
    }
}
void GarbageCollector::collectGarbage() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(10));

        std::cout << "\n=== [GarbageCollector] Iniciando ciclo ===" << std::endl;

        auto allEntries = memoryManager.getMemoryMap().getAllEntries();
        std::cout << "Total de entradas: " << allEntries.size() << std::endl;
        bool anyFreed = false;  // Declaramos la variable para verificar si se liberó alguna memoria

        for (const auto& [id, entry] : allEntries) {
            std::cout << "Procesando ID: " << id
                     << " | RefCount: " << entry.referenceCount
                     << " | Tipo: " << entry.type << std::endl;

            if (entry.referenceCount <= 0) {
                std::cout << "Intentando liberar ID: " << id << std::endl;
                if (memoryManager.Free(id)) {
                    std::cout << "Liberado exitosamente ID: " << id << std::endl;
                    anyFreed = true;  // Si se libera memoria, cambiamos el valor de anyFreed

                } else {
                    std::cerr << "Error al liberar ID: " << id << std::endl;
                }
            }
        }
        if (anyFreed) {
            std::cout << "[GarbageCollector] Intentando desfragmentar memoria..." << std::endl;
            memoryManager.Defragment();  // Llamamos a Defragment sin capturar su retorno
            std::cout << "[GarbageCollector] Desfragmentacion completada." << std::endl;
        }


        std::cout << "=== [GarbageCollector] Ciclo completado ===\n" << std::endl;
    }
}