#include "inquisition_network.hpp"
#include <cstring>
#include <errno.h>

Netquisition::Server::Server(int clientsock){
	this->sock = clientsock;
	this->c = true;
	if(clientsock < 0 || !this->is_up()) this->c = false;
}

Netquisition::Server::~Server(){
	/*this->messages.push_back(LogMessage("Closing connection..."));
#ifdef _INQUISITION_NETWORK_H_WIN
	closesocket(this->sock);
#else
	close(this->sock);
#endif
	this->messages.push_back(LogMessage("Done."));*/
}

bool Netquisition::Server::is_up(){
#ifdef _INQUISITION_NETWORK_H_WIN
	char buffer[256];
	int buffersize = 256;
	if(getsockopt(this->sock, SOL_SOCKET, SO_ERROR, buffer, &buffersize)){
		this->messages.push_back(LogMessage("Socket error."));
		return false;
	}
	return true;
#else
	int error = 0;
	socklen_t len = sizeof (error);
	getsockopt (this->sock, SOL_SOCKET, SO_ERROR, &error, &len);
	if(error != 0){
		this->messages.push_back(LogMessage("Socket error: " + std::string(strerror(error))));
		return false;
	}
	return true;
#endif
}

bool Netquisition::Server::send(Data data){
	if(!this->c || !data.valid || !this->is_up()) return false;
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

Netquisition::Data Netquisition::Server::recv(){
	if(!this->is_up()) return Data();
	size_t length = 1024;
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

std::vector<Netquisition::LogMessage> Netquisition::Server::get_messages(){
	std::vector<LogMessage> f(this->messages);
	this->messages.clear();
	return f;
}