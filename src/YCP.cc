/**
 * $Id$
 *
 * This is the path from Perl to YCP. It defines XSUBs.
 */

#include <y2/Y2Namespace.h>
#include <y2/Y2Component.h>
#include <y2/Y2ComponentCreator.h>
#include <ycp/y2log.h>
#include <ycp/YBlock.h>
#include <ycp/YExpression.h>
#include <ycp/YStatement.h>

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "YPerl.h"

static
Y2Namespace *
getNs (const char * ns_name, const char * func_name)
{
    // func_name is only for debugging
    Y2Component *c = Y2ComponentBroker::getNamespaceComponent (ns_name);
    if (c == NULL)
    {
	y2error ("No component can provide namespace %s for a Perl call of %s",
		 ns_name, func_name);
	return NULL;
    }

    Y2Namespace *ns = c->import (ns_name);
    if (ns == NULL)
    {
	y2error ("Component %p could not provide namespace %s for a Perl call of %s",
		 c, ns_name, func_name);
    }
    else
    {
	ns->initialize ();
    }
    return ns;
}

// forward declaration
YCPValue YCP_call_SCR (pTHX_ const char * func_name, const vector<SV *>& args);
YCPValue YCP_getset_variable (pTHX_ const char * ns_name, SymbolEntryPtr var_se, const vector<SV *>& args);

XS(XS_YCP_y2_logger); /* prototype to pass -Wmissing-prototypes */
XS(XS_YCP_y2_logger)
{
    // defines "items", the number of arguments
    dXSARGS;
    if (items != 6)
    {
	y2internal ("y2_logger must have 6 arguments");
	XSRETURN_EMPTY;
    }
    loglevel_t level = (loglevel_t) SvIV (ST (0));
    const char * comp = SvPV_nolen (ST (1));
    const char * file = SvPV_nolen (ST (2));
    int line = SvIV (ST (3));
    const char * function = SvPV_nolen (ST (4));
    const char * message = SvPV_nolen (ST (5));
    y2_logger (level, comp, file, line, function, "%s", message);
    XSRETURN_EMPTY;
}

