#include <bindingtypes.h>


VarBind_t *create_VarBind_Nothing(ObjectName_t *on) {
  VarBind_t *ret;
  ret = calloc(1, sizeof(VarBind_t));
  ret->name = *on;
  ret->choice.present = choice_PR_NOTHING;

  return ret;
}


VarBind_t *create_VarBind_Value(ObjectName_t *on, ObjectSyntax_t *os) {
  VarBind_t *ret;
  ret = calloc(1, sizeof(VarBind_t));
  ret->name = *on;
  ret->choice.present = choice_PR_value;
  ret->choice.choice.value = *os;

  return ret;
}


VarBind_t *create_VarBind_unSpecified(ObjectName_t *on, NULL_t n) {
  VarBind_t *ret;
  ret = calloc(1, sizeof(VarBind_t));
  ret->name = *on;
  ret->choice.present = choice_PR_unSpecified;
  ret->choice.choice.unSpecified = n;

  return ret;
}


VarBind_t *create_VarBind_noSuchObject(ObjectName_t *on, NULL_t n) {
  VarBind_t *ret;
  ret = calloc(1, sizeof(VarBind_t));
  ret->name = *on;
  ret->choice.present = choice_PR_noSuchObject;
  ret->choice.choice.noSuchObject = n;

  return ret;
}


VarBind_t *create_VarBind_noSuchInstance(ObjectName_t *on, NULL_t n) {
  VarBind_t *ret;
  ret = calloc(1, sizeof(VarBind_t));
  ret->name = *on;
  ret->choice.present = choice_PR_noSuchInstance;
  ret->choice.choice.noSuchInstance = n;

  return ret;
}


VarBind_t *create_VarBind_endOfMibView(ObjectName_t *on, NULL_t n) {
  VarBind_t *ret;
  ret = calloc(1, sizeof(VarBind_t));
  ret->name = *on;
  ret->choice.present = choice_PR_endOfMibView;
  ret->choice.choice.endOfMibView = n;

  return ret;
}


VarBindList_t *create_VarBindList(VarBind_t *vb) {
  VarBindList_t *ret;
  ret = calloc(1, sizeof(VarBindList_t));
  int r = ASN_SEQUENCE_ADD(&ret->list, vb);

  return r < 0 ? NULL : ret;
}
