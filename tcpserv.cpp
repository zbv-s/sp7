#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netdb.h>
#include <cstring>
#include <netinet/in.h>

using namespace std;

#define CLIENT_CLOSE_CONNECTION 'q'

bool find_exit(const char* msg);

void error(const char *msg)
{
      perror(msg);
      exit(0);
}

int main(int argc, char const* argv[]) {
	int sockfd, newsockfd, servlen, n;
        socklen_t clilen;
	struct sockaddr_un serv_addr, cli_addr;
        char buffer[256];
	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sockfd < 0) {error("Creating");exit(0);}
	else cout <<  "Success creating\n";
		
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sun_family = AF_UNIX;
        strcpy(serv_addr.sun_path, argv[1]);
        servlen=strlen(serv_addr.sun_path)+sizeof(serv_addr.sun_family);
       if(bind(sockfd, (struct sockaddr *)&serv_addr, servlen) <0) {error("Binding"); exit(0);}
	else cout<<"Binding success"<<endl;
	listen(sockfd, 5);
        clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,&clilen);
	
	if(newsockfd < 0 ) {error("Accepting"); exit(0);}
        else cout<<"Success accept"<<endl;
	bool isExit = false;
	while(newsockfd > 0) {
		strcpy(buffer, "=> Server Connected!\n");
		send(newsockfd, buffer, 256, 0);
		cout << "Connected" 
		     << endl
		     << "For exit enter q\n";
		cout << "Message from client: ";
		recv(newsockfd, buffer, 256, 0);
		cout << buffer << endl;
		if(find_exit(buffer)){isExit=true;}
		while(!isExit) {
			cout << "Message from server: ";
			cin.getline(buffer, 256);
			send(newsockfd, buffer, 255, 0);
			if(find_exit(buffer)){break;}

			cout << "Message from client: ";
			recv(newsockfd, buffer, 256, 0);
			cout << buffer << endl;
			if (find_exit(buffer)){break;}
			
		}
		isExit = false;
		exit(1);
	}
	return 0;
}

bool find_exit(const char* msg) {
	for(int i=0; i < strlen(msg); ++i){
		if(msg[i] == CLIENT_CLOSE_CONNECTION){
			return true;
		}
	}
	return false;
}
