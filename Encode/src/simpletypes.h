
#ifndef	_SIMPLETYPES_H_
#define	_SIMPLETYPES_H_


#include <SimpleSyntax.h>
#include <OCTET_STRING.h>


/*
Returns SimpleSyntax_t with the integer
value received as argument.
*/
SimpleSyntax_t* create_SimpleInteger(long integerValue);

/*
Returns SimpleSyntax_t with the string(char*)
received as argument.
*/
SimpleSyntax_t* create_SimpleString(char* stringValue);

/*
Returns SimpleSyntax_t with the Object identifier
received as argument.
*/
SimpleSyntax_t* create_SimpleObjectID(OBJECT_IDENTIFIER_t oid);

#endif
