//
// Created by Rachel on 09/03/2025.
//

#ifndef MPOINTER_H
#define MPOINTER_H
#include <grpcpp/grpcpp.h>
#include "../../Client/GRPC_Client.h"
#include <string>
#include <memory>
#include <typeinfo>
#include <stdexcept>



template <typename T> //platilla para los tipos de datos
class MPointer {
private:
    int memoryId;// identificar bloque de memoria en Memory Manager
    static std::shared_ptr<MemoryManagerClient> client; // cliente grpc
    //
    template <typename U>
    static std::string convertToString(const U& value) {
        if constexpr (std::is_arithmetic_v<U>) {
            return std::to_string(value);
        } else if constexpr (std::is_same_v<U, std::string>) {
            return value;
        } else {
            // serializar
            throw std::runtime_error(" no se logro la conversiondel tipo");
        }
    }
    template <typename U>
    static U convertFromString(const std::string& str) {
        if constexpr (std::is_same_v<U, int>) {
            return std::stoi(str);
        } else if constexpr (std::is_same_v<U, double>) {
            return std::stod(str);
        } else if constexpr (std::is_same_v<U, float>) {
            return std::stof(str);
        } else if constexpr (std::is_same_v<U, std::string>) {
            return str;
        } else {
            throw std::runtime_error("Tipo no compatible para la conversión");
        }
    }
public:
    //Contructor
    MPointer() : memoryId(-1) {}
    //iniciar la conexion con memory manager
    static void Init(const std:: string& serverAddress) {
        //crear canal de comunicacion con Memory Manager
        client = std::make_shared<MemoryManagerClient>(
            grpc::CreateChannel(serverAddress, grpc::InsecureChannelCredentials()));
    }
    // Memotdo estatico para crear un nuevo bloque de memoria;
    // Requerimiento #1:MPointer<int> myPtr = MPointer<int>::New();
    static MPointer<T> New() {
        //vereificamos el cliente
        if (!client) {
            throw std::runtime_error("MPointer no ha sido inicializado. Llame a Init() primero.");
        }
        //  aqui se pide para hacer el nuevo bloque de memoria en MM
        MPointer<T> newPtr;
        newPtr.memoryId = client->Create(sizeof(T),typeid(T).name());
        return newPtr;
    }
    //---------------------------
    //Sobrecargar los operadores
    //----------Operador *-----------------
    T operator*() const {
        if (memoryId == -1) {
            throw std::runtime_error("MPointer no válido");
        }
        std::string retrieved = client->Get(memoryId);
        return convertFromString<T>(retrieved);
    }
    //----------Operador * asignar valor -----------------
    void operator=(const T& value) {
        if (memoryId == -1) {
            throw std::runtime_error("MPointer no válido");
        }
        std::string convertedValue = convertToString(value);
        client->Set(memoryId, convertedValue);
    }
    // Sobrecarga de operador & para obtener ID de memoria
    int operator&() const {
        return memoryId;
    }
    //Método para obtener el ID de memoria
    int getMemoryId() const {
        return memoryId;
    }
    // comparacion con nullptr
    bool operator==(std::nullptr_t) const {
        return memoryId == -1;
    }

    bool operator!=(std::nullptr_t) const {
        return memoryId != -1;
    }
    // Destructor para liberar memoria
    //----------------- falta implementar
    ~MPointer() {
        if (memoryId != -1 && client) {
            // Parte de liberar memoria
            // client->Free(memoryId);
        }
    }

};
//iniciliazar el cliente estatico
template <typename T>
std::shared_ptr<MemoryManagerClient> MPointer<T>::client = nullptr;

#endif //MPOINTER_H
