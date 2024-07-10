#include <iostream>
#include <winsock2.h>
#include <cstring>

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
	SOCKET ClientSocket;
	struct sockaddr_in serverAddr;
	TargetData data;
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
	serverAddr.sin_addr.s_addr = inet_addr("192.168.1.1"); // �޸�Ϊ��������IP��ַ
	serverAddr.sin_port = htons(12345); // �޸�Ϊ������ʹ�õĶ˿�

	// ���ӷ�����
	if (connect(ClientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cerr << "Connect failed" << std::endl;
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "Connected to server" << std::endl;

	while (true) {
		// ģ���������
		data.state = 1;
		data.x = 10.0f;
		data.y = 20.0f;
		data.z = 30.0f;
		data.rotation = 45.0f;

		std::cout << "Sending TargetData:" << std::endl;
		printTargetData(data);

		// ��������
		bytesSent = send(ClientSocket, (char*)&data, sizeof(data), 0);
		if (bytesSent == SOCKET_ERROR) {
			std::cerr << "Send failed" << std::endl;
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

		// ��������
		bytesReceived = recv(ClientSocket, (char*)&data, sizeof(data), 0);
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

	// �رտͻ����׽���
	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}
