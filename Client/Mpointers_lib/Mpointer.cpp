#ifndef MPOINTER_CPP
#define MPOINTER_CPP

#include "Mpointer.h"
#include <iostream>
#include <typeinfo>
#include <stdexcept>

// Inicializar la conexión con el servidor
template <typename T>
void MPointer<T>::Init(const std::string& address) {
    if (initialized) {
        std::cout << "MPointer ya inicializado con servidor en " << serverAddress << std::endl;
        return;
    }

    serverAddress = address;
    auto channel = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
    stub_ = Proyecto1Datos2::MemoryManager::NewStub(channel);
    initialized = true;

    std::cout << "MPointer inicializado con servidor en " << address << std::endl;
}

// Obtener el tipo como string para comunicación con el servidor
template <typename T>
std::string MPointer<T>::getTypeString() {
    if constexpr (std::is_same_v<T, int>) {
        return "int";
    } else if constexpr (std::is_same_v<T, float>) {
        return "float";
    } else if constexpr (std::is_same_v<T, double>) {
        return "double";
    } else if constexpr (std::is_same_v<T, char>) {
        return "char";
    } else if constexpr (std::is_same_v<T, bool>) {
        return "bool";
    } else if constexpr (std::is_same_v<T, short>) {
        return "short";
    } else if constexpr (std::is_same_v<T, long>) {
        return "long";
    } else if constexpr (std::is_same_v<T, long long>) {
        return "long long";
    } else if constexpr (std::is_same_v<T, unsigned int>) {
        return "unsigned int";
    } else if constexpr (std::is_same_v<T, unsigned char>) {
        return "unsigned char";
    } else if constexpr (std::is_same_v<T, unsigned short>) {
        return "unsigned short";
    } else if constexpr (std::is_same_v<T, unsigned long>) {
        return "unsigned long";
    } else if constexpr (std::is_same_v<T, unsigned long long>) {
        return "unsigned long long";
    } else if constexpr (std::is_same_v<T, std::string>) {
        return "string";
    } else {
        // Tipo no soportado
        throw std::runtime_error("Tipo no soportado para MPointer");
    }
}

// Convertir valor T a variant para enviar al servidor
template <typename T>
std::variant<int, float, std::string> MPointer<T>::toVariant(const T& value) {
    if constexpr (std::is_same_v<T, int>) {
        return value;
    } else if constexpr (std::is_same_v<T, float>) {
        return value;
    } else if constexpr (std::is_same_v<T, std::string>) {
        return value;
    } else if constexpr (std::is_integral_v<T>) {
        // Convertir otros tipos integrales a int
        return static_cast<int>(value);
    } else if constexpr (std::is_floating_point_v<T>) {
        //Convertir otros tipos de punto flotante a float
        return static_cast<float>(value);
    } else {
        // Tipo no soportado
        throw std::runtime_error("Tipo no soportado para MPointer");
    }
}

// Convertir variant a valor T
template <typename T>
T MPointer<T>::fromVariant(const std::variant<int, float, std::string>& value) {
    if constexpr (std::is_same_v<T, int>) {
        if (std::holds_alternative<int>(value)) {
            return std::get<int>(value);
        }
    } else if constexpr (std::is_same_v<T, float>) {
        if (std::holds_alternative<float>(value)) {
            return std::get<float>(value);
        } else if (std::holds_alternative<int>(value)) {
            return static_cast<float>(std::get<int>(value));
        }
    } else if constexpr (std::is_same_v<T, std::string>) {
        if (std::holds_alternative<std::string>(value)) {
            return std::get<std::string>(value);
        }
    } else if constexpr (std::is_integral_v<T>) {
        // Convertir int
        if (std::holds_alternative<int>(value)) {
            return static_cast<T>(std::get<int>(value));
        }
    } else if constexpr (std::is_floating_point_v<T>) {
        // Convertir float a otros tipos de punto flotante
        if (std::holds_alternative<float>(value)) {
            return static_cast<T>(std::get<float>(value));
        } else if (std::holds_alternative<int>(value)) {
            return static_cast<T>(std::get<int>(value));
        }
    }

    // Si llegamos aquí, no pudimos convertir el valor
    throw std::runtime_error("Error de tipo en recuperación de valor MPointer");
}

// Modificar el metodo New() para soportar más tipos
template <typename T>
MPointer<T> MPointer<T>::New() {
    if (!initialized) {
        throw std::runtime_error("MPointer no inicializado. Llame a MPointer<T>::Init() primero.");
    }

    // Preparar la solicitud para crear memoria
    Proyecto1Datos2::CreateRequest request;
    std::string type = getTypeString();

    // Determinar el tamaño basado en el tipo
    size_t size;

    // Para strings, asignar un tamaño mínimo inicial que se ajustará en el Set
    if constexpr (std::is_same_v<T, std::string>) {
        size = 1; // Tamaño mínimo inicial para strings
        std::cout << "Creando MPointer<string> con tamano inicial mínimo (se ajustará al asignar un valor)" << std::endl;
    } else {
        size = sizeof(T);
    }

    request.set_size(size);
    request.set_type(type);

    // Enviar la solicitud al servidor
    Proyecto1Datos2::CreateResponse response;
    grpc::ClientContext context;

    std::cout << "Se solicita espacio en el bloque de memoria de tipo " << type;
    if constexpr (!std::is_same_v<T, std::string>) {
        std::cout << " con tamanno " << size << "))";
    }
    std::cout << std::endl;

    grpc::Status status = stub_->Create(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Error al reservar memoria: " + status.error_message());
    }

    int id = response.id();
    std::cout << "Bloque de memoria creado con ID: " << id << std::endl;

    return MPointer<T>(id);
}

