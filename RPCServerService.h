#pragma once
#include "echo.pb.h"
#include "TcpConnection.h"

using namespace google::protobuf;

class RpcServerService: public echo::EchoService
{
public:
	RpcServerService(TcpConnection *conn);
	~RpcServerService();

private:
	TcpConnection *_conn;
};

class EchoImplService: public RpcServerService
{
public:
	EchoImplService(TcpConnection* con);
	void Echo(RpcController* controller,
		const echo::RpcRequest* request,
		echo::RpcResponse* response,
		Closure* done);
};


class EchoBackImplService: public RpcServerService
{
public:
	EchoBackImplService(TcpConnection* conn);
	void Echo(RpcController* controller, const echo::RpcRequest* request, echo::RpcResponse* reponse, Closure* done);
	
private:
	echo::EchoService::Stub *stubService;
};