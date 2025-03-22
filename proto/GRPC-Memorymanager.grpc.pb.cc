// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: GRPC-MemoryManager.proto

#include "GRPC-MemoryManager.pb.h"
#include "GRPC-MemoryManager.grpc.pb.h"

#include <functional>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/impl/client_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/rpc_service_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace Proyecto1Datos2 {

static const char* MemoryManager_method_names[] = {
  "/Proyecto1Datos2.MemoryManager/Create",
  "/Proyecto1Datos2.MemoryManager/Set",
  "/Proyecto1Datos2.MemoryManager/Get",
  "/Proyecto1Datos2.MemoryManager/IncreaseRefCount",
  "/Proyecto1Datos2.MemoryManager/DecreaseRefCount",
};

std::unique_ptr< MemoryManager::Stub> MemoryManager::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< MemoryManager::Stub> stub(new MemoryManager::Stub(channel, options));
  return stub;
}

MemoryManager::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_Create_(MemoryManager_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Set_(MemoryManager_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Get_(MemoryManager_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_IncreaseRefCount_(MemoryManager_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_DecreaseRefCount_(MemoryManager_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status MemoryManager::Stub::Create(::grpc::ClientContext* context, const ::Proyecto1Datos2::CreateRequest& request, ::Proyecto1Datos2::CreateResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::Proyecto1Datos2::CreateRequest, ::Proyecto1Datos2::CreateResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Create_, context, request, response);
}

void MemoryManager::Stub::async::Create(::grpc::ClientContext* context, const ::Proyecto1Datos2::CreateRequest* request, ::Proyecto1Datos2::CreateResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::Proyecto1Datos2::CreateRequest, ::Proyecto1Datos2::CreateResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Create_, context, request, response, std::move(f));
}

void MemoryManager::Stub::async::Create(::grpc::ClientContext* context, const ::Proyecto1Datos2::CreateRequest* request, ::Proyecto1Datos2::CreateResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Create_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::Proyecto1Datos2::CreateResponse>* MemoryManager::Stub::PrepareAsyncCreateRaw(::grpc::ClientContext* context, const ::Proyecto1Datos2::CreateRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::Proyecto1Datos2::CreateResponse, ::Proyecto1Datos2::CreateRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Create_, context, request);
}

::grpc::ClientAsyncResponseReader< ::Proyecto1Datos2::CreateResponse>* MemoryManager::Stub::AsyncCreateRaw(::grpc::ClientContext* context, const ::Proyecto1Datos2::CreateRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncCreateRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MemoryManager::Stub::Set(::grpc::ClientContext* context, const ::Proyecto1Datos2::SetRequest& request, ::Proyecto1Datos2::SetResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::Proyecto1Datos2::SetRequest, ::Proyecto1Datos2::SetResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Set_, context, request, response);
}

void MemoryManager::Stub::async::Set(::grpc::ClientContext* context, const ::Proyecto1Datos2::SetRequest* request, ::Proyecto1Datos2::SetResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::Proyecto1Datos2::SetRequest, ::Proyecto1Datos2::SetResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Set_, context, request, response, std::move(f));
}

void MemoryManager::Stub::async::Set(::grpc::ClientContext* context, const ::Proyecto1Datos2::SetRequest* request, ::Proyecto1Datos2::SetResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Set_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::Proyecto1Datos2::SetResponse>* MemoryManager::Stub::PrepareAsyncSetRaw(::grpc::ClientContext* context, const ::Proyecto1Datos2::SetRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::Proyecto1Datos2::SetResponse, ::Proyecto1Datos2::SetRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Set_, context, request);
}

::grpc::ClientAsyncResponseReader< ::Proyecto1Datos2::SetResponse>* MemoryManager::Stub::AsyncSetRaw(::grpc::ClientContext* context, const ::Proyecto1Datos2::SetRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncSetRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MemoryManager::Stub::Get(::grpc::ClientContext* context, const ::Proyecto1Datos2::GetRequest& request, ::Proyecto1Datos2::GetResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::Proyecto1Datos2::GetRequest, ::Proyecto1Datos2::GetResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Get_, context, request, response);
}

void MemoryManager::Stub::async::Get(::grpc::ClientContext* context, const ::Proyecto1Datos2::GetRequest* request, ::Proyecto1Datos2::GetResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::Proyecto1Datos2::GetRequest, ::Proyecto1Datos2::GetResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Get_, context, request, response, std::move(f));
}

void MemoryManager::Stub::async::Get(::grpc::ClientContext* context, const ::Proyecto1Datos2::GetRequest* request, ::Proyecto1Datos2::GetResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Get_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::Proyecto1Datos2::GetResponse>* MemoryManager::Stub::PrepareAsyncGetRaw(::grpc::ClientContext* context, const ::Proyecto1Datos2::GetRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::Proyecto1Datos2::GetResponse, ::Proyecto1Datos2::GetRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Get_, context, request);
}

::grpc::ClientAsyncResponseReader< ::Proyecto1Datos2::GetResponse>* MemoryManager::Stub::AsyncGetRaw(::grpc::ClientContext* context, const ::Proyecto1Datos2::GetRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MemoryManager::Stub::IncreaseRefCount(::grpc::ClientContext* context, const ::Proyecto1Datos2::RefCountRequest& request, ::Proyecto1Datos2::RefCountResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::Proyecto1Datos2::RefCountRequest, ::Proyecto1Datos2::RefCountResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_IncreaseRefCount_, context, request, response);
}

void MemoryManager::Stub::async::IncreaseRefCount(::grpc::ClientContext* context, const ::Proyecto1Datos2::RefCountRequest* request, ::Proyecto1Datos2::RefCountResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::Proyecto1Datos2::RefCountRequest, ::Proyecto1Datos2::RefCountResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_IncreaseRefCount_, context, request, response, std::move(f));
}

