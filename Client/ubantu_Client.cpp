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
	int clientSocket;
	struct sockaddr_in serverAddr;
	TargetData data;
	int bytesSent, bytesReceived;

	// 创建套接字
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1) {
		std::cerr << "Socket creation failed" << std::endl;
		return 1;
	}

	// 设置服务器地址和端口
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("192.168.1.1"); // 修改为服务器的IP地址
	serverAddr.sin_port = htons(12345); // 修改为服务器使用的端口

	// 连接服务器
	if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
		std::cerr << "Connect failed" << std::endl;
		close(clientSocket);
		return 1;
	}

	std::cout << "Connected to server" << std::endl;

	while (true) {
		// 模拟填充数据
		data.state = 1;
		data.x = 10.0f;
		data.y = 20.0f;
		data.z = 30.0f;
		data.rotation = 45.0f;

		std::cout << "Sending TargetData:" << std::endl;
		printTargetData(data);

		// 发送数据
		bytesSent = send(clientSocket, (char*)&data, sizeof(data), 0);
		if (bytesSent == -1) {
			std::cerr << "Send failed" << std::endl;
			close(clientSocket);
			return 1;
		}

		// 接收数据
		bytesReceived = recv(clientSocket, (char*)&data, sizeof(data), 0);
		if (bytesReceived > 0) {
			std::cout << "Received from server:" << std::endl;
			printTargetData(data);
		} else if (bytesReceived == 0) {
			std::cout << "Server disconnected" << std::endl;
			break;
		} else {
			std::cerr << "Receive failed" << std::endl;
			break;
		}
	}

	// 关闭客户端套接字
	close(clientSocket);

	return 0;
}
