
#ifndef DECODETYPES_H
#define DECODETYPES_H

#include <stdlib.h>
#include <ApplicationSyntax.h>
#include <Message.h>
#include <PDUs.h>
#include <SimpleSyntax.h>

/**
* Structure to return on the decode functions
*/
typedef struct DecodeResult {
    short int isMessage;
    union choiceField {
        Message_t *message;
        PDUs_t *pdu;
    } choice;
    asn_dec_rval_t status;
} DecodeResult_t;

/**
* Function that decodes a buffer with encoded data in BER to a Message_t
* @params buffer - Buffer with data to decode
* @param bs - Buffer size
* @returns Structure with result and status
*/
DecodeResult_t decode_Message(char *buffer, int bs);

/**
* Function that decodes a PDU with encoded data in BER to a PDUs_t
* @params message - Message with encoded PDU
* @returns Structure with Result and status
*/
DecodeResult_t decode_PDU(Message_t *message);

#endif
