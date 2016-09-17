#include "inquisition_network.hpp"
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>

int port = 65533;

void serverthread(){
#ifdef _INQUISITION_NETWORK_H_WIN
	std::cout << "[S] Initializing Winsock2..." << std::endl;
	WSADATA wsadata;
	int error = WSAStartup(0x0202, &wsadata);
	if(error){
		std::cout << "[S] Error when initializing Winsock2. Will now halt.\n" << std::endl;
		exit(1);
	}

	if(wsadata.wVersion != 0x0202){
		std::cout << "[S] Error. Cannot get 0x0202 version of Winsock. Will now halt.\n" << std::endl;
		WSACleanup();
		exit(1);
	}
#endif
	
	std::cout << "[S] Creating socket for server" << std::endl;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("[S] Error when creating socket");
		exit(1);
	}else std::cout << "[S] OK" << std::endl;
	struct sockaddr_in server_addr;
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	//server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	std::cout << std::string("[S] Binding to 127.0.0.1:") << port << std::endl;
	if(bind(sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0){
		perror("[S] Error when binding to port");
		exit(1);
	}else std::cout << "[S] OK" << std::endl;
	std::cout << "[S] Listening on socket with interval 5..." << std::endl;
	if(listen(sock, 5) < 0){
		perror("[S] Error when listening");
		exit(1);
	}else std::cout << "[S] OK" << std::endl;

	std::cout << "[S] Waiting for connection..." << std::endl;
	struct sockaddr_in client_addr;
#ifdef _INQUISITION_NETWORK_H_WIN
	int client_addr_len = sizeof(client_addr);
#else
	socklen_t client_addr_len = sizeof(client_addr);
#endif
	int clientsock = accept(sock, (struct sockaddr*) &client_addr, &client_addr_len);
	if(clientsock < 0){
		perror("[S] Error when accepting connection");
		return;
	}else{
		std::cout << "[S] Got connection" << std::endl;
	}
	
	Netquisition::Server server(clientsock);
	std::cout << "[S] Server initialized." << std::endl;
	Netquisition::Data d = server.recv();
	while(d.is_valid() && d.get_data() != std::string("BYE")){
		std::cout << "[S] Server got: " << d.get_data() << std::endl;
		if(!server.send(Netquisition::Data("OK"))) break;
		d = server.recv();
	}
	
	std::cout << "[S] Closing connection..." << std::endl;
#ifdef _INQUISITION_NETWORK_H_WIN
	closesocket(clientsock);
	closesocket(sock);
	std::cout << "[S] Cleanning up Winsock2..." << std::endl;
	WSACleanup();
#else
	close(clientsock);
	close(sock);
#endif
	std::cout << "[S] Done." << std::endl;
}

void clientthread(){
	std::this_thread::sleep_for(std::chrono::seconds(1));
	Netquisition::Client c(Netquisition::Client::create_sock("127.0.0.1", port));
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "[C] Client initialized." << std::endl; std::cout.flush();
	if(!c.send(Netquisition::Data("Hi"))) std::cout << "[C] Failed to send 'Hi'" << std::endl; else std::cout << "[C] Sent 'Hi'." << std::endl;
	Netquisition::Data d = c.recv();
	if(d.is_valid() && d.get_data() == std::string("OK")){
		std::cout << "[C] Client got: " << d.get_data() << std::endl; std::cout.flush();
		c.send(Netquisition::Data("bye lel")); std::cout.flush();
		d = c.recv();
	}
	if(d.is_valid()) { std::cout << "[C] Client got: " << d.get_data() << std::endl; std::cout.flush(); }
}

int main(){
	std::cout << "Loading..." << std::endl;
	std::thread server(serverthread);
	std::thread client(clientthread);
	server.join();
	client.join();
	std::cout << "Done." << std::endl;
	return 0;
}