#!/usr/bin/perl
use Test::More tests => 18;

use lib "./modules";
use Ref;
#use Devel::Peek;

$ln = [];
Ref::RListInt($ln);
ok(@$ln == (), "ref to empty list");

$l = [0, 8, 333];
Ref::RListInt($l);
is_deeply($l, [1, 9, 334], "ref to non-empty list");

$s = 8;
Ref::RInt(\$s);
ok($s == 16, "ref to int");
Ref::PInt(\$s);
ok($s == 18, "ptr to int");

# bnc#408829
# start as number
$y = 5000000000;
#Dump($y);
Ref::RLLong(\$y);
ok($y == 15000000000, "ref to llong");
Ref::PLLong(\$y);
ok($y == 15000000003, "ptr to llong");

# start as string
$y = "5000000000";
#Dump($y);
Ref::RLLong(\$y);
ok($y == 15000000000, "ref to llong");
Ref::PLLong(\$y);
ok($y == 15000000003, "ptr to llong");

# error cases
$y = "zillion";
Ref::RLLong(\$y);
ok($y == 0, " non-numeric string becomes zero");
$y = [];
eval { Ref::RLLong(\$y); };
like($@, qr/Type error in argument 1/, " list produces exception");

$b = 0;
Ref::RBool(\$b);
ok($b, "ref to bool");
Ref::PBool(\$b);
ok(! $b, "ptr to bool");

$s = "hu";
Ref::RStr(\$s);
ok($s eq "hu.A",   "ref to string");
Ref::PStr(\$s);
ok($s eq "hu.A.B", "ptr to string");

$r = Ref::CRStr(\$s);
ok($r == 8, "ref to const string");
$r = Ref::CPStr(\$s);
ok($r == 8, "ptr to const string");

$b = 4;
Ref::REnum(\$b);
ok($b == 0, "ref to enum");
Ref::PEnum(\$b);
ok($b == 1, "ptr to enum");
