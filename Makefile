CC=g++
LD=g++

CFLAGS= -Wall -w -c -g -I./ -lboost_system -lpthread -lboost_thread
LDFLAGS= -lboost_system -lpthread -lboost_thread


test: RpcMain.o RPCServerService.o TcpConnection.o echo.pb.o TcpConnection.h echo.pb.h RPCServerService.h
	$(LD) RpcMain.o TcpConnection.o echo.pb.o RPCServerService.o -o test `pkg-config --cflags --libs protobuf`  $(LDFLAGS)

RpcMain.o: RpcMain.cpp TcpConnection.h protoMid echo.pb.h 
	$(CC) RpcMain.cpp -o RpcMain.o `pkg-config --cflags --libs protobuf` $(CFLAGS)
	
TcpConnection.o: TcpConnection.h TcpConnection.cpp
	$(CC) TcpConnection.cpp -o TcpConnection.o $(CFLAGS)

echo.pb.o: protoMid echo.pb.cc echo.pb.h
	$(CC) echo.pb.cc -o echo.pb.o $(CFLAGS)

protoMid: echo.proto
	protoc -I=. --cpp_out=. echo.proto
	
RPCServerService.o: RPCServerService.h RPCServerService.cpp
	$(CC) RPCServerService.cpp -o RPCServerService.o $(CFLAGS)

clean:
	rm -f test protoMid TcpConnection.o echo.pb.o RpcMain.o RPCServerService.o
	rm -f echo.pb.cc echo.pb.h