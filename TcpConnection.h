#pragma once
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <vector>
#include <google/protobuf/service.h>

using namespace boost::asio;
typedef boost::asio::ip::tcp::socket* sock_pt;

class TcpConnection: public google::protobuf::RpcChannel
{
public:
	TcpConnection(boost::asio::io_service &io);
	~TcpConnection();
	
	void sendMessage(std::string str);
	
	void write_handler(const boost::system::error_code &);
	void read_handler(const boost::system::error_code& ec,boost::shared_ptr<std::vector<char> > str);
	
	void addServcice(google::protobuf::Service* serv);
	sock_pt getSocket();
	
	void CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller,
                          const google::protobuf::Message* request,
                          google::protobuf::Message* response,
                          google::protobuf::Closure* done);
	
private:
	sock_pt _sock;
	std::vector<google::protobuf::Service*> rpcServices;
	
	void deal_rpc_data(boost::shared_ptr<std::vector<char> > str);
};

class TcpServer
{
public:
	TcpServer(boost::asio::io_service& io);
	void echo(std::string str);

private:
	boost::asio::ip::tcp::acceptor acceptor;
	TcpConnection *m_waitCon;
	std::vector<TcpConnection*> m_cons;
	boost::asio::io_service *m_io;
	
	void _start();
	void accept_hander(const boost::system::error_code& ec);
};

class TcpClient
{
public:
	TcpClient(boost::asio::io_service& io);
	TcpConnection* getConnection();

private:
	TcpConnection* m_conn;
	ip::tcp::endpoint ep;

	void connect_handler(const boost::system::error_code &ec, TcpConnection* conn);
};