#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<error.h>
#include<errno.h>


#define ERROR           -1
#define buffer_size     1024

void main(int argc, char *argv[])
{
	// initialization 
	char send_buf[buffer_size], recv_buf[buffer_size]; 
	char *str;
	char *arr1;
	char *val= (char *)malloc(buffer_size);
	char result[buffer_size], string1[buffer_size];
	char *strarr1= (char *)malloc(buffer_size);
	int client_sockfd,edge;
	int length1;
	int length2;
	int n;
	struct sockaddr_in edgeserver_addr;

	int edgeserver_port=23457;
	int line_buf=30;
	int lines=0;

	FILE *fp;
	
	// Check if the user has input any file
	if(argc!=2)
	{
		printf("Sufficient input arguments are not given\n");
		exit(-1);
	}
		
	// Sockfd is created for TCP connection
	if((client_sockfd=socket(PF_INET, SOCK_STREAM, 0))==ERROR)
	{
		perror("socket");
		exit(-1);
	}
	
	// Assigning family name, IP address: 127.0.0.1
	edgeserver_addr.sin_family = PF_INET;
	edgeserver_addr.sin_port   = htons(edgeserver_port);
	edgeserver_addr.sin_addr.s_addr=INADDR_ANY;
	bzero(&edgeserver_addr.sin_zero, 8);
	
	// connecting to the server

	if((connect(client_sockfd, (struct sockaddr *)&edgeserver_addr, sizeof(struct sockaddr_in)))==ERROR)
	{
		perror("connect");
		exit(-1);
	}

	printf("client is up and running\n");
		// file open and read
		fp=fopen(argv[1],"r");
		if(fp==NULL)
		{
			printf("File does not exists\n");
			exit(-1);
		}

		// transfer line by line to server using TCP connection
		while(fgets(send_buf,line_buf,fp))
		{
			send(client_sockfd,send_buf,1024,0);
			// check if Enter line is pressed, if yes stop excecuting
			str=(char *)send_buf;
			strcpy(val,str);
			if(val[0]=='\n')
				break;

			// count the number of line
			lines++;

			// Receive the line from the edge server
           		recv(client_sockfd,recv_buf,5024,0); 

			// Buffer the results
			arr1=(char *)recv_buf;
			strcpy(strarr1,arr1);
			sprintf(string1,"%s\n",strarr1);
	           	strcat(result,strarr1);
		}
		fclose(fp);		
		printf("The client has successfully finished sending %d lines to the edge server\n",lines);
		printf("The client has successfully finished receiving all computation results from the edge server\n");
		printf("The final computation result are:\n");
		printf("%s",result);
	close(client_sockfd);

}

