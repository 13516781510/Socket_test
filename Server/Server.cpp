//
// Created by 29451 on 2024/7/10.
//
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
int main() {
	WSADATA wsaData;
	SOCKET ServerSocket, ClientSocket;
	struct sockaddr_in serverAddr, clientAddr;
	int clientAddrSize = sizeof(clientAddr);
	char buffer[1024];
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
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(8080);

	// ���׽���
	if (bind(ServerSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
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

	// ���ܿͻ�������
	ClientSocket = accept(ServerSocket, (struct sockaddr *) &clientAddr, &clientAddrSize);
	if (ClientSocket == INVALID_SOCKET) {
		std::cerr << "Accept failed" << std::endl;
		closesocket(ServerSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "Client connected" << std::endl;

	// ��������
	bytesReceived = recv(ClientSocket, buffer, sizeof(buffer), 0);
	if (bytesReceived > 0) {
		std::cout << "Received: " << std::string(buffer, 0, bytesReceived) << std::endl;

		// �ش�����
		send(ClientSocket, buffer, bytesReceived, 0);
	}

	// �ر��׽���
	closesocket(ClientSocket);
	closesocket(ServerSocket);
	WSACleanup();

	return 0;
}
