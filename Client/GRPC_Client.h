// GRPC_Client.h
#ifndef GRPC_CLIENT_H
#define GRPC_CLIENT_H

#include <grpcpp/grpcpp.h>
#include "GRPC-Memorymanager.grpc.pb.h"
#include <string>

class MemoryManagerClient {
public:
    // Constructor que recibe un canal para comunicarse con el servidor gRPC
    explicit MemoryManagerClient(std::shared_ptr<grpc::Channel> channel);

    // Método para crear un objeto en el servidor
    std::string Create(int size, int id);

private:
    // Stub para las operaciones de gRPC (cliente)
    std::unique_ptr<Proyecto1Datos2::MemoryManager::Stub> stub_;
};

#endif // GRPC_CLIENT_H
