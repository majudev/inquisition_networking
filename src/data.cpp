#include "inquisition_network.hpp"
#include <cstring>

Netquisition::Data::Data() : valid(false), data(NULL) {}

Netquisition::Data::Data(const Netquisition::Data & data){
	this->valid = data.valid;
	if(!data.valid) return;
	this->length = data.length;
	this->data = new char[data.length];
	memcpy(this->data, data.data, data.length);
}

Netquisition::Data::Data(std::string data) : valid(true){
	this->length = data.size() + 1;
	this->data = new char [this->length];
	unsigned int x = 0;
	while(x != data.size()){
		this->data[x] = data[x];
		++x;
	}
	this->data[x] = '\0';
}

Netquisition::Data::Data(char* data, size_t length) : valid(true), length(length){
	this->data = new char [length];
	size_t x = 0;
	while(x != this->length){
		this->data[x] = data[x];
		++x;
	}
}

Netquisition::Data::~Data(){
	if(this->data != NULL) delete [] this->data;
	this->data = NULL;
}

std::string Netquisition::Data::get_data(){
	if(!this->valid) return std::string();
	if(this->data == NULL) return std::string();
	if(this->data[this->length - 1] != '\0') return std::string();
	return std::string(data);
}

size_t Netquisition::Data::get_data(char * buffer, size_t * length){
	if(*length < this->length || !this->valid) return 0;
	*length = this->length;
	size_t x = 0;
	while(x != this->length){
		buffer[x] = this->data[x];
		++x;
	}
	return this->length;
}

bool Netquisition::Data::is_valid(){
	return this->valid;
}

Netquisition::Data & Netquisition::Data::operator=(const Netquisition::Data & data){
	if(this != &data){
		this->valid = data.valid;
		if(data.valid){
			this->length = data.length;
			delete [] this->data;
			this->data = new char [data.length];
			memcpy(this->data, data.data, data.length);
		}
	}
	return *this;
}