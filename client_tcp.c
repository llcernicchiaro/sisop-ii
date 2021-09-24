#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 4000

int main(int argc, char *argv[])
{
  int sockfd, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char username[40];

  char buffer[256];
  if (argc < 4)
  {
    fprintf(stderr, "usage %s profile hostname port\n", argv[0]);
    exit(0);
  }

  server = gethostbyname(argv[2]);
  if (server == NULL)
  {
    fprintf(stderr, "ERROR, no such host\n");
    exit(0);
  }

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    printf("ERROR opening socket\n");

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
  bzero(&(serv_addr.sin_zero), 8);

  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    printf("ERROR connecting\n");

  printf("Usage (FOLLOW @user | SEND message to send): ");
  bzero(buffer, 256);
  fgets(buffer, 256, stdin);

  char str2[256];
  strcpy(str2, buffer);

  char *command = strtok(str2, " ");
  printf("%s\n", buffer);

  if (strcmp(command, "FOLLOW") == 0)
  {
    printf("You want to follow: %s\n", buffer);
  }
  else if (strcmp(command, "SEND") == 0)
  {
    printf("You want to send the message: %s\n", buffer);
  }
  else
  {
    printf("Invalid command, please try again\n");
    return 0;
  }

  /* write in the socket */
  n = write(sockfd, buffer, strlen(buffer));
  if (n < 0)
    printf("ERROR writing to socket\n");

  bzero(buffer, 256);

  /* read from the socket */
  n = read(sockfd, buffer, 256);
  if (n < 0)
    printf("ERROR reading from socket\n");

  printf("%s\n", buffer);

  close(sockfd);
  return 0;
}