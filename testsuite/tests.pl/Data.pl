#! /usr/bin/perl

use warnings;
use strict;

use Data::Dumper;
use YaST::YCP qw(:DATA);
YaST::YCP::Import ("Data");

print Data->loop ("Hello"), "\n";
print Dumper (Data->loop (42));
print Dumper (Data->loop (Integer (42)));
print Dumper (Data->loop (Boolean (1)));
print Dumper (Data->loop (Symbol ("next")));
print Dumper (Data->loop (Term ("PushButton", "next")));
