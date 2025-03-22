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

void MemoryManagerService::DumpMemoryState() {
    // Crea una carpeta si todavia no existe
    std::filesystem::create_directories(dumpFolder);

    // Obtiene el tiempo del computador para asignarlo a los dumps
    auto now = std::chrono::system_clock::now();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::ostringstream filename;
    filename << dumpFolder << "/memory_dump_"
             << std::put_time(std::localtime(&now_time), "%Y%m%d_%H%M%S")
             << "_" << std::setfill('0') << std::setw(3) << millis.count()
             << ".txt";

    std::string fullPath = filename.str();

    // Create and write to dump file
    std::ofstream dumpFile(fullPath);
    if (dumpFile.is_open()) {
        dumpFile << "Estado de memoria actual:\n";
        dumpFile << "============================\n";

        // Dump memory map entries
        const auto& memoryMap = memoryManager.getMemoryMap();
        for (const auto& pair : memoryMap.getAllEntries()) {
            const auto& entry = pair.second;
            dumpFile << "ID: " << entry.id
                     << ", Type: " << entry.type
                     << ", Size: " << entry.size
                     << ", Address: " << entry.blockPointer;

            // Imprime el valor segun su tipo
            if (entry.type == "int") {
                dumpFile << ", Value: " << *static_cast<int*>(entry.blockPointer);
            } else if (entry.type == "float") {
                dumpFile << ", Value: " << *static_cast<float*>(entry.blockPointer);
            } else if (entry.type == "string") {
                dumpFile << ", Value: \"" << static_cast<char*>(entry.blockPointer) << "\"";
            }

            dumpFile << std::endl;
        }

        dumpFile.close();
        std::cout << "Memory dump generado en: " << fullPath << std::endl;
    } else {
        std::cerr << "Error: No se pudo crear el archivo dump " << std::endl;
    }
}

grpc::Status MemoryManagerService::Create(grpc::ServerContext* context,
                                         const Proyecto1Datos2::CreateRequest* request,
                                         Proyecto1Datos2::CreateResponse* response) {
    std::string type = request->type();
    size_t size = request->size();

    std::cout << "Solicitud Create() recibida con size: " << size
              << " y type: " << type << std::endl;

    // Crea una asignacion de memoria
    int id = memoryManager.create(size, type);

    if (id > 0) {
        std::cout << "La memoria se asigno con exito al Id: " << id << std::endl;
        response->set_id(id);
        DumpMemoryState();
        return grpc::Status::OK;
    }

    std::cout << "Error: No se pudo asignar la memoria" << std::endl;
    return grpc::Status(grpc::StatusCode::RESOURCE_EXHAUSTED, "No hay suficiente memoria disponible.");
}

grpc::Status MemoryManagerService::Set(grpc::ServerContext* context,
                                      const Proyecto1Datos2::SetRequest* request,
                                      Proyecto1Datos2::SetResponse* response) {
    int id = request->id();
    bool success = false;

    // Handle different value types
    if (request->has_int_value()) {
        int value = request->int_value();
        std::cout << "Solicitud Set() recibida con Id: " << id
                  << " y int value: " << value << std::endl;
        success = memoryManager.set(id, value);
    } else if (request->has_float_value()) {
        float value = request->float_value();
        std::cout << "Solicitud Set() recibida con Id: " << id
                  << " y float value: " << value << std::endl;
        success = memoryManager.set(id, value);
    } else if (request->has_string_value()) {
        std::string value = request->string_value();
        std::cout << "Solicitud Set() recibida con Id: " << id
                  << " y string value: \"" << value << "\"" << std::endl;
        success = memoryManager.set(id, value);
    } else {
        return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "No se recibio un valor");
    }

    response->set_success(success);

    DumpMemoryState();
    return grpc::Status::OK;
}

grpc::Status MemoryManagerService::Get(grpc::ServerContext* context,
                                      const Proyecto1Datos2::GetRequest* request,
                                      Proyecto1Datos2::GetResponse* response) {
    int id = request->id();

    std::cout << "Solicitud Get() recibida con Id: " << id << std::endl;

    ValueType value = memoryManager.get(id);

    // Set del valor aprodiado segun su tipo
    if (std::holds_alternative<int>(value)) {
        response->set_int_value(std::get<int>(value));
    } else if (std::holds_alternative<float>(value)) {
        response->set_float_value(std::get<float>(value));
    } else if (std::holds_alternative<std::string>(value)) {
        response->set_string_value(std::get<std::string>(value));
    }

    return grpc::Status::OK;
}

grpc::Status MemoryManagerService::IncreaseRefCount(grpc::ServerContext* context,
                                                  const Proyecto1Datos2::RefCountRequest* request,
                                                  Proyecto1Datos2::RefCountResponse* response) {
    // Placeholder implementation
    response->set_success(true);
    DumpMemoryState();
    return grpc::Status::OK;
}

grpc::Status MemoryManagerService::DecreaseRefCount(grpc::ServerContext* context,
                                                  const Proyecto1Datos2::RefCountRequest* request,
                                                  Proyecto1Datos2::RefCountResponse* response) {
    // Placeholder implementation
    response->set_success(true);
    DumpMemoryState();
    return grpc::Status::OK;
}

void RunServer(int port, int memsize, const std::string& dumpFolder) {
    // Convierte MB a bytes
    size_t memsizeBytes = static_cast<size_t>(memsize) * 1024 * 1024;

    // Crea el memory maneger
    MemoryManager memoryManager(memsizeBytes);

    // Crea el servidor
    MemoryManagerService service(memoryManager, dumpFolder);

    // Build y Start del servidor
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

    server->Wait(); //El servidor entra en espera de solicitudes
}