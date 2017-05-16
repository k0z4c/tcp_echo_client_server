#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h> 
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h> 
#include<stdio.h>
#include<string.h>
//socket instantiating
int main(){
	int sid = socket(AF_INET, SOCK_STREAM, 0);
	//socket initialization 
	struct sockaddr_in addrport; 
	addrport.sin_family = AF_INET;	
	addrport.sin_addr = inet_aton("127.0.0.1", &addrport);
	addrport.sin_port = htons(80);

	struct sockaddr_in myaddr; 
	myaddr.sin_family = AF_INET; 
	myaddr.sin_addr = inet_aton("192.168.1.102", &myaddr);

	if((bind(sid, (struct sockaddr*) &myaddr, sizeof(myaddr)) ) < 0){
		printf("error %s\n", strerror(errno));
	}

	//connection
	if(connect(sid, (struct sockaddr*) &addrport, sizeof(addrport)) < 0)
		printf("error connect %s\n", strerror(errno));

	//transmit packets
	char msg[] = "Hello world!";
	send(sid, msg, sizeof(msg), 0);

	//recv some data from echo server
	char buffer[24]; 
	int in;

	printf("receiving..\n");

	errno = 0;
	if((in = recv(sid, &buffer, sizeof(buffer), 0)) < 0){
		printf("%s", strerror(errno));
	}
	printf("%s\n", buffer);
	close(sid);
	sleep(5);

	return 0;
}