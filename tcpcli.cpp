#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netdb.h>
#include <cstring>
#include <netinet/in.h>

#define SERVER_CLOSE_CONNECTION 'q'

using namespace std;
 void error(const char *msg)
{
      perror(msg);
      exit(0);
}

bool find_exit(const char* msg);

int main(int argc, char const* argv[]) {
	
	int sockfd, servlen, n;
	
	struct sockaddr_un serv_addr;
        char buffer[256];
	bzero((char *)&serv_addr, sizeof(serv_addr));
        serv_addr.sun_family = AF_UNIX;
        strcpy(serv_addr.sun_path, argv[1]);
servlen = strlen(serv_addr.sun_path)+sizeof(serv_addr.sun_family);
	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sockfd < 0) {error("Creating socket"); exit(0);}
        else cout<<"Success creating socket"<<endl;
        if(connect(sockfd, (struct sockaddr *)&serv_addr, servlen) <0) error("Connecting");
        else cout<<"Connected"<<endl;

	recv(sockfd, buffer, 256, 0);
	cout << "For exit enter q\n";
	while(true) {
		cout << "Message from client: ";
		cin.getline(buffer, 256);
		send(sockfd, buffer, 255, 0);
		if(find_exit(buffer)){break;}

		cout << "Message from server: ";
		recv(sockfd, buffer, 256, 0);
		cout << buffer << endl;
		if (find_exit(buffer)){break;}
	}
	//cout << "\n Good bye!" << endl;
	close(sockfd);
	return 0;
}

bool find_exit(const char* msg) {
	for(int i=0; i < strlen(msg); ++i){
		if(msg[i] == SERVER_CLOSE_CONNECTION){
			return true;
		}
	}
	return false;
}
