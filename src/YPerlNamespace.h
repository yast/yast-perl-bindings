// -*- c++ -*-
#include <y2/Y2Namespace.h>
#include <y2/Y2Function.h>
#include <ycp/YStatement.h>

/**
 * YaST interface to a Perl module
 */
class YPerlNamespace : public Y2Namespace
{
private:
    string m_name;		//! this namespace's name, eg. XML::Writer
public:
    /**
     * Construct an interface. The module must be already loaded
     * @param name eg "XML::Writer"
     */
    YPerlNamespace (string name);

    virtual ~YPerlNamespace ();

    //! what namespace do we implement
    virtual const string name () const { return m_name; }
    //! used for error reporting
    virtual const string filename () const;

    //! unparse. useful  only for YCP namespaces??
    virtual string toString () const;
    //! called when evaluating the import statement
    // constructor is handled separately
    virtual YCPValue evaluate (bool cse = false);

    virtual Y2Function* createFunctionCall (const string name);
};
