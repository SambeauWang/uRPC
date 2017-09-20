# uRPC
uRPC 是使用boost::asio和protobuf 编写的一个RPC程序。

## 依赖
+ boost::asio 主要用于网络通信
+ protobuf 用于数据序列化

## 安装依赖库--linux
(1) 安装protobuf   
```
# 下载protobuf源代码
wget https://protobuf.googlecode.com/files/protobuf-2.5.0.tar.gz

# 解压，编译，安装
$ tar zxvf protobuf-2.5.0.tar.gz 
$ cd protobuf-2.5.0 
$ ./configure 
$ make 
$ make check 
$ make install
```

(2) 安装boost
```
apt-get install libboost-dev
```

## 编译
```
make
```
