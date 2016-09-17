/** 
* The MIT License (MIT)
* Copyright (c) 2016 Micha? Maj
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
* and associated documentation files (the "Software"), to deal in the Software without restriction, 
* including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
* and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
* subject to the following conditions: The above copyright notice and this permission notice shall be 
* included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* Submitting bug reports of unofficial builds is leading cause of male impotence.
*
* I'm sure you didn't expect the Spanish Inquisition.
*/

#ifndef _INQUISITION_NETWORK_H_
#define _INQUISITION_NETWORK_H_

#include <string>
#include <vector>
#include <ctime>
#include <inquisition_cipher.hpp>

#if defined(_WIN32) || defined(WIN32)
#include <Windows.h>
#include <winsock2.h>
#include <io.h>
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#define _INQUISITION_NETWORK_H_WIN
#elif defined(__unix__)
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define _INQUISITION_NETWORK_H_UNI
#endif

namespace Netquisition{
	class LogMessage{
		public:
			std::string get_msg();
			time_t get_time();
		protected:
			std::string msg;
			time_t time;
			LogMessage(std::string msg);
			friend class Client;
			friend class Server;
	};
	
	class Data{
		public:
			Data(const Data &);
			Data(std::string data);
			Data(char* data, size_t length);
			~Data();
			std::string get_data();
			size_t get_data(char * buffer, size_t * length);
			bool is_valid();
			Data & operator= (const Data & data);
		protected:
			Data();
			bool valid;
			char * data;
			size_t length;
			friend class Client;
			friend class Server;
	};
	
	class Client{
		public:
			static int create_sock(std::string address, int port);
			Client(int socket);
			~Client();
			bool connected();
			bool send(Data data);
			Data recv();
			std::vector<LogMessage> get_messages();
		protected:
			bool c;
			std::vector<LogMessage> messages;
			int sock;
	};
	
	class Server{
		public:
			Server(int clientsock);
			~Server();
			bool is_up();
			bool send(Data data);
			Data recv();
			std::vector<LogMessage> get_messages();
		protected:
			bool c;
			std::vector<LogMessage> messages;
			int sock;
	};
}
#endif