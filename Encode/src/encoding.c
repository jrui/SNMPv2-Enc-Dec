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


int main(int argc, char const *argv[]) {
  ApplicationSyntax_t *application_s = createIpAddress("127.0.0.1");
  ObjectSyntax_t *object_syntax_a = create_ObjSynt_Application(application_s);

  ObjectName_t *object_name = create_ObjectName("1.0.2", 5);
  VarBind_t *var_bind = create_VarBind_Value(object_name, object_syntax_a);
  VarBindList_t *varlist = create_VarBindList(var_bind);
  SetRequest_PDU_t *setRequestPDU = create_SetRequestPDU(1, 0, 0, varlist);

  uint8_t *b = malloc(sizeof(uint8_t) * BUFF_SIZE);
  asn_enc_rval_t ret1 = encode_PDUs_SetRequest(setRequestPDU, b);
  ANY_t *data = create_ANY(b, ret1.encoded);

  uint8_t *buf3 = malloc(sizeof(uint8_t) * BUFF_SIZE);
  asn_enc_rval_t ret2 = encode_Message("public", 2, data, buf3);


  int port; const char ip[15];
  printf("Please insert target IP: ");    scanf("%s", &ip);
  printf("Please insert target Port: ");  scanf("%d", &port);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  socklen_t udp_socket_size = sizeof(addr);
  int sent = sendto(sock, buf3, BUFF_SIZE, 0, (struct sockaddr *)&addr, udp_socket_size);

  return 0;
}
