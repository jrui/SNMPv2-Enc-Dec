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
