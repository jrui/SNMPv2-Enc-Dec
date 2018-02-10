
#include <complextypes.h>

/**
* Private function to allocate space for an ApplicationSyntax_t
*/
ApplicationSyntax_t* allocate() {
    ApplicationSyntax_t *res;
    res = (ApplicationSyntax_t*) malloc(sizeof(ApplicationSyntax_t));
    return res;
}

ApplicationSyntax_t* create_Nothing() {
    ApplicationSyntax_t *res = allocate();
    res -> present = ApplicationSyntax_PR_NOTHING;
    return res;
}

ApplicationSyntax_t* create_IpAddress(char* ipAddress) {
    ApplicationSyntax_t *res = allocate();
    OCTET_STRING_t* octetString;
    octetString = (OCTET_STRING_t*) malloc(sizeof(OCTET_STRING_t));
    int code = OCTET_STRING_fromString(octetString, ipAddress);
    if(code < 0) {
        res -> present = ApplicationSyntax_PR_NOTHING;
        return res;
    }
    res -> present = ApplicationSyntax_PR_ipAddress_value;
    res -> choice.ipAddress_value = (IpAddress_t) *octetString;
    return res;
}

ApplicationSyntax_t* create_CounterValue(long counterValue) {
    ApplicationSyntax_t *res = allocate();
    Counter32_t counter;
    // Counter32_t is a simple long, so it can be casted
    counter = (Counter32_t) counterValue;
    res -> present = ApplicationSyntax_PR_counter_value;
    res -> choice.counter_value = counter;
    return res;
}

ApplicationSyntax_t* create_BigCounterValue(uint8_t *counterValue, size_t size) {
    ApplicationSyntax_t *res = allocate();
    ASN__PRIMITIVE_TYPE_t *baseType;
    INTEGER_t counter;
    baseType = (ASN__PRIMITIVE_TYPE_t*) malloc(sizeof(ASN__PRIMITIVE_TYPE_t));
    baseType -> buf = counterValue;
    baseType -> size = size;
    counter = (INTEGER_t) *baseType;
    res -> present = ApplicationSyntax_PR_big_counter_value;
    res -> choice.big_counter_value = (Counter64_t) counter;
    return res;
}

ApplicationSyntax_t* create_TimeTicks(long timeTicks) {
    ApplicationSyntax_t *res = allocate();
    // TimeTicks_t is a simple long, so it can be casted
    TimeTicks_t timeTicksValue = (TimeTicks_t) timeTicks;
    res -> present = ApplicationSyntax_PR_timeticks_value;
    res -> choice.timeticks_value = timeTicksValue;
    return res;
}

ApplicationSyntax_t* create_Unsigned32(unsigned long value) {
    ApplicationSyntax_t *res = allocate();
    // Unsigned32_t is an unsigned long, so it can be casted
    Unsigned32_t valueRes = (Unsigned32_t) value;
    res -> present = ApplicationSyntax_PR_unsigned_integer_value;
    res -> choice.unsigned_integer_value = valueRes;
    return res;
}

ApplicationSyntax_t* create_Opaque(char* value) {
    ApplicationSyntax_t *res = allocate();
    // As Opaque_t is the same as OCTET_STRING_t, let's create one
    OCTET_STRING_t* octetString;
    octetString = (OCTET_STRING_t*) malloc(sizeof(OCTET_STRING_t));
    int code = OCTET_STRING_fromString(octetString, value);
    if(code < 0) {
        res -> present = ApplicationSyntax_PR_NOTHING;
        return res;
    }
    res -> present = ApplicationSyntax_PR_arbitrary_value;
    res -> choice.arbitrary_value = (Opaque_t) *octetString;
    return res;
}


ANY_t *create_ANY(uint8_t *buf, int size) {
  ANY_t *ret;
  ret = calloc(1, sizeof(ANY_t));
  ret->buf = buf;
  ret->size = size;

  return ret;
}
