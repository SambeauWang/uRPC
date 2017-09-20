#include "TcpConnection.h"
#include "RPCServerService.h"
#include <boost/bind.hpp>
#include <iostream>


TcpConnection::TcpConnection(boost::asio::io_service&io): _sock(new boost::asio::ip::tcp::socket(io))
{
}

TcpConnection::~TcpConnection()
{
	
}

void TcpConnection::sendMessage(std::string str)
{
	std::cout<<"send data:"<<str<<std::endl;
	_sock->async_write_some(boost::asio::buffer(str.c_str(), strlen(str.c_str())), boost::bind(&TcpConnection::write_handler, this, boost::asio::placeholders::error));
}

void TcpConnection::write_handler(const boost::system::error_code &error)
{
	std::cout<<"send data success:"<<std::endl;
}

void TcpConnection::read_handler(const boost::system::error_code &ec, boost::shared_ptr<std::vector<char> > str)
{
	if(ec)
	{
		std::cout<<"recv data err:"<<str<<std::endl;
	}
	
	std::cout<<"recv data:"<<&(*str)[0]<<std::endl;
	deal_rpc_data(str);
	
	boost::shared_ptr<std::vector<char> > str2(new std::vector<char>(100, 0));
	_sock->async_read_some(boost::asio::buffer(*str2), boost::bind(&TcpConnection::read_handler, this, boost::asio::placeholders::error, str2));
}

void TcpConnection::deal_rpc_data(boost::shared_ptr<std::vector<char> > str)
{
	echo::RpcRequest request;
	int fid = (*str)[0] - '0';
	
	request.ParseFromString(&(*str)[1]);
	rpcServices[0]->CallMethod(rpcServices[0]->GetDescriptor()->method(fid), NULL, &request, NULL, NULL);
}

void TcpConnection::addServcice(google::protobuf::Service* serv)
{
	rpcServices.push_back(serv);
}

sock_pt TcpConnection::getSocket()
{
	return _sock;
}

void TcpConnection::CallMethod(const google::protobuf::MethodDescriptor* method, google::protobuf::RpcController* controller, const google::protobuf::Message* request, google::protobuf::Message* response, google::protobuf::Closure* done){
	char c[2];
	c[0]= '0'+method->index();//这里func id有一定的数量限制。
	c[1] = 0;
	std::string cstr = std::string(c);
	cstr+=request->SerializeAsString();
	sendMessage(cstr);
}

// TcpServer
TcpServer::TcpServer(boost::asio::io_service& io): m_io(&io), acceptor(io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 18089))
{
	m_waitCon = NULL;
	_start();
}

void TcpServer::_start()
{
	m_waitCon = new TcpConnection(*m_io);
	// TODO: add servcice;
	m_waitCon->addServcice(new EchoBackImplService(m_waitCon));
	
	acceptor.async_accept(*m_waitCon->getSocket(), boost::bind(&TcpServer::accept_hander, this, boost::asio::placeholders::error));
}

void TcpServer::accept_hander(const boost::system::error_code& ec)
{
	std::cout<<"client is connected."<<std::endl;
	boost::shared_ptr<std::vector<char> > str(new std::vector<char>(100, 0));
	
	m_waitCon->getSocket()->async_read_some(boost::asio::buffer(*str), boost::bind(&TcpConnection::read_handler, m_waitCon, boost::asio::placeholders::error, str));
	m_cons.push_back(m_waitCon);
	
	_start();
}

void TcpServer::echo(std::string str)
{
	std::cout<<"echo data: "<<str<<std::endl;
}


TcpClient::TcpClient(boost::asio::io_service& m_io): m_conn(new TcpConnection(m_io)),ep(ip::address::from_string("127.0.0.1"), 18089)
{
	m_conn->addServcice(new EchoImplService(m_conn));
	m_conn->getSocket()->async_connect(ep, boost::bind(&TcpClient::connect_handler, this, boost::asio::placeholders::error, m_conn));
}

TcpConnection* TcpClient::getConnection()
{
	return m_conn;
}

void TcpClient::connect_handler(const boost::system::error_code &ec, TcpConnection* conn)
{
	if(ec)
	{
		std::cout<<"connect failed"<<std::endl;
		return;
	}
	
	boost::shared_ptr<vector<char> > str(new std::vector<char>(100, 0));
	m_conn->getSocket()->async_read_some(boost::asio::buffer(*str), boost::bind(&TcpConnection::read_handler, conn, boost::asio::placeholders::error, str));
}


