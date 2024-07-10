#include <iostream>
#include "../TCP.hpp"
#pragma comment(lib, "ws2_32.lib")
TargetData data;
TCP::Client CLIENT("192.168.96.133",12345);


int main() {
	if (!CLIENT.Connect()) {
		std::cerr << "Connect failed" << std::endl;
		return 1;
	}
	while (true) {
		detect_and_location_task(data);
		std::cout << "Sending TargetData:" << std::endl;
		printTargetData(data);
		// 发送数据
		if(!CLIENT.sendData(data)){
			std::cerr << "Send failed" << std::endl;
			CLIENT.close();
			return 1;
		}
		// 接收数据
		CLIENT.receiveData(data);
		}
	CLIENT.close();
	return 0;
	}

