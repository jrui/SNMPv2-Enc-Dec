#ifndef	_BINDINGTYPES_H_
#define	_BINDINGTYPES_H_

#include <NULL.h>
#include <ObjectName.h>
#include <ObjectSyntax.h>
#include <VarBind.h>
#include <VarBindList.h>


/**
*   Function that creates a varbind with a given object name
* and returns a pointer to a VarBind_t
* @param on - ObjectName_t value of var bind name
* @return - Pointer to the new VarBind_t variable
**/
VarBind_t *create_VarBind_Nothing(ObjectName_t *on);


/**
*   Funtion that creates a new varbind from an object name and
* an object sintax pointer
* @param on - ObjectName_t value of var bind name
* @param os - ObjectSyntax_t
* @return - Pointer to the new VarBind_t variable
**/
VarBind_t *create_VarBind_Value(ObjectName_t *on, ObjectSyntax_t *os);


/**
*   Funtions that create a new varbind from an object name and
* an NULL_t pointer
* @param on - ObjectName_t value of var bind name
* @param os - NULL_t
* @return - Pointer to the new VarBind_t variable
**/
VarBind_t *create_VarBind_unSpecified(ObjectName_t *on, NULL_t n);
VarBind_t *create_VarBind_noSuchObject(ObjectName_t *on, NULL_t n);
VarBind_t *create_VarBind_noSuchInstance(ObjectName_t *on, NULL_t n);
VarBind_t *create_VarBind_endOfMibView(ObjectName_t *on, NULL_t n);


/**
*   Function that returns a new VarBindList, with one VarBind
* element associated.
* @param vb - The first VarBindof the VarbindList
* @return - Pointer to the new VarBindList created
**/
VarBindList_t *create_VarBindList(VarBind_t *vb);

#endif
