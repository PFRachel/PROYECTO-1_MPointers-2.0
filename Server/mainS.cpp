//
// Created by Rachel on 09/03/2025.
//

#include <iostream>
#include <cstdlib>
#include <filesystem>
#include "GRPC_Server.h"

int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cerr << "Uso:./mem-mgr --port LISTEN_PORT --memsize SIZE_MB --dumpFolder DUMP_FOLDER" << std::endl;
        return 1;
    }

    int port = 0;
    int memsize = 0;
    std::string subfolder;
    // Analizar argumentos de la línea de comandos
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--port") {
            port = std::stoi(argv[i + 1]);
        } else if (std::string(argv[i]) == "--memsize") {
            memsize = std::stoi(argv[i + 1]);
        } else if (std::string(argv[i]) == "--dumpFolder") {
            subfolder = argv[i + 1];
        }
    }
    // Verificar que todos los argumentos fueron recibidos correctamente
    if (port == 0 || memsize == 0 || subfolder.empty()) {
        std::cerr << "Faltan argumentos. Uso: ./mem-mgr --port LISTEN_PORT --memsize SIZE_MB --dumpFolder DUMP_FOLDER" << std::endl;
        return 1;
    }

    // Construtor la ruta al directorio Dump
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path projectRoot;

    if (currentPath.filename() == "cmake-build-debug") {
        projectRoot = currentPath.parent_path();
    } else {
        projectRoot = currentPath;
    }

    // Create un folder dentro de la carpeta Dump que registra cada cambio en memoria
    std::filesystem::path dumpPath = projectRoot / "Dump" / subfolder;
    std::string dumpFolder = dumpPath.string();

    std::cout << "Usando dump folder: " << dumpFolder << std::endl;

    // Ejecutar el servidor con los parámetros
    RunServer(port, memsize, dumpFolder);
    return 0;
}