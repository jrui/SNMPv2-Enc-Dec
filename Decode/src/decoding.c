#include <stdio.h>
#include <unistd.h>

#include <decodetypes.h>
#include <pduparser.h>
#include <INTEGER.h>

int menu();
int menuResult();
FILE* getOutPointer();
void read_from_file(char *fileName, FILE *fp);
void read_from_udp(int port, FILE *fp);
int* parseOID(ObjectName_t oid);
void print_var_bind(ParsedVarBind_t vb, FILE *fp);
void print_message_info(Message_t *msg, FILE *fp);
void print_data_pdu(Message_t *msg, ParsedPdu_t *parsedPDU,
                    char *type, FILE *fp);
void print_data_bulk_pdu(Message_t *msg, ParsedBulkPdu_t *parsedPDU, FILE *fp);


int main(int argc, char const *argv[]) {
  int opt, port = 9999;
  char *fileName = malloc(128);
  FILE *fp;
  while((opt = menu()) != 0) {
    switch(opt) {
      case 1:
        printf("File:\n");
        //this is not good, using just for this test application
        scanf("%s", fileName);
        fileName[strlen(fileName)] = '\0';
        fp = getOutPointer();
        read_from_file(fileName, fp);
        break;
      case 2:
        fp = getOutPointer();
        printf("Reading Socket on port %d\n\n", port);
        read_from_udp(port, fp);
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

int menuResult() {
  int opt;
  printf("[1] - Result to File\n");
  printf("[2] - Result to StdOut\n");
  printf("\n\nOption:  ");
  scanf("%d", &opt);
  return opt;
}

FILE* getOutPointer() {
    int opt;
    int status = 1;
    char *fileName = malloc(128);
    FILE *res;
    while(status) {
        opt = menuResult();
        switch (opt) {
            case 1:
                printf("File:\n");
                //this is not good, using just
                //for this test application
                scanf("%s", fileName);
                fileName[strlen(fileName)] = '\0';
                res = fopen(fileName, "w");
                if(!res)
            		printf("Unable to open file! Try again\n");
                else status = 0;
                break;
            case 2:
                res = stdout;
                status = 0;
                break;
            default:
                res = stdout;
                status = 0;
        }
    }
    return res;
}

void parse_pdu(Message_t *msg, PDUs_t *pdu, FILE *fp) {
    ParsedPdu_t *parsed = NULL;
    ParsedBulkPdu_t *parsedBulk = NULL;
    switch (pdu -> present) {
        case PDUs_PR_get_request:
            parsed = parse_GetRequest(&(pdu -> choice.get_request));
            print_data_pdu(msg, parsed, "get-request", fp);
            break;
    	case PDUs_PR_get_next_request:
            parsed = parse_GetNextRequest(&(pdu -> choice.get_next_request));
            print_data_pdu(msg, parsed, "get-next-request", fp);
            break;
    	case PDUs_PR_get_bulk_request:
            parsedBulk = parse_GetBulkRequest(&(pdu -> choice.get_bulk_request));
            print_data_bulk_pdu(msg, parsedBulk, fp);
            break;
    	case PDUs_PR_response:
            parsed = parse_Response(&(pdu -> choice.response));
            print_data_pdu(msg, parsed, "response", fp);
            break;
    	case PDUs_PR_set_request:
            parsed = parse_SetRequest(&(pdu -> choice.set_request));
            print_data_pdu(msg, parsed, "set-request", fp);
            break;
    	case PDUs_PR_inform_request:
            parsed = parse_InformRequest(&(pdu -> choice.inform_request));
            print_data_pdu(msg, parsed, "inform-request", fp);
            break;
    	case PDUs_PR_snmpV2_trap:
            parsed = parse_SnmpV2Trap(&(pdu -> choice.snmpV2_trap));
            print_data_pdu(msg, parsed, "snmpV2-trap", fp);
            break;
    	case PDUs_PR_report:
            parsed = parse_Report(&(pdu -> choice.report));
            print_data_pdu(msg, parsed, "report", fp);
            break;
    }
}

void decode_Data(char *buffer, int bs, FILE *fp) {
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
            parse_pdu(decMessage.choice.message, decPDU.choice.pdu, fp);
        } else perror("decode failed on pdu");
    } else perror("decode failed on message");
}

void read_from_file(char *fileName, FILE *fp) {
    char buffer[1024];
    int buffer_size = 1024, i = 0;
    FILE *ptr;
    ptr = fopen(fileName, "rb");
    if(!ptr) {
		printf("Unable to open file!");
		return;
	}
    while(fread(&(buffer[i++]), 1, 1, ptr) && i < 1024);
    decode_Data(buffer, i, fp);
}


void read_from_udp(int port, FILE *fp) {
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

  decode_Data(buffer, recv, fp);
}

void print_var_bind(ParsedVarBind_t vb, FILE *fp) {
    INTEGER_t integer;
    long value; unsigned long u_value;
    int i;
    char *string;
    // vb -> oid;
    fprintf(fp, "Object oid: ");
    int *oid = parseOID(vb.oid);
    for(i = 0; i < vb.oid.size; i++) {
        fprintf(fp, "%d", oid[i]);
        if(i != vb.oid.size - 1)
            fprintf(fp, ".");
    }
    fprintf(fp, "\n");
    switch (vb.type) {
        case UNSPECIFIED:
            fprintf(fp, "value: unSpecified\n");
            break;
        case INTEGER:
            value = vb.choice.integer_value;
            fprintf(fp, "value: %d\n", value);
            break;
        case STRING:
            string = vb.choice.string_value;
            fprintf(fp, "value: %s\n", string);
            break;
        case OBJECT_ID:
            //not done
            fprintf(fp, "value: ");
            int *oid = parseOID(vb.choice.object_id_value);
            for(i = 0; i < vb.oid.size; i++) {
                fprintf(fp, "%d", oid[i]);
                if(i != vb.oid.size - 1)
                    fprintf(fp, ".");
            }
            fprintf(fp, "\n");
            break;
        case IP_ADDRESS:
            string = (char *) vb.choice.ipAddress_value.buf;
            fprintf(fp, "value: %s\n", string);
            break;
        case COUNTER_32:
            value = vb.choice.counter_32_value;
            fprintf(fp, "value: %d\n", value);
            break;
        case COUNTER_64:
            integer = (INTEGER_t) vb.choice.counter_64_value;
            asn_INTEGER2long(&integer, &value);
            fprintf(fp, "value: %d\n", value);
            break;
        case TIMETICKS:
            value = vb.choice.timeticks_value;
            fprintf(fp, "value: %d\n", value);
            break;
        case OPAQUE:
            string = vb.choice.opaque;
            fprintf(fp, "value: %s\n", string);
            break;
        case UNSIGNED_INT_32:
            u_value = vb.choice.unsigned_int_value;
            fprintf(fp, "value: %d", u_value);
            break;
    }
}

int* parseOID(ObjectName_t oid) {
    int i;
    int *res = malloc(sizeof(int) * oid.size);
    for(i = 0; i < oid.size; i++)
        res[i] = (int) oid.buf[i];
    return res;
}

void print_message_info(Message_t *msg, FILE *fp) {
    char *string = (char *) msg -> community.buf;
    fprintf(fp, "Version: %d\n", msg -> version);
    fprintf(fp, "Community string: %s\n", string);
}

void print_data_pdu(Message_t *msg, ParsedPdu_t *parsedPDU,
                    char *type, FILE *fp) {
    ParsedVarBind_t *var_bindings = parsedPDU -> variable_bindings;
    int size = parsedPDU -> size, i;
    print_message_info(msg, fp);
    fprintf(fp, "%s\n", type);
    fprintf(fp, "Objects:\n");
    for(i = 0; i < size; i++) {
        print_var_bind(var_bindings[i], fp);
    }
}

void print_data_bulk_pdu(Message_t *msg, ParsedBulkPdu_t *parsedPDU, FILE *fp) {
    ParsedVarBind_t *var_bindings = parsedPDU -> variable_bindings;
    int size = parsedPDU -> size, i;
    print_message_info(msg, fp);
    fprintf(fp, "get-bulk-request\n");
    fprintf(fp, "nr=%d ", parsedPDU -> non_repeaters);
    fprintf(fp, "mr=%d ", parsedPDU -> max_repetitions);
    fprintf(fp, "\nObjects:\n");
    for(i = 0; i < size; i++) {
        print_var_bind(var_bindings[i], fp);
    }
}
