#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h> 
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<stdio.h>
#include<string.h> 

#define MAX_PENDING 5

int main(){

	int sid = socket(PF_INET, SOCK_STREAM, 0); 

	struct sockaddr_in addr;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(80);

	errno = 0;
	if(bind(sid, (struct sockaddr*) &addr, sizeof(addr)) < 0){
		printf("error in binding %s\n", strerror(errno));
		return -1;
	}

	errno = 0;
	if(listen(sid, MAX_PENDING < 0)){
		printf("error in listening %s\n", strerror(errno));
		return -1;
	}
	struct sockaddr_in client_addr;
	unsigned int client_len;
	int client_sock;
	char buffer[24]; 
	int msglen;
	int out;

	while(1){
		errno = 0; 
		printf("waiting for a connection from the space...\n");
		if((client_sock = accept(sid, (struct sockaddr*) &client_addr, &client_len)) < 0){
			printf("error in accept %s", strerror(errno));
		}

		printf("address: %s\n", inet_ntoa(client_addr.sin_addr));

		printf("connection accepted\n"); 
		if((msglen = recv(client_sock, &buffer, sizeof(buffer), 0)) < 0){
			printf("error in receiving %s", strerror(errno));
		}
		printf("message received\n");

		while(msglen > 0){
			errno = 0;
			if((out = send(client_sock, &buffer, sizeof(buffer), 0)) < 0){
				printf("error: %s\n", strerror(errno)); 
			}
			msglen -= out;
		}
		printf("message replied\n");
		close(client_sock);
		printf("socket close\n");

	}
	return 0;
}