#!/usr/bin/perl
# author: Martin Lazar <mlazar@suse.cz>
use warnings;
use strict;

my $outdir = ".";
if ($#ARGV >= 0 && $ARGV[0] =~ /--outdir=(.*)/) {
    $outdir = $1;
    shift;
}

my %unknown;
while(<>){
    if (/^package (?:(.*)::)?(.*?);$/) {
	my $mod = $2;
	(my $dir=$1||"") =~ s|::|/|g;
	system "mkdir -p '$outdir/$dir'";
	open OUT, ">>$outdir/$dir/$mod.pm";
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
    while (s/"(&?)(list|deque|queue|vector)<\(([^"]*)\)>"/["$1list", "$3"]/gi) {} #"

    # reference
    
    # all other
    my $x = "";
    while (/"([^"]*)"/) { #"
	$_ = $';
	my $c = $1;
	$x .= $`;
	(my $b = $c) =~ s/^&//;
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

(my $progname = $0) =~ s|.*/||;
foreach(sort keys %unknown) {
    next if /::/;
    print STDERR "$progname: Warning: unknown data type $_\n";
}
