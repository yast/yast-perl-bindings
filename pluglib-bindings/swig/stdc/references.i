
%typemap(in) double *(double dvalue), double &(double dvalue), float *(float dvalue), float &(float dvalue)
{
  SV *sv;
  if (!SvROK($input) || !(sv = SvRV($input)) || !(SvNOK(sv) || SvIOK(sv)) {
    SWIG_croak("Type error in argument $argnum of $symname. Expected a REFERENCE to FLOAT.\n");
  }
  dvalue = ($*1_ltype)SvNV(sv);
  $1 = &dvalue;
}

%typemap(in) int *(int dvalue), int &(int dvalue),
    short *(short dvalue), short &(short dvalue),
    long *(long dvalue), long &(long dvalue),
    signed char *(signed char dvalue), signed char &(signed char dvalue),
    bool *(bool dvalue), bool &(bool dvalue),
    enum SWIGTYPE *($*1_ltype dvalue), enum SWIGTYPE &($*1_ltype dvalue)
{
  SV *sv;
  if (!SvROK($input) || !(sv = SvRV($input)) || !SvIOK(sv)) {
    SWIG_croak("Type error in argument $argnum of $symname. Expected a REFERENCE to INTEGER.\n");
  }
  dvalue = ($*1_ltype)SvIV(sv);
  $1 = &dvalue;
}

%typemap(in) unsigned int *(unsigned int dvalue), unsigned int &(unsigned int dvalue),
    unsigned short *(unsigned short dvalue), unsigned short &(unsigned short dvalue),
    unsigned long *(unsigned long dvalue), unsigned long &(unsigned long dvalue),
    unsigned char *(unsigned char dvalue), unsigned char &(unsigned char dvalue)
{
  SV *sv;
  if (!SvROK($input) || !(sv = SvRV($input)) || !SvIOK(sv)) {
    SWIG_croak("Type error in argument $argnum of $symname. Expected a REFERENCE to INTEGER.\n");
  }
  dvalue = ($*1_ltype)SvUV(sv);
  $1 = &dvalue;
}

%typemap(in) long long *(long long dvalue), long long &(long long dvalue)
{
  SV *sv;
  if (!SvROK($input) ||
      !(sv = SvRV($input)) ||
      !(SvIOK(sv) || SvNOK(sv) || SvPOK(sv)) ) {
    SWIG_croak("Type error in argument $argnum of $symname. Expected a REFERENCE to INTEGER_IN_STRING.\n");
  }
  char * coerce = SvPV_nolen(sv);
  TO_QUAD(sv, &dvalue, sizeof(dvalue), $1_descriptor);
  $1 = &dvalue;
}

%typemap(in) unsigned long long *(unsigned long long dvalue), unsigned long long &(unsigned long long dvalue)
{
  SV *sv;
  if (!SvROK($input) ||
      !(sv = SvRV($input)) ||
      !(SvIOK(sv) || SvNOK(sv) || SvPOK(sv)) ) {
    SWIG_croak("Type error in argument $argnum of $symname. Expected a REFERENCE to INTEGER_IN_STRING.\n");
  }
  char * coerce = SvPV_nolen(sv);
  TO_UQUAD(sv, &dvalue, sizeof(dvalue), $1_descriptor);
  $1 = &dvalue;
}



%typemap(argout) const double *, const double &, const float  *, const float &;
%typemap(argout) double *, double &, float  *, float &
{
  SV *sv = SvRV($input);
  sv_setnv(sv, (double) *$1);
}


%typemap(argout) const int*, const int&, const short*, const short&, const long*, const long&, 
	const signed char*, const signed char &, const bool *, const bool &,
	const enum SWIGTYPE *, const enum SWIGTYPE &;
%typemap(argout) int*, int&, short*, short&, long*, long&, 
	signed char*, signed char &, bool *, bool &,
	enum SWIGTYPE *, enum SWIGTYPE &
{
  SV *sv = SvRV($input);
  sv_setiv(sv, (IV) *$1);
}

%typemap(argout) const unsigned int *, const unsigned int &,
       const unsigned short *, const unsigned short &,
       const unsigned long *, const unsigned long &,
       const unsigned char *, const unsigned char &;
%typemap(argout) unsigned int *, unsigned int &,
       unsigned short *, unsigned short &,
       unsigned long *, unsigned long &,
       unsigned char *, unsigned char &
{
  SV *sv = SvRV($input);
  sv_setuv(sv, (UV) *$1);
}

%typemap(argout) const long long *, const long long &;
%typemap(argout) long long *, long long &
{
  SV *sv = SvRV($input);
  FROM_QUAD(sv, $1, sizeof($*1_ltype), $1_descriptor);
}

%typemap(argout) const unsigned long long *, const unsigned long long &;
%typemap(argout) unsigned long long *, unsigned long long &
{
  SV *sv = SvRV($input);
  FROM_UQUAD(sv, $1, sizeof($*1_ltype), $1_descriptor);
}
