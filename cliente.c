#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

int main(int argc, char const *argv[])
{
  int SocketFD, endlen;
  struct sockaddr_in end_ip;
  char buffer[1000];

  SocketFD = socket(PF_INET, SOCK_DGRAM, 0);

  if (SocketFD == -1) {
    perror("Erro");
  }
  printf("Socket criado: %d\n", SocketFD);

  memset(&end_ip, 0, sizeof end_ip);
  end_ip.sin_family = PF_INET;
  end_ip.sin_port = htons(3000);
  end_ip.sin_addr.s_addr = inet_addr("127.0.0.1");
  time_t seconds;

   seconds = time(NULL);

  sendto(SocketFD, "Um Cliente", sizeof "Um Cliente", 0, (struct sockaddr *) &end_ip, sizeof (struct sockaddr));

  int retorno = recvfrom(SocketFD, buffer, sizeof buffer, 0, (struct sockaddr *) &end_ip, &endlen);

  printf("%s\n", buffer);
  printf("Tempo de resposta: %ld segundos\n", time(NULL) - seconds);
  while(1);
  return 0;
}
