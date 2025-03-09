cmake_minimum_required(VERSION 3.30)
project(Proyecto__1_Datos_2)




set(CMAKE_CXX_STANDARD 17)

cmake_minimum_required(VERSION 3.30)
project(Proyecto__1_Datos_2)

set(CMAKE_CXX_STANDARD 17)

# Usar vcpkg como toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Buscar gRPC y Protobuf
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# Incluir las rutas de los headers
include_directories("C:/vcpkg/installed/x64-windows/include")

# Agregar ejecutable para el CLIENTE
add_executable(GRPC_Client GRPC_Client.cpp GRPC-Memorymanager.pb.cc GRPC-Memorymanager.grpc.pb.cc)
target_link_libraries(GRPC_Client gRPC::grpc++ protobuf::libprotobuf)

# Agregar ejecutable para el SERVIDOR
add_executable(GRPC_Server GRPC_Server.cpp GRPC-Memorymanager.pb.cc GRPC-Memorymanager.grpc.pb.cc)
target_link_libraries(GRPC_Server gRPC::grpc++ protobuf::libprotobuf)