#include <boost/thread/thread.hpp>
#include <iostream>

#include "RPCServerService.h"
#include "TcpConnection.h"
#include "echo.pb.h"

using namespace boost::asio;

int main()
{
	int app_type;
	io_service io;
	
	std::cout<<"0:sever, other:client"<<std::endl;
	std::cin>>app_type;
	
	if(0==app_type)
	{
		TcpServer *server = new TcpServer(io);
		io.run();
	}
	else
	{
		TcpClient* client = new TcpClient(io);
		boost::thread t(boost::bind(&boost::asio::io_service::run, &io));
		while(true)
		{
			string in;
			std::cin>>in;
			echo::RpcRequest request;
			request.set_message(in);
			
			echo::EchoService* service = new echo::EchoService::Stub(client->getConnection());

			service->Echo(NULL, &request, NULL, NULL);
		}
	}
	
	return 0;
}