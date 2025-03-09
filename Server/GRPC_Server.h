#ifndef GRPC_SERVER_H
#define GRPC_SERVER_H

#include <grpcpp/grpcpp.h>
#include "GRPC-Memorymanager.grpc.pb.h"
#include <string>

class MemoryManagerService final : public Proyecto1Datos2::MemoryManager::Service {
public:
    grpc::Status Create(grpc::ServerContext* context, const Proyecto1Datos2::CreateRequest* request, Proyecto1Datos2::CreateResponse* response) override;
    grpc::Status Set(grpc::ServerContext* context, const Proyecto1Datos2::SetRequest* request, Proyecto1Datos2::SetResponse* response) override;
    grpc::Status Get(grpc::ServerContext* context, const Proyecto1Datos2::GetRequest* request, Proyecto1Datos2::GetResponse* response) override;
    grpc::Status IncreaseRefCount(grpc::ServerContext* context, const Proyecto1Datos2::RefCountRequest* request, Proyecto1Datos2::RefCountResponse* response) override;
    grpc::Status DecreaseRefCount(grpc::ServerContext* context, const Proyecto1Datos2::RefCountRequest* request, Proyecto1Datos2::RefCountResponse* response) override;
};

void RunServer(int port, int memsize, const std::string& dumpFolder);

#endif // GRPC_SERVER_H
