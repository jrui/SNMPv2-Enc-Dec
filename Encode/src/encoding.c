#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <bindingtypes.h>
#include <complextypes.h>
#include <objecttypes.h>
#include <pdutypes.h>
#include <simpletypes.h>
#include <encodetypes.h>

#define CLEAR() printf("\e[1;1H\e[2J");
long pdu_id = 0;

void userEncodeMethod_Menu();
void userRequest_Menu();
asn_enc_rval_t read_MessageInfo(uint8_t *buff, int opt);
asn_enc_rval_t get_Message(int opt, char *s, int version, uint8_t *buff);
asn_enc_rval_t get_EncodedPDU(int opt, uint8_t *b);
VarBindList_t *get_VarLists(int opt);
VarBind_t *select_VarBind_Type(int opt, ObjectName_t *obj_name);
void user_objectTypeMenu();
ObjectSyntax_t *select_Syntax_Type(int opt);
void user_valueType_Menu();
void user_write_Menu();


int main(int argc, char const *argv[]) {
  int opt;
  asn_enc_rval_t error;
  uint8_t *buff = malloc(BUFF_SIZE);
  userEncodeMethod_Menu();
  scanf("%d", &opt);

  switch(opt) {
    case 1:
      //Produce packet from cli
      error = read_MessageInfo(buff, opt);
      break;
    case 2:
      //Read packet from file
      CLEAR();
      printf("UNSUPORTED\n");
      break;
    case 0:
    default:
      printf("Exiting!\n");
      break;
  }

  int port; const char ip[15];
  char *string = malloc(BUFF_SIZE);
  user_write_Menu();
  scanf("%d", &opt);
  switch(opt) {
    case 1:
      printf("Please insert filename: ");
      scanf("%s", string);
      FILE *fp = fopen(string, "wb");
      fwrite(buff, 1, strlen(buff), fp);
      fclose(fp);
      break;
    case 2:
      printf("Please insert target IP: ");    scanf("%s", &ip);
      printf("Please insert target Port: ");  scanf("%d", &port);
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip);
      int sock = socket(AF_INET, SOCK_DGRAM, 0);
      socklen_t udp_socket_size = sizeof(addr);
      int sent = sendto(sock, buff, strlen(buff)+1, 0, (struct sockaddr *)&addr, udp_socket_size);
      break;
    case 3:
      printf("%s, %d\n", buff, strlen(buff));
      printf("%X\n", buff);
      break;
  }
  return 0;
}


void userEncodeMethod_Menu() {
  CLEAR();
  printf("[1] - Produce encode packet from CLI\n");
  printf("[2] - Encode packet from file [WorkInProgress]\n");
  printf("[0] - Exit\n\n");
  printf("Option: ");
}


asn_enc_rval_t read_MessageInfo(uint8_t *buf, int opt) {
  asn_enc_rval_t error;
  switch(opt) {
    case 1:
      printf("Community String: ");
      char *s = malloc(BUFF_SIZE);
      scanf("%s", s);
      printf("Protocol Version (Only v2 Supported) [2]: ");
      int version;
      scanf("%d", &version);

      error = get_Message(opt, s, version, buf);
      break;
    case 2:
      CLEAR();
      printf("UNSUPORTED\n");
      break;
  }
  return error;
}


asn_enc_rval_t get_Message(int opt, char *cs, int v, uint8_t *buff) {
  ANY_t *data;
  uint8_t *b = malloc(sizeof(uint8_t) * BUFF_SIZE);
  asn_enc_rval_t pdus;

  switch(opt) {
    case 1:
      pdus = get_EncodedPDU(opt, b);
      break;
    case 2:
    //LER DE FICHEIRO
      break;
  }

  if(pdus.encoded >= 0) {
    data = create_ANY(b, pdus.encoded);

    asn_enc_rval_t ret = encode_Message(cs, v, data, buff);
    if(ret.failed_type != RC_OK) return ret;
  }
  return pdus;
}


