
#include <complextypes.h>

ApplicationSyntax* createNothing() {
    ApplicationSyntax *res;

//    res -> present = ApplicationSyntax_PR_ipAddress_value;
//    res -> choice.ipAddress_value = (IpAddress_t) octetString;
    return res;
}

ApplicationSyntax* createIpAddress(char* ipAddress) {
    ApplicationSyntax *res;
    int size = strlen(ipAddress);
    OCTET_STRING_t* octetString = malloc(sizeof(OCTET_STRING_t) * size);
    // -1 de error
    // 0 se correu bem
    // nao esquecer de verificar
    OCTET_STRING_fromString(octetString, ipAddress);
    res = malloc(sizeof(ApplicationSyntax));
    res -> present = ApplicationSyntax_PR_ipAddress_value;
    res -> choice.ipAddress_value = (IpAddress_t) octetString;
    return res;
}
