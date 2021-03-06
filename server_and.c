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
#define Buffer_data  1024



// Converting a decimal integer to binary integer
// Refferred to http://stackoverflow.com/questions/5488377/converting-an-integer-to-binary-in-c
int int_bin(int number)
{
        if (number==0) return 0;
        if (number==1) return 1;
        return (number%2)+10*int_bin(number/2);
}

// Converting a binary integer to dcimal integer
// Refferred to http://www.sanfoundry.com/c-program-binary-number-into-decimal/
unsigned int bin_int(unsigned num)
{
        int base=1, rem, decimal=0;
        while(num>0)
        {
                rem=num%10;
                decimal=decimal+rem*base;
                num=num/10;
                base=base*2;
         }
        return decimal;
}



void main()
{

	// initialization
        char *val=(char *)malloc(Buffer_data);
        char *val_str;
        char store[Buffer_data];
        char *arr, *str[6], sen[Buffer_data] ;
	char send_edge[Buffer_data];
        int and_sockfd, edge_sockfd, len1, len, recv_len;
        int op1=0,op2=0,result=0;
        int op11=0, op22=0, result1=0;
        int i=0,j=1, count=0;
        struct sockaddr_in andser;
        struct sockaddr_in edgeser, edg;
	struct hostent *hp;
        len=sizeof(andser);

	// Socket fd created for udp connection for receiving files from edge server
	if((and_sockfd=socket(PF_INET, SOCK_DGRAM, 0))== ERROR)
        {
                perror("server_and socket: ");
                exit(-1);
        }
	
	// Assigning family,  IP address: 127.0.0.1 and port numbers
        bzero(&andser, len);
        andser.sin_family=PF_INET;
        andser.sin_addr.s_addr=inet_addr("127.0.0.1");
        andser.sin_port  = htons(22457);
       	len1=sizeof(edgeser);
	
        // Socket fd created for udp connection for sending files to edge server
        if((edge_sockfd=socket(PF_INET, SOCK_DGRAM, 0))== ERROR)
        {
                perror("server_and socket: ");
                exit(-1);
        }
	
	// Assigning family,  IP address: 127.0.0.1 and port numbers
        bzero(&edgeser, len1);
	edgeser.sin_family=PF_INET;
	hp=gethostbyname("localhost");
        if(hp==0)
        {
                perror("unknown host");
        }
        bcopy((char *)hp->h_addr, (char *)&edgeser.sin_addr,hp->h_length);
	edgeser.sin_port  = htons(24457);

	// binding a socket to the port number
        if(bind(and_sockfd,(struct sockaddr *)&andser,len)== ERROR)
        {
                perror("bind:");
                exit(-1);
        }

        recv_len=sizeof(struct sockaddr_in);

        printf("The Server AND is up and running using UDP on port %d\n", ntohs(andser.sin_port));

        while(1)
        {
                memset(store, sizeof(store), 0);


		// Receiving data from the server
                while((recvfrom(and_sockfd,store,1024,0,(struct sockaddr *)&edg,&recv_len))!=ERROR)
                {
			// Print for every client excecution
			if(j==1)
			{
				printf("The Server AND starts receiving lines from the edge server for AND computation.\n The computation results are:\n");
				j=0;
			}
	                i=0;
                        val_str=(char *)store;
			
			// decimate the file to get the operands

                        strcpy(val,val_str);
                        arr=strtok(val,",\n");
                        while(arr!=NULL)
                        {
                            str[i]=strdup(arr);
                            arr=strtok(NULL,",\n");
                            i++;
                        }
                        if(i<3)
                        break;

			// operands are converted to integer decimal values
                        op1=atoi(str[2]);
                        op2=atoi(str[3]);
			op11=bin_int(op1);
                        op22=bin_int(op2);
			// AND operation is performed bit wise
                        result1=op11 & op22;
			// Integer conversion decimal to binary 
                        result=int_bin(result1);
                        sprintf(sen,"%d",result);
                        printf("%d and %d = %s\n",op1,op2,sen);
                        count++;
                        sprintf(send_edge,"%s,%d,%d and %d = %s\n",str[0],result,op1,op2,sen);
			// Send to edge server                
                        if((sendto(and_sockfd,send_edge,1024,0,(struct sockaddr *)&edgeser,recv_len))>0);
                }


        printf("The Server AND has successfully received %d lines from the edge server and finished all AND computations\n",count);
        count=0;
	j=1;
        printf("The Server AND has successfully finished sending all computation results to the edge server\n");
        }
        close(and_sockfd);

}

                                      