// Destructor
template <typename T>
MPointer<T>::~MPointer() {
    if (initialized && isValid()) {
        try {
            // Decrementar el contador de referencias
            Proyecto1Datos2::RefCountRequest request;
            request.set_id(memoryId);

            Proyecto1Datos2::RefCountResponse response;
            grpc::ClientContext context;

            grpc::Status status = stub_->DecreaseRefCount(&context, request, &response);

            if (status.ok()) {
                std::cout << "" << std::endl;
            } else {
                std::cerr << "Error al decrementar contador de referencias para ID " << memoryId
                          << ": " << status.error_message() << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Excepcion en destructor de MPointer: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Excepcion desconocida en destructor de MPointer" << std::endl;
        }
    }
}

// Operador de desreferencia (para leer)
template <typename T>
T MPointer<T>::operator*() const {
    if (!initialized) {
        throw std::runtime_error("MPointer no inicializado. Llame a MPointer<T>::Init() primero.");
    }

    if (!isValid()) {
        throw std::runtime_error("MPointer inválido (ID <= 0)");
    }

    // Preparar la solicitud para obtener el valor
    Proyecto1Datos2::GetRequest request;
    request.set_id(memoryId);

    // Enviar la solicitud al servidor
    Proyecto1Datos2::GetResponse response;
    grpc::ClientContext context;

    std::cout << "Obteniendo valor del bloque de memoria ID: " << memoryId << std::endl;

    grpc::Status status = stub_->Get(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Error al obtener valor: " + status.error_message());
    }

    // Convertir la respuesta al tipo T
    std::variant<int, float, std::string> value;

    if (response.has_int_value()) {
        value = response.int_value();
    } else if (response.has_float_value()) {
        value = response.float_value();
    } else if (response.has_string_value()) {
        value = response.string_value();
    } else {
        throw std::runtime_error("No se devolvió ningún valor del servidor");
    }

    return fromVariant(value);
}

// Clase auxiliar para manejar la asignación a través del operador *
template <typename T>
class MPointerRef {
private:
    MPointer<T>& pointer;

public:
    MPointerRef(MPointer<T>& ptr) : pointer(ptr) {}

    // Operador de asignación
    T& operator=(const T& value) {
        if (!MPointer<T>::initialized) {
            throw std::runtime_error("MPointer no inicializado. Llame a MPointer<T>::Init() primero.");
        }

        if (!pointer.isValid()) {
            throw std::runtime_error("MPointer inválido (ID <= 0)");
        }

        // Preparar la solicitud para establecer el valor
        Proyecto1Datos2::SetRequest request;
        request.set_id(pointer.getId());

        // Establecer el valor según el tipo
        std::string type = MPointer<T>::getTypeString();

        if (type == "int" || std::is_integral_v<T>) {
            request.set_int_value(static_cast<int>(value));
        } else if (type == "float" || std::is_floating_point_v<T>) {
            request.set_float_value(static_cast<float>(value));
        } else if (type == "string") {
            const std::string& strValue = static_cast<std::string>(value);
            request.set_string_value(strValue);

            // Para strings, mostrar la longitud
            std::cout << "Estableciendo string de longitud " << strValue.length() << " en ID: " << pointer.getId() << std::endl;
        }

        // Enviar la solicitud al servidor
        Proyecto1Datos2::SetResponse response;
        grpc::ClientContext context;

        std::cout << "Estableciendo valor en bloque de memoria ID: " << pointer.getId() << std::endl;

        grpc::Status status = MPointer<T>::stub_->Set(&context, request, &response);

        if (!status.ok()) {
            throw std::runtime_error("Error al establecer valor: " + status.error_message());
        }

        if (!response.success()) {
            throw std::runtime_error("El servidor reportó un fallo al establecer el valor");
        }

        // Devolver una referencia al valor (aunque es ficticio, ya que el valor real está en el servidor)
        static T dummy_value = value;
        return dummy_value;
    }

    // Conversión implícita a T para permitir lecturas
    operator T() const {
        return *pointer;
    }
};

// Operador de desreferencia (para escribir)
template <typename T>
MPointerRef<T> MPointer<T>::operator*() {
    return MPointerRef<T>(*this);
}

// Operador de dirección
template <typename T>
MPointer<T>* MPointer<T>::operator&() {
    return this;
}

// Operador de asignación
template <typename T>
MPointer<T>& MPointer<T>::operator=(const MPointer<T>& other) {
    if (this != &other) {
        // Si este puntero ya tiene un ID válido, decrementar su contador de referencias
        if (isValid()) {
            try {
                Proyecto1Datos2::RefCountRequest request;
                request.set_id(memoryId);

                Proyecto1Datos2::RefCountResponse response;
                grpc::ClientContext context;

                grpc::Status status = stub_->DecreaseRefCount(&context, request, &response);

                if (status.ok()) {
                    std::cout << "" << std::endl;
                } else {
                    std::cerr << "Error al decrementar contador de referencias para ID " << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Excepcion en operator=: " << e.what() << std::endl;
            }
        }

        // Asignar el nuevo ID
        memoryId = other.memoryId;

        // Incrementar el contador de referencias del nuevo ID
        if (isValid()) {
            try {
                Proyecto1Datos2::RefCountRequest request;
                request.set_id(memoryId);

                Proyecto1Datos2::RefCountResponse response;
                grpc::ClientContext context;

                grpc::Status status = stub_->IncreaseRefCount(&context, request, &response);

                if (status.ok()) {
                    std::cout << "" << std::endl;
                } else {
                    std::cerr << "Error al incrementar contador de referencias para ID " << memoryId
                              << ": " << status.error_message() << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Excepcion en operator=: " << e.what() << std::endl;
            }
        }
    }
    return *this;
}

#endif // MPOINTER_CPP