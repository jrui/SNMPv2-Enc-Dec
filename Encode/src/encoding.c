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
#include <objecttypes.h>


int main(int argc, char const *argv[]) {
  SimpleSyntax_t *simple;
  long integer_value;
  simple = calloc(1, sizeof(SimpleSyntax_t));
  simple->present = SimpleSyntax_PR_integer_value;
  simple->choice.integer_value = integer_value;

  ObjectSyntax_t *object_syntax;
  /*object_syntax = calloc(1, sizeof(ObjectSyntax_t));
  object_syntax->present = ObjectSyntax_PR_simple;
  object_syntax->choice.simple = *simple;
*/
  object_syntax = create_ObjSynt_Simple(simple);

  uint8_t *name = "a";
  size_t name_size = 1;
  ObjectName_t *object_name;
  /*object_name = calloc(1, sizeof(ObjectName_t));
  object_name->buf = name;
  object_name->size = name_size;
*/
  object_name = create_ObjectName(name, name_size);

  /*VarBind_t *var_bind;
  NULL_t nullt = 1;
  var_bind = calloc(1, sizeof(VarBind_t));
  var_bind->name = *object_name;
  //var_bind->choice.present = choice_PR_value;
  var_bind->choice.present = choice_PR_unSpecified;
  //var_bind->choice.choice.value = *object_syntax;
  var_bind->choice.choice.unSpecified = nullt;
*/
  VarBind_t *var_bind;
  NULL_t nullt = (NULL_t) 1;
  var_bind = create_VarBind_unSpecified(object_name, nullt);

  /*VarBindList_t *varlist;
  varlist = calloc(1, sizeof(VarBindList_t));
  int r = ASN_SEQUENCE_ADD(&varlist->list, var_bind);
*/
  VarBindList_t *varlist;
  varlist = create_VarBindList(var_bind);


  SetRequest_PDU_t *setRequestPDU;
  long requestID = 1;
  setRequestPDU = calloc(1, sizeof(PDU_t));
  setRequestPDU->request_id = requestID;
  setRequestPDU->error_index = 0;
  setRequestPDU->error_status = 0;
  setRequestPDU->variable_bindings = *varlist;

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
