#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

// Função para cuidar do filho
static void TratarFilhos(int signo) {
  pid_t ProcessoFilho;
  int status;

  do {
    ProcessoFilho = waitpid(-1, &status, WNOHANG);
  } while( ProcessoFilho != -1);
  signal(SIGCHLD, TratarFilhos);
}

int main(int argc, char const *argv[])
{
  int SocketFD, lenRemoto;
  struct sockaddr_in end_ip, EndRemoto;
  char buffer[1000];
  pid_t ProcessoFilho;

  // Controle dos filhos
  signal(SIGCHLD, TratarFilhos);

  // Criando socket para domínio IPv4, tipo datagram e protocolo UDP
  SocketFD = socket(PF_INET, SOCK_DGRAM, 0);

  // Verificando erros
  if (SocketFD == -1) {
    perror("Erro");
  }
  printf("Socket criado: %d\n", SocketFD);

  // Configurando endereço
  memset(&end_ip, 0, sizeof end_ip);
  end_ip.sin_family = PF_INET;
  end_ip.sin_port = htons(3000);
  end_ip.sin_addr.s_addr = inet_addr("192.168.43.222");

  // Atribuindo endereço ao socket
  if (bind(SocketFD, (struct sockaddr *) &end_ip, sizeof(struct sockaddr)) == -1) {
    perror("Erro");
  }

  while (1){
    // Esperando mensagem
    int retorno = recvfrom(SocketFD, buffer, sizeof buffer, 0, (struct sockaddr*) &EndRemoto, &lenRemoto);

    // Criando Fork do processo
    ProcessoFilho = fork();
    if (ProcessoFilho == -1) {
      perror("Falha ao criar fork");
      continue;
    }
    else if (ProcessoFilho > 0)
    {
      printf("Processo filho criado\n");
      continue;
    }
    // Tratando requisição e respondendo
    printf("Sou o filho e recebi essa resposta: %s\n", buffer);
    sleep(5);
    retorno = sendto(SocketFD, "Hey", sizeof "Hey", 0, (struct sockaddr *) &EndRemoto, sizeof (struct sockaddr));
    if (retorno == -1) {
      perror("Erro ao responder:");
    }
    // Terminando processo
    exit(0);
  };
  return 0;
}
