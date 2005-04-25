#!/usr/bin/perl

use lib "./modules";
use Ref;
use Data::Dumper;

$ln = [];
Ref::RListInt($ln);
print Dumper($ln);

$l = [0, 8, 333];
print Dumper($l);
Ref::RListInt($l);
print Dumper($l);

$s = 8;
print "$s\n";
Ref::RInt(\$s);
print "$s\n";
Ref::PInt(\$s);
print "$s\n";

$s = "hu";
print "$s\n";
8==Ref::RStr(\$s) || die;
print "$s\n";
8==Ref::PStr(\$s) || die;
print "$s\n";
8==Ref::CRStr(\$s) || die;
print "$s\n";
8==Ref::CPStr(\$s) || die;
print "$s\n";
