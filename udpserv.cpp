#include <iostream>
#include <sys/types.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <netinet/in.h>

using namespace std;

#define SOCKET_CLOSE_CONNECTION 'q'

void error(const char *msg)
{
      perror(msg);
      exit(0);
}

bool find_exit(const char* msg);

int main(int argc, char const* argv[]) 
{
	int sock, msg, servlen;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	socklen_t clilen;
        char buffer[1024];
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0) {error("Opening socket");exit(0);}
	cout <<  "Success open" << endl;
        servlen = sizeof(serv_addr);
	serv_addr.sin_port = htons(atoi(argv[1]));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	if( bind(sock,(struct sockaddr *)&serv_addr, servlen)<0) error("Binding");

	bool isExit = false;
	clilen = sizeof(cli_addr);
	while(!isExit) 
	{
		cout << "Datagram from client: ";
		msg = recvfrom(sock, buffer, 1024, 0, (struct sockaddr *)&cli_addr, &clilen);
		cout << buffer << endl;
		if (find_exit(buffer)){break;}
		if(msg > 0) 
		{sendto(sock, "Message is received", 1024, 0, (struct sockaddr *)&cli_addr, clilen);}
	}

	close(sock);
	return 0;
}

bool find_exit(const char* msg) 
{
	for(int i=0; i < strlen(msg); ++i)
	{
		if(msg[i] == SOCKET_CLOSE_CONNECTION)
		{
			return true;
		}
	}
	return false;
}
