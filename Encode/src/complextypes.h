
#ifndef	_COMPLEXTYPES_H_
#define	_COMPLEXTYPES_H_

#include <OCTET_STRING.h>
#include <IpAddress.h>
#include <ApplicationSyntax.h>

ApplicationSyntax* createNothing();
/*
    ipAddress example "192.136.23.23" or "localhost"
*/
ApplicationSyntax* createIpAddress(char* ipAddress);
