// GRPC_Client.h
#ifndef GRPC_CLIENT_H
#define GRPC_CLIENT_H

#include <grpcpp/grpcpp.h>
#include "GRPC-Memorymanager.grpc.pb.h"
#include <string>
#include <variant>

class MemoryManagerClient {
public:
    // Constructor que recibe un canal para comunicarse con el servidor gRPC
    explicit MemoryManagerClient(std::shared_ptr<grpc::Channel> channel);

    // Metodo para definir un tipo de dato que se guardara en memoria
    std::string Create(int size, std::string type);

    //Metodo para guardar un valor en la posicion de memoria del id en el bloque
    std::string Set(int id, const std::variant<int, float, std::string>& value);

    //Metodo para obtener un dato en determinada posicion de memoria Id
    std::string Get(int id);

    //Metodo para hacer incrementar el numero de referencias de determinado Id (Por implementar
    std::string IncreaseRefCount(int id);

    //Metodo para hacer decrementar el numero de referencias de determinado Id (Por implementar9
    std::string DecreaseRefCount(int id);

private:
    // Stub para las operaciones de gRPC (cliente)
    std::unique_ptr<Proyecto1Datos2::MemoryManager::Stub> stub_;
};

#endif // GRPC_CLIENT_H
