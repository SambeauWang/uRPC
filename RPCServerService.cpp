#include "RPCServerService.h"
#include <iostream>

RpcServerService::RpcServerService(TcpConnection *conn): _conn(conn)
{
	conn->addServcice(this);
}

RpcServerService::~RpcServerService()
{
	
}

EchoImplService::EchoImplService(TcpConnection* con): RpcServerService(con)
{
	
}

void EchoImplService::Echo(RpcController* controller,
		const echo::RpcRequest* request,
		echo::RpcResponse* response,
		Closure* done)
{
	std::cout<<"echo impl: "<<request->message()<<std::endl;
}

EchoBackImplService::EchoBackImplService(TcpConnection* conn): RpcServerService(conn)
{
	stubService = new echo::EchoService::Stub(conn);
}


void EchoBackImplService::Echo(RpcController* controller, const echo::RpcRequest* request, echo::RpcResponse* reponse, Closure* done)
{
	std::cout<<"stub call echo: "<<request->message()<<std::endl;
	echo::RpcRequest requestBack;
	requestBack.set_message(request->message());
	stubService->Echo(NULL, &requestBack, NULL, NULL);
}