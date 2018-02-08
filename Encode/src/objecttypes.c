#include <objecttypes.h>


ObjectSyntax_t *create_ObjSynt_Nothing() {
  ObjectSyntax_t *ret;
  ret = calloc(1, sizeof(ObjectSyntax_t));
  ret->present = ObjectSyntax_PR_NOTHING;

  return ret;
}


ObjectSyntax_t *create_ObjSynt_Simple(SimpleSyntax_t *ss) {
  ObjectSyntax_t *ret;
  ret = calloc(1, sizeof(ObjectSyntax_t));
  ret->present = ObjectSyntax_PR_simple;
  ret->choice.simple = *ss;

  return ret;
}


ObjectSyntax_t *create_ObjSynt_Application(ApplicationSyntax_t *as) {
  ObjectSyntax_t *ret;
  ret = calloc(1, sizeof(ObjectSyntax_t));
  ret->present = ObjectSyntax_PR_application_wide;
  ret->choice.application_wide = *as;

  return ret;
}


ObjectName_t *create_ObjectName(uint8_t *name, size_t size) {
  ObjectName_t *ret;
  ret = calloc(1, sizeof(ObjectName_t));
  ret->buf = name;
  ret->size = size;

  return ret;
}
