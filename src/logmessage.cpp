#include "inquisition_network.hpp"

Netquisition::LogMessage::LogMessage(std::string msg) : msg(msg){
	std::time(&this->time);
}

std::string Netquisition::LogMessage::get_msg(){
	return this->msg;
}

time_t Netquisition::LogMessage::get_time(){
	return this->time;
}