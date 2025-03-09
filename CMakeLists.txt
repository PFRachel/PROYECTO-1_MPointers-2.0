cmake_minimum_required(VERSION 3.30)
project(Proyecto__1_Datos_2)



set(CMAKE_CXX_STANDARD 17)

# Usar vcpkg como toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Buscar gRPC y Protobuf
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# Incluir las rutas de los headers
include_directories(
        "C:/vcpkg/installed/x64-windows/include"
        "${CMAKE_SOURCE_DIR}/Client"
        "${CMAKE_SOURCE_DIR}/Server"
        "${CMAKE_SOURCE_DIR}/proto"
)

# Agregar ejecutable para el servidor
add_executable(server
        Server/GRPC_Server.cpp
        proto/GRPC-Memorymanager.pb.cc
        proto/GRPC-Memorymanager.grpc.pb.cc
        Test/Linked_list.cpp
        Test/Linked_list.h
        Test/test_memory_manager.cpp
        Test/test_memory_manager.h
        Test/test_mpointers.cpp
        Test/test_mpointers.h
        Server/Mpointers_lib/MPointer.cpp
        Server/Mpointers_lib/MPointer.h
        Server/Memory_manager/GarbageCollector.cpp
        Server/Memory_manager/GarbageCollector.h
        Server/Memory_manager/MemoryBlock.cpp
        Server/Memory_manager/MemoryBlock.h
        Server/Memory_manager/MemoryManager.cpp
        Server/Memory_manager/MemoryManager.h
)
target_link_libraries(server gRPC::grpc++ protobuf::libprotobuf)

# Agregar ejecutable para el cliente
add_executable(client
        Client/GRPC_Client.cpp
        proto/GRPC-Memorymanager.pb.cc
        proto/GRPC-Memorymanager.grpc.pb.cc
)
target_link_libraries(client gRPC::grpc++ protobuf::libprotobuf)