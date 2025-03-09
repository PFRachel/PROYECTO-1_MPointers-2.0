cmake_minimum_required(VERSION 3.30)
project(Proyecto1Datos2)

set(CMAKE_CXX_STANDARD 17)

# Usar vcpkg como toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Buscar gRPC y Protobuf
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)

# Incluir las rutas de los headers
include_directories(
        "C:/vcpkg/installed/x64-windows/include"
        "${CMAKE_SOURCE_DIR}/Client"
        "${CMAKE_SOURCE_DIR}/Server"
        "${CMAKE_SOURCE_DIR}/proto"
        "C:/vcpkg/installed/x64-windows/include/protobuf"  # ruta a los encabezados de protobuf
        "C:/vcpkg/installed/x64-windows/include/grpc"      # ruta a los encabezados de gRPC
)


# 🛠 Generar automáticamente los archivos de gRPC y Protobuf
set(PROTO_DIR "${CMAKE_SOURCE_DIR}/proto")
set(PROTO_SRC "${PROTO_DIR}/GRPC-Memorymanager.proto")
set(PROTO_GEN_SRC "${PROTO_DIR}/GRPC-Memorymanager.pb.cc")
set(PROTO_GEN_HDR "${PROTO_DIR}/GRPC-Memorymanager.pb.h")
set(GRPC_GEN_SRC "${PROTO_DIR}/GRPC-Memorymanager.grpc.pb.cc")
set(GRPC_GEN_HDR "${PROTO_DIR}/GRPC-Memorymanager.grpc.pb.h")

add_custom_command(
        OUTPUT ${PROTO_GEN_SRC} ${PROTO_GEN_HDR} ${GRPC_GEN_SRC} ${GRPC_GEN_HDR}
        COMMAND protobuf::protoc
        ARGS --proto_path=${PROTO_DIR} --cpp_out=${PROTO_DIR} --grpc_out=${PROTO_DIR}
        --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
        ${PROTO_SRC}
        DEPENDS ${PROTO_SRC}
)

# Agrupar los archivos generados
add_library(memory_proto ${PROTO_GEN_SRC} ${GRPC_GEN_SRC})
target_link_libraries(memory_proto PUBLIC protobuf::libprotobuf gRPC::grpc++)

# Agregar ejecutable para el servidor
add_executable(mem-mgr
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
        Server/mainS.cpp
)
target_link_libraries(mem-mgr memory_proto gRPC::grpc++ Threads::Threads)

# Agregar ejecutable para el cliente
add_executable(client
        Client/GRPC_Client.cpp
        Client/mainC.cpp
        proto/GRPC-Memorymanager.pb.cc
        proto/GRPC-Memorymanager.grpc.pb.cc
)
target_link_libraries(client memory_proto gRPC::grpc++ Threads::Threads)
