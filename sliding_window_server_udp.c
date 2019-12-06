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
 #include <arpa/inet.h>

#define SERVER_PORT 5432
#define BUF_SIZE 4096
#define MaxCharLen 100
#define RWS 7
#pragma pack(1)
struct message{
  long long int random;
  unsigned long int mLen;
  unsigned int num;
  char data[MaxCharLen];
};
#pragma pack(0)
#pragma pack(1)
struct ack_message{
  int Sequence_no;
};
#pragma pack(0)
int unpack_fileData(struct message * RX){
printf("Seq = %u ,Rand = %lld , data lenght =  %lu ,data = ",(*RX).num,(*RX).random , (*RX).mLen );
fwrite((*RX).data, (*RX).mLen , sizeof(char),stdout);
printf("\n");
}

int main(int argc, char * argv[]){
  
  struct sockaddr_in sin;
  struct sockaddr_storage client_addr;
  char clientIP[INET_ADDRSTRLEN]; /* For IPv4 addresses */
  socklen_t client_addr_len;
  
  char buf[BUF_SIZE];
  char PrevMsg[BUF_SIZE];
  struct ack_message AckMsg;
  

  int len;
  int s;
  char *host;
  struct hostent *hp;

  /* Create a socket */
  if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("server: socket");
    exit(1);
  }
 
 
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
    
  
  client_addr_len = sizeof(client_addr);
  int i=0;
  int pk=0;
  /* Receive messages from clients*/
while(1){
  len = recvfrom(s, buf, sizeof(buf), 0,(struct sockaddr *)&client_addr, &client_addr_len);
  struct message RX;
  memcpy(&RX , buf, sizeof(RX)) ;
  inet_ntop(client_addr.ss_family,&(((struct sockaddr_in *)&client_addr)->sin_addr),clientIP, INET_ADDRSTRLEN);
  //unpack_fileData(&RX);
  //printf("%d : Bytes of  => ,%lld , %lu , %s\n",len,RX.random , RX.mLen , RX.data);
  //printf("Server got message from %s: %s [%d bytes]\n", clientIP, buf, len);
  if((pk%RWS) == RX.num){
    printf("%d : Bytes of  => ,%lld , %lu \n",len,RX.random ,RX.mLen);
    fwrite(RX.data,2, sizeof(char),stdout);
    pk++;
  }
  if(strcmp(PrevMsg,buf)!=0) memcpy(PrevMsg,buf,sizeof(buf));  
  else printf("Recieved a Repeat TX");
  // this Section Simulates a Bad Connection :P
  // with the ack loat setup 
  //sprintf(AckMsg,"%d ACK`d %d  \n",RX.num,i);
  AckMsg.Sequence_no = RX.num;
  if(rand()%10 < 9)sendto(s, &AckMsg, sizeof(struct ack_message), 0,(struct sockaddr *)&client_addr, client_addr_len) ;
  i++;
  }
}

