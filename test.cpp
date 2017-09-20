#include "echo.pb.h"
#include <iostream>
#include <string>

using namespace std;
using namespace echo;

int main(int argc, char* argv[])
{
	RpcRequest req;
	req.set_serviceid("test01");
	req.set_functionid("test02");
	req.set_message("test03");
	
	string res;
	req.SerializeToString(&res);
	
	cout<<res<<endl;
	
	return 0;
}