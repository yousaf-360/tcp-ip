#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int main() 
{

    int mySocket;
    struct sockaddr_in server_addr;
    
    if((mySocket = socket(AF_INET, SOCK_STREAM, 0)) <0) 
    {
    	printf("Could not create socket\n");
		exit(1);        
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");


    if(connect(mySocket, (struct sockaddr*)&server_addr, sizeof(server_addr)) <0) 
    {
        printf("Failed to connect to the server\n");
        exit(1);
    }
    printf("Connected to the server\n");

    
    char buffer[500] = {'\0'};

    memset(buffer, '\0', sizeof(buffer));
    fgets(buffer, sizeof(buffer), stdin);

   
    if(send(mySocket, buffer, strlen(buffer), 0) <0) 
    {
        printf("Send failed\n");
        exit(1);
    }
    
    memset(buffer, '\0', sizeof(buffer));
    if(recv(mySocket, buffer, sizeof(buffer), 0) <0) 
    {
        printf("Recv failed\n");
        exit(1);
    }
 
    printf("SERVER: %s\n", buffer);

  
    fgets(buffer, sizeof(buffer), stdin);

  
    if(send(mySocket, buffer, strlen(buffer), 0) <0) 
    {
        printf("Send failed\n");
        exit(1);
    }

   
    if(recv(mySocket, buffer, sizeof(buffer), 0) <0) 
    {
        printf("Recv failed\n");
        exit(1);
    }
    printf("SERVER: %s\ns", buffer);


    close(mySocket);

    return 0;
}
