#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<errno.h>
#include<error.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<memory.h>
#include<netdb.h>

#define ERROR        -1
#define Client_Queue 5
#define Buffer_data  1024

// Function for UDP protocol: both Transmission and Reception
char* udp_clientsoc(char send_str[1024], int port_no, int ed_sockfd)
{
	// Function variable declaration and initialization
	char *val=(char *)malloc(Buffer_data);
	char *val_str;
	char *recv_store=(char *)malloc(Buffer_data);
	char *recvstore=(char *)malloc(Buffer_data);

	val_str=(char *)send_str;
	strcpy(val,val_str);

	int udp_sockfd, len;
        struct sockaddr_in andser, orser;
	struct hostent *hp;
	struct hostent *hp1;

	len=sizeof(struct sockaddr_in);
	
	// UDP connection for AND

	if(port_no==22457)
	{

	// Socket fd created for transmission of lines to Backend seerver AND
	if((udp_sockfd=socket(PF_INET, SOCK_DGRAM, 0))== ERROR)
	{
		perror("edgeserver client socket: ");
		exit(-1);
	}
	// Assigning family, IP address: 127.0.0.1, and host no: 22457
	bzero(&andser, len);
	andser.sin_family=PF_INET;
	hp=gethostbyname("localhost");
	if(hp==0)
	{
		perror("unknown host");
	}
	bcopy((char *)hp->h_addr, (char *)&andser.sin_addr,hp->h_length);
	andser.sin_port  = htons(port_no);

	// Send the lines to Backend server AND
	if((sendto(udp_sockfd,val,1024,0,(struct sockaddr *)&andser,len))>0);
	if (send_str!="done\n")
	{
	// Receive the lines from Backend server And using port no 24457
	recvfrom(ed_sockfd,recv_store,1024,0,(struct sockaddr *)&andser,&len);
        return recv_store;
	}
	else return NULL;
	}

	// UDP connection for OR

	else if(port_no==21457)
	{

	// Socket fd created for transmission of lines to Backend seerver OR
	
	if((udp_sockfd=socket(PF_INET, SOCK_DGRAM, 0))== ERROR)
        {
                perror("edgeserver client socket: ");
                exit(-1);
        }

	// Assigning family, IP address: 127.0.0.1, and host no: 21457
        bzero(&orser, len);
        orser.sin_family=PF_INET;
        hp1=gethostbyname("localhost");
        if(hp1==0)
        {
                perror("unknown host");
        }
        bcopy((char *)hp1->h_addr, (char *)&orser.sin_addr,hp1->h_length);
        orser.sin_port  = htons(port_no);

	// Send the lines to Backend server OR
        if((sendto(udp_sockfd,val,1024,0,(struct sockaddr *)&orser,len))>0);
        if (send_str!="done\n")
        {
	// Receive the lines from Backend server OR using port no 24457
        recvfrom(ed_sockfd,recvstore,1024,0,(struct sockaddr *)&orser,&len);
	return recvstore;
        }
        else return NULL;
	}		
	
}





