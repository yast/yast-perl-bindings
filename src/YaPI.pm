package YaPI;

BEGIN {
    push @INC, '/usr/share/YaST2/modules';
}

use strict;

use Exporter;
our @ISA = qw(Exporter);
our @EXPORT = qw(textdomain);

use YaST::YCP;
use ycp;

use Locale::gettext ("!textdomain");
use POSIX ();     # Needed for setlocale()

POSIX::setlocale(&POSIX::LC_MESSAGES, "");

our %TYPEINFO;
my %__error = ();
my $VERSION = "";
our @CAPABILITIES = ();

BEGIN { $TYPEINFO{Interface} = ["function", "any"]; }
sub Interface {
    my $self = shift;
    my @ret = ();

    no strict "refs";
    my %TI = %{"${self}::TYPEINFO"};

    foreach my $k (keys %TYPEINFO) {
        $TI{$k} = $TYPEINFO{$k};
    }

    foreach my $funcName (sort keys %TI) {
        my @dummy = @{$TI{$funcName}};
        my $hash = {};

        $hash->{'functionName'} = $funcName;
        $hash->{'return'}       = $dummy[1];
        splice(@dummy, 0, 2);
        $hash->{'argument'} = \@dummy;
        push @ret, $hash;
    }
    return \@ret;
}

BEGIN { $TYPEINFO{Version} = ["function", "string"]; }
sub Version {
    my $self = shift;
    no strict "refs";
    return ${"${self}::VERSION"};
}

BEGIN { $TYPEINFO{Supports} = ["function", "boolean", "string"]; }
sub Supports {
    my $self = shift;
    my $cap  = shift;

    no strict "refs";
    my @c = @{"${self}::CAPABILITIES"};
    foreach my $k (@CAPABILITIES) {
        push @c, $k;
    }

    return !!grep( ($_ eq $cap), @c);
}


BEGIN { $TYPEINFO{SetError} = ["function", "boolean", ["map", "string", "any" ]]; }
sub SetError {
    my $self = shift;
    %__error = @_;
    if( !$__error{package} && !$__error{file} && !$__error{line})
    {
        @__error{'package','file','line'} = caller();
    }
    if ( defined $__error{summary} ) {
        y2error($__error{code}."[".$__error{line}.":".$__error{file}."] ".$__error{summary});
    } else {
        y2error($__error{code});
    }
    return undef;
}

BEGIN { $TYPEINFO{Error} = ["function", ["map", "string", "any"] ]; }
sub Error {
    my $self = shift;
    return \%__error;
}

=head2 i18n

    use YaPI;
    textdomain "mydomain";

Just use C<_("my text")> to mark text to be translated.

These must not be used any longer:

 #  use Locale::gettext;
 #  sub _ { ... }

These don't hurt but aren't necessary:

 #  use POSIX ();
 #  POSIX::setlocale(LC_MESSAGES, "");    # YaPI calls it itself now

=head3 textdomain

Calls Locale::gettext::textdomain
and also
remembers an association between the calling package and the
domain. Later calls of _ use this domain as an argument to dgettext.

=cut

# See also bug 38613 where untranslated texts were seen because
# a random textdomain was used instead of the proper one.
my %textdomains;

sub textdomain
{
    my $domain = shift;
    my $package = caller;

    $textdomains{$package} = $domain;
    return Locale::gettext::textdomain ($domain);
}

=head3 _

Calls Locale::gettext::dgettext, supplying the textdomain of the calling
package (set by calling textdomain).

Note: functions with _ are automaticaly exported to main::

=cut

sub _ {
    my $msgid = shift;
    my $package = caller;
    my $domain = $textdomains{$package};
    return Locale::gettext::dgettext ($domain, $msgid);
}

1;
