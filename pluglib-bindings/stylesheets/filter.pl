#!/usr/bin/perl
# author: Martin Lazar <mlazar@suse.cz>

while(<>){
    if (/^package (?:(.*)::)?(.*?);$/) {
	$mod = $2;
	($dir=$1) =~ s|::|/|g;
	system "mkdir -p 'modules/$dir'";
	open OUT, ">>modules/$dir/$mod.pm";
    }
    # remove known prefixes
    s/\bstd:://g;
    s/\bBLOCXX_NAMESPACE:://gi;
    s/\bblocxx[0-9]*:://gi;

    s/\bp\./&/g; # pointer
    s/\br\./&/g; # reference
    s/\bq\(const\)\.//g; # constant
    
    # integer
    s/\b((S|Signed|U|Unsigned) ?)?Int(eger)?[0-9]*\b/integer/gi;
    s/\b((S|Signed|U|Unsigned) ?)?Long( ?Long)?( ?Integer)?\b/integer/gi;
    s/\b((S|Signed|U|Unsigned) ?)?Short( ?Integer)?\b/integer/gi;
    s/\b(Signed|Unsigned)\b/integer/gi;
    s/\b(Size|Size_t)\b/integer/gi;

    # boolean
    s/\bBool(ean)?\b/boolean/gi;
    s/\bTruth\b/boolean/gi;
    
    # float
    s/\bReal(32|64)?\b/float/gi;
    s/\bDouble\b/float/gi;
    s/\bFloat\b/float/gi;
    
    # string
    s/\bString\b/string/gi;
    
    # list && map
    while (s/"(&?)(list|deque|queue|vector)<\(([^"]*)\)>"/["$1list", "$3"]/gi) {} #"
    while (s/"(&?)map<\(([^"]*),([^"]*)\)>"/["$1map", "$2", "$3"]/gi) {}

    # reference
    
    # all other
    $x = "";
    while (/"([^"]*)"/) { #"
	$_ = $';
	$c = $1;
	$x .= $`;
	($b = $c) =~ s/^&//;
	if ($b ne "map" && $b ne "list" && $b ne "integer" && $b ne "void"
	    && $b ne "any" && $b ne "boolean" && $b ne "string" && $b ne "function")
	{
	    $unknown{$b}++;
	    $c = "any";
	}
	$x .= "\"$c\"";
    }
    print OUT $x . $_;
}

foreach(sort keys %unknown) {
    next if /::/;
    print STDERR "Warning: unknown data type $_\n";
}
