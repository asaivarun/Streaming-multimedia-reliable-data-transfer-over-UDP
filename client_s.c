

/* CSD 304 Computer Networks, Fall 2016
   Lab 2, client
   Team: 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT 5432
#define BUF_SIZE 50001


int main(int argc, char * argv[]){
  //int ack;
  FILE *fp;
  uint8_t ack;
  uint8_t ack_recv;
  struct hostent *hp;
  struct sockaddr_in sin;
  char *host;
  char buf[BUF_SIZE];
  int s;
  int len;
  char clientIP[INET_ADDRSTRLEN]; /* For IPv4 addresses */
  socklen_t client_addr_len;
  struct sockaddr_storage client_addr;
  int client_seq_no=-1;
  if ((argc==2)||(argc == 3)) {
    host = argv[1];
  }
  else {
    fprintf(stderr, "usage: client serverIP [download_filename(optional)]\n");
    exit(1);
  }

  if(argc == 3) {
    fp = fopen(argv[2], "w");
    if (fp == NULL) {
      fprintf(stderr, "Error opening output file\n");
      exit(1);
    }
  }


  /* translate host name into peer's IP address */
  hp = gethostbyname(host);
  if (!hp) {
    fprintf(stderr, "client: unknown host: %s\n", host);
    exit(1);
  }
  else
    printf("Host %s found!\n", argv[1]);

  /* build address data structure */
  memset((char *)&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length);
  sin.sin_port = htons(SERVER_PORT);
  

  /* create socket */
  if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("client: socket");
    exit(1);
  }
  
  /* active open */
  if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
    
      perror("client: connect");
      close(s);
      exit(1);
    }
  else{
    
    printf("Client connected to %s:%d.\n", argv[1], SERVER_PORT);
    printf("To play the music, pipe the downlaod file to a player, e.g., ALSA, SOX, VLC: cat recvd_file.wav | vlc -\n"); 
    }
  
  

  /* send message to server */  
  fgets(buf, sizeof(buf), stdin);
  buf[BUF_SIZE-1] = '\0';
  len = strlen(buf) + 1;
  send(s, buf, len, 0);
  int i=0;
  
  /* get reply, display it or store in a file*/ 
  /* Add code to receive unlimited data and either display the data
  \   or if specified by the user, store it in the specified file. */
  client_addr_len=sizeof(client_addr);
 fp=fopen("s.avi","wb");
  int length;
 while(length=recvfrom(s, buf, sizeof(buf), 0,(struct sockaddr *)&client_addr, &client_addr_len)){
  //ack_recv=ack;
  ack=buf[0];
  if(strcmp(buf,"BYE")==0){
    printf("BYE\n");
    break;
    }
    printf("packet recieved:%d\n",ack);

  //printf("%d\n",length);
    if (ack==client_seq_no+1)
    {
       sendto(s,&ack,sizeof(uint8_t),0,(struct sockaddr *)&client_addr, client_addr_len);
        long int recv=fwrite(&buf[1],1,50000,fp);
      printf("Ack sent:%d\n",ack);
      client_seq_no+=1;
        if(client_seq_no==255)
        {
        client_seq_no=-1;
        /* code */
        }
      }  
      else{
        sendto(s,&ack,sizeof(uint8_t),0,(struct sockaddr *)&client_addr, client_addr_len);
        //long int recv=fwrite(&buf[1],1,50000,fp);
        printf("Ack didn't reach:%d\n",ack);
      //printf("bYTES WRITTEN %ld\n",recv);
      //printf("%d packet number\n",i);
        i+=1;

      }
  
      

    
    
  }
  fclose(fp);
  
}

