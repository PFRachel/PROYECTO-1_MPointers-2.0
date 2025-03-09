//
// Created by Rachel on 08/03/2025.
//

#include "GRPC_Server.h"
#include <iostream>
#include <memory>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>

grpc::Status MemoryManagerService::Create(grpc::ServerContext* context, const Proyecto1Datos2::CreateRequest* request, Proyecto1Datos2::CreateResponse* response) {
    response->set_id(1);  // Simulación de creación
    return grpc::Status::OK;
}

grpc::Status MemoryManagerService::Set(grpc::ServerContext* context, const Proyecto1Datos2::SetRequest* request, Proyecto1Datos2::SetResponse* response) {
    response->set_success(true);
    return grpc::Status::OK;
}
grpc::Status MemoryManagerService::Get(grpc::ServerContext* context, const Proyecto1Datos2::GetRequest* request, Proyecto1Datos2::GetResponse* response) {
    response->set_value("example_value");
    return grpc::Status::OK;
}

grpc::Status MemoryManagerService::IncreaseRefCount(grpc::ServerContext* context, const Proyecto1Datos2::RefCountRequest* request, Proyecto1Datos2::RefCountResponse* response) {
    response->set_success(true);
    return grpc::Status::OK;
}

grpc::Status MemoryManagerService::DecreaseRefCount(grpc::ServerContext* context, const Proyecto1Datos2::RefCountRequest* request, Proyecto1Datos2::RefCountResponse* response) {
    response->set_success(true);
    return grpc::Status::OK;
}
void RunServer(int port, int memsize, const std::string& dumpFolder) {
    std::string server_address = "0.0.0.0:" + std::to_string(port);
    MemoryManagerService service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());


    std::cout << "==================================" << std::endl;
    std::cout << " Servidor GRPC iniciado" << std::endl;
    std::cout << " Escuchando en el puerto: " << port << std::endl;
    std::cout << " Size de memoria: " << memsize << " MB" << std::endl;
    std::cout << " Carpeta de dumps: " << dumpFolder << std::endl;
    std::cout << "==================================" << std::endl;

    server->Wait();// El servidor entra en espera de solicitudes

}