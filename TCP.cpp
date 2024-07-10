//
// Created by 29451 on 2024/7/10.
//

#include "TCP.hpp"

namespace TCP {

	// Client class implementation

	Client::Client() {
	}

	Client::Client(char *ip, int port) {
		this->ip = ip;
		this->port = port;
	}

	Client::~Client() {
		closesocket(this->ClientSocket);
		WSACleanup();
	}

	bool Client::Connect() {
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			std::cerr << "WSAStartup failed" << std::endl;
		}
		this->ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (this->ClientSocket == INVALID_SOCKET) {
			std::cerr << "Socket creation failed" << std::endl;
			WSACleanup();
		}
		sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = inet_addr(this->ip);
		serverAddr.sin_port = htons(this->port);

		if (connect(this->ClientSocket, (sockaddr *) &serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
			std::cerr << "Connect failed" << std::endl;
			closesocket(ClientSocket);
			WSACleanup();
			return false;
		}
		std::cout << "Connected to server" << std::endl;
		return true;
	}

	void Client::close() {
		closesocket(ClientSocket);
		WSACleanup();
	}

	bool Client::sendData(const TargetData &data) {
		int bytesSent = send(this->ClientSocket, (char *) &data, sizeof(data), 0);
		if (bytesSent == SOCKET_ERROR) {
			std::cerr << "Send failed" << std::endl;
			return false;
		}
		return true;
	}

	void Client::receiveData(TargetData &data) {
		int bytesReceived = recv(this->ClientSocket, (char *) &data, sizeof(data), 0);
		if (bytesReceived > 0) {
			std::cout << "Received TargetData:" << std::endl;
			std::cout << "State: " << (int) data.state << std::endl;
			std::cout << "X: " << data.x << std::endl;
			std::cout << "Y: " << data.y << std::endl;
			std::cout << "Z: " << data.z << std::endl;
			std::cout << "Rotation: " << data.rotation << std::endl;
		} else if (bytesReceived == 0) {
			std::cout << "Server disconnected" << std::endl;
		} else {
			std::cerr << "Receive failed" << std::endl;
		}
	}

	Server::Server() {}

	Server::Server(char *ip, int port) {
		this->ip = ip;
		this->port = port;
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			std::cerr << "WSAStartup failed" << std::endl;
		}
		this->ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (this->ClientSocket == INVALID_SOCKET) {
			std::cerr << "Socket creation failed" << std::endl;
			WSACleanup();
		}

		sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = inet_addr(ip);
		serverAddr.sin_port = htons(port);

		if (bind(this->ClientSocket, (sockaddr *) &serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
			std::cerr << "Bind failed" << std::endl;
			closesocket(this->ClientSocket);
			WSACleanup();
		}
		if (listen(this->ClientSocket, SOMAXCONN) == SOCKET_ERROR) {
			std::cerr << "Listen failed" << std::endl;
			closesocket(this->ClientSocket);
			WSACleanup();
		}
		std::cout << "Waiting for connections..." << std::endl;
	}


	bool Server::Accecp() {
		sockaddr_in clientAddr;
		int clientAddrSize = sizeof(clientAddr);
		this->ClientSocket = accept(this->ClientSocket, (sockaddr *) &clientAddr, &clientAddrSize);
		if (ClientSocket == INVALID_SOCKET) {
			std::cerr << "Accept failed" << std::endl;
			closesocket(this->ClientSocket);
			WSACleanup();
			return false;
		}

		std::cout << "Client connected" << std::endl;
		return true;
	}

	void Server::close() {
		closesocket(this->ClientSocket);
		WSACleanup();
	}

	bool Server::sendData(const TargetData &data) {
		int bytesSent = send(this->ClientSocket, (char *) &data, sizeof(data), 0);
		if (bytesSent == SOCKET_ERROR) {
			std::cerr << "Send failed" << std::endl;
			return false;
		}
		return true;
	}

	void Server::receiveData(TargetData &data) {
		int bytesReceived = recv(this->ClientSocket, (char *) &data, sizeof(data), 0);
		if (bytesReceived > 0) {
			std::cout << "Received TargetData:" << std::endl;
			std::cout << "State: " << (int) data.state << std::endl;
			std::cout << "X: " << data.x << std::endl;
			std::cout << "Y: " << data.y << std::endl;
			std::cout << "Z: " << data.z << std::endl;
			std::cout << "Rotation: " << data.rotation << std::endl;
		} else {
			std::cerr << "Receive failed" << std::endl;
		}
	}

	Server::~Server() {
		closesocket(ClientSocket);
		WSACleanup();
	}

}

float generateRandomFloat(float min, float max) {
	return min + static_cast<float>(rand())
	             / (static_cast<float>(RAND_MAX / (max - min))
	             );
}

void detect_and_location_task(TargetData &data) {
	data.state = (uint8_t) generateRandomFloat(0.0f, 10.0f);// 随机生成 x
	data.x = generateRandomFloat(0.0f, 10.0f);// 随机生成 x
	data.y = generateRandomFloat(0.0f, 10.0f);// 随机生成 x
	data.z = generateRandomFloat(0.0f, 10.0f); // 随机生成 x
	data.rotation = generateRandomFloat(0.0f, 360.0f);// 随机生成 rotation
	Sleep(100);
}

void printTargetData(const TargetData &data) {
	std::cout << "State: " << (int) data.state << std::endl;
	std::cout << "X: " << data.x << std::endl;
	std::cout << "Y: " << data.y << std::endl;
	std::cout << "Z: " << data.z << std::endl;
	std::cout << "Rotation: " << data.rotation << std::endl;
}
