#ifndef PDUPARSER_H
#define PDUPARSER_H

#include <PDUs.h>
#include <VarBindList.h>

/**
    Possible types on a ParsedVarBind structure
*/
typedef enum PresentType {
    NOTHING,
    UNSPECIFIED,
    NO_SUCH_OBJECT,
    NO_SUCH_INSTANCE,
    END_OF_MIB_VIEW,
    INTEGER,
    STRING,
    OBJECT_ID,
    IP_ADDRESS,
    COUNTER_32,
    COUNTER_64,
    TIMETICKS,
    OPAQUE,
    UNSIGNED_INT_32
} Type;

/**
* Structure that has the parsed Varbind to represent
* each one present on the PDUs_t
*/
typedef struct ParsedVarBind {
    Type type;
    uint8_t *oid;
    union vbu {
        NULL_t unSpecified;
        NULL_t noSuchObject;
        NULL_t noSuchInstance;
        NULL_t endOfMibView;
        long integer_value;
        char* string_value;
        OBJECT_IDENTIFIER_t* object_id_value;
        IpAddress_t* ipAddress_value;
        long counter_32_value;
        Counter64_t* counter_64_value;
        long timeticks_value;
        char* opaque;
        unsigned long unsigned_int_value;
    } choice;
} ParsedVarBind_t;

/**
* Structure for a ParsedPdu to return on the functions
* that parse a given PDUs_t
*/
typedef struct ParsedPdu {
    PDUs_PR present;
    long request_id;
	long error_status;
	long error_index;
    // size and list of ParsedVarBind_t
    int size;
	ParsedVarBind_t *variable_bindings;
} ParsedPdu_t;

/**
* Structure for a ParsedBulkPdu to return on the function
* that parse a given BulkPDU
*/
typedef struct ParsedBulkPdu {
    long request_id;
    long non_repeaters;
	long max_repetitions;
    // size and list of ParsedVarBind_t
    int size;
	ParsedVarBind_t *variable_bindings;
} ParsedBulkPdu_t;

/**
* Function that assembles ParsedPdu for a GetRequest
* @param pdu - GetRequest pdu to parse
* @returns Assembled ParsedPdu with all the info inside the given pdu
*/
ParsedPdu_t* parse_GetRequest(GetRequest_PDU_t *pdu);

/**
* Function that assembles ParsedPdu for a GetNextRequest
* @param pdu - GetNextRequest pdu to parse
* @returns Assembled ParsedPdu with all the info inside the given pdu
*/
ParsedPdu_t* parse_GetNextRequest(GetNextRequest_PDU_t *pdu);

/**
* Function that assembles ParsedBulkPdu for a GetBulkRequest
* @param pdu - GetBulkRequest pdu to parse
* @returns Assembled ParsedBulkPdu with all the info inside the given pdu
*/
ParsedBulkPdu_t* parse_GetBulkRequest(GetBulkRequest_PDU_t *pdu);

/**
* Function that assembles ParsedPdu for a Response
* @param pdu - Response pdu to parse
* @returns Assembled ParsedPdu with all the info inside the given pdu
*/
ParsedPdu_t* parse_Response(Response_PDU_t *pdu);

/**
* Function that assembles ParsedPdu for a SetRequest
* @param pdu - SetRequest pdu to parse
* @returns Assembled ParsedPdu with all the info inside the given pdu
*/
ParsedPdu_t* parse_SetRequest(SetRequest_PDU_t *pdu);

/**
* Function that assembles ParsedPdu for a InformRequest
* @param pdu - InformRequest pdu to parse
* @returns Assembled ParsedPdu with all the info inside the given pdu
*/
ParsedPdu_t* parse_InformRequest(InformRequest_PDU_t *pdu);

/**
* Function that assembles ParsedPdu for a Snmpv2Trap
* @param pdu - Snmpv2Trap pdu to parse
* @returns Assembled ParsedPdu with all the info inside the given pdu
*/
ParsedPdu_t* parse_SnmpV2Trap(SNMPv2_Trap_PDU_t *pdu);

/**
* Function that assembles ParsedPdu for a Report
* @param pdu - Report pdu to parse
* @returns Assembled ParsedPdu with all the info inside the given pdu
*/
ParsedPdu_t* parse_Report(Report_PDU_t *pdu);


#endif
