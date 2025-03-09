//
// Created by Rachel on 08/03/2025.
//

#include "GRPC_Client.h"
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "GRPC-Memorymanager.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using Proyecto__1_Datos_2::Greeter;
using Proyecto__1_Datos_2::HelloRequest;
using Proyecto__1_Datos_2::HelloReply;

class GreeterClient {
public:
    GreeterClient(std::shared_ptr<Channel> channel) : stub_(Greeter::NewStub(channel)) {}

    std::string SayHello(const std::string& user) {
        HelloRequest request;
        request.set_name(user);

        HelloReply reply;
        ClientContext context;

        Status status = stub_->SayHello(&context, request, &reply);

        if (status.ok()) {
            return reply.message();
        } else {
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<Greeter::Stub> stub_;
};

int main() {
    GreeterClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    std::string response = client.SayHello("World");
    std::cout << "Greeter received: " << response << std::endl;
    return 0;

}