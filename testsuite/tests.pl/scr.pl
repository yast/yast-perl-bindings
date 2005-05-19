#! /usr/bin/perl

use warnings;
use strict;

use YaST::YCP;
YaST::YCP::Import ("SCR");

my $path = ".target.size";
my $arg = $0;
my $size = -1;
# This does not work yet: 
# Couldn't find an agent to handle '.ests."pl/scr".pl'
# Hmm, that's a path form of (t)ests.pl/scr.pl
#my $size = SCR::Read ($path, $arg);
print "Read ($path, $arg): $size\n";
