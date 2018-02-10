#ifndef	_ENCODETYPES_H_
#define	_ENCODETYPES_H_

#include <OCTET_STRING.h>
#include <ANY.h>
#include <Message.h>

#include <GetBulkRequest-PDU.h>
#include <GetNextRequest-PDU.h>
#include <GetRequest-PDU.h>
#include <InformRequest-PDU.h>
#include <PDUs.h>
#include <Report-PDU.h>
#include <Response-PDU.h>
#include <SetRequest-PDU.h>
#include <SNMPv2-Trap-PDU.h>


#define BUFF_SIZE 1024

/**
*
*/
asn_enc_rval_t encode_Message(char *cs, long v, ANY_t *data, uint8_t *buf);


/**
*   Functions that create the specified PDUs_t, this return types are comprise
* the more specific PDU_t types. Like both the previous funtions, the following
* also alocates memory.
* @param - A given variable PDU_t type to be associated
* @param b - Buffer to fill with encoded data
* with each one of the PDUs
* @return - Error code, number of bytes read
**/
asn_enc_rval_t encode_PDUs_Nothing(uint8_t *b);
asn_enc_rval_t encode_PDUs_GetRequest(GetRequest_PDU_t *gr, uint8_t *b);
asn_enc_rval_t encode_PDUs_GetNextRequest(GetNextRequest_PDU_t *gnr, uint8_t *b);
asn_enc_rval_t encode_PDUs_GetBulkRequest(GetBulkRequest_PDU_t *gbr, uint8_t *b);
asn_enc_rval_t encode_PDUs_Response(Response_PDU_t *r, uint8_t *b);
asn_enc_rval_t encode_PDUs_SetRequest(SetRequest_PDU_t *sr, uint8_t *b);
asn_enc_rval_t encode_PDUs_InformRequest(InformRequest_PDU_t *ir, uint8_t *b);
asn_enc_rval_t encode_PDUs_SNMPV2Trap(SNMPv2_Trap_PDU_t *snmp, uint8_t *b);
asn_enc_rval_t encode_PDUs_Report(Report_PDU_t *r, uint8_t *b);


#endif
