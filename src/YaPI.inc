my %__error = ();

BEGIN { $TYPEINFO{Interface} = ["function", ["list", "any"]]; }
sub Interface {
    my @ret = ();

    foreach my $funcName (sort keys %TYPEINFO) {
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
    return ${"${self}::VERSION"};
}

BEGIN { $TYPEINFO{Supports} = ["function", "boolean", "string"]; }
sub Supports {
    my $cap  = shift;
    
    return !!grep( ($_ eq $cap), @CAPABILITIES);
}

BEGIN { $TYPEINFO{Error} = ["function", ["map", "string", "any"] ]; }
sub Error {
    return \%__error;
}

sub SetError {
    %__error = @_;
    if( !$__error{package} && !$__error{file} && !$__error{line})
    {
        @__error{'package','file','line'} = caller();
    }
    if ( defined $__error{summary} ) {
        y2error($__error{code}."[".$__error{line}.":".$__error{file}."] ".$__error{summary});
    } else {
        y2error($__error{code}."[".$__error{line}.":".$__error{file}."]");
    }
    return undef;
}
