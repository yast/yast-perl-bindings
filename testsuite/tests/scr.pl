#! /usr/bin/perl

use warnings;
use strict;

use YaST::YCP;
YaST::YCP::Import ("SCR");

my $path = ".target.size";
my $arg = $0;
my $size = SCR::Read ($path, $arg);
print "Read ($path, $arg): $size\n";

