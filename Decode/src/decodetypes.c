
#include <decodetypes.h>

DecodeResult_t decode_Message(char *buffer, int bs) {
    Message_t *message = 0;
    DecodeResult_t *result = malloc(sizeof(struct DecodeResult));
    asn_dec_rval_t rval = asn_decode(0, ATS_BER, &asn_DEF_Message,
        (void **) &message, buffer, bs);
    result -> isMessage = 1;
    result -> choice.message = message;
    result -> status = rval;
    return *result;
}

DecodeResult_t decode_PDU(Message_t *message) {
    PDUs_t *pdu = 0;
    DecodeResult_t *result = malloc(sizeof(struct DecodeResult));
    asn_dec_rval_t rval = asn_decode(0, ATS_BER, &asn_DEF_PDUs,
        (void **) &pdu, message->data.buf, message->data.size);
    result -> isMessage = 0;
    result -> choice.pdu = pdu;
    result -> status = rval;
    return *result;
}
