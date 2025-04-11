#ifndef MPOINTER_H
#define MPOINTER_H

#include <memory>
#include <string>
#include <variant>
#include <type_traits>
#include <grpcpp/grpcpp.h>
#include "../../proto/GRPC-Memorymanager.grpc.pb.h"

// Declaración anticipada de la clase auxiliar
template <typename T>
class MPointerRef;

template <typename T>
class MPointer {
private:
    // ID del bloque de memoria en el Memory Manager
    int memoryId;

    // Stub para comunicación gRPC
    static std::unique_ptr<Proyecto1Datos2::MemoryManager::Stub> stub_;

    // Flag para verificar si la inicialización se ha realizado
    static bool initialized;

    // Dirección del servidor
    static std::string serverAddress;

    // Constructor privado - solo se puede crear a través de New()
    MPointer(int id) : memoryId(id) {}

    // Obtener el tipo como string para comunicación con el servidor
    static std::string getTypeString();

    // Convertir valor T a variant para enviar al servidor
    static std::variant<int, float, std::string> toVariant(const T& value);

    // Convertir variant a valor T
    static T fromVariant(const std::variant<int, float, std::string>& value);

public:
    // Constructor de copia (necesario para std::variant)
    MPointer(const MPointer& other) : memoryId(other.memoryId) {
        if (initialized && isValid()) {
            try {
                // Incrementar el contador de referencias
                Proyecto1Datos2::RefCountRequest request;
                request.set_id(memoryId);

                Proyecto1Datos2::RefCountResponse response;
                grpc::ClientContext context;

                grpc::Status status = stub_->IncreaseRefCount(&context, request, &response);

                if (status.ok()) {
                    std::cout << "MPointer copiado, contador de referencias para ID " << memoryId
                              << " incrementado a " << response.count() << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Excepción en constructor de copia: " << e.what() << std::endl;
            }
        }
    }

    // Inicializar la conexión con el servidor
    static void Init(const std::string& address);

    // Crear un nuevo puntero (reserva memoria en el servidor)
    static MPointer<T> New();

    // Destructor
    ~MPointer();

    // Operador de desreferencia (para leer)
    T operator*() const;

    // Operador de desreferencia (para escribir)
    MPointerRef<T> operator*();

    // Operador de dirección
    MPointer<T>* operator&();

    // Operador de asignación
    MPointer<T>& operator=(const MPointer<T>& other);

    // Obtener el ID de memoria
    int getId() const { return memoryId; }

    // Verificar si el puntero es válido
    bool isValid() const { return memoryId > 0; }

    // Hacer que MPointerRef sea amigo para acceder a miembros privados
    friend class MPointerRef<T>;
};

// Inicialización de variables estáticas
template <typename T>
std::unique_ptr<Proyecto1Datos2::MemoryManager::Stub> MPointer<T>::stub_ = nullptr;

template <typename T>
bool MPointer<T>::initialized = false;

template <typename T>
std::string MPointer<T>::serverAddress = "";

#include "Mpointer.cpp"  // Incluir la implementación para templates

#endif // MPOINTER_H