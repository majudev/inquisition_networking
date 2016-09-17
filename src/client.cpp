#include "inquisition_network.hpp"
#include <cstring>

int Netquisition::Client::create_sock(std::string address, int port){
	int sock;
	//this->c = false;
#ifdef _INQUISITION_NETWORK_H_WIN
	//this->messages.push_back(LogMessage("Initializing WinSock2..."));
	WSADATA wsadata;
	int error = WSAStartup(0x0202, &wsadata);
	if(error){
		//this->messages.push_back(LogMessage("Error when initializing Winsock2. Will now halt.\n"));
		//perror("Error");
		return -1;
	}

	if(wsadata.wVersion != 0x0202){
		//this->messages.push_back(LogMessage("Error. Cannot get 0x0202 version of Winsock. Will now halt.\n"));
		WSACleanup();
		return -1;
	}
#endif
	//this->messages.push_back(LogMessage("Creating socket..."));
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		//perror("Error when creating socket");
		return -1;
	}
	struct hostent * server = gethostbyname(address.c_str());
	if(server == NULL){
		//this->messages.push_back(LogMessage("Error: wrong hostname!"));
		return -1;
	}
	struct sockaddr_in server_addr;
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &server_addr.sin_addr.s_addr, server->h_length);
	server_addr.sin_port = htons(port);
	//this->messages.push_back(LogMessage("Connecting to " + address + ":" + itostr(port)));
	if(connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
		//perror("Error when connecting");
		return -1;
	}else{
		//this->messages.push_back(LogMessage("Got connection!"));
		//this->c = true;
		return sock;
	}
}

Netquisition::Client::Client(int socket) : sock(socket){
	if(socket < 0) { this->c = false; return; }
	this->c = true;
}

Netquisition::Client::~Client(){
	this->send(Data("BYE"));
	this->messages.push_back(LogMessage("Closing connection..."));
#ifdef _INQUISITION_NETWORK_H_WIN
	closesock(this->sock);
	this->messages.push_back(LogMessage("Cleaning up Winsock2..."));
	WSACleanup();
#else
	close(this->sock);
#endif
}

bool Netquisition::Client::connected(){
	return this->c;
}

bool Netquisition::Client::send(Data data){
	if(!this->c || !data.valid) return false;
#ifdef _INQUISITION_NETWORK_H_WIN
	int res = send(this->sock, data.data, data.length, 0);
#else
	int res = write(this->sock, data.data, data.length);
#endif
	if(res < 0){
		this->messages.push_back(LogMessage("Failed to send message!"));
	}
	return !(res < 0);
}

Netquisition::Data Netquisition::Client::recv(){
	size_t length = 1024*1024*10;
	char * buffer = new char [length];
#ifdef _INQUISITION_NETWORK_H_WIN
	size_t res = recv(this->sock, buffer, length, 0);
#else
	size_t res = read(this->sock, buffer, length);
#endif
	if(res < 0){
		this->messages.push_back(LogMessage("Failed to recv message!"));
		delete [] buffer;
		return Data();
	}
	Data tr(buffer, res);
	delete [] buffer;
	return tr;
}

std::vector<Netquisition::LogMessage> Netquisition::Client::get_messages(){
	std::vector<LogMessage> f(this->messages);
	this->messages.clear();
	return f;
}