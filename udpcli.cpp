#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <netinet/in.h>

#define SOCKET_CLOSE_CONNECTION 'q'

using namespace std;

void error(const char *msg)
{
      perror(msg);
      exit(0);
}

bool find_exit(const char* msg);

int main(int argc, char const* argv[]) 
{
	int sock, n;
        unsigned int len;
	struct sockaddr_in cli_addr;
	struct sockaddr_in serv_addr;
	struct hostent *hp;
	char buffer[256];	
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0) 
	{error("Socket");exit(0);}
       	serv_addr.sin_port = htons(atoi(argv[2]));
	serv_addr.sin_family = AF_INET;
	hp = gethostbyname(argv[1]);
	if ( hp == 0 ) error("Unknown host");
	bcopy((char *)hp->h_addr, 
			(char *)&serv_addr.sin_addr,
			hp->h_length);
	len = sizeof(cli_addr);
	cout << "For exit enter q" << endl;
	while(true) 
	{
		cout << "Message from client:";
		cin.getline(buffer, 256);
		n = sendto(sock, buffer, 255, 0,(struct sockaddr *)&serv_addr,len);
		if(n < 0) {error("Sendto"); exit(1);}
		if(find_exit(buffer)){break;}
		
		cout << "Message from server: ";
		recvfrom(sock, buffer, 256, 0,(struct sockaddr *)&cli_addr, &len);
		cout << buffer << endl;
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
