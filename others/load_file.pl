#!/usr/bin/env perl

my $buffer ="";
my $file = "junk2";
open($FILE, "$file");
read($FILE, $buffer, 5000);

print("$buffer\n");
