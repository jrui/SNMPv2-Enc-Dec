#include <stdio.h>
#include <unistd.h>
#include <ApplicationSyntax.h>
#include <Message.h>
#include <PDUs.h>
#include <SimpleSyntax.h>


int menu();
void read_from_file();
void read_from_udp(int port);


int main(int argc, char const *argv[]) {
  int opt, port = 9999;
  while((opt = menu()) != 0) {
    switch(opt) {
      case 1:
        printf("UNSUPORTED\n");
        //read_from_file();
        break;
      case 2:
        printf("Reading Socket on port %d\n\n", port);
        read_from_udp(port);
        break;
      default:
        break;
    }
    sleep(3);
    printf("\e[1;1H\e[2J");
  }
  return 0;
}


int menu() {
  int opt;
  printf("[1] - Read from File\n");
  printf("[2] - Read from UDP\n");
  printf("[0] - Exit\n\nOption:  ");
  scanf("%d", &opt);
  return opt;
}


void read_from_file() {
  //...
}


void read_from_udp(int port) {
  char buffer[1024], buff[128];
  int buffer_size = 1024;
  struct sockaddr_in addr;

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  socklen_t udp_socket_size = sizeof(addr);
  bind(sock, (struct sockaddr *)&addr, udp_socket_size);
  int recv = recvfrom(sock, buffer, buffer_size, 0, (struct sockaddr *)&addr, &udp_socket_size);

  Message_t *message = 0;
  asn_dec_rval_t rval = asn_decode(0, ATS_BER, &asn_DEF_Message,
      (void **) &message, buffer, recv);

  FILE *fp = stdout;
  xer_fprint(fp, &asn_DEF_Message, message);
  if(rval.consumed == -1) {
    snprintf(buff, 128, "%d\0", rval.code);
    strcpy(buff, strcat("Error decoding, code = ", buff));
    printf("%s\n", buff);
  }
  else {
    PDUs_t *pdu = 0;
    rval = asn_decode(0, ATS_BER, &asn_DEF_PDUs, (void **) &pdu,
        message->data.buf, message->data.size);
    if(rval.consumed == -1) {
      snprintf(buff, 128, "%d\0", rval.code);
      strcpy(buff, strcat("Error decoding, code = ", buff));
      printf("%s\n", buff);
    }
    else xer_fprint(fp, &asn_DEF_PDUs, pdu);
  }
}
