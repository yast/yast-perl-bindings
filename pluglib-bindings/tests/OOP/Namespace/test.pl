#!/usr/bin/perl

use lib "./modules";
use Simple;

$xyz = Simple::create_xyz();
Simple::xyz::swig_x_set($xyz, 8);
Simple::xyz::decX($xyz);
print "xyz.x = ", Simple::xyz::swig_x_get($xyz), "\n";
