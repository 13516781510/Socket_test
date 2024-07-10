#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

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
	WSADATA wsaData;
	SOCKET ServerSocket, ClientSocket;
	struct sockaddr_in serverAddr, clientAddr;
	int clientAddrSize = sizeof(clientAddr);
	TargetData data;
	int bytesReceived;

	// ��ʼ��Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "WSAStartup failed" << std::endl;
		return 1;
	}

	// �����׽���
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET) {
		std::cerr << "Socket creation failed" << std::endl;
		WSACleanup();
		return 1;
	}

	// ���÷�������ַ�Ͷ˿�
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("192.168.1.1"); // �޸�Ϊ���IP��ַ
	serverAddr.sin_port = htons(12345); // �޸�Ϊ����ʹ�õĶ˿�

	// ���׽���
	if (bind(ServerSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cerr << "Bind failed" << std::endl;
		closesocket(ServerSocket);
		WSACleanup();
		return 1;
	}

	// ��������
	if (listen(ServerSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << "Listen failed" << std::endl;
		closesocket(ServerSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "Waiting for connections..." << std::endl;

	while (true) {
		// ���ܿͻ�������
		ClientSocket = accept(ServerSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
		if (ClientSocket == INVALID_SOCKET) {
			std::cerr << "Accept failed" << std::endl;
			closesocket(ServerSocket);
			WSACleanup();
			return 1;
		}

		std::cout << "Client connected" << std::endl;

		while (true) {
			// ��������
			bytesReceived = recv(ClientSocket, (char*)&data, sizeof(data), 0);
			if (bytesReceived > 0) {
				std::cout << "Received TargetData:" << std::endl;
				printTargetData(data);

				// �ش�����
				send(ClientSocket, (char*)&data, sizeof(data), 0);
			} else if (bytesReceived == 0) {
				std::cout << "Client disconnected" << std::endl;
				break;
			} else {
				std::cerr << "Receive failed" << std::endl;
				break;
			}
		}

		closesocket(ClientSocket);
	}

	// �رշ������׽���
	closesocket(ServerSocket);
	WSACleanup();

	return 0;
}
