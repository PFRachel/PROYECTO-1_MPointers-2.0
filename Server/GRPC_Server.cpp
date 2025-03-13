//
// Created by Rachel on 08/03/2025.
//

#include "GRPC_Server.h"
#include <iostream>
#include <memory>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include "Memory_manager/MemoryManager.h"
MemoryManagerService::MemoryManagerService(MemoryManager& memMgr, const std::string& dumpPath)
    : memoryManager(memMgr), dumpFolder(dumpPath) {}

//funcion generar el archivo de dump con el estado de la memoria
void MemoryManagerService::DumpMemoryState() {
    //crear el directorio si no existe
    std::filesystem::create_directories(dumpFolder);
    //obtener la fecha y hora actual con milisegundos
    auto now = std::chrono::system_clock::now();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch())%1000;
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::ostringstream filename;
    filename << dumpFolder << "/memory_dump_"
            << std::put_time(std::localtime(&now_time), "%Y%m%d_%H%M%S")
             << "_" << std::setfill('0') << std::setw(3) << millis.count()
             << ".txt";
    // crear y escribir en el archivo de dump
    std::ofstream dumpFile(filename.str());
    if (dumpFile.is_open()) {
        dumpFile << "Estado actual de la memoria:\n";
        dumpFile << "============================\n";
        dumpFile << "[Aquí deberías incluir la estructura de datos que representa la memoria]\n";
        dumpFile.close();
    }else {
        std::cerr << "Error: No se pudo crear el archivo de dump en " << filename.str() << std::endl;
    }
}
grpc::Status MemoryManagerService::Create(grpc::ServerContext* context, const Proyecto1Datos2::CreateRequest* request, Proyecto1Datos2::CreateResponse* response) {
    int id = request->id();  // ✅ Obtener el ID enviado por el cliente
    size_t size = request->size();  // ✅ Obtener tamaño

    std::cout << "Recibida solicitud de Create() con ID: " << id << " y tamano: " << size << std::endl;

    if (id == 0) {
        std::cerr << "⚠️ Advertencia: El cliente está enviando ID = 0" << std::endl;
    }

    void* mem = memoryManager.allocate(size, id);  // ✅ Intentar asignar memoria
    if (mem) {
        std::cout << "Memoria asignada correctamente con ID: " << id << std::endl;
        response->set_id(id);  // ✅ Devolver el ID en la respuesta
        DumpMemoryState();
        return grpc::Status::OK;
    }

    std::cout << "Error: No se pudo asignar memoria para ID: " << id << std::endl;
    return grpc::Status(grpc::StatusCode::RESOURCE_EXHAUSTED, "No hay suficiente memoria.");
}

grpc::Status MemoryManagerService::Set(grpc::ServerContext* context, const Proyecto1Datos2::SetRequest* request, Proyecto1Datos2::SetResponse* response) {
    response->set_success(true);
    DumpMemoryState();  // Generar dump después de modificar la memoria
    return grpc::Status::OK;
}
grpc::Status MemoryManagerService::Get(grpc::ServerContext* context, const Proyecto1Datos2::GetRequest* request, Proyecto1Datos2::GetResponse* response) {
    response->set_value("example_value");
    DumpMemoryState();  // Generar dump después de modificar la memoria
    return grpc::Status::OK;
}

grpc::Status MemoryManagerService::IncreaseRefCount(grpc::ServerContext* context, const Proyecto1Datos2::RefCountRequest* request, Proyecto1Datos2::RefCountResponse* response) {
    response->set_success(true);
    DumpMemoryState();  // Generar dump después de modificar la memoria
    return grpc::Status::OK;
}

grpc::Status MemoryManagerService::DecreaseRefCount(grpc::ServerContext* context, const Proyecto1Datos2::RefCountRequest* request, Proyecto1Datos2::RefCountResponse* response) {
    response->set_success(true);
    DumpMemoryState();  // Generar dump después de modificar la memoria
    return grpc::Status::OK;
}
void RunServer(int port, int memsize, const std::string& dumpFolder) {
    MemoryManager memoryManager(memsize);
    MemoryManagerService service(memoryManager,dumpFolder);

    grpc::ServerBuilder builder;
    std::string server_address = "0.0.0.0:" + std::to_string(port);
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