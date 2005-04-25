#ifndef _YAST_PERLRUN
#define _YAST_PERLRUN

void SWIG_YAST_MakePtr(SV *sv, void *ptr, swig_type_info *t, int flags) {
    char result[1024];
    snprintf(result, 1024, "_%p_%s", ptr, t->name);
    sv_setpv(sv, result);
}
int SWIG_YAST_ConvertPtr(SV *sv, void **ptr, swig_type_info *t, int flags) {
    char *c, *c1;
    if ((!sv) || (!SvPOK(sv))) return -1;
    c = SvPV_nolen(sv);
    if (!c || *c != '_' || !c[1]) return -1;
    *ptr = (void*)strtol(++c, &c1, 0);
    if (!c1 || *c1 != '_' || !c1[1]) return -1;
    if (t && !SWIG_TypeCheck(++c1,t)) return -1;
    return 0;
}
#undef SWIG_MakePtr
#undef SWIG_ConvertPtr
#define SWIG_MakePtr(sv, ptr, type, flags) SWIG_YAST_MakePtr(sv, ptr, type, flags)
#define SWIG_ConvertPtr(sv, ptr, type, flags) SWIG_YAST_ConvertPtr(sv, ptr, type, flags)

#endif /* _YAST_PERLRUN */
