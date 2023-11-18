#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>

//Global to store voters
char Voter_List[50][100];        
char Casted_List[50][100];

void Savingfunc(char *client, char *vote) 
{
	FILE *save2 = fopen("Output.txt", "a");
	strcat(client, " - ");
	fputs(client, save2);
	fputs(vote, save2);
	printf("%s\n", vote);

	fclose(save2);
}

void addToCasted_List(char *client) 
{
	int i;
	for(i=0; Casted_List[i][0] != '\0'; i++) 
	{
	//do nothing 
	}
	strcpy(Casted_List[i], client);
}

bool Check_voted(char *buf) 
{
	int i;
	for(i=0; Casted_List[i][0] != '\0'; i++) 
	{
		int len = strlen(buf)-1;
		if(strncmp(Casted_List[i], buf, len) == 0)
			return true;
	}
	return false;
}

bool Validitiy_Check(char *buf) 
{
	int i;
	for(i=0; Voter_List[i][0]!='\0'; i++) 
	{
		int len = strlen(buf)-1;
		if(strncmp(Voter_List[i], buf, len) == 0) 
			return true;
	}
	return false;
}

void *createVotingRoom(void *clientSocket) 
{
		int socket = *(int *)clientSocket;
		char buffer[500] = {'\0'};
		recv(socket, buffer, sizeof(buffer), 0);
		printf("CLIENT: %s", buffer);
		if(Validitiy_Check(buffer) == false) 
		{
			strcpy(buffer, "Your name is not valid.Press Enter to close connection");
			send(socket, buffer, strlen(buffer), 0);
		}
		else if(Check_voted(buffer) == true) 
		{
			strcpy(buffer, "A client can only vote ONCE. \n Press Enter to Close Connection\n");
			send(socket, buffer, sizeof(buffer), 0);
		}
		else
		{
		char name[100];
		strncpy(name, buffer, strlen(buffer)-1);

		char candidatesMsg[500];
		strcpy(candidatesMsg, "Welcome. Please specify your Symbol:\n");
		FILE *fin = fopen("Candidates_List.txt", "r");	
		char temp[100];
		while(fgets(temp, sizeof(temp), fin) != NULL) 
		{
			strcat(candidatesMsg , temp);
		}

		fclose(fin);
		
		send(socket, candidatesMsg, strlen(candidatesMsg), 0);
		memset(buffer, '\0', sizeof(buffer));
		recv(socket, buffer, sizeof(buffer), 0);
		addToCasted_List(name);

		Savingfunc(name, buffer);
		strcpy(buffer, "Thank you for casting the vote. Your vote has been recorded.\nClosing Connection.\n");
		send(socket, buffer, sizeof(buffer), 0);
		}
	close(socket);
	free(clientSocket);
	pthread_exit(NULL);
}

int main() 
{
	//fill the array by reading voter list for affirmation purpose
   FILE *fin = fopen("Voters_List.txt", "r");

   
    int i;
    for(i=0; fgets(Voter_List[i], sizeof(Voter_List[i]), fin) != NULL; i++) 
    {
    	//do nopthing
    }
    
    fclose(fin);

	int server_socket;
	struct sockaddr_in server_addr;
	if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) <0) 
	{
		printf("Could not create socket\n");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(2000);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) <0) 
	{
		printf("Bind failed\n");
		exit(1);
	}

	if(listen(server_socket, 3) < 0) 
	{
		printf("Failed to listen on the socket\n");
		exit(1);
	}
	printf("Listening for incoming connections\n");
	
	while(1) 
	{
		int client_socket = accept(server_socket, NULL, NULL);
		if(client_socket < 0) 
		{
			printf("Accept failed\n");
			exit(1);
		}

		int *client_socket_ptr = (int*) malloc(sizeof(int));
		*client_socket_ptr = client_socket;
		pthread_t Serverthread;
		pthread_create(&Serverthread, NULL, createVotingRoom, (void*)client_socket_ptr);
		
	}
	close(server_socket);

	return 0;
}
