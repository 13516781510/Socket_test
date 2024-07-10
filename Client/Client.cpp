//
// Created by 29451 on 2024/7/10.
//
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
	WSADATA wsaData;
	SOCKET ClientSocket;
	struct sockaddr_in serverAddr;
	char buffer[1024] = "Hello, Server!";
	int bytesSent, bytesReceived;

	// ��ʼ��Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "WSAStartup failed" << std::endl;
		return 1;
	}

	// �����׽���
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET) {
		std::cerr << "Socket creation failed" << std::endl;
		WSACleanup();
		return 1;
	}

	// ���÷�������ַ�Ͷ˿�
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(8080);

	// ���ӷ�����
	if (connect(ClientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cerr << "Connect failed" << std::endl;
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "Connected to server" << std::endl;

	// ��������
	bytesSent = send(ClientSocket, buffer, strlen(buffer), 0);
	if (bytesSent == SOCKET_ERROR) {
		std::cerr << "Send failed" << std::endl;
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// ��������
	bytesReceived = recv(ClientSocket, buffer, sizeof(buffer), 0);
	if (bytesReceived > 0) {
		std::cout << "Received from server: " << std::string(buffer, 0, bytesReceived) << std::endl;
	}

	// �ر��׽���
	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}
