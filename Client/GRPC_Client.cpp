#include "GRPC_Client.h"
#include <iostream>

MemoryManagerClient::MemoryManagerClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(Proyecto1Datos2::MemoryManager::NewStub(channel)) {}

std::string MemoryManagerClient::Create(int size, int id) {
    Proyecto1Datos2::CreateRequest request;
    request.set_size(size);
    request.set_id(id);  // ✅ Asegurar que el ID se está enviando correctamente

    Proyecto1Datos2::CreateResponse response;
    grpc::ClientContext context;

    std::cout << "Enviando Create() con ID: " << id << " y tamano: " << size << std::endl;

    grpc::Status status = stub_->Create(&context, request, &response);

    if (status.ok()) {
        std::cout << "Respuesta del servidor: ID = " << response.id() << std::endl;
        return "Servidor responde con ID: " + std::to_string(response.id());
    } else {
        return "Error al conectar con el servidor";
    }

}