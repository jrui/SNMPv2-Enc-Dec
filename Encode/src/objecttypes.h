#ifndef	_OBJECTTYPES_H_
#define	_OBJECTTYPES_H_

#include <ObjectSyntax.h>
#include <ObjectName.h>
#include <OCTET_STRING.h>


/**
*   This function creates and returns the address of a new
* ObjectName_t. Fills the present field with the NOTHING
* type.
* @return - Pointer to a new ObjectSyntax_t
**/
ObjectSyntax_t *create_ObjSynt_Nothing();


/**
*   This function creates a new ObjectSyntax_t from a given
* SimpleSyntax element.
* @param ss - Pointer to a SimpleSyntax_t to incorporate in
* the return element
* @return - Pointer to a new ObjectSyntax_t
**/
ObjectSyntax_t *create_ObjSynt_Simple(SimpleSyntax_t *ss);


/**
*   This function creates a new ObjectSyntax_t from a given
* ApplicationSyntax element.
* @param ss - Pointer to a ApplicationSyntax_t to incorporate
* in the return element
* @return - Pointer to a new ObjectSyntax_t
**/
ObjectSyntax_t *create_ObjSynt_Application(ApplicationSyntax_t *as);


/**
*   This function creates a new ObjectName element from a
* given uint8 pointer and a size.
* @param name - name to be given to the object (equivalent to
* an octet string)
* @param size - the size of the first element
* @return - Pointer to a new ObjectName_t
**/
ObjectName_t *create_ObjectName(uint8_t *name, size_t size);

#endif