XS(XS_YCP_call_ycp); /* prototype to pass -Wmissing-prototypes */
XS(XS_YCP_call_ycp)
{
    // defines "items", the number of arguments
    dXSARGS; // must be called only once per xsub because it calls POPMARK

    /*
     * Plan:
     *  get the namespace name
     *  get the function name
     *  get the ns from the component broker (importing it, if necessary)
     *  find the function
     *  check its declaration, trying to convert the actual parameters
     *     to the desired ones
     *  call it
     *  pass the return value - if it's a list and perl wanted a list, so be it
     */

    // calling convention:
    //  namespace name (without trailing ::)
    //  function name
    //  function arguments
    if (items < 2 || !SvPOK (ST (0)) || !SvPOK (ST (1)))
    {
	y2internal ("Perl called YCP without specifying a namespace and a name");
	XSRETURN_EMPTY;
    }

    const char * ns_name = SvPV_nolen (ST (0));
    const char * func_name = SvPV_nolen (ST (1));

    // The perl interpreter must not get reinitialized.
    // Therefore we push what we got (aTHX) to YPerl so that it does
    // not try to construct its own.
    YPerl *yperl = YPerl::yPerl (aTHX); // access the singleton object

    YCPValue ret_yv = YCPNull ();

    // access the parameters via a vector because using dXSARGS more
    // than once per XSUB call messes up Perl call stacks
    vector<SV *> args;
    args.reserve (items - 2);
    I32 i;
    for (i = 2; i < items; ++i)
    {
	args.push_back (ST (i));
    }

    // this is a hack before the builtin namespaces get a uniform interface:
    if (! strcmp (ns_name, "SCR"))
    {
	ret_yv = YCP_call_SCR (aTHX_ func_name, args);
    }
    else
    {
	Y2Namespace *ns = getNs (ns_name, func_name);
	if (ns == NULL)
	{
	    XSRETURN_EMPTY;
	}

	// we want either a function or a variable
	// so find a symbol of an unspecified category
	TableEntry *sym_te = ns->table ()->find (func_name);

	if (sym_te == NULL)
	{
	    y2error ("No such symbol %s::%s", ns_name, func_name);
	    XSRETURN_EMPTY;
	}

	if (sym_te->sentry ()->isVariable () ||
	    sym_te->sentry ()->isReference ())
	{
	    ret_yv = YCP_getset_variable (aTHX_ ns_name, sym_te->sentry (), args);
	}
	else
	{ // no indent yet

	Y2Function *func_call = ns->createFunctionCall (func_name);
	if (func_call == NULL)
	{
	    y2error ("No such function %s::%s", ns_name, func_name);
	    XSRETURN_EMPTY;
	}

	// before we attach the parameters, we have to convert them
	// from Perl to YCP. That means knowing the destination type beforehand.
	// (Later if we get overloading, this will become messy)

	// go through the actual parameters
	unsigned j;
	for (j = 0; j < args.size (); ++j)
	{
	    // convert the value according to the expected type:

	    constTypePtr param_tp = func_call->wantedParameterType ();

	    YCPValue param_v = yperl->fromPerlScalar (args[j], param_tp);
	    if (param_v.isNull ())
	    {
		// an error has already been reported, now refine it.
		y2error ("... when passing parameter #%u to %s::%s",
			 j, ns_name, func_name);
		XSRETURN_EMPTY;
	    }

	    // Attach the parameter
	    bool ok = func_call->appendParameter (param_v);
	    if (!ok)
	    {
		// TODO really need to know the place in Perl code
		// where we were called from.
		XSRETURN_EMPTY;
	    }
	} // for each actual parameter

	bool ok = func_call->finishParameters ();
	if (!ok)
	{
	    // TODO really need to know the place in Perl code
	    // where we were called from.
	    XSRETURN_EMPTY;
	}
	// go call it now!
	y2debug ("Perl is calling %s::%s", ns_name, func_name);
	ret_yv = func_call->evaluateCall ();
	delete func_call;

	} // if not variable
    }

    if (ret_yv.isNull ())
    {
	XSRETURN_EMPTY;
    }

    y2debug ("YCP returned %s", ret_yv->toString ().c_str ());

    I32 context = GIMME_V;	// what context were we called in?
    if (context == G_VOID)
    {
	y2debug ("void context, returning nothing");
	XSRETURN_EMPTY;
    }
#if 0
    // disabled to be consistent with YPerl::call - disregard array context
    else if (context == G_ARRAY && ret_yv->isList ())
    {
	y2debug ("returning a list");
	// return a list iff they want a list and we have it
	YCPList ret_lv = ret_yv->asList ();
	int ret_len = ret_lv->size ();

	SP -= items; // reset stack pointer (declared by dXSARGS)
	EXTEND (SP, ret_len); // make room for the list
	for (int i = 0; i < ret_len; ++i)
	{
	    // put return values, no need to XPUSHs
	    PUSHs (sv_2mortal (yperl->newPerlScalar (ret_lv->value (i), false)));
	}
	XSRETURN (ret_len);
    }
#endif
    else
    {
	y2debug ("returning a scalar");
	ST (0) = sv_2mortal (yperl->newPerlScalar (ret_yv, false));
	XSRETURN (1);
    }
}


