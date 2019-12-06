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
#include <unistd.h>


#define SERVER_PORT 5432
#define BUF_SIZE 4096
#define MaxCharLen 100
#define SWS 7


#pragma pack(1)
struct message{
  long long int random;   // change this too network byte order also for proper platform independent utility
  unsigned long int mLen; // change this to network byte order
  unsigned int num;
  char data [MaxCharLen];
};
#pragma pack(0)
#pragma pack(1)
struct ack_message{
  int Sequence_no;
};
#pragma pack(0)

struct message* PrepTX(){
  char String[256];
  struct message * a = malloc(sizeof(struct message));
  (*a).random = rand();
  scanf(" %s",String);
  (*a).mLen = strlen(String);
  memcpy((*a).data,String,strlen(String));
  (*a).data[(*a).mLen] = '\0';
  return a;
}

int main(int argc, char * argv[]){
  
  struct hostent *hp;
  struct sockaddr_in sin;
  char *host;
  char buf[BUF_SIZE];
  char buf2[BUF_SIZE];
  int s;
  int len;
  struct message* SendingWindow[SWS];
  unsigned int root = 0;
  unsigned int top = 0;
  if ((argc==2)||(argc == 3)) {
    host = argv[1];
  }
  else {
    fprintf(stderr, "usage: client serverIP [download_filename(optional)]\n");
    exit(1);
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
  
  printf("Client will get data from to %s:%d.\n", argv[1], SERVER_PORT);
  printf("To play the music, pipe the downlaod file to a player, e.g., ALSA, SOX, VLC: cat recvd_file.wav | vlc -\n"); 
  
  
  
 socklen_t srv_addr_len;
 srv_addr_len = sizeof(sin);
  /* send message to server */  
    struct timeval tv;
    tv.tv_sec =  1;  /* 30 Secs Timeout */
    tv.tv_usec = 0;  // Not init'ing this can cause strange errors
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

  struct message * ToSend;
  while(1){
    //fgets(buf, sizeof(buf), stdin);
    //buf[BUF_SIZE-1] = '\0';
    //len = strlen(buf) + 1;
    
    // get new elements to sending window
    while(top<root+SWS){
      if(top > SWS)free(SendingWindow[top%SWS]);
      SendingWindow[top%SWS] = PrepTX();
      top ++; 
    }
    int SWS_t = 0;
    while(SWS_t < SWS){
      ToSend = SendingWindow[(root+SWS_t)%SWS];
      (*ToSend).num = SWS_t;
      //printf("%lld , %lu , %s",(*ToSend).random , (*ToSend).mLen , (*ToSend).data);
      unsigned int SS = sizeof(long long int) + sizeof(unsigned long int) +sizeof(int)+ strlen((*ToSend).data);
      if(rand()%10 < 9) sendto(s, ToSend, SS, 0, (struct sockaddr *)&sin, sizeof(sin)) ; // Simulates Packet lost
      SWS_t ++;
      //free(ToSend);
      }
    //printf("%s",buf);
    // this Section used to resend for stop and wait
    //while(recvfrom(s, buf2, sizeof(buf2), 0,(struct sockaddr *)&sin, &srv_addr_len) == -1){
    //  sendto(s, ToSend, SS, 0, (struct sockaddr *)&sin, sizeof(sin)) ;
    //}

    // Accept Responses and update send secction
    SWS_t = 0;
    int ackd;
    int last_Srcv = -1;
    struct ack_message AK;
    while(SWS_t < SWS){
      int Stat = recvfrom(s, buf2, sizeof(buf2), 0,(struct sockaddr *)&sin, &srv_addr_len);
      //printf("%d , %s",Stat,buf2);
      //sscanf(buf2,"%d ACK`d",&ackd);
      memcpy(&AK , buf2, sizeof(AK)) ;
      ackd = AK.Sequence_no;
      printf("----%d\n",ackd);
      
      if(ackd  - last_Srcv != 1){
        printf("Packet Drop ! Last successful Packet = %d \n",last_Srcv);
        root = root + last_Srcv+1;
        printf("%d,root \n",root) ;
        //recieve others and clear
        while(recvfrom(s, buf2, sizeof(buf2), 0,(struct sockaddr *)&sin, &srv_addr_len) != -1);
        SWS_t = SWS;
      }
      else last_Srcv = ackd;
      SWS_t++;
    }
    printf("====>%d,%d\n",root,top);
    // this section updates the send window
    usleep(5000);
  }
  return 0; 
}
