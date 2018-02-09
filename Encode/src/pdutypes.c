#include <pdutypes.h>


GetBulkRequest_PDU_t *create_GetBulkRequestPDU(long ri,
                long nr, long mr, VarBindList_t *vb) {
  BulkPDU_t *ret;
  ret = calloc(1, sizeof(BulkPDU_t));
  ret->request_id = ri;
  ret->non_repeaters = nr;
  ret->max_repetitions = mr;
  ret->variable_bindings = *vb;

  return (GetBulkRequest_PDU_t*) ret;
}


GetNextRequest_PDU_t *create_GetNextRequestPDU(long ri,
                long es, long ei, VarBindList_t *vb) {
  PDU_t *ret;
  ret = calloc(1, sizeof(PDU_t));
  ret->request_id = ri;
  ret->error_status = es;
  ret->error_index = ei;
  ret->variable_bindings = *vb;

  return (GetNextRequest_PDU_t*) ret;
}


GetRequest_PDU_t *create_GetRequestPDU(long ri,
                long es, long ei, VarBindList_t *vb) {
  PDU_t *ret;
  ret = calloc(1, sizeof(PDU_t));
  ret->request_id = ri;
  ret->error_status = es;
  ret->error_index = ei;
  ret->variable_bindings = *vb;

  return (GetRequest_PDU_t*) ret;
}


InformRequest_PDU_t *create_InformRequestPDU(long ri,
                long es, long ei, VarBindList_t *vb) {
  PDU_t *ret;
  ret = calloc(1, sizeof(PDU_t));
  ret->request_id = ri;
  ret->error_status = es;
  ret->error_index = ei;
  ret->variable_bindings = *vb;

  return (InformRequest_PDU_t*) ret;
}



Report_PDU_t *create_ReportPDU(long ri,
                long es, long ei, VarBindList_t *vb) {
  PDU_t *ret;
  ret = calloc(1, sizeof(PDU_t));
  ret->request_id = ri;
  ret->error_status = es;
  ret->error_index = ei;
  ret->variable_bindings = *vb;

  return (Report_PDU_t*) ret;
}


Response_PDU_t *create_ResponsePDU(long ri,
                long es, long ei, VarBindList_t *vb) {
  PDU_t *ret;
  ret = calloc(1, sizeof(PDU_t));
  ret->request_id = ri;
  ret->error_status = es;
  ret->error_index = ei;
  ret->variable_bindings = *vb;

  return (Response_PDU_t*) ret;
}


SetRequest_PDU_t *create_SetRequestPDU(long ri,
                long es, long ei, VarBindList_t *vb) {
  PDU_t *ret;
  ret = calloc(1, sizeof(PDU_t));
  ret->request_id = ri;
  ret->error_status = es;
  ret->error_index = ei;
  ret->variable_bindings = *vb;

  return (SetRequest_PDU_t*) ret;
}


SNMPv2_Trap_PDU_t *create_SNMPv2TrapPDU(long ri,
                long es, long ei, VarBindList_t *vb) {
  PDU_t *ret;
  ret = calloc(1, sizeof(PDU_t));
  ret->request_id = ri;
  ret->error_status = es;
  ret->error_index = ei;
  ret->variable_bindings = *vb;

  return (SNMPv2_Trap_PDU_t*) ret;
}


PDUs_t *create_PDUs_Nothing() {
  PDUs_t *ret;
  ret = calloc(1, sizeof(PDUs_t));
  ret->present = PDUs_PR_NOTHING;

  return ret;
}


PDUs_t *create_PDUs_GetRequest(GetRequest_PDU_t *gr) {
  PDUs_t *ret;
  ret = calloc(1, sizeof(PDUs_t));
  ret->present = PDUs_PR_get_request;
  ret->choice.get_request = *gr;

  return ret;
}


PDUs_t *create_PDUs_GetNextRequest(GetNextRequest_PDU_t *gnr) {
  PDUs_t *ret;
  ret = calloc(1, sizeof(PDUs_t));
  ret->present = PDUs_PR_get_next_request;
  ret->choice.get_next_request = *gnr;

  return ret;
}


PDUs_t *create_PDUs_GetBulkRequest(GetBulkRequest_PDU_t *gbr) {
  PDUs_t *ret;
  ret = calloc(1, sizeof(PDUs_t));
  ret->present = PDUs_PR_get_bulk_request;
  ret->choice.get_bulk_request = *gbr;

  return ret;
}

PDUs_t *create_PDUs_Response(Response_PDU_t *r) {
  PDUs_t *ret;
  ret = calloc(1, sizeof(PDUs_t));
  ret->present = PDUs_PR_response;
  ret->choice.response = *r;

  return ret;
}


PDUs_t *create_PDUs_SetRequest(SetRequest_PDU_t *sr) {
  PDUs_t *ret;
  ret = calloc(1, sizeof(PDUs_t));
  ret->present = PDUs_PR_set_request;
  ret->choice.set_request = *sr;

  return ret;
}


PDUs_t *create_PDUs_InformRequest(InformRequest_PDU_t *ir) {
  PDUs_t *ret;
  ret = calloc(1, sizeof(PDUs_t));
  ret->present = PDUs_PR_inform_request;
  ret->choice.inform_request = *ir;

  return ret;
}


PDUs_t *create_PDUs_SNMPV2Trap(SNMPv2_Trap_PDU_t *snmp) {
  PDUs_t *ret;
  ret = calloc(1, sizeof(PDUs_t));
  ret->present = PDUs_PR_snmpV2_trap;
  ret->choice.snmpV2_trap = *snmp;

  return ret;
}


PDUs_t *create_PDUs_Report(Report_PDU_t *r) {
  PDUs_t *ret;
  ret = calloc(1, sizeof(PDUs_t));
  ret->present = PDUs_PR_report;
  ret->choice.report = *r;

  return ret;
}
