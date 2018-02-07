/*
 * Sousa, Silva
 * Encoding API
*/

#ifndef	_ENCODER_H_
#define	_ENCODER_H_

uint8_t encodeGetRequest();
uint8_t encodeGetNextRequest();
uint8_t encodeGetBulkRequest();
uint8_t encodeInformRequest();
uint8_t encodeSetRequest();
uint8_t encodeResponseRequest();
uint8_t encodeSnmpv2Trap();
uint8_t encodeReport();
