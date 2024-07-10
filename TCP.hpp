//
// Created by 29451 on 2024/7/10.
//
#pragma once
#ifndef TCPTEST_TCP_HPP
#define TCPTEST_TCP_HPP


#include <winsock2.h>
#include <iostream>
#include <Windows.h>
struct TargetData {
	uint8_t state;
	float x;
	float y;
	float z;
	float rotation;
};
float generateRandomFloat(float min, float max);

void detect_and_location_task(TargetData &data);

void printTargetData(const TargetData &data);
namespace TCP {


	class Client {
	public:
		SOCKET ClientSocket;
		char *ip = "192.168.96.133";
		int port = 12345;

		Client();

		Client(char *ip, int port);

		~Client();

		bool Connect();

		void close();

		bool sendData(const TargetData &data);

		void receiveData(TargetData &data);
	};

	class Server {
	public:
		SOCKET ClientSocket;
		char *ip = "192.168.96.133";
		int port = 12345;

		Server();

		Server::Server(char *ip, int port);

		~Server();

		bool Accecp();

		void close();

		bool sendData(const TargetData &data);

		void receiveData(TargetData &data);
	};
}



#endif //TCPTEST_TCP_HPP
