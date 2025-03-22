#include "GRPC_Client.h"
#include <iostream>


MemoryManagerClient::MemoryManagerClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(Proyecto1Datos2::MemoryManager::NewStub(channel)) {}

std::string MemoryManagerClient::Create(int size, std::string type) {
    Proyecto1Datos2::CreateRequest request;
    request.set_size(size);
    request.set_type(type);

    Proyecto1Datos2::CreateResponse response;
    grpc::ClientContext context;

    std::cout << "Enviando Create() de Tamanno: " << size << " y tipo: " << type << std::endl;

    grpc::Status status = stub_->Create(&context, request, &response);

    if (status.ok()) {
        return "Memory created with ID: " + std::to_string(response.id());
    } else {
        return "Error: " + status.error_message();
    }
}

std::string MemoryManagerClient::Set(int id, const std::variant<int, float, std::string>& value) {
    Proyecto1Datos2::SetRequest request;
    request.set_id(id);

    // Set the appropriate value in the request based on the type
    if (std::holds_alternative<int>(value)) {
        request.set_int_value(std::get<int>(value));
        std::cout << "Sending Set() with ID: " << id << " and integer value: " << std::get<int>(value) << std::endl;
    } else if (std::holds_alternative<float>(value)) {
        request.set_float_value(std::get<float>(value));
        std::cout << "Sending Set() with ID: " << id << " and float value: " << std::get<float>(value) << std::endl;
    } else if (std::holds_alternative<std::string>(value)) {
        request.set_string_value(std::get<std::string>(value));
        std::cout << "Sending Set() with ID: " << id << " and string value: \"" << std::get<std::string>(value) << "\"" << std::endl;
    }

    Proyecto1Datos2::SetResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->Set(&context, request, &response);

    if (status.ok()) {
        if (response.success()) {
            return "El valor se guardo correctamente en el Id: " + std::to_string(id);
        } else {
            return "No se pudo guardar el valor en el Id: " + std::to_string(id);
        }
    } else {
        return "Error: " + status.error_message();
    }
}

std::string MemoryManagerClient::Get(int id) {
    Proyecto1Datos2::GetRequest request;
    request.set_id(id);

    Proyecto1Datos2::GetResponse response;
    grpc::ClientContext context;

    std::cout << "Enviando un Get() con ID: " << id << std::endl;

    grpc::Status status = stub_->Get(&context, request, &response);

    if (status.ok()) {
        std::string result;
        if (response.has_int_value()) {
            result = "Valor del ID" + std::to_string(id) + ": " + std::to_string(response.int_value());
        } else if (response.has_float_value()) {
            result = "Valor del ID " + std::to_string(id) + ": " + std::to_string(response.float_value());
        } else if (response.has_string_value()) {
            result = "Valor del ID " + std::to_string(id) + ": \"" + response.string_value() + "\"";
        } else {
            result = "El ID no tiene valor asignado " + std::to_string(id);
        }
        return result;
    } else {
        return "Error: " + status.error_message();
    }
}

std::string MemoryManagerClient::IncreaseRefCount(int id) {
    Proyecto1Datos2::RefCountRequest request;
    request.set_id(id);

    Proyecto1Datos2::RefCountResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->IncreaseRefCount(&context, request, &response);

    if (status.ok()) {
        if (response.success()) {
            return "RefCount Incremento: " + std::to_string(id);
        } else {
            return "RefCount Incremento: " + std::to_string(id);
        }
    } else {
        return "Error: " + status.error_message();
    }
}

std::string MemoryManagerClient::DecreaseRefCount(int id) {
    Proyecto1Datos2::RefCountRequest request;
    request.set_id(id);

    Proyecto1Datos2::RefCountResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->DecreaseRefCount(&context, request, &response);

    if (status.ok()) {
        if (response.success()) {
            return "Decremento RefCount: " + std::to_string(id);
        } else {
            return "Decremento RefCount: " + std::to_string(id);
        }
    } else {
        return "Error: " + status.error_message();
    }
}