void MemoryManager::Stub::async::IncreaseRefCount(::grpc::ClientContext* context, const ::Proyecto1Datos2::RefCountRequest* request, ::Proyecto1Datos2::RefCountResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_IncreaseRefCount_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::Proyecto1Datos2::RefCountResponse>* MemoryManager::Stub::PrepareAsyncIncreaseRefCountRaw(::grpc::ClientContext* context, const ::Proyecto1Datos2::RefCountRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::Proyecto1Datos2::RefCountResponse, ::Proyecto1Datos2::RefCountRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_IncreaseRefCount_, context, request);
}

::grpc::ClientAsyncResponseReader< ::Proyecto1Datos2::RefCountResponse>* MemoryManager::Stub::AsyncIncreaseRefCountRaw(::grpc::ClientContext* context, const ::Proyecto1Datos2::RefCountRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncIncreaseRefCountRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MemoryManager::Stub::DecreaseRefCount(::grpc::ClientContext* context, const ::Proyecto1Datos2::RefCountRequest& request, ::Proyecto1Datos2::RefCountResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::Proyecto1Datos2::RefCountRequest, ::Proyecto1Datos2::RefCountResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_DecreaseRefCount_, context, request, response);
}

void MemoryManager::Stub::async::DecreaseRefCount(::grpc::ClientContext* context, const ::Proyecto1Datos2::RefCountRequest* request, ::Proyecto1Datos2::RefCountResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::Proyecto1Datos2::RefCountRequest, ::Proyecto1Datos2::RefCountResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DecreaseRefCount_, context, request, response, std::move(f));
}

void MemoryManager::Stub::async::DecreaseRefCount(::grpc::ClientContext* context, const ::Proyecto1Datos2::RefCountRequest* request, ::Proyecto1Datos2::RefCountResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DecreaseRefCount_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::Proyecto1Datos2::RefCountResponse>* MemoryManager::Stub::PrepareAsyncDecreaseRefCountRaw(::grpc::ClientContext* context, const ::Proyecto1Datos2::RefCountRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::Proyecto1Datos2::RefCountResponse, ::Proyecto1Datos2::RefCountRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_DecreaseRefCount_, context, request);
}

::grpc::ClientAsyncResponseReader< ::Proyecto1Datos2::RefCountResponse>* MemoryManager::Stub::AsyncDecreaseRefCountRaw(::grpc::ClientContext* context, const ::Proyecto1Datos2::RefCountRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncDecreaseRefCountRaw(context, request, cq);
  result->StartCall();
  return result;
}

MemoryManager::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MemoryManager_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MemoryManager::Service, ::Proyecto1Datos2::CreateRequest, ::Proyecto1Datos2::CreateResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MemoryManager::Service* service,
             ::grpc::ServerContext* ctx,
             const ::Proyecto1Datos2::CreateRequest* req,
             ::Proyecto1Datos2::CreateResponse* resp) {
               return service->Create(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MemoryManager_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MemoryManager::Service, ::Proyecto1Datos2::SetRequest, ::Proyecto1Datos2::SetResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MemoryManager::Service* service,
             ::grpc::ServerContext* ctx,
             const ::Proyecto1Datos2::SetRequest* req,
             ::Proyecto1Datos2::SetResponse* resp) {
               return service->Set(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MemoryManager_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MemoryManager::Service, ::Proyecto1Datos2::GetRequest, ::Proyecto1Datos2::GetResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MemoryManager::Service* service,
             ::grpc::ServerContext* ctx,
             const ::Proyecto1Datos2::GetRequest* req,
             ::Proyecto1Datos2::GetResponse* resp) {
               return service->Get(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MemoryManager_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MemoryManager::Service, ::Proyecto1Datos2::RefCountRequest, ::Proyecto1Datos2::RefCountResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MemoryManager::Service* service,
             ::grpc::ServerContext* ctx,
             const ::Proyecto1Datos2::RefCountRequest* req,
             ::Proyecto1Datos2::RefCountResponse* resp) {
               return service->IncreaseRefCount(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MemoryManager_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MemoryManager::Service, ::Proyecto1Datos2::RefCountRequest, ::Proyecto1Datos2::RefCountResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MemoryManager::Service* service,
             ::grpc::ServerContext* ctx,
             const ::Proyecto1Datos2::RefCountRequest* req,
             ::Proyecto1Datos2::RefCountResponse* resp) {
               return service->DecreaseRefCount(ctx, req, resp);
             }, this)));
}

MemoryManager::Service::~Service() {
}

::grpc::Status MemoryManager::Service::Create(::grpc::ServerContext* context, const ::Proyecto1Datos2::CreateRequest* request, ::Proyecto1Datos2::CreateResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MemoryManager::Service::Set(::grpc::ServerContext* context, const ::Proyecto1Datos2::SetRequest* request, ::Proyecto1Datos2::SetResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MemoryManager::Service::Get(::grpc::ServerContext* context, const ::Proyecto1Datos2::GetRequest* request, ::Proyecto1Datos2::GetResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MemoryManager::Service::IncreaseRefCount(::grpc::ServerContext* context, const ::Proyecto1Datos2::RefCountRequest* request, ::Proyecto1Datos2::RefCountResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MemoryManager::Service::DecreaseRefCount(::grpc::ServerContext* context, const ::Proyecto1Datos2::RefCountRequest* request, ::Proyecto1Datos2::RefCountResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace Proyecto1Datos2

