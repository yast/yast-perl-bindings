
%{
#include <string>
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

/* convert any to SV */
void SwigConvertToSv(SV *&sv, bool &x) { sv_setiv(sv, x);}
void SwigConvertToSv(SV *&sv, const std::string &x) { sv_setpv(sv,x.c_str());}

void SwigConvertToSv(SV *&sv, signed char &x) { sv_setiv(sv, x); }
void SwigConvertToSv(SV *&sv, signed short &x) { sv_setiv(sv, x); }
void SwigConvertToSv(SV *&sv, signed int x) { sv_setiv(sv, x); }
void SwigConvertToSv(SV *&sv, signed long x) { sv_setiv(sv, x); }
void SwigConvertToSv(SV *&sv, signed long long &x) { char temp[256]; sprintf(temp,"%lld", x); sv_setpv(sv, temp); }

void SwigConvertToSv(SV *&sv, unsigned char &x) { sv_setuv(sv, x); }
void SwigConvertToSv(SV *&sv, unsigned short &x) { sv_setuv(sv, x); }
void SwigConvertToSv(SV *&sv, unsigned int &x) { sv_setuv(sv, x); }
void SwigConvertToSv(SV *&sv, unsigned long &x) { sv_setuv(sv, x); }
void SwigConvertToSv(SV *&sv, unsigned long long &x) { char temp[256]; sprintf(temp,"%llu", x); sv_setpv(sv, temp); }


/* convert from SV to any */
void SwigConvertFromSv(SV* sv, std::string &x) { x = std::string(SvPV_nolen(sv)); }
void SwigConvertFromSv(SV* sv, bool &x) { x = SvIV(sv); }

void SwigConvertFromSv(SV* sv, signed char &x) { x = SvIV(sv); }
void SwigConvertFromSv(SV* sv, signed short &x) { x = SvIV(sv); }
void SwigConvertFromSv(SV* sv, signed int &x) { x = SvIV(sv); }
void SwigConvertFromSv(SV* sv, signed long &x) { x = SvIV(sv); }
void SwigConvertFromSv(SV* sv, signed long long &x) { x = strtoll(SvPV_nolen(sv), 0, 0); }

void SwigConvertFromSv(SV* sv, unsigned char &x) { x = SvUV(sv); }
void SwigConvertFromSv(SV* sv, unsigned short &x) { x = SvUV(sv); }
void SwigConvertFromSv(SV* sv, unsigned int &x) { x = SvUV(sv); }
void SwigConvertFromSv(SV* sv, unsigned long &x) { x = SvUV(sv); }
void SwigConvertFromSv(SV* sv, unsigned long long &x) { x = strtoull(SvPV_nolen(sv), 0, 0); }

%}
