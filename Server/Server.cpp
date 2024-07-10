#include <iostream>
#include "../TCP.hpp"
#pragma comment(lib, "ws2_32.lib")
TargetData data;
TCP::Server SERVER("192.168.0.100",12345);
int main() {
	while (true) {
		if (!SERVER.Accecp()) {
			std::cerr << "Accept failed" << std::endl;
			SERVER.close();
			return 1;
		}
		while (true) {
			SERVER.receiveData(data);
		}
	}
	SERVER.close();
	return 0;
}

