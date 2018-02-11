

#include <pduparser.h>

/*
* Helper function to treat SimpleSyntax inside a ObjectSyntax,
* based on what's inside
*/
ParsedVarBind_t* treatSimpleSyntax(VarBind_t *var_bind) {
    ParsedVarBind_t *result = malloc(sizeof(ParsedVarBind_t));
    ObjectSyntax_t obj = var_bind -> choice.choice.value;
    SimpleSyntax_t simpleSyn = obj.choice.simple;
    long integerValue;
    char *string;
    result -> oid = var_bind -> name;
    switch (simpleSyn.present) {
        case SimpleSyntax_PR_NOTHING:
            result -> type = NOTHING;
            break;/* No components present */
        case SimpleSyntax_PR_integer_value:
            result -> type = INTEGER;
            integerValue = (long) simpleSyn.choice.integer_value;
            result -> choice.integer_value = integerValue;
            break;
        case SimpleSyntax_PR_string_value:
            result -> type = STRING;
            string = (char*) simpleSyn.choice.string_value.buf;
            result -> choice.string_value = string;
            break;
        case SimpleSyntax_PR_objectID_value:
            result -> type = OBJECT_ID;
            result -> choice.object_id_value = &simpleSyn.choice.objectID_value;
            break;
    }
    return result;
}


/*
* Helper function to treat ApplicationSyntax inside a ObjectSyntax,
* based on what's inside
*/
ParsedVarBind_t* treatApplicationSyntax(VarBind_t *var_bind) {
    ParsedVarBind_t *result = malloc(sizeof(ParsedVarBind_t));
    ObjectSyntax_t obj = var_bind -> choice.choice.value;
    ApplicationSyntax_t appSyn = obj.choice.application_wide;
    Counter64_t aux;
    long value; unsigned long u_value;
    char *string;
    result -> oid = var_bind -> name;
    switch (appSyn.present) {
        case ApplicationSyntax_PR_NOTHING:
            result -> type = NOTHING;
            break;/* No components present */
        case ApplicationSyntax_PR_ipAddress_value:
            result -> type = IP_ADDRESS;
            result -> choice.ipAddress_value = &appSyn.choice.ipAddress_value;
            break;
        case ApplicationSyntax_PR_counter_value:
            result -> type = COUNTER_32;
            value = (long) appSyn.choice.counter_value;
            result -> choice.counter_32_value = value;
            break;
        case ApplicationSyntax_PR_timeticks_value:
            result -> type = TIMETICKS;
            value = (long) appSyn.choice.timeticks_value;
            result -> choice.timeticks_value = value;
            break;
        case ApplicationSyntax_PR_arbitrary_value:
            result -> type = OPAQUE;
            string = (char*) appSyn.choice.arbitrary_value.buf;
            result -> choice.string_value = string;
            break;
        case ApplicationSyntax_PR_big_counter_value:
            result -> type = COUNTER_64;
            aux = appSyn.choice.big_counter_value;
            result -> choice.counter_64_value = &aux;
            break;
        case ApplicationSyntax_PR_unsigned_integer_value:
            result -> type = UNSIGNED_INT_32;
            u_value = (unsigned long) appSyn.choice.unsigned_integer_value;
            result -> choice.unsigned_int_value = u_value;
            break;
    }
    return result;
}

/*
* Helper function to treat ObjectSyntax inside a VarBind,
* based on what's inside
*/
ParsedVarBind_t* treatObjectSyntax(VarBind_t *var_bind) {
    ParsedVarBind_t *result;
    switch (var_bind -> choice.choice.value.present) {
        case ObjectSyntax_PR_NOTHING:
            result = malloc(sizeof(ParsedVarBind_t));
            result -> oid = var_bind -> name;
            result -> type = NOTHING;
            break;
    	case ObjectSyntax_PR_simple:
            result = treatSimpleSyntax(var_bind);
            break;
    	case ObjectSyntax_PR_application_wide:
            result = treatApplicationSyntax(var_bind);
            break;
    }
    return result;
}

/*
* Helper function to parse a single VariableBind
*/
ParsedVarBind_t* treatVarbind(VarBind_t *var_bind) {
    ParsedVarBind_t *result = malloc(sizeof(ParsedVarBind_t));
    NULL_t aux;
    result -> oid = var_bind -> name;
    switch (var_bind -> choice.present) {
        case choice_PR_NOTHING:
            result -> type = NOTHING;
        	break;
    	case choice_PR_value:
            free(result);
            result = treatObjectSyntax(var_bind);
            break;
    	case choice_PR_unSpecified:
            result -> type = UNSPECIFIED;
            aux = var_bind -> choice.choice.unSpecified;
            result -> choice.unSpecified = aux;
            break;
    	case choice_PR_noSuchObject:
            result -> type = NO_SUCH_OBJECT;
            aux = var_bind -> choice.choice.noSuchObject;
            result -> choice.noSuchObject = aux;
            break;
    	case choice_PR_noSuchInstance:
            result -> type = NO_SUCH_INSTANCE;
            aux = var_bind -> choice.choice.noSuchInstance;
            result -> choice.noSuchInstance = aux;
            break;
    	case choice_PR_endOfMibView:
            result -> type = END_OF_MIB_VIEW;
            aux = var_bind -> choice.choice.endOfMibView;
            result -> choice.endOfMibView = aux;
            break;
    }
    return result;
}

