#include <encodetypes.h>


asn_enc_rval_t encode_Message(char *cs, long v, ANY_t *data, uint8_t *buf) {
  OCTET_STRING_t *community = (OCTET_STRING_t *) malloc(sizeof(OCTET_STRING_t));
  int code = OCTET_STRING_fromString(community, cs);
  Message_t *msg = calloc(1, sizeof(Message_t));
  if(code == -1) perror("Error encoding message!");
  else {
    msg->version = v;
    msg->community = *community;
    msg->data = *data;

    FILE *fp = stdout;
    xer_fprint(fp, &asn_DEF_Message, msg);
  }
  return asn_encode_to_buffer(0, ATS_BER, &asn_DEF_Message, msg, buf, BUFF_SIZE);
}


asn_enc_rval_t encode(PDUs_t *r, uint8_t *b) {
  FILE *fp = stdout;
  xer_fprint(fp, &asn_DEF_PDUs, r);

  return asn_encode_to_buffer(0, ATS_BER, &asn_DEF_PDUs, r, b, BUFF_SIZE);
}


asn_enc_rval_t encode_PDUs_Nothing(uint8_t *b) {
  PDUs_t *ret;
  ret = calloc(1, sizeof(PDUs_t));
  ret->present = PDUs_PR_NOTHING;

  return encode(ret, b);
}


asn_enc_rval_t encode_PDUs_GetRequest(GetRequest_PDU_t *gr, uint8_t *b) {
  PDUs_t *ret;
  ret = calloc(1, sizeof(PDUs_t));
  ret->present = PDUs_PR_get_request;
  ret->choice.get_request = *gr;

  return encode(ret, b);
}


asn_enc_rval_t encode_PDUs_GetNextRequest(GetNextRequest_PDU_t *gnr, uint8_t *b) {
  PDUs_t *ret;
  ret = calloc(1, sizeof(PDUs_t));
  ret->present = PDUs_PR_get_next_request;
  ret->choice.get_next_request = *gnr;

  return encode(ret, b);
}


asn_enc_rval_t encode_PDUs_GetBulkRequest(GetBulkRequest_PDU_t *gbr, uint8_t *b) {
  PDUs_t *ret;
  ret = calloc(1, sizeof(PDUs_t));
  ret->present = PDUs_PR_get_bulk_request;
  ret->choice.get_bulk_request = *gbr;

  return encode(ret, b);
}

asn_enc_rval_t encode_PDUs_Response(Response_PDU_t *r, uint8_t *b) {
  PDUs_t *ret;
  ret = calloc(1, sizeof(PDUs_t));
  ret->present = PDUs_PR_response;
  ret->choice.response = *r;

  return encode(ret, b);
}


asn_enc_rval_t encode_PDUs_SetRequest(SetRequest_PDU_t *sr, uint8_t *b) {
  PDUs_t *ret;
  ret = calloc(1, sizeof(PDUs_t));
  ret->present = PDUs_PR_set_request;
  ret->choice.set_request = *sr;

  return encode(ret, b);
}


asn_enc_rval_t encode_PDUs_InformRequest(InformRequest_PDU_t *ir, uint8_t *b) {
  PDUs_t *ret;
  ret = calloc(1, sizeof(PDUs_t));
  ret->present = PDUs_PR_inform_request;
  ret->choice.inform_request = *ir;

  return encode(ret, b);
}


asn_enc_rval_t encode_PDUs_SNMPV2Trap(SNMPv2_Trap_PDU_t *snmp, uint8_t *b) {
  PDUs_t *ret;
  ret = calloc(1, sizeof(PDUs_t));
  ret->present = PDUs_PR_snmpV2_trap;
  ret->choice.snmpV2_trap = *snmp;

  return encode(ret, b);
}


asn_enc_rval_t encode_PDUs_Report(Report_PDU_t *r, uint8_t *b) {
  PDUs_t *ret;
  ret = calloc(1, sizeof(PDUs_t));
  ret->present = PDUs_PR_report;
  ret->choice.report = *r;

  return encode(ret, b);
}
