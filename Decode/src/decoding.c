#include <stdio.h>
#include <unistd.h>

#include <decodetypes.h>
#include <pduparser.h>

int menu();
void read_from_file(char *fileName);
void read_from_udp(int port);


int main(int argc, char const *argv[]) {
  int opt, port = 9999;
  char *fileName = malloc(128);
  while((opt = menu()) != 0) {
    switch(opt) {
      case 1:
        printf("File:\n");
        //this is not good, using just for this test application
        scanf("%s", fileName);
        read_from_file(fileName);
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

void print_Data(ParsedPdu_t *parsedPDU) {
    //print decoded PDU
}

void parse_pdu(PDUs_t *pdu) {
    ParsedPdu_t *parsed = NULL;
    ParsedBulkPdu_t *parsedBulk = NULL;
    switch (pdu -> present) {
        case PDUs_PR_get_request:
            parsed = parse_GetRequest(&(pdu -> choice.get_request));
            break;
    	case PDUs_PR_get_next_request:
            parsed = parse_GetNextRequest(&(pdu -> choice.get_next_request));
            break;
    	case PDUs_PR_get_bulk_request:
            parsedBulk = parse_GetBulkRequest(&(pdu -> choice.get_bulk_request));
            break;
    	case PDUs_PR_response:
            parsed = parse_Response(&(pdu -> choice.response));
            break;
    	case PDUs_PR_set_request:
            parsed = parse_SetRequest(&(pdu -> choice.set_request));
            break;
    	case PDUs_PR_inform_request:
            parsed = parse_InformRequest(&(pdu -> choice.inform_request));
            break;
    	case PDUs_PR_snmpV2_trap:
            parsed = parse_SnmpV2Trap(&(pdu -> choice.snmpV2_trap));
            break;
    	case PDUs_PR_report:
            parsed = parse_Report(&(pdu -> choice.report));
            break;
    }
    if(parsed == NULL) {
        if(parsedBulk == NULL);
        else {}
    }
    else {}
}

void decode_Data(char *buffer, int bs) {
    DecodeResult_t decMessage;
    decMessage = decode_Message(buffer, bs);
    if(decMessage.status.consumed != -1) {
        //DEBUGGING
        //FILE *fp = stdout;
        //xer_fprint(fp, &asn_DEF_Message, decMessage.choice.message);
        DecodeResult_t decPDU = decode_PDU(decMessage.choice.message);
        if(decPDU.status.consumed != -1) {
            //DEBUGGING
            //xer_fprint(fp, &asn_DEF_PDUs, decPDU.choice.pdu);
            parse_pdu(decPDU.choice.pdu);
        } else perror("decode failed on pdu");
    } else perror("decode failed on message");
}

void read_from_file(char *fileName) {
    unsigned char buffer[1024];
    int buffer_size = 1024, recv;
    FILE *ptr;
    ptr = fopen(fileName, "rb");
    recv = fread(buffer, buffer_size, 1, ptr);
    decode_Data(buffer, recv);
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
