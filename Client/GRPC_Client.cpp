// GRPC_Client.cpp
#include "GRPC_Client.h"
#include <iostream>

MemoryManagerClient::MemoryManagerClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(Proyecto1Datos2::MemoryManager::NewStub(channel)) {}

std::string MemoryManagerClient::Create(int size, const std::string& type) {
    Proyecto1Datos2::CreateRequest request;
    request.set_size(size);
    request.set_type(type);

    Proyecto1Datos2::CreateResponse response;
    grpc::ClientContext context;

    // Llamada al método gRPC

    grpc::Status status = stub_->Create(&context, request, &response);

    if (status.ok()) {
        // Aquí debes acceder al campo adecuado
        return "Servidor responde con ID: " + std::to_string(response.id());
    } else {
        return "Error al conectar con el servidor";
    }
}
