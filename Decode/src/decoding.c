#include <stdio.h>
#include <unistd.h>

#include <decodetypes.h>
#include <pduparser.h>
#include <INTEGER.h>

int menu();
void read_from_file(char *fileName);
void read_from_udp(int port);
char* parseOID(uint8_t *oid);
void print_var_bind(ParsedVarBind_t vb);
void print_message_info(Message_t *msg);
void print_data_pdu(Message_t *msg, ParsedPdu_t *parsedPDU, char *type);
void print_data_bulk_pdu(Message_t *msg, ParsedBulkPdu_t *parsedPDU);


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

void parse_pdu(Message_t *msg, PDUs_t *pdu) {
    ParsedPdu_t *parsed = NULL;
    ParsedBulkPdu_t *parsedBulk = NULL;
    switch (pdu -> present) {
        case PDUs_PR_get_request:
            parsed = parse_GetRequest(&(pdu -> choice.get_request));
            print_data_pdu(msg, parsed, "get-request");
            break;
    	case PDUs_PR_get_next_request:
            parsed = parse_GetNextRequest(&(pdu -> choice.get_next_request));
            print_data_pdu(msg, parsed, "get-next-request");
            break;
    	case PDUs_PR_get_bulk_request:
            parsedBulk = parse_GetBulkRequest(&(pdu -> choice.get_bulk_request));
            print_data_bulk_pdu(msg, parsedBulk);
            break;
    	case PDUs_PR_response:
            parsed = parse_Response(&(pdu -> choice.response));
            print_data_pdu(msg, parsed, "response");
            break;
    	case PDUs_PR_set_request:
            parsed = parse_SetRequest(&(pdu -> choice.set_request));
            print_data_pdu(msg, parsed, "set-request");
            break;
    	case PDUs_PR_inform_request:
            parsed = parse_InformRequest(&(pdu -> choice.inform_request));
            print_data_pdu(msg, parsed, "inform-request");
            break;
    	case PDUs_PR_snmpV2_trap:
            parsed = parse_SnmpV2Trap(&(pdu -> choice.snmpV2_trap));
            print_data_pdu(msg, parsed, "snmpV2-trap");
            break;
    	case PDUs_PR_report:
            parsed = parse_Report(&(pdu -> choice.report));
            print_data_pdu(msg, parsed, "report");
            break;
    }
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
            parse_pdu(decMessage.choice.message, decPDU.choice.pdu);
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

void print_var_bind(ParsedVarBind_t vb) {
    INTEGER_t *integer;
    long value; unsigned long u_value;
    char *string;
    // vb -> oid;
    printf("Object oid: %s\n", parseOID(vb.oid));
    switch (vb.type) {
        case NOTHING:
            printf("value: NOTHING\n");
            break;
        case INTEGER:
            value = vb.choice.integer_value;
            printf("value: %d\n", value);
            break;
        case STRING:
            string = vb.choice.string_value;
            printf("value: %s\n", string);
            break;
        case OBJECT_ID:
            //not done
            printf("value: object_id\n");
            break;
        case IP_ADDRESS:
            string = (char *) vb.choice.ipAddress_value -> buf;
            printf("value: %s\n", string);
            break;
        case COUNTER_32:
            value = vb.choice.counter_32_value;
            printf("value: %d\n", value);
            break;
        case COUNTER_64:
            integer = (INTEGER_t *) vb.choice.counter_64_value;
            asn_INTEGER2long(integer, &value);
            printf("value: %d\n", value);
            break;
        case TIMETICKS:
            value = vb.choice.timeticks_value;
            printf("value: %d\n", value);
            break;
        case OPAQUE:
            string = vb.choice.opaque;
            printf("value: %s\n", string);
            break;
        case UNSIGNED_INT_32:
            u_value = vb.choice.unsigned_int_value;
            printf("value: %d", u_value);
            break;
    }
}

char* parseOID(uint8_t *oid) {
    //not done
    return (char*) oid;
}

void print_message_info(Message_t *msg) {
    char *string = (char *) msg -> community.buf;
    printf("Version: %d\n", msg -> version);
    printf("Community string: %s\n", string);
}

void print_data_pdu(Message_t *msg, ParsedPdu_t *parsedPDU, char *type) {
    ParsedVarBind_t *var_bindings = parsedPDU -> variable_bindings;
    int size = parsedPDU -> size, i;
    print_message_info(msg);
    printf("%s\n", type);
    printf("Objects:\n");
    for(i = 0; i < size; i++) {
        print_var_bind(var_bindings[i]);
    }
}

void print_data_bulk_pdu(Message_t *msg, ParsedBulkPdu_t *parsedPDU) {
    ParsedVarBind_t *var_bindings = parsedPDU -> variable_bindings;
    int size = parsedPDU -> size, i;
    print_message_info(msg);
    printf("get-bulk-request\n");
    printf("nr=%d ", parsedPDU -> non_repeaters);
    printf("mr=%d ", parsedPDU -> max_repetitions);
    printf("\nObjects:\n");
    for(i = 0; i < size; i++) {
        print_var_bind(var_bindings[i]);
    }
}
