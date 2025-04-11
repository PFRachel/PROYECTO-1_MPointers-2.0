#include "GRPC_Client.h"
#include <iostream>

MemoryManagerClient::MemoryManagerClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(Proyecto1Datos2::MemoryManager::NewStub(channel)) {}

std::string MemoryManagerClient::Create(int size, std::string type) {
    try {
        Proyecto1Datos2::CreateRequest request;
        request.set_size(size);
        request.set_type(type);

        Proyecto1Datos2::CreateResponse response;
        grpc::ClientContext context;

        std::cout << "Enviando Create() de Tamanno: " << size << " y tipo: " << type << std::endl;

        grpc::Status status = stub_->Create(&context, request, &response);

        if (status.ok()) {
            return "Memoria creada con Id: " + std::to_string(response.id());
        } else {
            return "Error: " + status.error_message();
        }
    } catch (const std::exception& e) {
        return "Error en Create: " + std::string(e.what());
    } catch (...) {
        return "Error desconocido en Create";
    }
}

std::string MemoryManagerClient::Set(int id, const std::variant<int, float, std::string>& value) {
    try {
        Proyecto1Datos2::SetRequest request;
        request.set_id(id);

        //valor apropiado en la solicitud por tipo de acuerdo al set
        if (std::holds_alternative<int>(value)) {
            request.set_int_value(std::get<int>(value));
            std::cout << "Enviando Set() con ID: " << id << " y entero: " << std::get<int>(value) << std::endl;
        } else if (std::holds_alternative<float>(value)) {
            request.set_float_value(std::get<float>(value));
            std::cout << "Enviando Set() con ID: " << id << " y flotante: " << std::get<float>(value) << std::endl;
        } else if (std::holds_alternative<std::string>(value)) {
            const std::string& strValue = std::get<std::string>(value);
            request.set_string_value(strValue);
            std::cout << "Enviando Set() con ID: " << id << " y string: \"" << strValue << "\" (longitud: "
                      << strValue.length() << ")" << std::endl;
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
    } catch (const std::exception& e) {
        return "Error en Set: " + std::string(e.what());
    } catch (...) {
        return "Error desconocido en Set";
    }
}

std::string MemoryManagerClient::Get(int id) {
    try {
        Proyecto1Datos2::GetRequest request;
        request.set_id(id);

        Proyecto1Datos2::GetResponse response;
        grpc::ClientContext context;

        std::cout << "Enviando un Get() con ID: " << id << std::endl;

        grpc::Status status = stub_->Get(&context, request, &response);

        if (status.ok()) {
            std::string result;
            if (response.has_int_value()) {
                result = "Valor del ID " + std::to_string(id) + ": " + std::to_string(response.int_value());
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
    } catch (const std::exception& e) {
        return "Error en Get: " + std::string(e.what());
    } catch (...) {
        return "Error -1";
    }
}

std::string MemoryManagerClient::GetReferenceCount(int id) {
    try {
        Proyecto1Datos2::RefCountRequest request;
        request.set_id(id);

        Proyecto1Datos2::RefCountResponse response;
        grpc::ClientContext context;

        grpc::Status status = stub_->GetReferenceCount(&context, request, &response);

        if (status.ok()) {
            if (response.success()) {
                return "RefCount para ID " + std::to_string(id) + ": " + std::to_string(response.count());
            } else {
                return "ID " + std::to_string(id) + " no encontrado";
            }
        } else {
            return "Error: " + status.error_message();
        }
    } catch (const std::exception& e) {
        return "Error en GetReferenceCount: " + std::string(e.what());
    } catch (...) {
        return "Error desconocido en GetReferenceCount";
    }
}

std::string MemoryManagerClient::IncreaseRefCount(int id) {
    try {
        Proyecto1Datos2::RefCountRequest request;
        request.set_id(id);

        Proyecto1Datos2::RefCountResponse response;
        grpc::ClientContext context;

        grpc::Status status = stub_->IncreaseRefCount(&context, request, &response);

        if (status.ok()) {
            if (response.success()) {
                return "RefCount incrementado para ID " + std::to_string(id) + ": " + std::to_string(response.count());
            } else {
                return "No se pudo incrementar RefCount para ID " + std::to_string(id);
            }
        } else {
            return "Error: " + status.error_message();
        }
    } catch (const std::exception& e) {
        return "Error en IncreaseRefCount: " + std::string(e.what());
    } catch (...) {
        return "Error -1";
    }
}

std::string MemoryManagerClient::DecreaseRefCount(int id) {
    try {
        Proyecto1Datos2::RefCountRequest request;
        request.set_id(id);

        Proyecto1Datos2::RefCountResponse response;
        grpc::ClientContext context;

        grpc::Status status = stub_->DecreaseRefCount(&context, request, &response);

        if (status.ok()) {
            if (response.success()) {
                return "RefCount decrementado para ID " + std::to_string(id) + ": " + std::to_string(response.count());
            } else {
                return "No se pudo decrementar RefCount para ID " + std::to_string(id);
            }
        } else {
            return "Error: " + status.error_message();
        }
    } catch (const std::exception& e) {
        return "Error en DecreaseRefCount: " + std::string(e.what());
    } catch (...) {
        return "Error -1 en DecreaseRefCount";
    }
}

std::string MemoryManagerClient::Free(int id) {
    try {
        Proyecto1Datos2::FreeRequest request;
        request.set_id(id);

        Proyecto1Datos2::FreeResponse response;
        grpc::ClientContext context;

        grpc::Status status = stub_->Free(&context, request, &response);

        if (status.ok()) {
            if (response.success()) {
                return "Memoria liberada para ID: " + std::to_string(id);
            } else {
                return "No se pudo liberar la memoria para ID: " + std::to_string(id);
            }
        } else {
            return "Error: " + status.error_message();
        }
    } catch (const std::exception& e) {
        return "Error en Free: " + std::string(e.what());
    } catch (...) {
        return "Error desconocido en Free";
    }
}