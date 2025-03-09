//
// Created by Rachel on 09/03/2025.
//

#include <iostream>
#include <cstdlib>
#include "GRPC_Server.h"

int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cerr << "Uso: (./mem-mgr)./server.exe --port LISTEN_PORT --memsize SIZE_MB --dumpFolder DUMP_FOLDER" << std::endl;
        return 1;
    }

    int port = 0;
    int memsize = 0;
    std::string dumpFolder;
    // Analizar argumentos de la línea de comandos
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--port") {
            port = std::stoi(argv[i + 1]);
        } else if (std::string(argv[i]) == "--memsize") {
            memsize = std::stoi(argv[i + 1]);
        } else if (std::string(argv[i]) == "--dumpFolder") {
            dumpFolder = argv[i + 1];
        }
    }
    // Verificar que todos los argumentos fueron recibidos correctamente
    if (port == 0 || memsize == 0 || dumpFolder.empty()) {
        std::cerr << "Faltan argumentos. Uso: ./server --port LISTEN_PORT --memsize SIZE_MB --dumpFolder DUMP_FOLDER" << std::endl;
        return 1;
    }

    // Ejecutar el servidor con los parámetros
    RunServer(port, memsize, dumpFolder);
    return 0;

}
