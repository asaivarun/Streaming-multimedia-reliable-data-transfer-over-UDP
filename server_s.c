/* CSD 304 Computer Networks, Fall 2016
   Lab 2, server
   Team: 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdint.h>
#include <fcntl.h>
 #include <poll.h>
//#include <intypes.h>
#define SERVER_PORT 5432
#define BUF_SIZE 50001



int main(int argc, char * argv[]){
	uint8_t frno=0;
	uint8_t  a=0;

	uint8_t ack_recv;
	fd_set readfds;
	struct timeval tv;
  struct sockaddr_in sin;
  struct sockaddr_storage client_addr;
  char clientIP[INET_ADDRSTRLEN]; /* For IPv4 addresses */
  struct hostent *hp;
  socklen_t client_addr_len;
  char buf[BUF_SIZE];
  int len;
  int s;
  char *host;

  tv.tv_sec=1;
  tv.tv_usec=0;
  //struct timespec t,time1;
  //t.tv_sec=0.5;
  //t.tv_nsec=500000000L; 		
  
  /* Declarations for file(s) to be sent 
     ...
  */
  
  /* For inserting delays, use nanosleep()
     struct timespec ... */ 
 

  /* To get filename from commandline */
  /* if (argc==...) {} */
   
   
  /* Create a socket */
  if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("server: socket");
    exit(1);
  }
  //int new_s=socket(PF_INET, SOCK_DGRAM, 0);
  //setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,(char*)&tv,sizeof(struct timeval));
 
 
  /* build address data structure and bind to all local addresses*/
  memset((char *)&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
 
  /* If socket IP address specified, bind to it. */
  if(argc == 2) {
    host = argv[1];
    hp = gethostbyname(host);
    if (!hp) {
      fprintf(stderr, "server: unknown host %s\n", host);
      exit(1);
    }
    memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length);
  }
  /* Else bind to 0.0.0.0 */
  else
    sin.sin_addr.s_addr = INADDR_ANY;
  
  sin.sin_port = htons(SERVER_PORT);
  
  if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
    perror("server: bind");
    exit(1);
  }
  
  else{
    /* Add code to parse IPv6 addresses */
    inet_ntop(AF_INET, &(sin.sin_addr), clientIP, INET_ADDRSTRLEN);
    printf("Server is listening at address %s:%d\n", clientIP, SERVER_PORT);
  }
  
  printf("Client needs to send \"GET\" to receive the file %s\n", argv[1]);
    
  int n=s+1;
  client_addr_len = sizeof(client_addr);
  //int ack_recv;
  
  /* Receive messages from clients*/
  while(len = recvfrom(s, buf, sizeof(buf), 0,
		       (struct sockaddr *)&client_addr, &client_addr_len)){
	//printf("Inside The Loop\n");
  	
	int i=0;
    
    inet_ntop(client_addr.ss_family,
	      &(((struct sockaddr_in *)&client_addr)->sin_addr),
	      clientIP, INET_ADDRSTRLEN);
    buf[strlen(buf)-1]='\0';
    printf("Server got message from %s: %s [%d bytes]\n", clientIP, buf, len);

    /* Send to client */
    /* Add code to send file if the incoming message is GET */
    if(strcmp(buf,"GET")==0)
      {
		//printf("Inside IF\n");
		//printf("Before File Pointer");
          FILE *fp=fopen("sherlock.avi","rb");
		//printf("After File pointer");
        //char buf[40960];
        long int length;
        fseek(fp,0L,SEEK_END);
	//printf("After Seeking End");
        length=ftell(fp);
        fseek(fp,0L,SEEK_SET);
        //printf("%ld\n",length);
        //msg=(char *) malloc((length+1)*sizeof(char));
        //long int len1=fread(buf,1,40960,fp);
        //printf("%ld\n",len1);
        //printf("%ld\n",len1 );
	//printf("BOOM");
        FILE *prev;
        while(length>=0){
		//printf("vineeth\n");
        		//printf("packet is tuffed with num:%d\n",a);
        		buf[0]=a;
        		//buf[1]=frno;
        		prev=fp;

                long int k=fread(&buf[1],1,50000,fp);
                //printf("%ld\n",k);
			

                //fseek(fp,0,SEEK_CUR);
                /*if ((len = sendto(s, buf, sizeof(buf), 0,
        (struct sockaddr *)&client_addr, 
        client_addr_len)) == -1) {
      perror("server: sendto");
      exit(1);
    }*/

    	//printf("%d\n", i);
      
      //usleep(1000);
      //memset(buf, 0, sizeof(buf));
      struct pollfd fd;
      fd.fd=s;
      fd.events=POLLIN;
      sendto(s, buf, sizeof(buf), 0,
        (struct sockaddr *)&client_addr, 
        client_addr_len);
      printf("packet sent: %d\n", a);
      //usleep(1000);
      		
      //printf("packet sent:%d\n",i);
      //int block1=0;
      while(1)
	{
		
		int block=poll(&fd,1,750);
	
	
		
		
			
		
	  
      		
	
		//int block=select(n,&readfds,NULL,NULL,&tv);
		printf("%d\n",block);
			if(block==0)
  			{
  				printf("Ack not recieved\n");
  				sendto(s, buf, sizeof(buf), 0,
        (struct sockaddr *)&client_addr, 
        client_addr_len);
  				printf("packet sent again %d\n", a);
  				

  			
  				


  				//fp=prev;
  			
  			
  		//recvfrom(s,&ack_recv, sizeof(uint8_t),0,
		       //(struct sockaddr *)&client_addr, &client_addr_len);
  		//printf("DAFT CUNT:%d\n", ack_recv);
  		//printf("%d\n",ack_recv);
  		//printf("Recieved ack Num:%d:Wrong\n",ack_recv);
  			//fp=prev;
  			

  		
  		//fflush((FILE*)s);
  		//fflush(fp);
  		//fflush(prev);  		
  	}	
  	else if(block==-1)
  	{
  		perror("Select");
  	}

  	else{
  		//recvfrom(s,&ack_recv, sizeof(uint8_t),0,
		       //(struct sockaddr *)&client_addr, &client_addr_len);
  		

  			//printf("%d\n",ack_recv );
  			//int num=recvfrom(s,&ack_recv, sizeof(uint8_t),0,
		       //(struct sockaddr *)&client_addr, &client_addr_len;
  			//printf("Its not blocked ");
  	//printf("recieved num:%d\n",num);
  			
  				recvfrom(s,&ack_recv, sizeof(uint8_t),0,
		       (struct sockaddr *)&client_addr, &client_addr_len);
  				//fflush(s);
  		
  				if(a==ack_recv || a== ack_recv-1)
  				{
  				
  				printf("Recieved Acked num:%d\n",ack_recv);
  		 		/*if(a==1)
  		 		{
  		 			a-=1;
  		 		}
  		 		else if(a==0)
  		 		{
  		 			a+=1;
  		 		}*/
  		 		a+=1;
  		 		length-=50000;
  		 		i+=1;
  		 		memset(buf, 0, sizeof(buf));
  		 		break;
  			
  			


	}

      //recvfrom(s,&ack_recv, sizeof(uint8_t),0,
		       //(struct sockaddr *)&client_addr, &client_addr_len);
	
	//printf("%d\n",block);
	//printf("%d\n",block);
	//printf("sent %d\n",i);
	
  //usleep(10000);
	
  

  			}
  			
  		
 
}	
  	
  
  
    	//nanosleep(&t,&time1);

        
        //long int l=ftell(fp);
        //printf("%ld\n", l);
       

  
        }
        //break;

        strcpy(buf, "BYE");
  sendto(s, buf, sizeof(buf), 0, 
   (struct sockaddr*)&client_addr, client_addr_len);



        
      
      }
      //setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,(char*)&tv,sizeof(struct timeval));

    
    
    //memset(buf, 0, sizeof(buf));
  }
  
  /* Send BYE to signal termination */
  /*strcpy(buf, "BYE");
  sendto(s, buf, sizeof(buf), 0, 
	 (struct sockaddr*)&client_addr, client_addr_len);*/
  
}

