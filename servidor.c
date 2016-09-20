#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char const *argv[])
{
  int SocketFD, lenRemoto;
  struct sockaddr_in end_ip, EndRemoto;
  char buffer[1000];

  SocketFD = socket(PF_INET, SOCK_DGRAM, 0);

  if (SocketFD == -1) {
    perror("Erro");
  }
  printf("Socket criado: %d\n", SocketFD);
  printf("Iniciando servidor\n");

  memset(&end_ip, 0, sizeof end_ip);
  end_ip.sin_family = PF_INET;
  end_ip.sin_port = htons(3000);
  end_ip.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (bind(SocketFD, (struct sockaddr *) &end_ip, sizeof(struct sockaddr)) == -1) {
    perror("Erro");
  }


  while (1){
    int retorno = recvfrom(SocketFD, buffer, sizeof buffer, 0, (struct sockaddr*) &EndRemoto, &lenRemoto);
    printf("%s\n", buffer);
    // printf("%s\n", inet_ntoa(EndRemoto.sin_addr));
    sleep(5);
    sendto(SocketFD, "Hey", sizeof "Hey", 0, (struct sockaddr *) &EndRemoto, sizeof (struct sockaddr));
    if (retorno == -1) {
      perror("ERro:");
    }
  };
  return 0;
}
