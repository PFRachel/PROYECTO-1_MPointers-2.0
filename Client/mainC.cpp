//
// Created by Rachel on 09/03/2025.
//
#include "GRPC_Client.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: ./client <IP:PORT>" << std::endl;
        return 1;
    }

    // El primer argumento es la dirección IP y puerto del servidor
    std::string serverAddress = argv[1];


    if (serverAddress.empty()) {
        std::cerr << "Error: Dirección del servidor no especificada." << std::endl;
        return 1;
    }

    MemoryManagerClient client(grpc::CreateChannel(serverAddress, grpc::InsecureChannelCredentials()));

    std::cout << "Conectando al servidor en: " << serverAddress << std::endl;
    std::string response = client.Create(100, 1);  // Asumí que 100 es el tamaño y "int" es el tipo de datos que estás creando
    std::cout << response << std::endl;


    return 0;
}

