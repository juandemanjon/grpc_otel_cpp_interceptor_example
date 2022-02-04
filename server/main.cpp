#include <iostream>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "demo.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

// Logic and data behind the server's behavior.
class DemoServiceImpl final : public hipstershop::CartService::Service {
  public:
    virtual ::grpc::Status AddItem(::grpc::ServerContext* context, const ::hipstershop::AddItemRequest* request, ::hipstershop::Empty* response)
    {
      return ::grpc::Status(::grpc::StatusCode::OK, "");
    }
    virtual ::grpc::Status GetCart(::grpc::ServerContext* context, const ::hipstershop::GetCartRequest* request, ::hipstershop::Cart* response)
    {
      response->set_user_id("foo-bar");
      return ::grpc::Status(::grpc::StatusCode::OK, "");
    }
    virtual ::grpc::Status EmptyCart(::grpc::ServerContext* context, const ::hipstershop::EmptyCartRequest* request, ::hipstershop::Empty* response)
    {
      return ::grpc::Status(::grpc::StatusCode::OK, "");
    }
};

class SpanInterceptor : public grpc::experimental::Interceptor {

public:
  SpanInterceptor(grpc::experimental::ServerRpcInfo *info) : info_(info) { ; }

  void Intercept(grpc::experimental::InterceptorBatchMethods *methods) override
  {
    methods->Proceed();
  }

private:
    grpc::experimental::ServerRpcInfo* info_;
};

class SpanInterceptorFactory : public grpc::experimental::ServerInterceptorFactoryInterface
{
  public:
	SpanInterceptorFactory() {}
	grpc::experimental::Interceptor *CreateServerInterceptor(
		grpc::experimental::ServerRpcInfo *info) override
    {
      return new SpanInterceptor(info);
    }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  
  DemoServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);

  std::vector<std::unique_ptr<grpc::experimental::ServerInterceptorFactoryInterface>> creators;
  creators.push_back(std::unique_ptr<grpc::experimental::ServerInterceptorFactoryInterface>(
                new SpanInterceptorFactory()));

  builder.experimental().SetInterceptorCreators(std::move(creators));

  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int, char**) {
    RunServer();
}