/*
* Helper function to parse a VariableBindList
*/
ParsedVarBind_t* parse_VarBinds(VarBindList_t var_bindings) {
    ParsedVarBind_t *pVarBindList;
    int var_list_size = var_bindings.list.count, i;
    VarBind_t* var_bind;
    pVarBindList = malloc(sizeof(ParsedVarBind_t) * var_list_size);
    for(i = 0; i < var_list_size; i++) {
        var_bind = var_bindings.list.array[i];
        pVarBindList[i] = *treatVarbind(var_bind);
    }
    return pVarBindList;
}

/*
* Helper function to set common stuff on multiple pdu types
*/
void setCodesAndId(PDU_t *pduIn, ParsedPdu_t *pduOut) {
    pduOut -> request_id   = pduIn -> request_id;
    pduOut -> error_status = pduIn -> error_status;
    pduOut -> error_index  = pduIn -> error_index;
}

ParsedPdu_t* parse_GetRequest(GetRequest_PDU_t *pdu) {
    ParsedPdu_t *result = (ParsedPdu_t*) malloc(sizeof(ParsedPdu_t));
    VarBindList_t var_bindings = pdu -> variable_bindings;
    ParsedVarBind_t *list = parse_VarBinds(var_bindings);
    setCodesAndId(pdu, result);
    result -> present = PDUs_PR_get_request;
    result -> size = var_bindings.list.count;
    result -> variable_bindings = list;
    return result;
}

ParsedPdu_t* parse_GetNextRequest(GetNextRequest_PDU_t *pdu) {
    ParsedPdu_t *result = (ParsedPdu_t*) malloc(sizeof(ParsedPdu_t));
    VarBindList_t var_bindings = pdu -> variable_bindings;
    ParsedVarBind_t *list = parse_VarBinds(var_bindings);
    setCodesAndId(pdu, result);
    result -> present = PDUs_PR_get_next_request;
    result -> size = var_bindings.list.count;
    result -> variable_bindings = list;
    return result;
}

ParsedBulkPdu_t* parse_GetBulkRequest(GetBulkRequest_PDU_t *pdu) {
    ParsedBulkPdu_t *result = (ParsedBulkPdu_t*) malloc(sizeof(ParsedBulkPdu_t));
    VarBindList_t var_bindings = pdu -> variable_bindings;
    ParsedVarBind_t *list = parse_VarBinds(var_bindings);
    result -> request_id = pdu -> request_id;
    result -> non_repeaters = pdu -> non_repeaters;
    result -> max_repetitions = pdu -> max_repetitions;
    result -> size = var_bindings.list.count;
    result -> variable_bindings = list;
    return result;
}

ParsedPdu_t* parse_Response(Response_PDU_t *pdu) {
    ParsedPdu_t *result = (ParsedPdu_t*) malloc(sizeof(ParsedPdu_t));
    VarBindList_t var_bindings = pdu -> variable_bindings;
    ParsedVarBind_t *list = parse_VarBinds(var_bindings);
    setCodesAndId(pdu, result);
    result -> present = PDUs_PR_response;
    result -> size = var_bindings.list.count;
    result -> variable_bindings = list;
    return result;
}

ParsedPdu_t* parse_SetRequest(SetRequest_PDU_t *pdu) {
    ParsedPdu_t *result = (ParsedPdu_t*) malloc(sizeof(ParsedPdu_t));
    VarBindList_t var_bindings = pdu -> variable_bindings;
    ParsedVarBind_t *list = parse_VarBinds(var_bindings);
    setCodesAndId(pdu, result);
    result -> present = PDUs_PR_set_request;
    result -> size = var_bindings.list.count;
    result -> variable_bindings = list;
    return result;
}

ParsedPdu_t* parse_InformRequest(InformRequest_PDU_t *pdu) {
    ParsedPdu_t *result = (ParsedPdu_t*) malloc(sizeof(ParsedPdu_t));
    VarBindList_t var_bindings = pdu -> variable_bindings;
    ParsedVarBind_t *list = parse_VarBinds(var_bindings);
    setCodesAndId(pdu, result);
    result -> present = PDUs_PR_inform_request;
    result -> size = var_bindings.list.count;
    result -> variable_bindings = list;
    return result;
}

ParsedPdu_t* parse_SnmpV2Trap(SNMPv2_Trap_PDU_t *pdu) {
    ParsedPdu_t *result = (ParsedPdu_t*) malloc(sizeof(ParsedPdu_t));
    VarBindList_t var_bindings = pdu -> variable_bindings;
    ParsedVarBind_t *list = parse_VarBinds(var_bindings);
    setCodesAndId(pdu, result);
    result -> present = PDUs_PR_snmpV2_trap;
    result -> size = var_bindings.list.count;
    result -> variable_bindings = list;
    return result;
}

ParsedPdu_t* parse_Report(Report_PDU_t *pdu) {
    ParsedPdu_t *result = (ParsedPdu_t*) malloc(sizeof(ParsedPdu_t));
    VarBindList_t var_bindings = pdu -> variable_bindings;
    ParsedVarBind_t *list = parse_VarBinds(var_bindings);
    setCodesAndId(pdu, result);
    result -> present = PDUs_PR_report;
    result -> size = var_bindings.list.count;
    result -> variable_bindings = list;
    return result;
}