asn_enc_rval_t get_EncodedPDU(int opt, uint8_t *b) {
  asn_enc_rval_t ret;
  PDU_t *pdu;
  VarBindList_t *varlist;

  switch(opt) {
    case 1:
      //Selecionar tipo de PDU, Nothing, GetRequest,...
      userRequest_Menu();
      int opt2;
      scanf("%d", &opt2);
      varlist = get_VarLists(opt);
      switch(opt2) {
        case 1: pdu = (PDU_t *) create_GetRequestPDU(pdu_id++,0,0,varlist);
                ret = encode_PDUs_GetRequest((GetRequest_PDU_t *) pdu, b);
                break;
        case 2: pdu = (PDU_t *) create_GetNextRequestPDU(pdu_id++,0,0,varlist);
                ret = encode_PDUs_GetNextRequest((GetNextRequest_PDU_t *) pdu, b);
                break;
        case 3: printf("Number of non repeaters: ");
                long nr = 0, mr = 0;
                scanf("%d", &nr);
                printf("Number of maximum repetitions: ");
                scanf("%d", &mr);
                pdu = (PDU_t *) create_GetBulkRequestPDU(pdu_id++,nr,mr,varlist);
                ret = encode_PDUs_GetBulkRequest((GetBulkRequest_PDU_t *) pdu, b);
                break;
        case 4: pdu = (PDU_t *) create_ResponsePDU(pdu_id++,0,0,varlist);
                ret = encode_PDUs_Response((Response_PDU_t *) pdu, b);
                break;
        case 5: pdu = (PDU_t *) create_SetRequestPDU(pdu_id++,0,0,varlist);
                ret = encode_PDUs_SetRequest((SetRequest_PDU_t *) pdu, b);
                break;
        case 6: pdu = (PDU_t *) create_InformRequestPDU(pdu_id++,0,0,varlist);
                ret = encode_PDUs_InformRequest((InformRequest_PDU_t *) pdu, b);
                break;
        case 7: pdu = (PDU_t *) create_SNMPv2TrapPDU(pdu_id++,0,0,varlist);
                ret = encode_PDUs_SNMPV2Trap((SNMPv2_Trap_PDU_t *) pdu, b);
                break;
        case 8: pdu = (PDU_t *) create_ReportPDU(pdu_id++,0,0,varlist);
                ret = encode_PDUs_Report((Report_PDU_t *) pdu, b);
                break;
      }
      break;
    case 2:
    //LER DE FICHEIRO
      break;
  }

  return ret;
}


void userRequest_Menu() {
  CLEAR();
  printf("Select type of PDU\n");
  printf("[1] - GetRequest\n");
  printf("[2] - GetNextRequest\n");
  printf("[3] - GetBulkRequest\n");
  printf("[4] - Response\n");
  printf("[5] - SetRequest\n");
  printf("[6] - InformRequest\n");
  printf("[7] - SNMPv2Trap\n");
  printf("[8] - Report\n\n");
  printf("Option: ");
}


VarBindList_t *get_VarLists(int opt) {
  VarBindList_t *ret;
  char *obj_name_str = malloc(BUFF_SIZE);
  uint8_t *obj_name_byte = malloc(BUFF_SIZE);
  int i = 0, first = 1;
  ObjectName_t *obj_name;
  char opt2[10] = "Y";

  switch(opt) {
    case 1:
      while(opt2[0] == 'Y' || opt2[0] == 'y') {
        CLEAR();
        printf("Insert the OID [1.2.5]: ");
        scanf("%s", obj_name_str);
        char *temp = strtok(obj_name_str, ".");
        while(temp != NULL) {
            obj_name_byte[i++] = atoi(temp);
            temp = strtok(NULL, ".");
        }
        obj_name = create_ObjectName(obj_name_byte, (size_t) i);
        VarBind_t *var_bind = select_VarBind_Type(opt, obj_name);
        if(first) {
          ret = create_VarBindList(var_bind);
          first = 0;
        }
        else ASN_SEQUENCE_ADD(&ret->list, var_bind);
        i = 0;
        obj_name_str = malloc(BUFF_SIZE);
        obj_name_byte = malloc(BUFF_SIZE);
        printf("Add new Object? [Y/N]: ");
        scanf("%s", opt2);
      }
      break;
    case 2:
      break;
  }
  return ret;
}


