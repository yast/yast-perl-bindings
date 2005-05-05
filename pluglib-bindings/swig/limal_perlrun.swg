/*
 * file:	limal_perlrun.swg
 * author:	Martin Lazar <mlazar@suse.cz>
 *
 * makece and convert ptr and packed object
 *
 * $Id$
 */


#ifndef _LIMAL_PERLRUN
#define _LIMAL_PERLRUN

void SWIG_LIMAL_MakePtr(SV *sv, void *ptr, swig_type_info *t, int flags) {
    char result[1024];
    snprintf(result, 1024, "_%p_%s", ptr, t->name);
    sv_setpv(sv, result);
}

int SWIG_LIMAL_ConvertPtr(SV *sv, void **ptr, swig_type_info *t, int flags) {
    char *c, *c1;
    if ((!sv) || (!SvPOK(sv))) return -1;
    c = SvPV_nolen(sv);
    if (!c || *c != '_' || !c[1]) return -1;
    *ptr = (void*)strtol(++c, &c1, 0);
    if (!c1 || *c1 != '_' || !c1[1]) return -1;
    if (t && !SWIG_TypeCheck(++c1,t)) return -1;
    return 0;
}
/*
void SWIG_LIMAL_MakePackedObj(SV *sv, void *ptr, int sz, swig_type_info *type) {
  char result[1024];
  char *r = result;
  if ((2*sz + 1 + strlen(type->name)) > 1000) return;
  r += sprintf("_$*(r++) = '_';
  r = SWIG_PackData(r,ptr,sz);
  strcpy(r,type->name);
  sv_setpv(sv, result);
}

int SWIG_LIMAL_ConvertPacked(SV *obj, void *ptr, int sz, swig_type_info *ty, int flags) {
  swig_cast_info *tc;
  const char  *c = 0;

  if ((!obj) || (!SvOK(obj))) return -1;
  c = SvPV(obj, PL_na);
  // Pointer values must start with leading underscore 
  if (*c != '_') return -1;
  c++;
  if (sz<0) {
    char *c1 = strchr(c, '_');
    if (!c1) return -1;
    sz = (c1-c)/2;
  }
  c = SWIG_UnpackData(c,ptr,sz);
  if (ty) {
    tc = SWIG_TypeCheck(c,ty);
    if (!tc) return -1;
  }
  return 0;
}
*/

#undef SWIG_MakePtr
#undef SWIG_ConvertPtr
//#undef SWIG_MakePackObj
//#undef SWIG_ConvertPackedObj

#define SWIG_MakePtr(sv, ptr, type, flags) SWIG_LIMAL_MakePtr(sv, ptr, type, flags)
#define SWIG_ConvertPtr(sv, ptr, type, flags) SWIG_LIMAL_ConvertPtr(sv, ptr, type, flags)
//#define SWIG_MakePackedObj(sv, p, s, type) SWIG_LIMAL_MakePackedObj(sv, p, s, type)
//#define SWIG_ConvertPacked(obj, p, s, type, flags) SWIG_LIMAL_ConvertPacked(obj, p, s, type, flags)


#endif /* _LIMAL_PERLRUN */
