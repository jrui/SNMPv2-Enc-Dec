#include <stdio.h>
#include <ANY.h>
#include <Message.h>
#include <ObjectName.h>
#include <ObjectSyntax.h>
#include <OCTET_STRING.h>
#include <PDU.h>
#include <PDUs.h>
#include <SetRequest-PDU.h>
#include <SimpleSyntax.h>
#include <VarBind.h>
#include <VarBindList.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//Test includes
#include <bindingtypes.h>
#include <complextypes.h>
#include <objecttypes.h>
#include <pdutypes.h>
#include <simpletypes.h>



int main(int argc, char const *argv[]) {
  SimpleSyntax_t *simple = createSimpleInteger((long) 10);
  ObjectSyntax_t *object_syntax_s = create_ObjSynt_Simple(simple);

  ApplicationSyntax_t *application_s = createIpAddress("127.0.0.1");
  ObjectSyntax_t *object_syntax_a = create_ObjSynt_Application(application_s);


  ObjectName_t *object_name = create_ObjectName("1.0.2", 5);
  VarBind_t *var_bind = create_VarBind_Value(object_name, object_syntax_a);
  VarBindList_t *varlist = create_VarBindList(var_bind);
  SetRequest_PDU_t *setRequestPDU = create_SetRequestPDU(1, 0, 0, varlist);


//FALTA IMPLEMENTAR PDUs_t


  //present -> indica o tipo do CPU
  PDUs_t *pdu;
  pdu = calloc(1, sizeof(PDUs_t));
  pdu->present = PDUs_PR_set_request;
  pdu->choice.set_request = *setRequestPDU;

  //buffer -> guardada a mensagem
  //ret.encoded -> nymero de bytes, -1 para erro
  //ret.failed_type -> tipos que nao foram codificados
  uint8_t *buffer = malloc(sizeof(uint8_t) * 1024);
  size_t buffer_size = 1024;
  asn_enc_rval_t ret = asn_encode_to_buffer(0, ATS_BER, &asn_DEF_PDUs, pdu, buffer, buffer_size);

  //printf("\n\n%d\n\n", ret.encoded);

  ANY_t *data;
  data = calloc(1, sizeof(ANY_t));
  data->buf = buffer;
  data->size = ret.encoded;

  long version = 2;
  char string[128];
  strcpy(string, "public");
  OCTET_STRING_t *community;
  community = malloc(sizeof(OCTET_STRING_t));
  OCTET_STRING_fromString(community, string);
  Message_t *message;
  message = calloc(1, sizeof(Message_t));
  message->version = version;
  message->community = *community;
  message->data = *data;

  uint8_t *buffer_final = malloc(sizeof(uint8_t) * 1024);
  size_t buffer_final_size = 1024;
  asn_enc_rval_t ret2 = asn_encode_to_buffer(0, ATS_BER, &asn_DEF_Message, message, buffer_final, buffer_final_size);

  FILE *fp = stdout;
  xer_fprint(fp, &asn_DEF_Message, message);

  int port;
  const char ip[15];
  printf("Please insert target IP: ");
  scanf("%s", &ip);
  printf("Please insert target Port: ");
  scanf("%d", &port);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  socklen_t udp_socket_size = sizeof(addr);
  int sent = sendto(sock, buffer_final, buffer_final_size, 0, (struct sockaddr *)&addr, udp_socket_size);

  return 0;
}