void main()
{
	// Initialization

	struct sockaddr_in edgeserver;
	struct sockaddr_in client_addr;
	struct hostent *hp2;
	struct sockaddr_in edgeser;

	char client_recv[Buffer_data];
	char *val_str=(char *)malloc(Buffer_data);
	char key[Buffer_data], and_key[Buffer_data], or_key[Buffer_data], send_str[Buffer_data];
	char *str;
	char *recept=(char *)malloc(Buffer_data);
	char *recept1=(char *)malloc(Buffer_data);
	char *arr, *arr1, *small[4], *small1[4], print_and[Buffer_data], print_or[Buffer_data];
	char client_and[Buffer_data], client_or[Buffer_data];
	char *newstr;
	char *new_str=(char *)malloc(Buffer_data);
	char *new1str;
	char *new1_str=(char *)malloc(Buffer_data);
	
	int edge_sockfd;
	int ed_sockfd;
	int client;
	int length_edge=sizeof(struct sockaddr_in);
	int len_client;
	int count=0;
	int and1=0;
	int or1=0;
	int k=0, l=0;
	int len1;
	int bind_val=1;
	int bind1_val=1;

	//creating a TCP socket for receiving and sending lines from the client with port no.: 23457
	if((edge_sockfd=socket(PF_INET, SOCK_STREAM, 0))== ERROR)
	{
		perror("edgeserver socket: ");
		exit(-1);
	}
	// Assigning family, IP address: 127.0.0.1, and host no: 23457
	edgeserver.sin_family = PF_INET;
	edgeserver.sin_addr.s_addr=inet_addr("127.0.0.1");
	edgeserver.sin_port  = htons(23457);
	bzero(&edgeserver.sin_zero, 8);

	// Creating a UDP socket for receiving files from the Backend servers with port no.: 24457
        if((ed_sockfd=socket(PF_INET, SOCK_DGRAM, 0))== ERROR)
        {
                perror("edgeserver client socket: ");
                exit(-1);
        }
	// Assigning family, IP address: 127.0.0.1, and host no: 24457
        bzero(&edgeser, length_edge);
        edgeser.sin_family=PF_INET;
        hp2=gethostbyname("localhost");
        if(hp2==0)
        {
                perror("unknown host");
        }
        bcopy((char *)hp2->h_addr, (char *)&edgeser.sin_addr,hp2->h_length);
        edgeser.sin_port  = htons(24457);

	// Setsockopt is used to delete the previously existing bind connection
	if((setsockopt(edge_sockfd,SOL_SOCKET, SO_REUSEADDR, &(bind_val), sizeof(int)))==ERROR)
	{
		perror("setsockopt");
		exit(-1);

	}

	if((setsockopt(ed_sockfd,SOL_SOCKET, SO_REUSEADDR, &(bind1_val), sizeof(int)))==ERROR)
        {
                perror("setsockopt1");
                exit(-1);

        }
	
	// Binding the socket to the port number 
	if((bind(edge_sockfd, (struct sockaddr *)&edgeserver, length_edge))== ERROR)
	{
		perror("bind: ");
		exit(-1);
	}


        if((bind(ed_sockfd, (struct sockaddr *)&edgeser, length_edge))== ERROR)
        {
                perror("bind1: ");
                exit(-1);
        }

	// Waiting for a request from the client
	if((listen(edge_sockfd, Client_Queue))== ERROR)
	{
		perror("listen: ");
		exit(-1);
	}
	printf("edgeserver is up and running\n");
	

	len1=sizeof(edgeser);
        
	while(1)
	{
		// Accept the request from the client
		if((client = accept(edge_sockfd, (struct sockaddr *)&client_addr, &length_edge))== ERROR)
		{
			perror("accept");
			exit(-1);
		}
		// receive the lines from the client
		while(	recv(client, client_recv, Buffer_data, 0))
		{
			// Sequence the line to know the order of reception and transfering
            str=(char *)client_recv;
			strcpy(val_str,str);
			sprintf(key,"%d,",count);
			strcat(key,val_str);
			
			// Send the line to Backend servers

			// If or, send to Server_or
			if(val_str[0]=='o')
			{
			// UDP socket connection for sending and receiving lines to Backend server
	       		recept1=udp_clientsoc(key,21457,ed_sockfd);
			
			if(recept1!=NULL)
                        {
			// Decimate the received string from Backend or
			newstr=(char *)recept1;
                        strcpy(new_str,newstr);
			arr=strtok(new_str,",\n");
                        while(arr!=NULL)
                        {
			    // strdup works as strcpy
                            small[k]=strdup(arr);
                            arr=strtok(NULL,",\n");
                            k++;
                        }
			sprintf(client_or,"%s\n",small[1]);

			// send back the result to client through TCP connection
	                send(client,client_or,1024,0);

			// Store the received lines in a buffer to display later
                        sprintf(print_or,"%s\n",small[2]);
			strcat(or_key,print_or);
			k=0;
		        }
			// Count the number of or lines 
			or1++;
			}


			// If and, send to Server_and

			else if(val_str[0]=='a')
			{
			// UDP socket connection for sending and receiving lines to Backend server
			recept=udp_clientsoc(key,22457,ed_sockfd);
			if(recept!=NULL)
			{
			// Decimate the received string from Backend Server AND
			new1str=(char *)recept;
                        strcpy(new1_str,new1str);
			arr1=strtok(new1_str,",\n");
			while(arr1!=NULL)
				{
				// strdup works as strcpy
				small1[l]=strdup(arr1);
				arr1=strtok(NULL,",\n");
				l++;
				}
			sprintf(client_and,"%s\n",small1[1]);

			// send back the result to client through TCP connection
			send(client,client_and,1024,0); 

			// Store the received lines in a buffer to display later
			sprintf(print_and,"%s\n",small1[2]);
			strcat(and_key,print_and);
			l=0;						
		        }
			// Count the number of and lines 
		       	and1++;
			}

			// Count the total number of lines received
		  	count++;
                }

		// End condition is sent for the Backend servers to realize that task is completed
		udp_clientsoc("done\n",22457,ed_sockfd);	
		udp_clientsoc("done\n",21457,ed_sockfd);
		close(client);
		
		printf("The edge server has received %d lines from the client using TCP over port %d\n", count, ntohs(edgeserver.sin_port));
		printf("The edge has successfully sent %d lines to Backend-Server OR\n",or1);
		printf("The edge has successfully sent %d lines to Backend-Server AND\n",and1);
		printf("The edge server starts receiving the computation results from Backend-Server OR and Backend-Server AND using UDP over port %d\n",ntohs(edgeser.sin_port));
		printf("The computational results are:\n");
		printf("%s",or_key);
                printf("%s",and_key);
		printf("The edge server has successfully finished receiving all computation results from Backend-server OR and Backend-server AND\n");
		printf("The edge server has successfully finished sending all the computation results to the client\n");
		bzero(or_key,sizeof(or_key));
		bzero(and_key,sizeof(and_key));
		count=0;
		and1=0;
		or1=0;

	}
	
} 
