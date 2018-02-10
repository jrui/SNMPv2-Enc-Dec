#ifndef	_PDUTYPES_H_
#define	_PDUTYPES_H_

#include <BulkPDU.h>
#include <PDU.h>
#include <VarBindList.h>

#include <GetBulkRequest-PDU.h>
#include <GetNextRequest-PDU.h>
#include <GetRequest-PDU.h>
#include <InformRequest-PDU.h>
#include <PDUs.h>
#include <Report-PDU.h>
#include <Response-PDU.h>
#include <SetRequest-PDU.h>
#include <SNMPv2-Trap-PDU.h>



/**
*   Function that creates an PDU element, and returns
* the pointer to the generated structure. This Function
* also alocates the required memory.
* @param ri - The Request ID of the PDU
* @param nr - Value of non repeaters
* @param mr - Number os maximum repetitions
* @param vb - A given variable binding list to be associated
* with the PDU itself
* @return - Pointer to the newly generated structure
**/
GetBulkRequest_PDU_t *create_GetBulkRequestPDU(long ri,
                long nr, long mr, VarBindList_t *vb);


/**
*   Functions that create the specified PDU, this return types are equivalent
* to the more general type PDU_t, however they are more specific. Like
* the previously defined funtion, this also alocates memory.
* @param ri - The Request ID of the PDU
* @param es - Value of the Error Status element
* @param ei - Value of the Error Index element
* @param vb - A given variable binding list to be associated
* with each one of the PDUs
* @return - Pointer to the newly generated structure
**/
GetNextRequest_PDU_t *create_GetNextRequestPDU(long ri,
                long es, long ei, VarBindList_t *vb);

GetRequest_PDU_t *create_GetRequestPDU(long ri,
                long es, long ei, VarBindList_t *vb);

InformRequest_PDU_t *create_InformRequestPDU(long ri,
                long es, long ei, VarBindList_t *vb);

Report_PDU_t *create_ReportPDU(long ri,
                long es, long ei, VarBindList_t *vb);

Response_PDU_t *create_ResponsePDU(long ri,
                long es, long ei, VarBindList_t *vb);

SetRequest_PDU_t *create_SetRequestPDU(long ri,
                long es, long ei, VarBindList_t *vb);

SNMPv2_Trap_PDU_t *create_SNMPv2TrapPDU(long ri,
                long es, long ei, VarBindList_t *vb);


#endif