YCPValue YCP_call_SCR (pTHX_ const char * func_name, const vector<SV *>& args)
{
    // access directly the statically declared builtins
    extern StaticDeclaration static_declarations;
    string qualified_name_s = string ("SCR::") + func_name;
    const char *qualified_name = qualified_name_s.c_str ();

/*
  this does not work across namespaces
    TableEntry *bi_te = static_declarations.symbolTable ()->find (qualified_name);
    if (bi_te == NULL)
    {
	y2error ("No such builtin %s",qualified_name);
	return YCPNull ();
    }

    SymbolEntry *bi_se = bi_te->sentry ();
    assert (bi_se != NULL);
    assert (bi_se->isBuiltin ());
    declaration_t bi_dt = bi_se->declaration ();
*/
    declaration_t *bi_dt = static_declarations.findDeclaration (qualified_name);
    if (bi_dt == NULL)
    {
	y2error ("No such builtin '%s'", qualified_name);
	return YCPNull ();
    }

    // construct a builtin call using the proper overloaded builtin
    YEBuiltin *bi_call = new YEBuiltin (bi_dt);

    // attach the parameters:

    // we would like to know the destination type so that we could
    // convert eg a Perl scalar to a YCP symbol, but because the
    // builtins may be overloaded, let's say we want Any

    // maybe a special exceptional hack to make Path for the 1st argument?

    YPerl *yperl = YPerl::yPerl (aTHX); // access the singleton object
    // go through the actual parameters
    unsigned j;
    for (j = 0; j < args.size (); ++j)
    {
	// convert the value according to the expected type:
	constTypePtr param_tp = (j == 0)? Type::Path : Type::Any;

	YCPValue param_v = yperl->fromPerlScalar (args[j], param_tp);
	if (param_v.isNull ())
	{
	    // an error has already been reported, now refine it.
	    // Can't know parameter name?
	    y2error ("... when passing parameter #%u to builtin %s",
		     j, qualified_name);
	    return YCPNull ();
	}
	// Such YConsts without a specific type produce invalid
	// bytecode. (Which is OK here)
	// The actual parameter's YCode becomes owned by the function call?
	YConst *param_c = new YConst (YCode::ycConstant, param_v);
	// for attaching the parameter, must get the real type so that it matches
	constTypePtr act_param_tp = Type::vt2type (param_v->valuetype ());
	// Attach the parameter
	// Returns NULL if OK, Type::Error if excessive argument
	// Other errors (bad code, bad type) shouldn't happen
	constTypePtr err_tp = bi_call->attachParameter (param_c, act_param_tp);
	if (err_tp != NULL)
	{
	    if (err_tp->isError ())
	    {
		// TODO really need to know the place in Perl code
		// where we were called from.
		y2error ("Excessive parameter to builtin %s",
			 qualified_name);
	    }
	    else
	    {
		y2internal ("attachParameter returned %s",
			    err_tp->toString ().c_str ());
	    }
	    return YCPNull ();
	}
    } // for each actual parameter

    // now must check if we got fewer parameters than needed
    // or there was another error while resolving the overload
    constTypePtr err_tp = bi_call->finalize ();
    if (err_tp != NULL)
    {
	// apparently the error was already reported?
	y2error ("Error type %s when finalizing builtin %s",
		 err_tp->toString ().c_str (), qualified_name);
	return YCPNull ();
    }

    // go call it now!
    y2debug ("Perl is calling builtin %s", qualified_name);
    YCPValue ret_yv = bi_call->evaluate (false /* no const subexpr elim */);
    delete bi_call;

    return ret_yv;
}

/*
 * If we got no arguments, return the variable value.
 * If we got one argument, set the variable value (return YCPVoid).
 * Otherwise error (and return YCPNull)
 * ns_name is for error reporting
 */
YCPValue YCP_getset_variable (pTHX_ const char * ns_name, SymbolEntryPtr var_se, const vector<SV *>& args)
{
    YCPValue ret_yv = YCPNull ();
    unsigned n = args.size ();
    // first two args are the namespace and variable name
    if (n == 0)
    {
	// get
	ret_yv = var_se->value ();
    }
    else if (n == 1)
    {
	// set
	YPerl *yperl = YPerl::yPerl (aTHX); // access the singleton object
	YCPValue val_yv = yperl->fromPerlScalar (args[0], var_se->type ());
	if (val_yv.isNull ())
	{
	    // an error has already been reported, now refine it.
	    y2error ("... when setting value of %s::%s",
		     ns_name, var_se->name ());
	    return YCPNull ();
	}
	ret_yv = var_se->setValue (val_yv);
    }
    else
    {
	y2error ("Variable %s: don't know what to do, %u arguments",
		 var_se->name (), n);
    }
    return ret_yv;
}

/* called by XSLoader::load ('YaST::YCP') */
#ifdef __cplusplus
extern "C"
#endif
XS(boot_YaST__YCP); /* prototype to pass -Wmissing-prototypes */
XS(boot_YaST__YCP)
{
    dXSARGS;
    // get rid of warning: unused variable `I32 items'
    I32 __attribute__ ((unused)) foo = items;
    char* file = __FILE__;

    XS_VERSION_BOOTCHECK ;

    if (Y2ComponentBroker::createClient ("wfm") == 0)
    {
	y2error ("Cannot create server WFM component");
    }

    newXS("YaST::YCP::call_ycp", XS_YCP_call_ycp, file);
    newXS("YaST::YCP::y2_logger", XS_YCP_y2_logger, file);
    XSRETURN_YES;
}
