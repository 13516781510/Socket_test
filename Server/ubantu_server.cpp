#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

struct TargetData {
	uint8_t state;
	float x;
	float y;
	float z;
	float rotation;
};

void printTargetData(const TargetData& data) {
	std::cout << "State: " << (int)data.state << std::endl;
	std::cout << "X: " << data.x << std::endl;
	std::cout << "Y: " << data.y << std::endl;
	std::cout << "Z: " << data.z << std::endl;
	std::cout << "Rotation: " << data.rotation << std::endl;
}

int main() {
	int serverSocket, clientSocket;
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t clientAddrSize = sizeof(clientAddr);
	TargetData data;
	int bytesReceived;

	// 创建套接字
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		std::cerr << "Socket creation failed" << std::endl;
		return 1;
	}

	// 设置服务器地址和端口
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("192.168.1.1"); // 修改为你的IP地址
	serverAddr.sin_port = htons(12345); // 修改为你想使用的端口

	// 绑定套接字
	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
		std::cerr << "Bind failed" << std::endl;
		close(serverSocket);
		return 1;
	}

	// 监听连接
	if (listen(serverSocket, 5) == -1) {
		std::cerr << "Listen failed" << std::endl;
		close(serverSocket);
		return 1;
	}

	std::cout << "Waiting for connections..." << std::endl;

	while (true) {
		// 接受客户端连接
		clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
		if (clientSocket == -1) {
			std::cerr << "Accept failed" << std::endl;
			close(serverSocket);
			return 1;
		}

		std::cout << "Client connected" << std::endl;

		while (true) {
			// 接收数据
			bytesReceived = recv(clientSocket, (char*)&data, sizeof(data), 0);
			if (bytesReceived > 0) {
				std::cout << "Received TargetData:" << std::endl;
				printTargetData(data);

				// 回传数据
				send(clientSocket, (char*)&data, sizeof(data), 0);
			} else if (bytesReceived == 0) {
				std::cout << "Client disconnected" << std::endl;
				break;
			} else {
				std::cerr << "Receive failed" << std::endl;
				break;
			}
		}

		close(clientSocket);
	}

	// 关闭服务器套接字
	close(serverSocket);

	return 0;
}
