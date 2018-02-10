
#ifndef	_COMPLEXTYPES_H_
#define	_COMPLEXTYPES_H_

#include <OCTET_STRING.h>
#include <ApplicationSyntax.h>
#include <ANY.h>
#include <Message.h>


/**
* Function that creates an ApplicationSyntax_t setting its present
* value to NOTHING and returns a pointer to it.
* @return Returns an ApplicationSyntax_t with nothing in it
*/
ApplicationSyntax_t* createNothing();


/**
* Function that creates an ApplicationSyntax_t with an IpAddress_t
* and returns a pointer to it.
* @params ipAddress - string that specifies the ipAddress
*                    ("192.136.23.23" or "localhost" for example)
* @returns ApplicationSyntax_t pointer to NOTHING if the string is not valid
*          or to an ApplicationSyntax_t with the given IpAddress value
*/
ApplicationSyntax_t* createIpAddress(char* ipAddress);


/**
* Function that creates an ApplicationSyntax_t with a normal counter
* (Counter32_t - same as a long), returning a pointer to it.
* @param counterValue - value of the counter
* @returns ApplicationSyntax_t pointer to the value created
*/
ApplicationSyntax_t* createCounterValue(long counterValue);


/**
* Function that creates an ApplicationSyntax_t with a big counter
* (Counter64_t), returning a pointer to it.
* @param counterValue - value of the counter
* @param size - size of the counter in bytes
* @returns ApplicationSyntax_t pointer to the value created
*/
ApplicationSyntax_t* createBigCounterValue(uint8_t *counterValue, size_t size);


/**
* Function that creates an ApplicationSyntax_t with an unsigned int 32bit
* (Unsigned32_t aka unsigned long), returning a pointer to it
* @param value - unsigned value
* @returns ApplicationSyntax_t pointer to the value created
*/
ApplicationSyntax_t* createUnsigned32(unsigned long value);


/**
* Function that creates an ApplicationSyntax_t with an Opaque_t
* and returns a pointer to it.
* @params value - string that specifies the Opaque_t(aka OCTET_STRING_t)
* @returns ApplicationSyntax_t pointer to NOTHING if the string is not valid
*          or to an ApplicationSyntax_t with the given Opaque_t value
*/
ApplicationSyntax_t* createOpaque(char* value);


/**
*   Funtion used to create an element ANY_t and return the pointer
* of that same element.
* @param buf - The value to be associated with the ANY_t buffer
* @param size - The size of the previously specified buffer
* @return - Pointer to the new structure
*/
ANY_t *create_ANY(uint8_t *buf, int size);


#endif