VarBind_t *select_VarBind_Type(int opt, ObjectName_t *obj_name) {
  VarBind_t *ret;
  int opt2, value;
  ObjectSyntax_t *obj_syntax;

  switch(opt) {
    case 1:
      user_objectTypeMenu();
      scanf("%d", &opt2);
        switch(opt2) {
          case 1:
            obj_syntax = select_Syntax_Type(opt);
            ret = create_VarBind_Value(obj_name, obj_syntax);
            break;
          case 2:
            printf("Insert value: ");
            scanf("%d", &value);
            ret = create_VarBind_unSpecified(obj_name, (NULL_t) value);
            break;
          case 3:
            printf("Insert value: ");
            scanf("%d", &value);
            ret = create_VarBind_noSuchObject(obj_name, (NULL_t) value);
            break;
          case 4:
            printf("Insert value: ");
            scanf("%d", &value);
            ret = create_VarBind_noSuchInstance(obj_name, (NULL_t) value);
            break;
          case 5:
            printf("Insert value: ");
            scanf("%d", &value);
            ret = create_VarBind_endOfMibView(obj_name, (NULL_t) value);
            break;
          case 6:
            ret = create_VarBind_Nothing(obj_name);
            break;
        }
      break;
    case 2:
    //LER DE FICHEIRO
      break;
  }

  return ret;
}


void user_objectTypeMenu() {
  CLEAR();
  printf("Select object type:\n[1] - Value\n");
  printf("[2] - UnSpecified\n");
  printf("[3] - noSuchObject\n");
  printf("[4] - noSuchInstance\n");
  printf("[5] - endOfMibView\n");
  printf("[6] - Nothing\n\nOption: ");
}


ObjectSyntax_t *select_Syntax_Type(int opt) {
  ApplicationSyntax_t *apps;
  SimpleSyntax_t *sst;
  ObjectSyntax_t *ret;
  int opt2, integer;
  char string[1024];

  switch(opt) {
    case 1:
      user_valueType_Menu();
      scanf("%d", &opt2);
      switch(opt2) {
        case 1:
          printf("Insert integer value: ");
          scanf("%d", &integer);
          sst = create_SimpleInteger(integer);
          ret = create_ObjSynt_Simple(sst);
          break;
        case 2:
          printf("Insert string value: ");
          scanf("%s", string);
          sst = create_SimpleString(string);
          ret = create_ObjSynt_Simple(sst);
          break;
        case 3:
          printf("Insert IpAddress [10.0.2.1]: ");
          scanf("%s", string);
          apps = create_IpAddress(string);
          ret = create_ObjSynt_Application(apps);
          break;
        case 4:
          printf("Insert CounterValue: ");
          scanf("%d", &integer);
          apps = create_CounterValue(integer);
          ret = create_ObjSynt_Application(apps);
          break;
        case 5:
          printf("Insert Unsigned32: ");
          scanf("%d", &integer);
          apps = create_Unsigned32(integer);
          ret = create_ObjSynt_Application(apps);
          break;
        case 6:
          printf("Insert Opaque value: ");
          scanf("%s", string);
          apps = create_Opaque(string);
          ret = create_ObjSynt_Application(apps);
          break;
        case 7:
          apps = create_Nothing();
          ret = create_ObjSynt_Application(apps);
      }
      break;
    case 2:
      //LER DE FICHEIRO
      break;
  }
  return ret;
}


void user_valueType_Menu() {
  CLEAR();
  printf("Select value type:\n");
  printf("[1] - Integer\n");
  printf("[2] - String\n");
  printf("[3] - IpAddress\n");
  printf("[4] - CounterValue\n");
  printf("[5] - Unsigned32\n");
  printf("[6] - Opaque\n");
  printf("[7] - Nothing\n\nOption: ");
}


void user_write_Menu() {
  CLEAR();
  printf("Where to save PDU?\n");
  printf("[1] - File\n");
  printf("[2] - UDP agent\n");
  printf("[3] - Console in Hex\n");
  printf("[0] - Exit\n\nOption: ");
}
