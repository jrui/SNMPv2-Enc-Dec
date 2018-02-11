#include <stdio.h>
#include <unistd.h>
#include <decodetypes.h>


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

void print_Data(FILE *fp) {
    //print decoded PDU
}

void decode_Data(char *buffer, int bs) {
    DecodeResult_t decMessage;
    decMessage = decode_Message(buffer, bs);
    if(decMessage.status.consumed != -1) {
        FILE *fp = stdout;
        xer_fprint(fp, &asn_DEF_Message, decMessage.choice.message);
        DecodeResult_t decPDU = decode_PDU(decMessage.choice.message);
        if(decPDU.status.consumed != -1) {
            xer_fprint(fp, &asn_DEF_PDUs, decPDU.choice.pdu);
        } else {
            //need to have warnings like no OID
            perror("decode failed on pdu");
            //this needs to be better
        }
    } else perror("decode failed on message");
}

void read_from_file(char *fileName) {
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
  int recv = recvfrom(sock, buffer, buffer_size, 0,
            (struct sockaddr *)&addr, &udp_socket_size);

  decode_Data(buffer, recv);
}
